#include "NotebookDataSource.h"
#include "NotebookFile.h"
#include "NotebookMapping.h"
#include "album/AlbumDb.h"
#include "casket/CasketDb.h"
#include "catheter/CatheterDb.h"
#include "channel/Channel.h"
#include "dissolve/DissolveDb.h"
#include "mapping/MappingPointGroup.h"
#include "mapping/MappingPointGroupDb.h"
#include "mapping/MappingPointsDb.h"
#include "melt/MeltChannelDb.h"
#include "melt/MeltDb.h"
#include "melt/MeltOptionsDb.h"
#include "notebook/NotebookDb.h"
#include "notebook/NotebookScreenshot.h"
#include "notebook/NotebookSnapshot.h"
#include "profile/Profile.h"
#include "profile/ProfileManager.h"
#include "reseau/ReseauDb.h"
#include "study/scalar/ScalarDb.h"
#include "utility/ConnectCheck.h"
#include <QtConcurrent>

#include <album/Picture.h>

#include <QTimer>

NotebookDataSource::NotebookDataSource(QObject *parent)
    : QObject{parent}
{}

void NotebookDataSource::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;

    m_notebookDb = profile->notebookDb();
    QObject::connect(m_notebookDb, &NotebookDb::deleted, this, &NotebookDataSource::onNotebookDeleted);

    m_mappingPointGroupDb = profile->mappingPointGroupDb();
    QObject::connect(m_mappingPointGroupDb, &MappingPointGroupDb::added, this, &NotebookDataSource::onMappingPointGroupAdded);
    QObject::connect(m_mappingPointGroupDb, &MappingPointGroupDb::deleted, this, &NotebookDataSource::onMappingPointGroupDeleted);
    QObject::connect(m_mappingPointGroupDb, &MappingPointGroupDb::changed, this, &NotebookDataSource::onMappingPointGroupChanged);

    m_albumDb = profile->albumDb();
    QObject::connect(m_albumDb, &AlbumDb::shipFinished, this, &NotebookDataSource::onShipFinished);
    QObject::connect(m_albumDb, &AlbumDb::added, this, &NotebookDataSource::onAlbumAdded);
    QObject::connect(m_albumDb, &AlbumDb::deleted, this, &NotebookDataSource::onAlbumDeleted);

    emit profileChanged();
}

Profile *NotebookDataSource::profile() const {
    return m_profile;
}
void NotebookDataSource::setChannel(Channel *newChannel) {
    m_channel = newChannel;
}

void NotebookDataSource::onMappingPointGroupAdded(MappingPointGroup *group) {
    Q_ASSERT(group != nullptr);
    m_notebookDb->addMappingGroup(group->name(), group->id());
    m_notebookDb->save();
}

std::pair<qint32, NotebookMapping*> NotebookDataSource::getNotebookMapping(MappingPointGroup *group) {
    Q_ASSERT(group != nullptr);
    QList<Notebook*> notebooks = m_notebookDb->getNotebooks();
    for(qsizetype i = 0; i < notebooks.size(); ++i) {
        if (notebooks[i]->type() == Halve::MAPPING) {
            NotebookMapping* notebook=static_cast<NotebookMapping*>(notebooks[i]);
            if (notebook->mappingPointGroup() == group->id()) {
                return std::make_pair(i, notebook);
            }
        }
    }
    return std::make_pair(-1, nullptr);
}


void NotebookDataSource::onMappingPointGroupDeleted(MappingPointGroup *group) {
    Q_ASSERT(group != nullptr);
    auto [idx,notebook] = getNotebookMapping(group);
    if (idx != -1) {
        m_notebookDb->removeNotebook(notebook);
        m_notebookDb->save();
    }
}

void NotebookDataSource::onMappingPointGroupChanged(MappingPointGroup *group) {
    Q_ASSERT(group != nullptr);
    auto [idx,notebook] = getNotebookMapping(group);
    if (idx != -1) {
        notebook->setName(group->name());
        m_notebookDb->save();
    }
}

void NotebookDataSource::onAlbumAdded(const Picture *picture) {
    Q_ASSERT(picture != nullptr);
    Q_ASSERT(m_notebookDb != nullptr);
    QString nodeName = getNotebookFileName();
    if (picture->type() ==Picture::Image) {
        m_notebookDb->addScreenShot(nodeName, picture->portrait());
    } else {
        m_notebookDb->addScreenRecord(nodeName, picture->portrait());
    }
    m_notebookDb->save();
}

void NotebookDataSource::onAlbumDeleted(const Picture *picture){
    Q_ASSERT(m_notebookDb != nullptr);
    QList<Notebook*> notebooks = m_notebookDb->getNotebooks();
    for(qsizetype i = 0; i < notebooks.size(); ++i) {
        auto type = notebooks[i]->type();
        if (type == Halve::AUTORECORD || type == Halve::SCREENSHOT || type == Halve::SCREENRECORD) {
            NotebookFile* notebook=static_cast<NotebookFile*>(notebooks[i]);
            if (notebook->fileName() == picture->portrait()) {
                m_notebookDb->removeNotebook(notebook);
                m_notebookDb->save();
                break;
            }
        }
    }
}

