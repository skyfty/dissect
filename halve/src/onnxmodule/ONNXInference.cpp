#include "onnxinference.h"
#include "registration/ProgressReporter.h"
#include <QString>

ONNXInference::ONNXInference(QObject *parent) : QObject(parent)
{
    QString exeDir = QCoreApplication::applicationDirPath();

    //// 构建onnxmodel文件夹路径
    QDir modelDir(exeDir);
    if (!modelDir.cd("onnxmodel")) {
        qWarning() << "onnxmodel directory not found in:" << exeDir;
        return;
    }

    QString cfgName = "model.json";
    QString cfgPath = modelDir.filePath(cfgName);
    if (!QFileInfo(cfgPath).exists()) {
        qWarning() << "Failed to open cfg file:" << cfgPath;
        return;
    }
    /// 加载模型配置
    this->loadConfig(cfgPath);


    QString modelName = "model.onnx";
    QString modelPath = modelDir.filePath(modelName);
    if (!QFileInfo(modelPath).exists()) {
        qWarning() << "Failed to open model file:" << modelPath;
        return;
    }
    /// 加载ONNX模型
    this->loadModel(modelPath);
}


void ONNXInference::loadConfig(const QString & configFile)
{
    std::ifstream f(configFile.toStdString());
    if (!f.is_open()) {
        throw std::runtime_error("Failed to open preprocessing config file");
    }
    nlohmann::json config;
    f >> config;

    // 读取配置参数
    auto temp= config["intensity_normalization_settings"];
    m_normalizationScheme = temp[0];

    auto spacingJson = config["spacing"];
    for (size_t i = 0; i < 3; i++) {
        m_targetSpacing[i] = spacingJson[i];
    }

    auto patchSizeJson = config["patch_size"];
    for (size_t i = 0; i < 3; i++) {
        m_patchSize[i] = patchSizeJson[i];
    }

    m_numInputChannels = config["num_input_channels"];
    m_numClasses = config["num_classes"];
    for (const auto& className : config["class_names"]) {
        m_classNames.push_back(className);
    }

    m_intensityProperties = config["intensity_properties"];
}


void ONNXInference::loadModel(const QString &modelPath)
{

    Ort::SessionOptions sessionOptions;
    sessionOptions.SetIntraOpNumThreads(4);
    sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

    /// 创建会话
    session = Ort::Session(env,modelPath.toStdWString().c_str(),sessionOptions);

    /// 获取输入/输出名称
    Ort::AllocatorWithDefaultOptions allocator;
    size_t numInputNodes = session.GetInputCount();
    //m_inputNames.resize(numInputNodes);
    for(size_t i = 0; i< numInputNodes;++i){
        auto inputName = session.GetInputNameAllocated(i,allocator);
        m_inputNames.push_back(inputName.get());
    }

    /// 获取输出名称
    m_numOutputNodes = session.GetOutputCount();
    //m_outputNames.resize(m_numOutputNodes);
    for(size_t i =0 ; i < m_numOutputNodes; ++i){
        auto outputName = session.GetOutputNameAllocated(i,allocator);
        m_outputNames.push_back(outputName.get());
    }

}


void ONNXInference::loadNiftiFile(const QString& filePath)
{

    itk::NiftiImageIOFactory::RegisterOneFactory();
    using ReaderType = itk::ImageFileReader<ImageType>;
    auto reader = ReaderType::New();
    reader->SetFileName(filePath.toStdString().c_str());
    reader->Update();

    if (!reader->GetOutput()) {
        qWarning() << "Failed to read NIfTI file";
        return;
    }

    m_inputImage = reader->GetOutput();
}

