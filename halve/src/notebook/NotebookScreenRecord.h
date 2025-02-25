#ifndef NOTEBOOKSCREENRECORD_H
#define NOTEBOOKSCREENRECORD_H

#include "NotebookFile.h"

class NotebookScreenRecord : public NotebookFile
{
    Q_OBJECT
public:
    explicit NotebookScreenRecord(const QString &path,QObject *parent = nullptr);
    Halve::NotebookType type() const override;

};

#endif // NOTEBOOKSCREENRECORD_H
