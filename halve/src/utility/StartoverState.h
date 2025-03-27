#ifndef STARTOVERSTATE_H
#define STARTOVERSTATE_H

#include <QObject>
#include <QStringList.h>
class QWindow;

class StartoverState : public QObject
{
    Q_OBJECT
public:
    explicit StartoverState(QObject *parent = nullptr);
    static StartoverState *instance();
    void watch(int timeout = 30000);
    static void runAsAdmin(const QString &path, const QStringList & args);
    static void waitAndKill(int pid);
    static void registerDll();
    Q_INVOKABLE void set();
    Q_INVOKABLE void exit(int code);
    Q_INVOKABLE void raise(QWindow *wnd);
    Q_INVOKABLE void fullScreen(QWindow *wnd, bool b);

    bool isRunning() const;

signals:
    void finished();

private:
};

#endif // STARTOVERSTATE_H
