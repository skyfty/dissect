#ifndef PROFILESEGMENT_H
#define PROFILESEGMENT_H

#include "ProfileFile.h"

class ProfileSnapshot : public ProfileFile
{
    Q_OBJECT
    Q_PROPERTY(NotebookSnapshot *snapshot READ snapshot CONSTANT FINAL)
public:
    explicit ProfileSnapshot(const QString &id,const QString &workPath, NotebookSnapshot* snapshot,QObject *parent = nullptr);
    ~ProfileSnapshot();
    void save() override;
    Halve::ProfileType type() const override;
    NotebookSnapshot *snapshot() const;
private:
    QPointer<NotebookSnapshot> m_snapshot;
};

#endif // PROFILESEGMENT_H
