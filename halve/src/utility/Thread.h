#ifndef THREAD_H
#define THREAD_H

#include <QFuture>
#include <QThread>

#include "FixedArray.h"
#include "NumberRange.h"

class Thread:public QThread
{
    Q_OBJECT

public:
    enum Type{
        UI,
        IO,
        DB,
        VTK,
    };
public:
    Thread(QObject *parent = nullptr);
    static bool currentlyOn(Type type);
    static bool currentlyOn(const QThread *thread);
};

namespace QtConcurrent {

template <int begin, int end, typename MapFunctor>
QFuture<void> map(MapFunctor &&map)
{
    static FixedArray<int, end - begin> temp;
    return QtConcurrent::map(temp.begin(), temp.end(), map);
}

template <typename MapFunctor>
QFuture<void> map(const NumberRange &range, MapFunctor &&map)
{
    return QtConcurrent::map(range.begin(), range.end(), map);
}
}

#endif // THREAD_H