void ONNXInference::runInference(const QString& outputPath){
    if(!session)
        return;

    std::vector<int64_t> inputShapeVec{ 1,
                                             static_cast<int64_t>(m_numInputChannels),
                                             static_cast<int64_t>(m_patchSize[0]),
                                             static_cast<int64_t>(m_patchSize[1]),
                                             static_cast<int64_t>(m_patchSize[2]) };

    //  定义推理回调函数
    auto inferenceFunc = [&](const std::vector<float>& patchData) -> std::vector<float> {
        // 创建输入tensor
        auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
        Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
            memory_info, const_cast<float*>(patchData.data()), patchData.size(),
            inputShapeVec.data(), inputShapeVec.size());

        // 运行推理
        const char* inputs[] = { "input" };
        const char* outputs[] = { "output" };
        std::vector<Ort::Value> outputTensors = session.Run(
            Ort::RunOptions{ nullptr }, inputs, &inputTensor, 1,
            outputs, m_numOutputNodes);

        // 处理输出结果
        float* outputData = outputTensors[0].GetTensorMutableData<float>();

        // 获取输出形状
        auto outputTensorInfo = outputTensors[0].GetTensorTypeAndShapeInfo();
        auto outputShape = outputTensorInfo.GetShape();
        size_t outputSize = outputTensorInfo.GetElementCount();

        // 复制输出数据
        std::vector<float> outputVector(outputData, outputData + outputSize);
        return outputVector;
    };
    
    try
    {
        auto segmentationMask = processLargeImage(m_inputImage, inferenceFunc);

        // 9. 保存分割结果
        if (m_progressReporter)
        {
            m_progressReporter->setStepWeight(0.1);
            m_progressReporter->setProgressText("writing results...");
        }
        using WriterType = itk::ImageFileWriter<MaskType>;
        auto writer = WriterType::New();
        writer->SetFileName(outputPath.toStdString().c_str());
        writer->SetInput(segmentationMask);
        writer->Update();
        if (m_progressReporter)
        {
            m_progressReporter->setProgressText("loading finished");
            m_progressReporter->complete();
        }
    }
    catch (const std::exception& e)
    {
        if (m_progressReporter)
        {
            m_progressReporter->setProgressText(QString("error occurred:%1").arg(e.what()));
            m_progressReporter->complete();
        }
    }
    
    return;
}

