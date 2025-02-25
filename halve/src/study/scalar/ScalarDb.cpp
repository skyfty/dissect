#include "Scalar.h"
#include "study/scalar/ScalarOptions.h"
#include "ScalarDb.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLinearGradient>
#include <QResource>
#include <qfile.h>
#include "utility/VtkUtil.h"

ScalarDb::ScalarDb(const QString &path, ScalarOptions *scalarOptions,QObject *parent)
    : QObject{parent},
      m_scalarOptions(scalarOptions)
{
    Q_ASSERT(!path.isEmpty());
    m_path = getStoragePath(path);
    if (QFile::exists(m_path)) {
        load();
    } else {
        initDefault();
    }
}

void ScalarDb::initDefault() {
    load(QResource(":/assets/configures/scalar.json").uncompressedData());
}

QString ScalarDb::getStoragePath(const QString &path) const
{
    return path + "/scalar.json";
}

void ScalarDb::backup(const QString &path)
{
    QString backupFile = getStoragePath(path);
    QFile::copy(m_path, backupFile);
}

void ScalarDb::load(const QByteArray &reseauJson) {
    auto scalarJson = QJsonDocument::fromJson(reseauJson.data()).object();
    if (scalarJson.contains("lat")) {
        m_latScalar = load(scalarJson["lat"].toObject());
    }
    if (scalarJson.contains("voltage")) {
        m_voltageScalar = load(scalarJson["voltage"].toObject());
    }
}

Scalar* ScalarDb::load(const QJsonObject &json) {
    Scalar* scalar = new Scalar(this);
    scalar->fromJson(json);
    connectSignals(scalar);
    return scalar;
}

void ScalarDb::connectSignals(Scalar *scalar) {
    QObject::connect(scalar, &Scalar::minChanged, this, std::bind(&ScalarDb::changed, this, scalar));
    QObject::connect(scalar, &Scalar::maxChanged, this, std::bind(&ScalarDb::changed, this, scalar));
    QObject::connect(scalar, &Scalar::posChanged, this, std::bind(&ScalarDb::adjusted, this, scalar));
    QObject::connect(scalar, &Scalar::heightChanged, this, std::bind(&ScalarDb::adjusted, this, scalar));
}

void ScalarDb::load() {
    QFile f(m_path);
    if (f.open(QIODevice::ReadOnly)) {
        load(f.readAll());
    }
}

Scalar* ScalarDb::getLatData() const {
    return m_latScalar;
}

Scalar* ScalarDb::getVoltageData() const {
    return m_voltageScalar;
}

bool ScalarDb::save() {
    QFile saveFile(m_path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonObject scalarJson;
    {
        QJsonObject json;
        m_latScalar->toJson(json);
        scalarJson["lat"] = json;
    }
    {
        QJsonObject json;
        m_voltageScalar->toJson(json);
        scalarJson["voltage"] = json;
    }
    return saveFile.write(QJsonDocument(scalarJson).toJson());
}


double ScalarDb::latMin() const
{
    return m_latScalar->min();
}

void ScalarDb::setLatMin(double newMin)
{
    if (qFuzzyCompare(m_latScalar->min(), newMin))
        return;
    m_latScalar->setMin(newMin);
    emit changed(m_latScalar);
    save();
}

double ScalarDb::latMax() const
{
    return m_latScalar->max();
}

void ScalarDb::setLatMax(double newMax)
{
    if (qFuzzyCompare(m_latScalar->max(), newMax))
        return;
    m_latScalar->setMax(newMax);
    emit changed(m_latScalar);
    save();
}


double ScalarDb::voltageMax() const {
    return m_voltageScalar->max();

}
void ScalarDb::setVoltageMax(double newMax) {
    if (qFuzzyCompare(m_voltageScalar->max(), newMax))
        return;
    m_voltageScalar->setMax(newMax);
    emit changed(m_voltageScalar);
    save();
}

double ScalarDb::voltageMin() const {
    return m_voltageScalar->min();
}

void ScalarDb::setVoltageMin(double newMin) {
    if (qFuzzyCompare(m_voltageScalar->min(), newMin))
        return;
    m_voltageScalar->setMin(newMin);
    emit changed(m_voltageScalar);
    save();
}

double ScalarDb::latPos() const
{
    return m_latScalar->pos();
}

void ScalarDb::setLatPos(double newMax) {
    if (qFuzzyCompare(m_latScalar->pos(), newMax))
        return;
    m_latScalar->setPos(newMax);
    emit changed(m_latScalar);
    save();
}

double ScalarDb::latHeight() const
{
    return m_latScalar->height();
}

void ScalarDb::setLatHeight(double newMin){
    if (qFuzzyCompare(m_latScalar->height(), newMin))
        return;
    m_latScalar->setHeight(newMin);
    emit changed(m_latScalar);
    save();
}

double ScalarDb::voltagePos() const {
    return m_voltageScalar->pos();

}
void ScalarDb::setVoltagePos(double newMax) {
    if (qFuzzyCompare(m_voltageScalar->pos(), newMax))
        return;
    m_voltageScalar->setPos(newMax);
    emit changed(m_voltageScalar);
    save();
}

double ScalarDb::voltageHeight() const {
    return m_voltageScalar->height();

}
void ScalarDb::setVoltageHeight(double newMin){
    if (qFuzzyCompare(m_voltageScalar->height(), newMin))
        return;
    m_voltageScalar->setHeight(newMin);
    emit changed(m_voltageScalar);
    save();
}

void ScalarDb::getLatColor(double v, QColor &color) {
    if (v > m_latScalar->pos()) {
        color = m_scalarOptions->highColor();
    } else if (v < latPos() - latHeight()) {
        color = m_scalarOptions->lowColor();
    } else {
        m_scalarOptions->getScalarColor(m_latScalar, v, color);
    }
}

void ScalarDb::getVoltageColor(double v, unsigned char color[3]) {
    QColor qcolor;
    getVoltageColor(v, qcolor);
    vtkutil::qColorToVtkColor(qcolor, color);
}

void ScalarDb::getLatColor(double v, unsigned char color[3]) {
    QColor qcolor;
    getLatColor(v, qcolor);
    vtkutil::qColorToVtkColor(qcolor, color);
}


void ScalarDb::getVoltageColor(double v, QColor& color) {
    if (v > m_voltageScalar->pos()) {
        color = m_scalarOptions->highColor();
    } else if (v < voltagePos() - voltageHeight()) {
        color = m_scalarOptions->lowColor();
    } else {
        m_scalarOptions->getScalarColor(m_voltageScalar, v, color);
    }
}
