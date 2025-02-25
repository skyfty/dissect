#ifndef PROFILEFILE_H
#define PROFILEFILE_H

#include <QObject>
#include "Profile.h"

class ProfileFile : public Profile
{
    Q_OBJECT;

public:
    ProfileFile(const QString &id,const QString &path, QObject *parent = nullptr);

    Halve::ProfileType type() const override;
    void save() override;
signals:

};

#endif // PROFILEFILE_H
