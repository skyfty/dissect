#ifndef NOTEBOOKFILE_H
#define NOTEBOOKFILE_H

#include "Notebook.h"

class NotebookFile : public Notebook
{
    Q_OBJECT
public:
    explicit NotebookFile(const QString &path,QObject *parent = nullptr);

    void fromJson(const QJsonObject &v) override;
    void toJson(QJsonObject &json) const override;

    QString fileName() const;
    void setFileName(const QString &newFileName);

signals:

private:
    QString m_fileName;
};

#endif // NOTEBOOKFILE_H
