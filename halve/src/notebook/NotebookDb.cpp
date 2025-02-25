#include "NotebookDb.h"

#include "Notebook.h"
#include "album/AlbumDb.h"
#include "notebook/NotebookAutoRecord.h"
#include "notebook/NotebookMapping.h"
#include "notebook/NotebookScreenRecord.h"
#include "notebook/NotebookScreenshot.h"
#include "notebook/NotebookSnapshot.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QResource>
#include "utility/FileUtils.h"

NotebookDb::NotebookDb(const QString &path,AlbumDb *albumDb,QObject *parent)
    : QObject{parent},m_albumDb(albumDb)
{
    m_idCounter = QDateTime::currentMSecsSinceEpoch();
    m_notebookPath = getNotebookStoragePath(path);
    QDir dir(m_notebookPath); if (!dir.exists(m_notebookPath))dir.mkdir(".");

    m_path = getStoragePath(path);
    if (QFile::exists(m_path)) {
        load();
    } else {
        initDefault();
    }
}


QString NotebookDb::getStoragePath(const QString &path) const
{
    return path + "/notebook.json";
}
QString NotebookDb::getNotebookStoragePath(const QString &path) const {
    return path + "/notebook";
}

void NotebookDb::initDefault() {
    load(QResource(":/assets/configures/notebook.json").uncompressedData());
}

void NotebookDb::backup(const QString &path)
{
    QString backupNotebookPath = getNotebookStoragePath(path);
    QDir dir(backupNotebookPath); if (!dir.exists(backupNotebookPath)) dir.mkdir(".");
    QString reseauPath = QString("%1/*.*").arg(m_notebookPath);
    file_utils::copyDirectory(reseauPath, backupNotebookPath);
    QString backupFile = getStoragePath(path);
    QFile::copy(m_path, backupFile);
}
bool NotebookDb::albumFileExists(const QJsonObject &json) {
    if (!json.contains("fileName")) {
        return false;
    }
    return m_albumDb->exists(json["fileName"].toString());
}

void NotebookDb::load(const QByteArray &catheterJson) {
    for(auto jsonArray:QJsonDocument::fromJson(catheterJson.data()).array()) {
        QJsonObject json = jsonArray.toObject();
        auto type = (Halve::NotebookType)json["type"].toInt();
        Notebook *notebook = nullptr;
        switch(type) {
        case Halve::SNAPSHOT: {
            notebook = new NotebookSnapshot(m_notebookPath, this);
            break;
        }
        case Halve::AUTORECORD: {
            if (albumFileExists(json)) {
                notebook = new NotebookAutoRecord(m_notebookPath, this);
            }
            break;
        }
        case Halve::SCREENSHOT: {
            if (albumFileExists(json)) {
                notebook = new NotebookScreenshot(m_notebookPath, this);
            }
            break;
        }
        case Halve::SCREENRECORD: {
            if (albumFileExists(json)) {
                notebook = new NotebookScreenRecord(m_notebookPath, this);
            }
            break;
        }
        case Halve::MAPPING: {
            notebook = new NotebookMapping(m_notebookPath, this);
            break;
        }
        default: {
            Q_ASSERT(false);
        }
        }
        if (notebook != nullptr) {
            notebook->fromJson(json);
            m_notebooks.push_back(notebook);
            connectSignals(notebook);
        }
    }
}

void NotebookDb::load() {
    QFile f(m_path);
    if (f.open(QIODevice::ReadOnly)) {
        load(f.readAll());
    }
}

