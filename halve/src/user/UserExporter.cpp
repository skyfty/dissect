
#include <QtConcurrent>
#include "profile/ProfileManager.h"
#include "user/UserExporter.h"
#include "user/User.h"
#include "utility/exporter/Exporter.h"

UserExporter::UserExporter(QObject *parent)
    : QObject{parent}
{
    m_exporter = new Exporter(this);
    QObject::connect(m_exporter, &Exporter::finished, this, &UserExporter::onCopyFinised);
}

UserExporter::~UserExporter()
{
    m_exporter->cancel();
}

void UserExporter::exportUsersToPath(const QStringList &newList,const QString& filePath) {
    m_list = newList;
    m_progressValue = 0;
    emit progressValueChanged();

    for(const QString userPath:m_list) {
        User* user = ProfileManager::instance()->getUser(userPath);
        if (user != nullptr) {
            QString destPath = QString("%1/%2_%3").arg(filePath, user->getName(), user->getIdCard());
            QString srcPath = user->getPath() + "\\*.*";
            m_exporter->copy(srcPath, destPath);
        }
    }
    emit isRunningChanged();
}

void UserExporter::onCopyFinised(const QString &srcPath, const QString &destPath) {
    m_progressValue++;
    emit progressValueChanged();
    if (m_progressValue == m_list.count()) {
        emit finished();
        emit isRunningChanged();
    }
}

bool UserExporter::isRunning() const
{
    return m_progressValue > 0 && m_progressValue < m_list.count();
}

double UserExporter::progressValue() const
{
    double v = 0; 
    if (m_list.count() > 0) {
        v = 1.0 * m_progressValue / m_list.count();
    }
    return v;
}
