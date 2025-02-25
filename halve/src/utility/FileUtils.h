#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>


namespace file_utils {
bool copyDirectory(QString sourceDir, QString destDir);
bool copyFile(QString source, QString dest);
bool copyFile(const std::wstring &s, const std::wstring &d);
QString getTempPath();
QString getCatheterMeshPath(const QString &meshName);
QString getCatheterMeshPath();

}

#endif // FILEUTILS_H
