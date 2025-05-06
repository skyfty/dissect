#pragma once
#include <QObject>

static const char* kNiftiCacheName = "nifti_cache.nii.gz";

class CtDataStorage : public QObject{
	Q_OBJECT
public:
	Q_INVOKABLE void convertCtDataToNifti(const QString& dicomDir);
private:
	void convertDicomToNifti(const QString& dicomDir, const QString& niftiPath);
};