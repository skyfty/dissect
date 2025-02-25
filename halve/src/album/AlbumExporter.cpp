
#include "profile/Profile.h"
#include "AlbumExporter.h"
#include "album/AlbumDb.h"
#include "album/Picture.h"
#include "utility/exporter/Exporter.h"

AlbumExporter::AlbumExporter(QObject *parent)
    : QObject{parent}
{
    m_exporter = new Exporter(this);
    QObject::connect(m_exporter, &Exporter::finished, this, &AlbumExporter::onCopyFinised);
}

AlbumExporter::~AlbumExporter()
{
    m_exporter->cancel();
}

void AlbumExporter::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    m_albumDb = profile->albumDb();
    emit profileChanged();
}

Profile *AlbumExporter::profile() const {
    return m_profile;
}

void AlbumExporter::exportToPath(const QString& destPath) {
    m_list.clear();
    setProgressValue(0);
    QList<Picture*> pictures = m_albumDb->getDatas();
    for(Picture* picture:pictures) {
        QString filePath = picture->filePath();
        m_list.append(filePath);
        m_exporter->copy(filePath, destPath);
    }
    emit runningChanged();
}

void AlbumExporter::onCopyFinised(const QString &srcPath, const QString &destPath) {
    setProgressValue(++m_progressValue);
    if (m_progressValue == m_list.count()) {
        setProgressValue(-1);
        emit finished();
        emit runningChanged();
    }
}

bool AlbumExporter::running() const
{
    return m_progressValue > 0;
}

double AlbumExporter::progressValue() const
{
    double v = 0;
    if (m_list.count() > 0) {
        v = 1.0 * m_progressValue / m_list.count();
    }
    return v;
}
void AlbumExporter::setProgressValue(int v) {
    m_progressValue = v;
    emit progressValueChanged();

}
