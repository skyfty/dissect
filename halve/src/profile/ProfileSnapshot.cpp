#include "ProfileSnapshot.h"
#include "ProfileTestCleaner.h"
#include <notebook/NotebookSnapshot.h>
#include <QDir>
#include <QStandardPaths>

ProfileSnapshot::ProfileSnapshot(const QString &id,const QString &workPath,NotebookSnapshot* snapshot,QObject *parent)
    : ProfileFile{id, workPath, parent},m_snapshot(snapshot)
{

}

ProfileSnapshot::~ProfileSnapshot() {
    //ProfileTestCleaner *cleaner = new ProfileTestCleaner(path());
    //cleaner->deleteLater();
}

void ProfileSnapshot::save() {
    ProfileFile::save();
}

Halve::ProfileType ProfileSnapshot::type() const
{
    return Halve::PROFILE_SNAPSHOT;
}

NotebookSnapshot *ProfileSnapshot::snapshot() const
{
    return m_snapshot;
}
