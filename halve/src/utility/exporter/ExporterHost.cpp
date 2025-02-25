#include "ExporterHost.h"
#include "utility/FileUtils.h"
#include <QtConcurrent>

ExporterHost::ExporterHost(QObject *parent)
    : ExporterSimpleSource{parent}
{}
void ExporterHost::init() {
    setState(State_Ready);
}

void ExporterHost::copy(const QString &srcPath, const QString &destPath)
{
    QtConcurrent::run([=]{
        QFileInfo fileInfo(srcPath);
        if (fileInfo.isDir() || srcPath.endsWith("*.*")) {
            file_utils::copyDirectory(srcPath, destPath);
        } else if (fileInfo.isFile()) {
            file_utils::copyFile(srcPath, destPath);
        }
    }).then(this, [=]{
        emit finished(srcPath, destPath);
    });
}