MaskPointer ONNXInference::processLargeImage (const ImagePointer& inputImage,
                               std::function<std::vector<float>(const std::vector<float>&)> inferenceFunc)
{


    // 1. 重采样到目标间距
    auto resampledImage = resampleToTargetSpacing(inputImage);

    // 2. 强度归一化
    auto normalizedImage = normalizeIntensity(resampledImage);

    // 3. 创建滑动窗口补丁
    auto patches = createSlidingWindowPatches(normalizedImage);

    // 4. 为每个补丁运行推理
    std::vector<std::vector<float>> patchResults;
    int patchCount = patches.size();
    int patchIndex = 0;
    m_progressReporter->setStepWeight(0.35);
    for (const auto& patch : patches) {
        std::vector<float> result = inferenceFunc(patch);
        patchResults.push_back(result);
        patchIndex++;
        if (m_progressReporter)
        {
            m_progressReporter->setProgressText("running inference function...");
            m_progressReporter->setStepProgressValue(patchIndex / (float)patchCount);
        }
    }
    m_progressReporter->completeStep();

    // 5. 合并结果创建概率图
    const ImageType::RegionType& region = normalizedImage->GetLargestPossibleRegion();
    const ImageType::SizeType& imageSize = region.GetSize();
    // 为每个类别创建一个概率图
    std::vector<ImagePointer> probabilityMaps(m_numClasses);
    std::vector<ImagePointer> weightMaps(m_numClasses);

    for (int classIdx = 0; classIdx < m_numClasses; classIdx++) {
        probabilityMaps[classIdx] = ImageType::New();
        probabilityMaps[classIdx]->SetRegions(region);
        probabilityMaps[classIdx]->SetSpacing(normalizedImage->GetSpacing());
        probabilityMaps[classIdx]->SetOrigin(normalizedImage->GetOrigin());
        probabilityMaps[classIdx]->SetDirection(normalizedImage->GetDirection());
        probabilityMaps[classIdx]->Allocate();
        probabilityMaps[classIdx]->FillBuffer(0);

        weightMaps[classIdx] = ImageType::New();
        weightMaps[classIdx]->SetRegions(region);
        weightMaps[classIdx]->SetSpacing(normalizedImage->GetSpacing());
        weightMaps[classIdx]->SetOrigin(normalizedImage->GetOrigin());
        weightMaps[classIdx]->SetDirection(normalizedImage->GetDirection());
        weightMaps[classIdx]->Allocate();
        weightMaps[classIdx]->FillBuffer(0);
    }

    // 定义滑动窗口参数(与创建补丁时一致)
    //float overlapFactor = 0.0f;
    std::array<size_t, 3> steps;
    for (size_t i = 0; i < 3; i++) {
        steps[i] = static_cast<size_t>(m_patchSize[i] * (1 - m_overlapFactor));
        if (steps[i] == 0) steps[i] = 1;
    }

    // 计算滑动窗口数量
    std::array<size_t, 3> numSteps;
    for (size_t i = 0; i < 3; i++) {
        numSteps[i] = 1 + (imageSize[i] - m_patchSize[i] + steps[i] - 1) / steps[i];
        if (imageSize[i] <= m_patchSize[i]) numSteps[i] = 1;
    }

    // 使用高斯权重进行补丁融合(模拟nnU-Net的行为)
    // 生成所有patch起始点表
    auto xStarts = getAdjustedStarts(imageSize[0], m_patchSize[0], steps[0]);
    auto yStarts = getAdjustedStarts(imageSize[1], m_patchSize[1], steps[1]);
    auto zStarts = getAdjustedStarts(imageSize[2], m_patchSize[2], steps[2]);

    size_t patchIdx = 0;
    m_progressReporter->setStepWeight(0.35);
    for (auto z : zStarts) {
        for (auto y : yStarts) {
            for (auto x : xStarts) {
                if (patchIdx >= patchResults.size()) continue;

                if (m_progressReporter)
                {
                    m_progressReporter->setProgressText("merging patches...");
                    m_progressReporter->setStepProgressValue(patchIdx / (float)patchCount);
                }
                ImageType::IndexType startIndex = { static_cast<long>(x),
                                                   static_cast<long>(y),
                                                   static_cast<long>(z) };

                // 预计算高斯权重
                std::vector<float> gaussianWeights = precomputeGaussianWeights(m_patchSize);

                for (int classIdx = 0; classIdx < m_numClasses; classIdx++) {
                    size_t pixelOffset = classIdx * (m_patchSize[0] * m_patchSize[1] * m_patchSize[2]);
                    
                    for (size_t pz = 0; pz < m_patchSize[2]; pz++) {
                        size_t zz = z + pz;
                        if (zz >= imageSize[2]) continue;
                        for (size_t py = 0; py < m_patchSize[1]; py++) {
                            size_t yy = y + py;
                            if (yy >= imageSize[1]) continue;
                            for (size_t px = 0; px < m_patchSize[0]; px++) {
                                size_t xx = x + px;
                                if (xx >= imageSize[0]) continue;  // 越界就跳过
                                ImageType::IndexType pixelIndex = { static_cast<long>(xx), static_cast<long>(yy), static_cast<long>(zz) };


                                size_t linearIdx = px + py * m_patchSize[0] + pz * m_patchSize[0] * m_patchSize[1];
                                float weight = gaussianWeights[linearIdx];
                                float prob = patchResults[patchIdx][pixelOffset + linearIdx];

                                // 更新概率图和权重图
                                probabilityMaps[classIdx]->SetPixel(
                                    pixelIndex, probabilityMaps[classIdx]->GetPixel(pixelIndex) + prob * weight);
                                weightMaps[classIdx]->SetPixel(
                                    pixelIndex, weightMaps[classIdx]->GetPixel(pixelIndex) + weight);
                            }
                        }
                    }
                }
                patchIdx++;
            }
        }
    }
    m_progressReporter->completeStep();
    // Create the finalMask properties
    auto finalMask = MaskType::New();
    finalMask->SetRegions(region);
    finalMask->SetSpacing(normalizedImage->GetSpacing());
    finalMask->SetOrigin(normalizedImage->GetOrigin());
    finalMask->SetDirection(normalizedImage->GetDirection());
    finalMask->Allocate();
    finalMask->FillBuffer(0);

    // 获取总像素数
    size_t N = region.GetNumberOfPixels();
    size_t num_classes = static_cast<size_t>(m_numClasses);

    // 获取所有概率和权重的原始buffer
    std::vector<const float*> prob_buffers(num_classes);
    std::vector<const float*> weight_buffers(num_classes);
    for (size_t c = 0; c < num_classes; ++c) {
        prob_buffers[c] = probabilityMaps[c]->GetBufferPointer();
        weight_buffers[c] = weightMaps[c]->GetBufferPointer();
    }
    unsigned char* mask_buf = finalMask->GetBufferPointer();

// 并行部分可考虑OpenMP，下面为串行（最简单移植最快）：
#pragma omp parallel for
    for (ptrdiff_t i = 0; i < static_cast<ptrdiff_t>(N); ++i) {
        unsigned char maxClassIdx = 0;
        float maxProb = 0.0f;
        for (size_t c = 0; c < num_classes; ++c) {
            float w = weight_buffers[c][i];
            if (w > 0.0f) {
                float p = prob_buffers[c][i] / w;
                if (p > maxProb) {
                    maxProb = p;
                    maxClassIdx = static_cast<unsigned char>(c);
                }
            }
        }
        mask_buf[i] = maxClassIdx;
    }

    return finalMask;
}

