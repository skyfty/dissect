#ifndef IOWORKET_H
#define IOWORKET_H

#include <QObject>
#include <QPointer>
#include <QtConcurrent>
#include <QtConcurrent/qtconcurrentstoredfunctioncall.h>

class QThreadPool;
class QRunnable;
class IOWorker;
extern IOWorker *ioWorker;

class IOWorker : public QObject
{
    Q_OBJECT
public:
    explicit IOWorker(QObject *parent = nullptr);
    ~IOWorker();
    void init();
    void exit();

    template <class Function, class ...Args>
    struct StoredFunctionCall : public QtConcurrent::StoredFunctionCall<Function, Args...>
    {
        typedef QtConcurrent::StoredFunctionCall<Function, Args...> Base;
        typedef QtConcurrent::InvokeResultType<Function, Args...> T;
        StoredFunctionCall(QtConcurrent::DecayedTuple<Function, Args...> &&_data)
            : QtConcurrent::StoredFunctionCall<Function, Args...>(std::move(_data))
        {

        }

        QFuture<T> start(const QtConcurrent::TaskStartParameters &parameters) {
            Base::promise.setThreadPool(parameters.threadPool);
            Base::promise.setRunnable(this);
            Base::promise.reportStarted();
            QFuture<T> theFuture = Base::promise.future();
            emit ioWorker->start(this);
            return theFuture;
        }
    protected:

    };


    template <class Function, class... Args>
    struct TaskResolver : public QtConcurrent::TaskResolverHelper<typename std::is_invocable<std::decay_t<Function>,
                                                                               std::decay_t<Args>...>::type, Function, Args...>
    {
        using TaskWithArgs = QtConcurrent::DecayedTuple<Function, Args...>;

        static auto run(TaskWithArgs &&args, const QtConcurrent::TaskStartParameters &startParameters) {
            return (new StoredFunctionCall<Function, Args...>(std::move(args)))->start(startParameters);
        }
    };

    template <class Function, class ...Args>
    static auto run(QThreadPool *pool, Function &&f, Args &&...args)
    {
        QtConcurrent::DecayedTuple<Function, Args...> tuple { std::forward<Function>(f),
                                              std::forward<Args>(args)... };
        return TaskResolver<std::decay_t<Function>, std::decay_t<Args>...>::run( std::move(tuple), QtConcurrent::TaskStartParameters { pool });
    }

    template <class Function, class ...Args>
    static auto run(QThreadPool *pool, std::reference_wrapper<const Function> &&functionWrapper,
        Args &&...args)
    {
        return run(pool, std::forward<const Function>(functionWrapper.get()), std::forward<Args>(args)...);
    }

    template <class Function, class ...Args>
    static auto run(Function &&f, Args &&...args)
    {
        return run(QThreadPool::globalInstance(), std::forward<Function>(f), std::forward<Args>(args)...);
    }
    // overload with a Promise Type hint, takes thread pool
    template <class PromiseType, class Function, class ...Args>
    static auto run(Function &&f, Args &&...args) {
        QtConcurrent::DecayedTuple<Function, Args...> tuple { std::forward<Function>(f),std::forward<Args>(args)... };
        return TaskResolver<std::decay_t<Function>, std::decay_t<Args>...>::run(std::move(tuple), QtConcurrent::TaskStartParameters {  });
    }

signals:
    void start(QRunnable *runner);

private:
    QPointer<QThread> m_thread;

private:
};

#endif // IOWORKET_H
