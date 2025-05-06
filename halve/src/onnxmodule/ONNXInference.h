#ifndef ONNXINFERENCE_H
#define ONNXINFERENCE_H

#pragma once
#include <QObject>
#include <QVector>
#include <onnxruntime_cxx_api.h>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QDir>

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImage.h>
#include <itkNiftiImageIOFactory.h>
#include <itkResampleImageFilter.h>
#include <itkIdentityTransform.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkConstantPadImageFilter.h>
#include <itkExtractImageFilter.h>
#include <json.hpp>
#include <fstream>
#include <iostream>


// 使用ITK的图像类型定义
using PixelType = float;
using ImageType = itk::Image<PixelType, 3>;
using ImagePointer = ImageType::Pointer;
using MaskType = itk::Image<unsigned char, 3>;
using MaskPointer = MaskType::Pointer;

/**
 * model路径为exe同路径下的   /onnxmodel/model.onnx
 * cfg  路径为exe同路径下的   /onnxmodel/model.json
*/



class ONNXInference : public QObject
{

    Q_OBJECT

public:
    explicit ONNXInference(QObject * parent = nullptr);

    Q_INVOKABLE void loadConfig(const QString & configFile);
    Q_INVOKABLE void loadModel(const QString &modelPath);
    Q_INVOKABLE void loadNiftiFile(const QString& filePath);
    Q_INVOKABLE void runInference(const QString& outputPath);

    Q_INVOKABLE void onnxModelInference(const QString& filePath,const QString& outputPath);

private:
    MaskPointer processLargeImage (const ImagePointer& inputImage,
                                   std::function<std::vector<float>(const std::vector<float>&)> inferenceFunc);

    // 图像重采样到目标间距
    ImagePointer resampleToTargetSpacing(const ImagePointer& inputImage);

    // 图像裁剪或填充到正确的大小
    ImagePointer cropOrPadToCorrectSize(const ImagePointer& inputImage);

    // 图像强度归一化
    ImagePointer normalizeIntensity(const ImagePointer& inputImage);

    // 创建滑动窗口补丁（如果需要）
    std::vector<std::vector<float>> createSlidingWindowPatches(const ImagePointer& inputImage);

    std::vector<size_t> getAdjustedStarts(size_t imgDim, size_t patchDim, size_t step);

    std::vector<float> precomputeGaussianWeights(const std::array<int, 3>& patchSize);

private:

    std::vector<const char*> m_inputNames;
    std::vector<const char*> m_outputNames;


    float m_overlapFactor = 0.5f;    // 默认值设为典型重叠比例

    std::string m_normalizationScheme;
    std::array<double, 3> m_targetSpacing;
    std::array<int, 3> m_patchSize;
    int m_numInputChannels;
    int m_numClasses;
    std::vector<std::string> m_classNames;
    nlohmann::json m_intensityProperties;


    ImagePointer m_inputImage;

    Ort::Env env{ ORT_LOGGING_LEVEL_WARNING, "QtONNX" };
    Ort::Session session{nullptr};

    size_t m_numOutputNodes;

};

#endif // ONNXINFERENCE_H
