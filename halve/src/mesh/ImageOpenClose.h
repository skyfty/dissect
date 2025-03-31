#ifndef IMAGEOPENCLOSE_H
#define IMAGEOPENCLOSE_H
#include "vtkImageAlgorithm.h"

class DilateErode;

class ImageOpenClose : public vtkImageAlgorithm
{
public:

    vtkTypeMacro(ImageOpenClose, vtkImageAlgorithm);

    static ImageOpenClose* New();

    ///@{
    /**
   * Set/Get the Dilate and Erode values to be used by this filter.
   */

    void SetOpenValue(int value);
    int GetOpenValue();

    void SetCloseValue(int value);
    int GetCloseValue();

    /**
   * Selects the size of gaps or objects removed.
   */
    void SetKernelSize(int size0, int size1, int size2);

protected:
    ~ImageOpenClose();
    ImageOpenClose();

    // Description:
    // the main function that does the work
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int port, vtkInformation* info) override;

private:
    int KernelSize[3];
    int DilateValue;
    int ErodeValue;
    DilateErode* m_dilateErode;

private:
    ImageOpenClose(const ImageOpenClose&) = delete;
    void operator=(const ImageOpenClose&) = delete;


};

#endif // IMAGEOPENCLOSE_H
