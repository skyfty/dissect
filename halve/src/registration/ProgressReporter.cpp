#include "ProgressReporter.h"

QString ProgressReporter::progressText() const
{
	return m_progressText;
}

void ProgressReporter::setProgressText(const QString& progressText)
{
	if (m_progressText == progressText)
	{
		return;
	}
	m_progressText = progressText;
	emit progressTextChanged();
}

double ProgressReporter::progressValue() const
{
	return m_progressValue;
}

void ProgressReporter::setStepProgressValue(double stepProgressValue)
{
	m_progressValue = m_beginStep + stepProgressValue*m_stepWeight;
	if (m_progressValue>1.0)
	{
		m_progressValue = 1.0;
	}
	emit progressValueChanged();
}

void ProgressReporter::completeStep()
{
	m_beginStep += m_stepWeight;
	m_stepWeight = 0.0;
}

void ProgressReporter::setStepWeight(double stepWeight)
{
	m_stepWeight = stepWeight;
}

void ProgressReporter::complete()
{
	m_beginStep = 0.0;
	m_stepWeight = 1.0;
	setStepProgressValue(1.0);
}
