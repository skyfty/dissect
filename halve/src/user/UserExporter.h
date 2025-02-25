#ifndef USEREXPORTER_H
#define USEREXPORTER_H

#include <QFutureWatcher>
#include <QObject>
class Exporter;

class UserExporter : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged FINAL)
    Q_PROPERTY(double progressValue READ progressValue NOTIFY progressValueChanged FINAL)

public:
    explicit UserExporter(QObject *parent = nullptr);
    ~UserExporter();
    Q_INVOKABLE void exportUsersToPath(const QStringList &newList, const QString& filePath);
    bool isRunning() const;
    double progressValue() const;

signals:
    void finished();
    void isRunningChanged();
    void progressValueChanged();

public slots:
    void onCopyFinised(const QString &srcPath, const QString &destPath);

private:
    QStringList m_list;
    qint32 m_progressValue = 0;
    QPointer<Exporter> m_exporter;
};

#endif // USEREXPORTER_H
