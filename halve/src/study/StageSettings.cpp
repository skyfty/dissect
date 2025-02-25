#include "StageSettings.h"
#include "study/StageOptions.h"

#include <QJsonArray>
#include <QJsonObject>

StageSettings::StageSettings(QObject *parent)
    : QObject{parent}
{}

void StageSettings::fromJson(const QJsonArray &json) {
    for(int i = 0; i < json.count(); ++i) {
        StageOptions* stageOptions = load(json.at(i).toObject());
        m_options[stageOptions->name()] = stageOptions;
    }
}

StageOptions* StageSettings::load(const QJsonObject &json) {
    StageOptions* stageOptions = new StageOptions(this);
    stageOptions->fromJson(json);
    return stageOptions;
}

QJsonValue StageSettings::toJson() const {
    QJsonArray json;
    for (auto i = m_options.cbegin(), end = m_options.cend(); i != end; ++i) {
        json.append(i.value()->toJson());
    }
    return json;
}

StageOptions *StageSettings::getOptions(const QString &name) {
    StageOptions *stageOptions = nullptr;
    if (!m_options.contains(name)) {
        m_options[name]=stageOptions = new StageOptions(this);
        stageOptions->setName(name);
    }else {
        stageOptions = m_options.value(name);
    }
    return stageOptions;
}
