#ifndef SCREENSHOTHELPER_H
#define SCREENSHOTHELPER_H

#include <QObject>
#include <QPointer>

class QProcess;

class ScreenRecord : public QObject
{
    Q_OBJECT
public:
    explicit ScreenRecord(QObject *parent = nullptr);
    ~ScreenRecord();
    void stop();
    void start(const QString &name);
    void start(const QString &name, std::chrono::milliseconds value);

    QString savePath() const;
    void setSavePath(const QString &newSavePath);

signals:
    void finished(const QString &savePath, const QString &filename);
    void savePathChanged();

private slots:
    void onScreenRecordFinised(const QString &savePath, const QString &filename);

private:
    void terminalProcess();

private:
    QPointer<QProcess> m_process;
    Qt::HANDLE m_event = nullptr;
    QString m_savePath;
};

#endif // SCREENSHOTHELPER_H
