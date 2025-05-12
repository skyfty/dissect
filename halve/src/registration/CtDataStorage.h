#pragma once
#include <QObject>
#include <QString>
#include "ProgressReporter.h"
#include <QPointer>

static const char* kNiftiCacheName = "nifti_cache.nii.gz";

class ProgressReporter;
class CtDataStorage : public QObject{
	Q_OBJECT;
	Q_PROPERTY(ProgressReporter* progressReporter READ progressReporter WRITE setProgressReporter);
public:
	Q_INVOKABLE bool convertCtDataToNifti(const QString& dicomDir);
	void setProgressReporter(ProgressReporter* reporter);
	ProgressReporter* progressReporter() const;
private:
	bool convertDicomToNifti(const QString& dicomDir, const QString& niftiPath);
private:
	QPointer<ProgressReporter> m_progressReporter;
};