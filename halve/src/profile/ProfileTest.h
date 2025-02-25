#ifndef PROFILETEST_H
#define PROFILETEST_H

#include "Profile.h"

class ProfileTest : public Profile
{
    Q_OBJECT
public:
    explicit ProfileTest(QObject *parent = nullptr);
    ~ProfileTest();
    Halve::ProfileType type() const override;

signals:

};

#endif // PROFILETEST_H
