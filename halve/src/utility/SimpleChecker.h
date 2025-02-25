#ifndef SIMPLECHECKER_H
#define SIMPLECHECKER_H

#include <QString>


class SimpleChecker
{
public:
    SimpleChecker(const QString &name);
    bool running();
    void activateWindow(const QString &name);

private:
    QString m_name;
    Qt::HANDLE m_handle = nullptr;
};

#endif // SIMPLECHECKER_H
