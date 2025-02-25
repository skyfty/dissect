#include "LayoutSaver.h"
#include "Profile.h"

#include <QFile>
#include <QResource>

LayoutSaver::LayoutSaver(QObject *parent)
    : QObject(parent)
    , KDDockWidgets::LayoutSaver(KDDockWidgets::RestoreOption_RelativeToMainWindow)
{
}

LayoutSaver::~LayoutSaver()
{
}

void LayoutSaver::setProfile(Profile* profile) {
    m_profile = profile;
}

bool LayoutSaver::setLayout(const QString &fileName) {
    if (fileName.startsWith(":/")) {
        return KDDockWidgets::LayoutSaver::restoreLayout(QResource(fileName).uncompressedData());
    } else {
        QString layoutFilePath = m_profile->path() + "\\" + fileName;
        if (!QFile::exists(layoutFilePath)) {
            return KDDockWidgets::LayoutSaver::restoreFromFile(layoutFilePath);
        }
        return false;
    }
}

bool LayoutSaver::save() {
    QString layoutFilePath = m_profile->path() + "\\" + m_profile->layoutFile();
    return KDDockWidgets::LayoutSaver::saveToFile(layoutFilePath);
}

bool LayoutSaver::restore() {
    QString layoutFilePath = m_profile->path() + "\\" + m_profile->layoutFile();
    if (!QFile::exists(layoutFilePath)) {
        restoreDefaultLayoutFile(layoutFilePath);
    }
    return KDDockWidgets::LayoutSaver::restoreFromFile(layoutFilePath);
}

void LayoutSaver::restoreDefaultLayoutFile(const QString &layoutFilePath) {
    auto data = QResource(":/layout/default.json").uncompressedData();
    QFile saveFile(layoutFilePath);
    if (saveFile.open(QIODevice::WriteOnly)) {
        saveFile.write(data);
    }
}

