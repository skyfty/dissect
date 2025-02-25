#include "Hotkey.h"
#include "profile/Profile.h"

#include <QJsonDocument>
#include <QResource>
#include <qjsonarray.h>
#include <qjsonobject.h>

Hotkey::Hotkey(QObject *parent)
    : QObject{parent}
{
    load();
}

Profile *Hotkey::profile() const {
    return m_profile;
}

void Hotkey::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;

    emit profileChanged();
}


void Hotkey::load() {
    auto res = QResource(":/assets/configures/hotkey.json");
    for(auto json:QJsonDocument::fromJson(res.uncompressedData().data()).array()) {
        load(json.toObject());
    }
}

void Hotkey::load(const QJsonObject &json) {
    QShortcut *shortcut = new QShortcut(this);
    shortcut->setKey(QKeySequence(json["key"].toString()));
    shortcut->setContext(Qt::ApplicationShortcut);
    m_shortcuts[shortcut] = json["name"].toString();

    QObject::connect(shortcut, &QShortcut::activated, this, [this, shortcut] {
        if (m_shortcuts.contains(shortcut)) {
            emit m_profile->action(m_shortcuts[shortcut]);
        }
    });
}
