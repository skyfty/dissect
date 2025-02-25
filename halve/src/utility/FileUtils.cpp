#include "FileUtils.h"
#include <shlobj.h>
#include <QString>
#include <QStandardPaths>
#include <QDateTime>
#include <QDir>

namespace file_utils {

bool copyDirectory(QString sourceDir, QString destDir) {
    std::wstring s = sourceDir.replace('/', '\\').toStdWString() + L'\0';
    std::wstring d = destDir.replace('/', '\\').toStdWString() + L'\0';
    return copyFile(s, d);
}
bool copyFile(QString source, QString dest) {
    std::wstring s = source.replace('/', '\\').toStdWString() + L'\0';
    std::wstring d = dest.replace('/', '\\').toStdWString() + L"\\\0" + L'\0';
    return copyFile(s, d);
}

bool copyFile(const std::wstring &s, const std::wstring &d) {
    SHFILEOPSTRUCT fileOp = {0};
    fileOp.hwnd = NULL;
    fileOp.wFunc = FO_COPY;
    fileOp.pFrom = s.c_str();
    fileOp.pTo = d.c_str();
    fileOp.fFlags =FOF_NO_UI | FOF_NOCONFIRMMKDIR;
    return (SHFileOperation(&fileOp) == 0);
}
QString getTempPath() {
    QString destPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    destPath += QString("\\%1").arg(QDateTime::currentSecsSinceEpoch());
    QDir dir(destPath); if (!dir.exists(destPath))dir.mkdir(".");
    return destPath;

}

QString getCatheterMeshPath() {
    static QString savePath;
    if (savePath.isEmpty()) {
        savePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        savePath += QString("\\catheter");
        QDir dir(savePath); if (!dir.exists(savePath))dir.mkdir(".");
    }
    return savePath;
}

QString getCatheterMeshPath(const QString &meshName) {
    return QString("%1\\%2").arg(file_utils::getCatheterMeshPath(), meshName);
}

}