bool NotebookDb::save() {
    QFile saveFile(m_path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonArray noteJson;
    for(Notebook* notebook:m_notebooks) {
        QJsonObject json;
        notebook->toJson(json);
        noteJson.append(json);
    }
    return saveFile.write(QJsonDocument(noteJson).toJson());
}

int NotebookDb::index(const Notebook *notebook) const
{
    return m_notebooks.indexOf(notebook);
}

int NotebookDb::index(qint64 id) const
{
    for(int i = 0; i < m_notebooks.size(); ++i) {
        Notebook *notebook = m_notebooks[i];
        if (notebook->id() == id) {
            return i;
        }
    }
    return -1;
}
int NotebookDb::indexOfName(const QString &name) const {
    for(int i = 0; i < m_notebooks.size(); ++i) {
        Notebook *notebook = m_notebooks[i];
        if (notebook->name() == name) {
            return i;
        }
    }
    return -1;
}

int NotebookDb::size() const
{
    return m_notebooks.size();
}

Notebook* NotebookDb::getData(int row) const
{
    if (row<0 || row > m_notebooks.size()) {
        return nullptr;
    }
    return m_notebooks[row];
}
Notebook* NotebookDb::getData(qint64 id) const {
    for(int i = 0; i < m_notebooks.size(); ++i) {
        Notebook *notebook = m_notebooks[i];
        if (notebook->id() == id) {
            return notebook;
        }
    }
    return nullptr;
}

void NotebookDb::connectSignals(Notebook *notebook) {
    QObject::connect(notebook, &Notebook::nameChanged, this, std::bind(&NotebookDb::changed, this, notebook));
}

Notebook* NotebookDb::addNotebook(Notebook* notebook) {
    notebook->setId(m_idCounter++);
    notebook->setCreateTime(QDateTime::currentDateTime());
    m_notebooks.push_front(notebook);
    connectSignals(notebook);
    emit added(notebook);
    return notebook;
}
Notebook* NotebookDb::addFileNotebook(NotebookFile* notebook,const QString& fileName) {
    notebook->setFileName(fileName);
    return addNotebook(notebook);
}
NotebookSnapshot* NotebookDb::addSnapshot(const QString& name,qint64 beginTime, qint64 endTime) {
    NotebookSnapshot* snapshot = new NotebookSnapshot(m_notebookPath, this);
    snapshot->setName(name);
    snapshot->setBeginTime(beginTime);
    snapshot->setEndTime(endTime);
    addNotebook(snapshot);
    return snapshot;
}

NotebookAutoRecord* NotebookDb::addAutoRecord(const QString& name,const QString& fileName) {
    NotebookAutoRecord* autoRecord = new NotebookAutoRecord(m_notebookPath, this);
    autoRecord->setName(name);
    addFileNotebook(autoRecord, fileName);
    return autoRecord;
}
NotebookMapping* NotebookDb::addMappingGroup(const QString& name,const QString& groupId){
    NotebookMapping* mappingGroup = new NotebookMapping(m_notebookPath, this);
    mappingGroup->setName(name);
    mappingGroup->setMappingPointGroup(groupId);
    addNotebook(mappingGroup);
    return mappingGroup;
}
NotebookScreenRecord* NotebookDb::addScreenRecord(const QString& name,const QString& fileName){
    NotebookScreenRecord* autoRecord = new NotebookScreenRecord(m_notebookPath, this);
    autoRecord->setName(name);
    addFileNotebook(autoRecord, fileName);
    return autoRecord;
}
NotebookScreenshot* NotebookDb::addScreenShot(const QString& name,const QString& fileName){
    NotebookScreenshot* autoRecord = new NotebookScreenshot(m_notebookPath, this);
    autoRecord->setName(name);
    addFileNotebook(autoRecord, fileName);
    return autoRecord;
}
void NotebookDb::removeNotebook(qint64 id) {
    for(Notebook *notebook:m_notebooks) {
        if (notebook->id() == id) {
            removeNotebook(notebook);
            break;
        }
    }
}

void NotebookDb::removeNotebook(Notebook *mesh){
    removeNotebookOnce(mesh);
    emit deleted(mesh);
}
void NotebookDb::removeNotebookOnce(Notebook* notebook) {
    m_notebooks.removeOne(notebook);
    notebook->clean();
}

void NotebookDb::removeNotebook(qint32 idx)
{
    Q_ASSERT(idx >= 0 && idx < m_notebooks.size());
    removeNotebook(m_notebooks[idx]);
}

QList<Notebook *> NotebookDb::getNotebooks()
{
    return m_notebooks;
}

Notebook *NotebookDb::getNotebook(qint64 id)
{
    for(Notebook *notebook:m_notebooks) {
        if (notebook->id() == id) {
            return notebook;
        }
    }
    return nullptr;
}

Notebook *NotebookDb::getNotebook(int idx)
{
    return m_notebooks[idx];
}

Notebook *NotebookDb::getNotebook(const QString &name)
{
    for(Notebook *notebook:m_notebooks) {
        if (notebook->name() == name) {
            return notebook;
        }
    }
    return nullptr;
}
QList<Notebook*> NotebookDb::getNotebooks(Halve::NotebookType type) {
    QList<Notebook*> list;
    for(Notebook *notebook:m_notebooks) {
        if (notebook->type() == type) {
            list.append(notebook);
        }
    }
    return list;
}
