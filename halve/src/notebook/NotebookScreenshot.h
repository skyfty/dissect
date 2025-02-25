#ifndef NOTEBOOKSCREENSHOT_H
#define NOTEBOOKSCREENSHOT_H

#include "NotebookFile.h"

class NotebookScreenshot : public NotebookFile
{
    Q_OBJECT
public:
    explicit NotebookScreenshot(const QString &path,QObject *parent = nullptr);
    Halve::NotebookType type() const override;

};

#endif // NOTEBOOKSCREENSHOT_H
