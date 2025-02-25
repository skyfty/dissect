#include "CatheterRepertory.h"
#include "catheter/CatheterDb.h"
#include "catheter/CatheterTempletDb.h"

#include <QFile>
#include <QJsonDocument>
#include <QResource>

CatheterRepertory::CatheterRepertory(const QString& path,QObject *parent)
    : QObject{parent},m_path(path)
{
    QString filePath = m_path + "/catheter.json";
    if (m_path.isEmpty() || !QFile::exists(filePath)) {
        initDefault(filePath);
    }
    m_catheterDb = new CatheterDb(m_path, this);
}

void CatheterRepertory::initDefault(const QString& filePath) {
    auto data = QResource(":/assets/configures/catheter.json").uncompressedData();
    QFile saveFile(filePath);
    if (saveFile.open(QIODevice::WriteOnly)) {
        saveFile.write(data);
    }
}

CatheterDb *CatheterRepertory::catheterDb() const
{
    return m_catheterDb;
}

void CatheterRepertory::save()
{
    m_catheterDb->save();
}

CatheterTypeDb *CatheterRepertory::catheterTypeDb()
{
    if (m_catheterTypeDb == nullptr) {
        m_catheterTypeDb = new CatheterTypeDb(m_path, this);
    }
    return m_catheterTypeDb;
}
CatheterTempletDb *CatheterRepertory::catheterTempletDb()
{
    if (m_catheterTempletDb == nullptr) {
        m_catheterTempletDb = new CatheterTempletDb(m_path, this);
    }
    return m_catheterTempletDb;
}
