#ifndef NOTEBOOKAUTORECORD_H
#define NOTEBOOKAUTORECORD_H

#include "NotebookFile.h"

class NotebookAutoRecord : public NotebookFile
{
    Q_OBJECT
public:
    explicit NotebookAutoRecord(const QString &path,QObject *parent = nullptr);
    Halve::NotebookType type() const override;

};

#endif // NOTEBOOKAUTORECORD_H
