#ifndef EXPORTERHOST_H
#define EXPORTERHOST_H

#include <QObject>
#include "rep_exporter_merged.h"

class ExporterHost : public ExporterSimpleSource
{
    Q_OBJECT
public:
    explicit ExporterHost(QObject *parent = nullptr);
    void init();

signals:

    // ExporterSource interface
public slots:
    void copy(const QString &srcPath, const QString &destPath) override;
};


#endif // EXPORTERHOST_H