void NotebookDataSource::takeSnapshot(NotebookSnapshot *snapshot) {
    QString snapshotPath = snapshot->snapshotPath();
    m_profile->backup(snapshotPath);
    m_profile->dissolveDb()->backup(snapshotPath);
    m_profile->casketDb()->backup(snapshotPath);
    m_profile->catheterDb()->backup(snapshotPath);
    m_profile->mappingPointGroupDb()->backup(snapshotPath);
    m_profile->mappingPointsDb()->backup(snapshotPath);
    m_profile->meltChannelDb()->backup(snapshotPath);
    m_profile->meltDb()->backup(snapshotPath);
    m_profile->meltOptionsDb()->backup(snapshotPath);
    m_profile->reseauDb()->backup(snapshotPath);
    m_profile->scalarDb()->backup(snapshotPath);
}

qint32 NotebookDataSource::autoRecordDuration() const
{
    return m_autoRecordDuration;
}

void NotebookDataSource::setAutoRecordDuration(qint32 newAutoRecordDuration)
{
    if (m_autoRecordDuration == newAutoRecordDuration)
        return;
    m_autoRecordDuration = newAutoRecordDuration;
    emit autoRecordDurationChanged();
}

bool NotebookDataSource::snapping() const
{
    return m_snapping;
}

void NotebookDataSource::setSnapping(bool newSnapping)
{
    if (m_snapping == newSnapping)
        return;
    m_snapping = newSnapping;
    emit snappingChanged();
}

void NotebookDataSource::beginSnapshot(qint64 beginTime) {
    m_snapshotBeginTime = beginTime;
    setSnapping(m_snapshotBeginTime != 0);
}

void NotebookDataSource::snapshot(qint64 endTime) {
    QString snapshotName = QString("Snapshot# %1").arg(m_profile->increaseSnapshotId());
    NotebookSnapshot *snapshot = m_notebookDb->addSnapshot(snapshotName, m_snapshotBeginTime, endTime);
    takeSnapshot(snapshot);
    m_notebookDb->save();
    m_snapshotBeginTime = 0;
    setSnapping(m_snapshotBeginTime != 0);
}

bool NotebookDataSource::autoRecordScreen() const
{
    return m_autoRecordScreen;
}

void NotebookDataSource::setAutoRecordScreen(bool newAutoRecordScreen)
{
    if (m_autoRecordScreen == newAutoRecordScreen)
        return;
    m_autoRecordScreen = newAutoRecordScreen;
    emit autoRecordScreenChanged();
}

void NotebookDataSource::autoRecord(const QString &name) {
    if (m_autoRecordScreen) {
        m_profile->albumDb()->ship(name, std::chrono::seconds(m_autoRecordDuration + 1));
    }
}

void NotebookDataSource::onShipFinished(const QString &name, const QString &savePath, const QString &fileName) {
    m_notebookDb->addAutoRecord(name, fileName);
    m_notebookDb->save();
}


void NotebookDataSource::open(qint64 id) {
    Notebook *notebook = m_notebookDb->getData(id);
    if (notebook == nullptr) {
        return;
    }
    switch(notebook->type()) {
    case Halve::MAPPING: {
        NotebookMapping* mapping = dynamic_cast<NotebookMapping*>(notebook);
        if (mapping != nullptr) {
            m_profile->setCurrentMappingPointGroup(mapping->mappingPointGroup());
        }
        break;
    }
    case Halve::AUTORECORD:
    case Halve::SCREENRECORD: {
        NotebookFile* notebookFile = dynamic_cast<NotebookFile*>(notebook);
        emit m_profile->playVideo(notebookFile->fileName());
        break;
    }
    case Halve::SCREENSHOT: {
        NotebookScreenshot* screenshot = dynamic_cast<NotebookScreenshot*>(notebook);
        emit m_profile->viewPicture(screenshot->fileName());
        break;
    }
    case Halve::SNAPSHOT: {
        NotebookSnapshot* snapshot = dynamic_cast<NotebookSnapshot*>(notebook);
        if (snapshot != nullptr) {
            emit m_profile->openSnapshot(snapshot);
        }
        break;
    }
    default: {
        break;
    }
    }
}

QString NotebookDataSource::getNotebookFileName() {
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}
void NotebookDataSource::onNotebookDeleted(Notebook *notebook) {
    switch(notebook->type()) {
    case Halve::AUTORECORD:
    case Halve::SCREENSHOT:
    case Halve::SCREENRECORD: {
        NotebookFile* notebookFile = dynamic_cast<NotebookFile*>(notebook);
        m_albumDb->remove(notebookFile->fileName());
        break;
    }
    }
}
