#include "NotebookAutoRecord.h"
#include "qjsonobject.h"

NotebookAutoRecord::NotebookAutoRecord(const QString &path,QObject *parent)
    : NotebookFile{path, parent}
{}
Halve::NotebookType NotebookAutoRecord::type() const {
    return Halve::AUTORECORD;
}
