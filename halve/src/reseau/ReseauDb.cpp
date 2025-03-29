#include "qdir.h"
#include "qjsonarray.h"
#include "qjsonobject.h"
#include <QJsonDocument>
#include <QResource>
#include <vtkIdList.h>
#include <vtkVector.h>
#include "Reseau.h"
#include "ReseauDb.h"
#include <utility/IOWorker.h>
#include "utility/FileUtils.h"

using namespace std::placeholders;
ReseauDb::ReseauDb(const QString &path,QObject *parent)
    : QObject{parent} {
    Q_ASSERT(!path.isEmpty());

    m_reseauPath = getReseausStoragePath(path);
    QDir dir(m_reseauPath); if (!dir.exists(m_reseauPath))dir.mkdir(".");

    m_path = getStoragePath(path);
    if (QFile::exists(m_path)) {
        load();
    } else {
        m_reseaus.push_back(load(loadDefaultReseau()));
    }
}
QString ReseauDb::getReseausStoragePath(const QString &path) const {
    return path + "/reseau";

}

QString ReseauDb::getStoragePath(const QString &path) const
{
    return path + "/reseau.json";
}

QJsonObject ReseauDb::loadDefaultReseau() {
    auto res = QResource(":/assets/configures/reseau.json");
    return QJsonDocument::fromJson(res.uncompressedData().data()).object();
}

Reseau* ReseauDb::load(const QJsonObject &json) {
    Reseau* reseau = new Reseau(m_reseauPath, this);
    reseau->fromJson(json);
    connectSignals(reseau);
    return reseau;
}
void ReseauDb::load(const QByteArray &catheterJson) {
    for(auto json:QJsonDocument::fromJson(catheterJson.data()).array()) {
        m_reseaus.push_back(load(json.toObject()));
    }
}

void ReseauDb::load() {
    QFile f(m_path);
    if (f.open(QIODevice::ReadOnly)) {
        load(f.readAll());
    }
}

bool ReseauDb::save() {
    QFile saveFile(m_path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonArray reseauJson;
    for(Reseau* reseau:m_reseaus) {
        QJsonObject json;
        reseau->toJson(json);
        reseauJson.append(json);
    }
    return saveFile.write(QJsonDocument(reseauJson).toJson());
}

void ReseauDb::backup(const QString &path)
{
    QString backupReseauPath = getReseausStoragePath(path);
    QDir dir(backupReseauPath); if (!dir.exists(backupReseauPath)) dir.mkdir(".");
    QString reseauPath = QString("%1/*.*").arg(m_reseauPath);
    file_utils::copyDirectory(reseauPath, backupReseauPath);
    QString backupFile = getStoragePath(path);
    QFile::copy(m_path, backupFile);
}

void ReseauDb::connectSignals(Reseau *reseau) {
    QObject::connect(reseau, &Reseau::changed, this, std::bind(&ReseauDb::changed, this, reseau));
    QObject::connect(reseau, &Reseau::apparentChanged, this, std::bind(&ReseauDb::apparentChanged, this, reseau));
    QObject::connect(reseau, &Reseau::dyestuffChanged, this, std::bind(&ReseauDb::dyestuffChanged, this, reseau));
}

void ReseauDb::remove(Reseau* reseau) {
    QMutexLocker locker(&m_mutex);
    auto iter = std::find(std::begin(m_reseaus), std::end(m_reseaus), reseau);
    if (iter != std::end(m_reseaus)) {
        m_reseaus.erase(iter);
        reseau->clean();
        emit deleted(reseau);
    }
}

void ReseauDb::remove(int row) {
    if (row < m_reseaus.size()) {
        remove(m_reseaus[row]);
    }
}
void ReseauDb::clean() {
    QMutexLocker locker(&m_mutex);
    for(Reseau* reseau: m_reseaus) {
        reseau->clean();
    }
    save();
    emit cleaned();
}

Reseau *ReseauDb::add(qint64 id, const QString &name)
{
    Reseau *reseau = new Reseau(m_reseauPath, this);
    reseau->setId(id);
    reseau->setName(name);
    connectSignals(reseau);
    {
        QMutexLocker locker(&m_mutex);
        m_reseaus.push_front(reseau);
    }
    emit added(reseau);
    return reseau;
}

Reseau* ReseauDb::add(qint64 id, const QString& name, const QColor& color, vtkIdList* ids, vtkSmartPointer<vtkPolyData> polyData) {
    Reseau* reseau = new Reseau(m_reseauPath, this);
    reseau->setId(id);
    reseau->setName(name);
    reseau->setDyestuff(color);
    reseau->setPointIds(ids);
    reseau->setPolyData(polyData);
    connectSignals(reseau);
    {
        QMutexLocker locker(&m_mutex);
        m_reseaus.push_front(reseau);
    }
    emit added(reseau);
    return reseau;
}

int ReseauDb::size() const
{
    return m_reseaus.size();
}

Reseau* ReseauDb::getData(int row)
{
    QMutexLocker locker(&m_mutex);
    if (row<0 || row > m_reseaus.size()) {
        return nullptr;
    }
    return m_reseaus[row];
}

Reseau* ReseauDb::getData(qint64 id) {
    QMutexLocker locker(&m_mutex);
    for(int i = 0; i < m_reseaus.size(); ++i) {
        Reseau *reseau = m_reseaus[i];
        if (reseau->id() == id) {
            return reseau;
        }
    }
    return nullptr;
}


QList<Reseau *> ReseauDb::getDatas()
{
    return m_reseaus;
}

QList<Reseau *> ReseauDb::getApparentDatas()
{
    QMutexLocker locker(&m_mutex);
    QList<Reseau *> list;
    for(int i = 0; i < m_reseaus.size(); ++i) {
        Reseau *reseau = m_reseaus[i];
        if (reseau->apparent()) {
            list.push_back(reseau);
        }
    }
    return list;
}

int ReseauDb::index(const Reseau *reseau)
{
    QMutexLocker locker(&m_mutex);
    return m_reseaus.indexOf(reseau);
}

int ReseauDb::index(qint64 id)
{
    QMutexLocker locker(&m_mutex);
    for(int i = 0; i < m_reseaus.size(); ++i) {
        Reseau *reseau = m_reseaus[i];
        if (reseau->id() == id) {
            return i;
        }
    }
    return -1;
}
int ReseauDb::indexOfName(const QString &name) {
    QMutexLocker locker(&m_mutex);
    for(int i = 0; i < m_reseaus.size(); ++i) {
        Reseau *reseau = m_reseaus[i];
        if (reseau->getName() == name) {
            return i;
        }
    }
    return -1;
}
QList<QPair<Reseau*, double>> ReseauDb::getDatas(double x, double y, double z) {
    QMutexLocker locker(&m_mutex);
    QList<QPair<Reseau*, double>> result;
    for(int i = 0; i < m_reseaus.size(); ++i) {
        Reseau *reseau = m_reseaus[i];
        if (reseau->apparent()) {
            result.append(std::make_pair(reseau, reseau->checkPoint(x, y, z)));
        }
    }
    std::sort(result.begin(), result.end(), [](const auto &r, const auto &l){
        return std::abs(r.second) < std::abs(l.second);
    });
    return result;
}

QList<QPair<Reseau*, double>> ReseauDb::getDatas(double x[3]) {
    return getDatas(x[0],x[1],x[2]);
}

QList<QPair<Reseau*, double>> ReseauDb::getDatas(const vtkVector3d &pos) {
    return getDatas(pos.GetX(),pos.GetY(),pos.GetZ());
}
