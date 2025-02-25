#ifndef PROFILETESTCLEANER_H
#define PROFILETESTCLEANER_H

#include <QObject>

class ProfileTestCleaner : public QObject
{
    Q_OBJECT
public:
    ProfileTestCleaner(const QString &path, QObject *parent = nullptr);
    ~ProfileTestCleaner();
private:
    QString m_path;
};

#endif // PROFILETESTCLEANER_H
