#include "NotebookFile.h"
#include "qjsonobject.h"

NotebookFile::NotebookFile(const QString &path,QObject *parent)
    : Notebook{path, parent}
{}

void NotebookFile::fromJson(const QJsonObject &json) {
    Notebook::fromJson(json);
    if (json.contains("fileName"))
        m_fileName = json["fileName"].toString();
}

void NotebookFile::toJson(QJsonObject &json) const {
    Notebook::toJson(json);
    json["fileName"] = m_fileName;

}

QString NotebookFile::fileName() const
{
    return m_fileName;
}

void NotebookFile::setFileName(const QString &newFileName)
{
    m_fileName = newFileName;
}
