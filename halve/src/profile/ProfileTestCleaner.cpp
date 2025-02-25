#include "ProfileTestCleaner.h"

#include <QDir>

ProfileTestCleaner::ProfileTestCleaner(const QString &path, QObject *parent)
    : QObject{parent}, m_path(path)
{}

ProfileTestCleaner::~ProfileTestCleaner()
{
    QDir dir(m_path);
    dir.removeRecursively();
}
