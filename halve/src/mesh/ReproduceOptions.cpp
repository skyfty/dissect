#include "mesh/ReproduceOptions.h"
#include <qjsonobject.h>

ReproduceOptions::ReproduceOptions(QObject *parent)
    : QObject{parent}
{

}

void ReproduceOptions::fromJson(const QJsonObject &json) {
    if (json.contains("iterations"))
        m_iterations = json["iterations"].toInteger();
    if (json.contains("kernelSize"))
        m_kernelSize = json["kernelSize"].toInteger();
    if (json.contains("catheterId"))
        m_catheterId = json["catheterId"].toString();

    if (json.contains("originalPoint")) {
        QJsonObject originalPointJson = json["originalPoint"].toObject();
        m_centerPoint.setX(originalPointJson["x"].toDouble());
        m_centerPoint.setY(originalPointJson["y"].toDouble());
        m_centerPoint.setZ(originalPointJson["z"].toDouble());
    }

}


QJsonValue ReproduceOptions::toJson() const {
    QJsonObject json;

    json["iterations"] = m_iterations;
    json["kernelSize"] = m_kernelSize;
    json["catheterId"] = m_catheterId;

    QJsonObject originalPointJson;
    originalPointJson["x"] = m_centerPoint.x();
    originalPointJson["y"] = m_centerPoint.y();
    originalPointJson["z"] = m_centerPoint.z();
    json["originalPoint"] = originalPointJson;

    return json;
}

int ReproduceOptions::iterations() const
{
    return m_iterations;
}

void ReproduceOptions::setIterations(int newIterations)
{
    if (m_iterations == newIterations)
        return;
    if (newIterations < 1 || newIterations > 100) {
        return;
    }
    m_iterations = newIterations;
    emit iterationsChanged();
    emit changed();
}

int ReproduceOptions::kernelSize() const
{
    return m_kernelSize;
}

void ReproduceOptions::setKernelSize(int newKernelSize)
{
    if (m_kernelSize == newKernelSize)
        return;
    m_kernelSize = newKernelSize;
    emit kernelSizeChanged();
    emit changed();
}

double ReproduceOptions::holeSize() const
{
    return m_holeSize;
}

void ReproduceOptions::setHoleSize(double newHoleSize)
{
    if (qFuzzyCompare(m_holeSize, newHoleSize))
        return;
    m_holeSize = newHoleSize;
    emit holeSizeChanged();
    emit changed();
}

QString ReproduceOptions::catheterId() const
{
    return m_catheterId;
}

void ReproduceOptions::setCatheterId(const QString &newCatheterId)
{
    if (m_catheterId == newCatheterId)
        return;
    m_catheterId = newCatheterId;
    emit catheterIdChanged();
    emit changed();
}
