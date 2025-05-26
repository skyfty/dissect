#include "ProgressCommand.h"
#include <vtkObjectFactory.h>
#include "ProgressReporter.h"

vtkStandardNewMacro(ProgressCommand);

void ProgressCommand::setProgressReporter(ProgressReporter* reporter)
{
    m_progressReporter = reporter;
}

void ProgressCommand::Execute(vtkObject* caller, unsigned long eventId, void* callData) {
    if (eventId == vtkCommand::ProgressEvent && m_progressReporter) {
        double value = *static_cast<double*>(callData);
        m_progressReporter->setStepProgressValue(value);
    }
}
