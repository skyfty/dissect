#ifndef EXPORTER_H
#define EXPORTER_H

#include <QObject>
#include "rep_exporter_merged.h"

class QProcess;

class Exporter : public QObject
{
    Q_OBJECT
public:
    explicit Exporter(QObject *parent = nullptr);
    ~Exporter();
    Q_INVOKABLE void launch();
    Q_INVOKABLE void cancel();
    Q_INVOKABLE void copy(const QString &srcPath, const QString &destPath);
signals:
    void finished(const QString &srcPath, const QString &destPath);

public slots:
    void onFinised();
    void onStarted();
    void onStateChanged();
    void onCopyFinised(const QString &srcPath, const QString &destPath);

private:
    QScopedPointer<QRemoteObjectNode> m_remoteNode;
    QScopedPointer<ExporterReplica> m_exporterReplica;
    QProcess *m_process = nullptr;
    QString m_host;
    QList<QPair<QString, QString>> m_list;
};

#endif // EXPORTER_H
