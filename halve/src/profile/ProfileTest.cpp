#include "ProfileTest.h"

#include <QDir>
#include <QStandardPaths>
#include "ProfileTestCleaner.h"

ProfileTest::ProfileTest(QObject *parent)
    : Profile{parent}
{
    m_id = "test";
    m_path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    m_path += QString("\\%1").arg(QDateTime::currentSecsSinceEpoch());
    QDir dir(m_path); if (!dir.exists(m_path))dir.mkdir(".");
}

Halve::ProfileType ProfileTest::type() const
{
    return Halve::PROFILE_TEMP;
}


ProfileTest::~ProfileTest()
{
    ProfileTestCleaner *cleaner = new ProfileTestCleaner(path());
    cleaner->deleteLater();
}
