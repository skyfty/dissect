#include <qjsonobject.h>
#include <QJsonDocument>
#include "AlbumDb.h"
#include <Windows.h>
#include "album/Picture.h"
#include "album/ScreenRecord.h"
#include "profile/Profile.h"
#include <vtkImageData.h>

#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QPixmap>
#include <QProcess>
#include <QScreen>
#include <QStandardPaths>
#include <vtkNew.h>
#include <vtkPNGWriter.h>

AlbumDb::AlbumDb(QString path,QObject *parent)
    : QObject{parent} {
    Q_ASSERT(!path.isEmpty());
    m_path = path + "/album.json";
    m_savePath = getPictureStoragePath(path);
    QDir dir(m_savePath);if (!dir.exists(m_savePath))dir.mkdir(".");
    load();
}

void AlbumDb::load() {
    QFile f(m_path);
    if (f.open(QIODevice::ReadOnly)) {
        QJsonArray meltJson = QJsonDocument::fromJson(f.readAll()).array();
        for(auto json:meltJson) {
            auto picJson = json.toObject();
            if (!picJson.contains("portrait")) {
                continue;
            }
            if (!exists(picJson["portrait"].toString())) {
                continue;
            }
            Picture* picture = new Picture(this);
            picture->fromJson(m_savePath, picJson);
            m_pictures.push_front(picture);
        }
    }
}

bool AlbumDb::save() {
    QFile saveFile(m_path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonArray json;
    for(const Picture* picture:m_pictures) {
        json.append(picture->toJson());
    }
    return saveFile.write(QJsonDocument(json).toJson());
}

void AlbumDb::ship(const QString &name, std::chrono::milliseconds value) {
    QString processName = QString::number(QDateTime::currentMSecsSinceEpoch());
    ScreenRecord *screenRecord = new ScreenRecord(this);
    screenRecord->setSavePath(m_savePath);
    QObject::connect(screenRecord, &ScreenRecord::finished, this, [this,processName,screenRecord, name, value](const QString &savePath, const QString &fileName){
        addPicture(savePath,fileName, Picture::Video);
        screenRecord->deleteLater();
        emit shipFinished(name, savePath, fileName);
    });
    screenRecord->start(processName, value);
}
qsizetype AlbumDb::size() const
{
    return m_pictures.size();
}
QList<Picture*> AlbumDb::getDatas() {
    return m_pictures;
}

Picture *AlbumDb::getData(int row)
{
    if (row > m_pictures.size()) {
        return nullptr;
    }
    return m_pictures[row];
}
Picture* AlbumDb::getData(const QString &portrait) {
    for(Picture* picture:m_pictures) {
        if (picture->portrait() == portrait) {
            return picture;
        }
    }
    return nullptr;
}

QString AlbumDb::screenshot() {
    QString fileName = QString("%1.png").arg(QDateTime::currentMSecsSinceEpoch());
    QString savePath = m_savePath + "/" + fileName;
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap screenshot = screen->grabWindow(0);
    screenshot.save(savePath);
    Picture* picture = addPicture(savePath,fileName, Picture::Image);
    emit added(picture);
    return savePath;
}

void AlbumDb::screenRecord(const QString &name) {
    ScreenRecord *screenRecord = nullptr;
    if (m_screenRecords.contains(name)) {
        screenRecord = m_screenRecords[name];
    } else {
        screenRecord = m_screenRecords[name] = new ScreenRecord(this);
    }
    screenRecord->setSavePath(m_savePath);
    QObject::connect(screenRecord, &ScreenRecord::finished, this, [this,processName = name](const QString &savePath, const QString &fileName){
        onScreenRecordFinised(processName, savePath, fileName);
    });
    screenRecord->start(name);
    emit screenRecordChanged(name);
}

void AlbumDb::onScreenRecordFinised(const QString &processName,const QString &savePath, const QString &filename) {
    if (m_screenRecords.contains(processName)) {
        m_screenRecords[processName]->deleteLater();
        m_screenRecords.remove(processName);
    }
    Picture* picture = addPicture(savePath,filename, Picture::Video);
    emit added(picture);
    emit screenRecordChanged(processName);
}

void AlbumDb::stopScreenRecord(const QString &name) {
    if (m_screenRecords.contains(name)) {
        m_screenRecords[name]->stop();
    }
}

 Picture* AlbumDb::addPicture(const QString &savePath, const QString &filename, int type) {
    Picture* picture = new Picture(this);
    picture->setFilePath(savePath);
    picture->setPortrait(filename);
    picture->setType((Picture::Type)type);
    picture->setCreateTime(QDateTime::currentDateTime());
    m_pictures.append(picture);
    save();
    return picture;
}

QString AlbumDb::savePath() const
{
    return m_savePath;
}

QString AlbumDb::getPictureStoragePath(const QString &path)
{
    return path + "/pictures";
}

bool AlbumDb::running(const QString &name) const
{
    return m_screenRecords.contains(name);
}

void AlbumDb::remove(Picture* picture) {
    Q_ASSERT(picture !=nullptr);
    int idx = m_pictures.indexOf(picture);
    if (idx != -1) {
        m_pictures.removeAt(idx);
        save();
        picture->deleteFile();
        picture->deleteLater();
        emit deleted(picture);
    }
}

void AlbumDb::remove(qsizetype row) {
    if (row < m_pictures.size()) {
        remove(m_pictures[row]);
    }
}
void AlbumDb::remove(const QString &path) {
    for(Picture* picture:m_pictures) {
        if (picture->portrait() == path) {
            remove(picture);
            break;
        }

    }
}
bool AlbumDb::exists(const QString &fileName) {
    return QFile::exists(m_savePath + "/" + fileName);
}
