#include "Thread.h"

#include <QCoreApplication>
#include <QThread>

Qt::HANDLE VtkRenderThreadHandle = 0;
Qt::HANDLE IoThreadHandle = 0;

Thread::Thread(QObject *parent)
    :QThread(parent)
{

}

bool Thread::currentlyOn(Type type) {
    switch(type) {
    case UI: {
        return QThread::currentThread() == QCoreApplication::instance()->thread();
    }
    case IO: {
        return IoThreadHandle == QThread::currentThreadId();
    }
    case VTK:
        return VtkRenderThreadHandle == QThread::currentThreadId();
    default: {
        return true;
    }
    }
}
bool Thread::currentlyOn(const QThread* thread) {
    return QThread::currentThread() == thread;
}