ImagePointer ONNXInference::resampleToTargetSpacing(const ImagePointer& inputImage)
{
    using ResampleFilterType = itk::ResampleImageFilter<ImageType, ImageType>;
    using TransformType = itk::IdentityTransform<double, 3>;
    using InterpolatorType = itk::LinearInterpolateImageFunction<ImageType, double>;

    auto resampleFilter = ResampleFilterType::New();
    auto transform = TransformType::New();
    auto interpolator = InterpolatorType::New();

    resampleFilter->SetTransform(transform);
    resampleFilter->SetInterpolator(interpolator);

    // 计算目标尺寸
    const ImageType::SpacingType& inputSpacing = inputImage->GetSpacing();
    const ImageType::RegionType& inputRegion = inputImage->GetLargestPossibleRegion();
    const ImageType::SizeType& inputSize = inputRegion.GetSize();

    ImageType::SpacingType outputSpacing;
    for (size_t i = 0; i < 3; i++) {
        outputSpacing[i] = m_targetSpacing[i];
    }

    ImageType::SizeType outputSize;
    for (size_t i = 0; i < 3; i++) {
        outputSize[i] = static_cast<size_t>(
            std::ceil(inputSize[i] * inputSpacing[i] / outputSpacing[i]));
    }
    auto minSize = *std::min_element(outputSize.begin(), outputSize.end());
    if (minSize <m_patchSize[0])
    {
        for (size_t i = 0; i < 3; i++) {
            outputSize[i] *= (128.0 / minSize);
            outputSpacing[i] = (inputSize[i] * inputSpacing[i] / outputSize[i]);
        }
    }

    resampleFilter->SetOutputSpacing(outputSpacing);
    resampleFilter->SetSize(outputSize);
    resampleFilter->SetOutputOrigin(inputImage->GetOrigin());
    resampleFilter->SetOutputDirection(inputImage->GetDirection());
    resampleFilter->SetDefaultPixelValue(0);
    resampleFilter->SetInput(inputImage);

    try {
        resampleFilter->Update();
        return resampleFilter->GetOutput();
    }
    catch (const itk::ExceptionObject& e) {
        std::cerr << "Exception during resampling: " << e << std::endl;
        throw;
    }
}

