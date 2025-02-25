#include "vtkm_worklet.h"

#include <QTest>
#include <vector>



class TestVtkm: public QObject
{
    Q_OBJECT
private slots:
    void test1();
};

//struct ggg {
//    template <typename T, typename U>
//    inline VTKM_EXEC_CONT auto operator()(const T& a, const U& b) const -> decltype(a + b)
//    {
//        return a + b;
//    }
//};
//// Define a worklet class for squaring the values in an array


void TestVtkm::test1()
{
//    vtkNew<vtkmContour > contour;
//    contour->SetInputData(imageData);
//    contour->ComputeNormalsOff();
//    contour->ComputeScalarsOff();
//    contour->ComputeGradientsOff();
//    contour->SetNumberOfContours(1);
//    contour->SetValue(0, 255.0);
//    contour->Update();

//    vtkm::io::VTKDataSetReader reader("D:\\vtk-examples\\src\\Testing\\Data\\kitchen.vtk");
//    vtkm::cont::DataSet ds_from_file = reader.ReadDataSet();
//    for (;;)
//    {
//        vtkm::filter::contour::Contour contour;
//        contour.SetActiveField("c1");
//        contour.SetNumberOfIsoValues(3);
//        contour.SetIsoValue(0, 0.05);
//        contour.SetIsoValue(1, 0.10);
//        contour.SetIsoValue(2, 0.15);

//        vtkm::cont::DataSet ds_from_contour = contour.Execute(ds_from_file);
//    }
//    {
//        std::fill(inputArray.begin(), inputArray.end(), 12);
//        std::transform(inputArray.begin(), inputArray.end(), outputArray.begin(), [](int a){
//            return a*a;

//        });
//    }

    std::vector<vtkm::Vec3f> outputArray;
    outputArray.resize(100000);

    std::vector<vtkm::Vec3f> inputArray;
    inputArray.resize(100000, 200);
    auto i = QDateTime::currentMSecsSinceEpoch();

//            std::transform(inputArray.begin(), inputArray.end(), outputArray.begin(), [](auto i){
//                return i * i;
//            });
    for(int i = 0; i < 50000; ++i) {

//        std::transform(inputArray.begin(), inputArray.end(), outputArray.begin(), [](auto input1){
//            return 0.333 * (input1 + input1 * 3 + input1 * 4) - input1 * 5;

//        });

        vtkm_worklet worklet;
        worklet.test(inputArray, outputArray);
    }



    qDebug() << QDateTime::currentMSecsSinceEpoch() - i;


    int a = 23;
//    {
//        vtkm::cont::ArrayHandle<int> outputArrayHandle = vtkm::cont::make_ArrayHandle(outputArray, vtkm::CopyFlag::Off);
//        vtkm::cont::ArrayHandle<int> inputArrayHandle = vtkm::cont::make_ArrayHandle(inputArray, vtkm::CopyFlag::Off);
////        vtkm::cont::Algorithm::Fill(inputArrayHandle, 12);
////        vtkm::cont::Invoker invoke;
//        efffff();
////        invoke(ss, inputArrayHandle, outputArrayHandle);
//    }




//


//    auto readPortal = outputArrayHandle.ReadPortal();
//    std::vector<int> vec(readPortal.GetNumberOfValues());
//    for (vtkm::Id i = 0; i < readPortal.GetNumberOfValues(); i++)
//    {
//        vec[i] = readPortal.Get(i);
//    }
//    vtkm::cont::ArrayCopy(outputArray, outputVector);

}
QTEST_MAIN(TestVtkm)
#include "tst_vtkm.moc"
