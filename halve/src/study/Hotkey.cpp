#include "Hotkey.h"
#include <QApplication>
#include <QWidget>
#include "profile/Profile.h"

#include <QJsonDocument>
#include <QResource>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <QMessageBox>

static QHash<QString, int> KeysMap = {
    {"F1", VK_F1},
    {"F2", VK_F2},
    {"F3", VK_F3},
    {"F4", VK_F4},
    {"F5", VK_F5},
    {"F6", VK_F6},
    {"F7", VK_F7},
    {"F8", VK_F8},
    {"F9", VK_F9},
    {"F10", VK_F10},
    {"F11", VK_F11},
    {"F12", VK_F12},
    {"A", 'A'},
    {"B", 'B'},
    {"C", 'C'},
    {"D", 'D'},
    {"E", 'E'},
    {"F", 'F'},
    {"G", 'G'},
    {"H", 'H'},
    {"I", 'I'},
    {"J", 'J'},
    {"K", 'K'},
    {"L", 'L'},
    {"M", 'M'},
    {"N", 'N'},
    {"O", 'O'},
    {"P", 'P'},
    {"Q", 'Q'},
    {"R", 'R'},
    {"S", 'S'},
    {"T", 'T'},
    {"U", 'U'},
    {"V", 'V'},
    {"W", 'W'},
    {"X", 'X'},
    {"Y", 'Y'},
    {"Z", 'Z'},
    {"0", '0'},
    {"1", '1'},
    {"2", '2'},
    {"3", '3'},
    {"4", '4'},
    {"5", '5'},
    {"6", '6'},
    {"7", '7'},
    {"8", '8'},
    {"9", '9'},
    {"CTRL", VK_CONTROL},
    {"ALT", VK_MENU},
    {"SHIFT", VK_SHIFT},
    {"ENTER", VK_RETURN},
    {"ESC", VK_ESCAPE},
    {"SPACE", VK_SPACE},
    {"TAB", VK_TAB},
    {"BACKSPACE", VK_BACK},
    {"DELETE", VK_DELETE},
    {"INSERT", VK_INSERT},
    {"HOME", VK_HOME},
    {"END", VK_END},
    {"PAGEUP", VK_PRIOR},
    {"PAGEDOWN", VK_NEXT},
    {"LEFT", VK_LEFT},
    {"RIGHT", VK_RIGHT},
    {"UP", VK_UP},
    {"DOWN", VK_DOWN}
};

Hotkey::Hotkey(QObject *parent)
    : QObject{parent}
{
    qApp->installNativeEventFilter(this);
    load();
}

Hotkey::~Hotkey() {
    for (auto id : m_shortcuts.keys()) {
        UnregisterHotKey(nullptr, id);
    }
    qApp->removeNativeEventFilter(this);
}

Profile *Hotkey::profile() const {
    return m_profile;
}

bool Hotkey::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) {
    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG") {
        MSG* msg = static_cast<MSG*>(message);
        if (msg != nullptr && msg->message == WM_HOTKEY) {
            int id = msg->wParam;
            if (m_shortcuts.contains(id)) {
                QString name = m_shortcuts[id];
                QMetaObject::invokeMethod(this, [this, name]() {
                    emit m_profile->action(name);
                }, Qt::QueuedConnection);
            }
        }
    }
    return false;
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
    QString key = json["key"].toString();
    if (KeysMap.contains(key)) {
        auto shortcutId = GlobalAddAtomA(key.toStdString().c_str());
        m_shortcuts[shortcutId] = json["name"].toString();
        RegisterHotKey(nullptr, shortcutId, 0, KeysMap[key]);
    }
}
