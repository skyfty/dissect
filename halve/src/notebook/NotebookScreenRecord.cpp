#include "NotebookScreenRecord.h"

NotebookScreenRecord::NotebookScreenRecord(const QString &path,QObject *parent)
    : NotebookFile{path, parent}
{}

Halve::NotebookType NotebookScreenRecord::type() const {
    return Halve::SCREENRECORD;
}