ImagePointer ONNXInference::cropOrPadToCorrectSize(
    const ImagePointer& inputImage) {

    const ImageType::RegionType& inputRegion = inputImage->GetLargestPossibleRegion();
    const ImageType::SizeType& inputSize = inputRegion.GetSize();

    // 初始化填充量为0
    ImageType::SizeType padSize;
    padSize.Fill(0);
    bool needPadding = false;

    // 计算需要填充的尺寸
    for (size_t i = 0; i < 3; i++) {
        if (inputSize[i] < static_cast<size_t>(m_patchSize[i])) {
            padSize[i] = m_patchSize[i] - inputSize[i];
            needPadding = true;
        }
    }

    if (needPadding) {
        // 正确计算上下边界
        ImageType::SizeType padLower, padUpper;
        for (size_t i = 0; i < 3; i++) {
            padLower[i] = padSize[i] / 2;      // 下边界取整除法
            padUpper[i] = padSize[i] - padLower[i];  // 剩余部分作为上边界
        }

        using PadFilterType = itk::ConstantPadImageFilter<ImageType, ImageType>;
        auto padFilter = PadFilterType::New();
        padFilter->SetInput(inputImage);
        padFilter->SetPadLowerBound(padLower);  // 使用计算后的下边界
        padFilter->SetPadUpperBound(padUpper);   // 使用计算后的上边界
        padFilter->SetConstant(0);

        try {
            padFilter->Update();
            return padFilter->GetOutput();
        }
        catch (const itk::ExceptionObject& e) {
            std::cerr << "Padding error: " << e.what() << std::endl;
            throw;
        }
    }
    else {
        return inputImage;
    }
}


