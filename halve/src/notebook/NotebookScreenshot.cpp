#include "NotebookScreenshot.h"

NotebookScreenshot::NotebookScreenshot(const QString &path,QObject *parent)
    : NotebookFile{path, parent}
{}


Halve::NotebookType NotebookScreenshot::type() const {
    return Halve::SCREENSHOT;
}

