#include "ChannelWatchWindow.h"

#include "channel/ChannelTrackDataElectricalTableModel.h"
#include "channel/ChannelTrackDataMagneticTableModel.h"
#include "rep_channel_merged.h"

#include <QSplitter>
#include <QTableWidget>
#include <QVBoxLayout>
#include "utility/VtkUtil.h"

#include <src/channel/ui_MainWindow.h>

#include <QActionGroup>
#include <vtkQuaternion.h>

const QString & ChannelWatchWindow::windowTitle("Channel Watch");

ChannelWatchWindow::ChannelWatchWindow(QWidget *parent)
    : QMainWindow{parent}
    , ui(new Ui::MainWindow)
{
    m_lastTime = QDateTime::currentDateTime();
    ui->setupUi(this);
    setWindowTitle(windowTitle);
    m_remoteNode.reset(new QRemoteObjectNode()); // create remote object node
    m_remoteNode->connectToNode(QUrl(QStringLiteral("local:channel"))); // connect with remote host node
    m_remoteNode->setHeartbeatInterval(1000);
    m_channelReplica.reset(m_remoteNode->acquire<ChannelReplica>()); // acquire replica of source from host node
    QObject::connect(m_channelReplica.data(), &ChannelReplica::trackData, this, &ChannelWatchWindow::onChannelTrackData);
    ui->splitter->setStretchFactor(0, 2);
    ui->splitter->setStretchFactor(1, 4);

    ui->electricalTable->setModel(new ChannelTrackDataElectricalTableModel(this));
    ui->magneticTable->setModel(new ChannelTrackDataMagneticTableModel(this));

    QActionGroup *refreshIntervalGroup = new QActionGroup(this);
    refreshIntervalGroup->addAction(ui->action1ms);
    refreshIntervalGroup->addAction(ui->action10ms);
    refreshIntervalGroup->addAction(ui->action100ms);
    refreshIntervalGroup->addAction(ui->action500ms);
    refreshIntervalGroup->addAction(ui->action1m);
    refreshIntervalGroup->addAction(ui->action3m);
    refreshIntervalGroup->addAction(ui->action5m);
    QObject::connect(refreshIntervalGroup, &QActionGroup::triggered, this, &ChannelWatchWindow::on_refreshIntervelChanged);
}

ChannelWatchWindow::~ChannelWatchWindow()
{
    delete ui;
}


void ChannelWatchWindow::onChannelTrackData(const ChannelTrackData &datas) {
    auto lastTime = QDateTime::currentDateTime();
    if (m_duration != 0 && m_lastTime.msecsTo(lastTime) < m_duration) {
        return;
    }
    m_lastTime = lastTime;

    auto coefficient = ui->actionCoefficient->isChecked() ? DefaultCoefficient : 1;
    {
        QStandardItemModel *model = static_cast<QStandardItemModel*>(ui->electricalTable->model());
        for (int i = 0; i < ElectricalPortAmount; ++i) {
            for(int j = 0; j < 3; j++) {
                model->item(i, j)->setText(QString::number(datas.m[i].pos[j] / coefficient));
            }
        }
    }
    {
        QStandardItemModel *model = static_cast<QStandardItemModel*>(ui->magneticTable->model());
        for (int i = 0; i < MagnetismPortAmount; ++i) {
            model->item(i, 0)->setText(QString::number(datas.n[i].status, 16));
            for(int j = 1; j < 4; j++) {
                model->item(i, j)->setText(QString::number(datas.n[i].pos[j -1]));
            }
            auto nq = datas.n[i].quaternion;
            double matrix[3][3];
            vtkQuaterniond(nq[3],nq[0],nq[1],nq[2]).ToMatrix3x3(matrix);
            vtkVector3d euler = vtkutil::determineEuler(matrix);
            for(int j = 4; j < 7; j++) {
                model->item(i, j)->setText(QString::number(euler[j - 4]));
            }
        }
    }
}

void ChannelWatchWindow::on_actionAlway_on_top_triggered()
{
    Qt::WindowFlags flags = windowFlags();
    if (ui->actionAlway_on_top->isChecked()) {
        setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(flags & ~Qt::WindowStaysOnTopHint);
    }
    show();
}

void ChannelWatchWindow::on_refreshIntervelChanged(QAction *action) {
    if (action == ui->action10ms) {
        m_duration = 10;
    } else if (action == ui->action100ms) {
        m_duration = 100;
    } else if (action == ui->action500ms) {
        m_duration = 500;
    } else if (action == ui->action1m) {
        m_duration = 1000;
    } else if (action == ui->action3m) {
        m_duration = 3000;
    } else if (action == ui->action5m) {
        m_duration = 5000;
    } else {
        m_duration = 0;
    }
}