ImagePointer ONNXInference::normalizeIntensity(const ImagePointer& inputImage)
{
    // 创建输出图像
    auto outputImage = ImageType::New();
    outputImage->SetRegions(inputImage->GetLargestPossibleRegion());
    outputImage->SetSpacing(inputImage->GetSpacing());
    outputImage->SetOrigin(inputImage->GetOrigin());
    outputImage->SetDirection(inputImage->GetDirection());
    outputImage->Allocate();

    // 根据normalizationScheme应用不同的归一化方法
    if (m_normalizationScheme == "CTNormalization") {
        //// CT数据通常使用直接的单位值裁剪
        if (m_normalizationScheme == "CTNormalization") {
            // 优先用配置，否则全部自动统计
            float mean, std, lower, upper;
            bool has_mean = m_intensityProperties.contains("mean");
            bool has_std = m_intensityProperties.contains("std");
            bool has_low = m_intensityProperties.contains("percentile_00_5");
            bool has_up = m_intensityProperties.contains("percentile_99_5");

            // ===== 属性全部有则取用，否则全部自动计算 =====
            std::vector<float> voxels;

            if (has_mean && has_std && has_low && has_up) {
                // 需要打开
                mean = m_intensityProperties["mean"];
                std = m_intensityProperties["std"];
                lower = m_intensityProperties["percentile_00_5"];
                upper = m_intensityProperties["percentile_99_5"];
            }
            else {
                // 先收集所有体素
                itk::ImageRegionConstIterator<ImageType> inputIt(
                    inputImage, inputImage->GetLargestPossibleRegion());
                for (inputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt) {
                    voxels.push_back(inputIt.Get());
                }
                if (voxels.empty())
                    throw std::runtime_error("Image has no voxels for normalization!");

                // 先排序
                std::vector<float> sorted = voxels;
                std::sort(sorted.begin(), sorted.end());

                auto percentile = [&](double per) -> float {
                    if (sorted.size() == 1) return sorted[0];
                    double idx = (per / 100.0) * (sorted.size() - 1);
                    size_t lo = static_cast<size_t>(std::floor(idx));
                    size_t hi = static_cast<size_t>(std::ceil(idx));
                    double wt = idx - lo;
                    return wt == 0 ? sorted[lo] : static_cast<float>(sorted[lo] * (1 - wt) + sorted[hi] * wt);
                };

                lower = percentile(0.5);
                upper = percentile(99.5);

                // 计算 mean
                double sum = 0.0;
                for (float v : voxels) sum += v;
                mean = static_cast<float>(sum / voxels.size());

                // 计算 std
                double vsum = 0.0;
                for (float v : voxels) {
                    double diff = v - mean;
                    vsum += diff * diff;
                }
                std = static_cast<float>(std::sqrt(vsum / voxels.size()));
            }

            const float eps = 1e-8f; // 防止除0

            itk::ImageRegionConstIterator<ImageType> inputIt(
                inputImage, inputImage->GetLargestPossibleRegion());
            itk::ImageRegionIterator<ImageType> outputIt(
                outputImage, outputImage->GetLargestPossibleRegion());

            for (inputIt.GoToBegin(), outputIt.GoToBegin();
                 !inputIt.IsAtEnd(); ++inputIt, ++outputIt) {
                float val = inputIt.Get();
                // clip
                val = std::min(std::max(val, lower), upper);
                // nnUNet（py）：clip→减mean→除std
                val = (val - mean) / std::max(std, eps);
                outputIt.Set(val);
            }
        }
    }
    else if (m_normalizationScheme == "Z-Score") {
        // Z-Score标准化：(x - mean) / std
        float mean = 0.0f;
        float std = 1.0f;

        // 从配置中提取CT值
        if (m_intensityProperties.contains("mean") &&
            m_intensityProperties.contains("std")) {
            // 需要打开
            mean = m_intensityProperties["mean"];
            std = m_intensityProperties["std"];
        }
        else {
            // 如果配置中没有，则计算
            itk::ImageRegionConstIterator<ImageType> inputIt(
                inputImage, inputImage->GetLargestPossibleRegion());

            double sum = 0.0;
            size_t count = 0;

            // 计算均值
            for (inputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt) {
                sum += inputIt.Get();
                count++;
            }
            mean = static_cast<float>(sum / count);

            // 计算标准差
            sum = 0.0;
            for (inputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt) {
                double diff = inputIt.Get() - mean;
                sum += diff * diff;
            }
            std = static_cast<float>(std::sqrt(sum / count));
        }

        // 应用Z-Score标准化
        itk::ImageRegionConstIterator<ImageType> inputIt(
            inputImage, inputImage->GetLargestPossibleRegion());
        itk::ImageRegionIterator<ImageType> outputIt(
            outputImage, outputImage->GetLargestPossibleRegion());

        for (inputIt.GoToBegin(), outputIt.GoToBegin();
             !inputIt.IsAtEnd(); ++inputIt, ++outputIt) {
            outputIt.Set((inputIt.Get() - mean) / std);
        }
    }
    else {
        // 默认使用简单的[0,1]归一化
        float minVal = std::numeric_limits<float>::max();
        float maxVal = std::numeric_limits<float>::lowest();

        // 查找最小和最大值
        itk::ImageRegionConstIterator<ImageType> inputIt(
            inputImage, inputImage->GetLargestPossibleRegion());

        for (inputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt) {
            float val = inputIt.Get();
            minVal = std::min(minVal, val);
            maxVal = std::max(maxVal, val);
        }

        // 应用归一化
        inputIt.GoToBegin();
        itk::ImageRegionIterator<ImageType> outputIt(
            outputImage, outputImage->GetLargestPossibleRegion());

        for (inputIt.GoToBegin(), outputIt.GoToBegin();
             !inputIt.IsAtEnd(); ++inputIt, ++outputIt) {
            outputIt.Set((inputIt.Get() - minVal) / (maxVal - minVal));
        }
    }

    return outputImage;
}


