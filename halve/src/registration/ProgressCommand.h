#pragma once
#include <vtkCommand.h>
#include <vtkSetGet.h>
#include "ProgressReporter.h"
#include <QPointer>

class ProgressCommand : public vtkCommand
{
public:
	static ProgressCommand* New();
	vtkTypeMacro(ProgressCommand, vtkCommand);
	void setProgressReporter(ProgressReporter* reporter);
public:
    void Execute(vtkObject* caller, unsigned long eventId, void* callData) override;
private:
	QPointer<ProgressReporter> m_progressReporter{ nullptr };
};
