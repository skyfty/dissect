#ifndef CHANNELWATCHWINDOW_H
#define CHANNELWATCHWINDOW_H

#include <QDateTime>
#include <QMainWindow>
#include <qremoteobjectnode.h>

class ChannelReplica;
class ChannelTrackData;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class ChannelWatchWindow : public QMainWindow
{
    Q_OBJECT
public:
    static const QString &windowTitle;

public:
    explicit ChannelWatchWindow(QWidget *parent = nullptr);
    ~ChannelWatchWindow();

private slots:
    void on_actionAlway_on_top_triggered();
    void on_refreshIntervelChanged(QAction *);
    void onChannelTrackData(const ChannelTrackData &datas);


private:
    Ui::MainWindow *ui;
    qint64 m_duration = 0;
    QDateTime m_lastTime;;
    QScopedPointer<QRemoteObjectNode> m_remoteNode;
    QScopedPointer<ChannelReplica> m_channelReplica;
};

#endif // CHANNELWATCHWINDOW_H