std::vector<std::vector<float>> ONNXInference::createSlidingWindowPatches(const ImagePointer& inputImage)
{
    const ImageType::RegionType& region = inputImage->GetLargestPossibleRegion();
    const ImageType::SizeType& imageSize = region.GetSize();

    std::array<size_t, 3> steps;
    for (size_t i = 0; i < 3; i++) {
        steps[i] = static_cast<size_t>(m_patchSize[i] * (1 - m_overlapFactor));
        if (steps[i] == 0) steps[i] = 1;
    }

    auto xStarts = getAdjustedStarts(imageSize[0], m_patchSize[0], steps[0]);
    auto yStarts = getAdjustedStarts(imageSize[1], m_patchSize[1], steps[1]);
    auto zStarts = getAdjustedStarts(imageSize[2], m_patchSize[2], steps[2]);

    std::vector<std::vector<float>> patches;
    using ExtractFilterType = itk::ExtractImageFilter<ImageType, ImageType>;

    for (auto z : zStarts) {
        for (auto y : yStarts) {
            for (auto x : xStarts) {
                ImageType::IndexType startIndex = { static_cast<long>(x),
                                                   static_cast<long>(y),
                                                   static_cast<long>(z) };
                ImageType::SizeType extractSize = {
                    (m_patchSize[0]),
                    (m_patchSize[1]),
                    (m_patchSize[2])};
                ImageType::RegionType extractRegion;
                extractRegion.SetIndex(startIndex);
                extractRegion.SetSize(extractSize);

                auto extractFilter = ExtractFilterType::New();
                extractFilter->SetInput(inputImage);
                extractFilter->SetExtractionRegion(extractRegion);
                extractFilter->Update();
                auto patchImage = extractFilter->GetOutput();

                std::vector<float> patchData(m_patchSize[0] * m_patchSize[1] * m_patchSize[2]);
                itk::ImageRegionConstIterator<ImageType> it(
                    patchImage, patchImage->GetLargestPossibleRegion());
                size_t index = 0;
                for (it.GoToBegin(); !it.IsAtEnd(); ++it, ++index) {
                    patchData[index] = it.Get();
                }
                patches.push_back(patchData);
            }
        }
    }
    return patches;
}

std::vector<size_t> ONNXInference::getAdjustedStarts(size_t imgDim, size_t patchDim, size_t step) {
    std::vector<size_t> starts;
    if (imgDim <= patchDim) { starts.push_back(0); return starts; }
    size_t lastPossible = imgDim - patchDim;
    for (size_t pos = 0; pos <= lastPossible; pos += step) {
        starts.push_back(pos);
        if (pos + step > lastPossible && pos != lastPossible) {
            starts.push_back(lastPossible);
            break;
        }
    }
    return starts;
}


std::vector<float> ONNXInference::precomputeGaussianWeights(const std::array<int, 3>& patchSize) {
    const int sx = patchSize[0], sy = patchSize[1], sz = patchSize[2];
    std::vector<float> weights(sx * sy * sz);

    // 补丁中心
    float cx = (sx - 1) / 2.0f;
    float cy = (sy - 1) / 2.0f;
    float cz = (sz - 1) / 2.0f;

    // σ 根据经验设为patchDim/8，可按需微调
    float sigmaX = sx / 8.0f;
    float sigmaY = sy / 8.0f;
    float sigmaZ = sz / 8.0f;

    const float twoSigmaX2 = 2.0f * sigmaX * sigmaX;
    const float twoSigmaY2 = 2.0f * sigmaY * sigmaY;
    const float twoSigmaZ2 = 2.0f * sigmaZ * sigmaZ;

    // 先求未归一化权重及其和
    float sum = 0.0f;
    for (int z = 0; z < sz; ++z) {
        for (int y = 0; y < sy; ++y) {
            for (int x = 0; x < sx; ++x) {
                int idx = x + y * sx + z * sx * sy;
                float dx = x - cx;
                float dy = y - cy;
                float dz = z - cz;
                float val = std::exp(
                    -(dx * dx / twoSigmaX2 +
                      dy * dy / twoSigmaY2 +
                      dz * dz / twoSigmaZ2)
                    );
                weights[idx] = val;
                sum += val;
            }
        }
    }

    // 总和归一化
    if (sum > 0.0f) {
        for (auto& w : weights) {
            w /= sum;
        }
    }
    return weights;
}


void ONNXInference::onnxModelInference(const QString& filePath,const QString& outputPath)
{
    if (!QFileInfo(filePath).exists()) {
        qWarning() << "nii.gz file not exists:" << filePath;
        return;
    }

    /// 加载nii.gz文件
    this->loadNiftiFile(filePath);

    /// 运行onnx模型推理
    this->runInference(outputPath);
}

void ONNXInference::setProgressReporter(ProgressReporter* reporter)
{
    m_progressReporter = reporter;
}
