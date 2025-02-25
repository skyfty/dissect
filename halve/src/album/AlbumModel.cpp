#include "profile/Profile.h"
#include "album/AlbumDb.h"
#include "Picture.h"
#include "AlbumModel.h"
#include "utility/ConnectCheck.h"

static const QString ScreenGrapName = "ScreenGrap";
static const QString ScreenRecordName ="ScreenRecord";

AlbumModel::AlbumModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void AlbumModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    m_albumDb = profile->albumDb();
    connect(m_albumDb, &AlbumDb::added, this, &AlbumModel::onAdded);
    connect(m_albumDb, &AlbumDb::screenRecordChanged, this, &AlbumModel::onScreenRecordChanged);
    emit profileChanged();
}

Profile *AlbumModel::profile() const {
    return m_profile;
}

void AlbumModel::onScreenRecordChanged(const QString &name) {
    if (ScreenGrapName == name) {
        emit screenGrapRunningChanged();
    } else {
        emit screenRecordRunningChanged();
    }
}

int AlbumModel::rowCount(const QModelIndex &parent) const
{
    return m_albumDb->size();
}

QVariant AlbumModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid()) {
        Picture *picture = m_albumDb->getData(index.row());
        if (picture != nullptr) {
            switch (role) {
            case Thumbnail:
                return picture->thumbnail();
            case Portrait:
                return picture->portrait();
            case Type:
                return picture->type();
            }
        }
    }
    return QVariant();

}
QString AlbumModel::getFilePath(const QString &fileName){
    QString filePath;
    Picture *picture = m_albumDb->getData(fileName);
    if (picture != nullptr) {
        filePath = picture->getFilePath();
    }
    return filePath;
}

QHash<int, QByteArray> AlbumModel::roleNames() const
{
    return {
            {Portrait, "portrait"},
            {Type, "type"},
            {Thumbnail, "thumbnail"},
            };
}

void AlbumModel::remove(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    m_albumDb->remove(row);
    endRemoveRows();
}

void AlbumModel::onAdded(const Picture* picture) {
    beginResetModel();
    endResetModel();
}

void AlbumModel::screenshot() {
    if (m_albumDb == nullptr) {
        return;
    }
    m_albumDb->screenshot();
}

void AlbumModel::startScreenGrap() {
    if (m_albumDb == nullptr) {
        return;
    }
    m_albumDb->screenRecord(ScreenGrapName);

}
void AlbumModel::stopScreenGrap() {
    if (m_albumDb == nullptr) {
        return;
    }
    m_albumDb->stopScreenRecord(ScreenGrapName);
}

bool AlbumModel::screenGrapRunning() const
{
    if (m_albumDb == nullptr) {
        return false;
    }
    return m_albumDb->running(ScreenGrapName);
}

void AlbumModel::startScreenRecord() {
    if (m_albumDb == nullptr) {
        return;
    }
    m_albumDb->screenRecord(ScreenRecordName);
}
void AlbumModel::stopScreenRecord() {
    if (m_albumDb == nullptr) {
        return;
    }
    m_albumDb->stopScreenRecord(ScreenRecordName);
}
bool AlbumModel::screenRecordRunning() const {
    if (m_albumDb == nullptr) {
        return false;
    }
    return m_albumDb->running(ScreenRecordName);
}
