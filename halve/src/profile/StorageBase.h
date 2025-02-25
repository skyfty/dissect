#ifndef STORAGEBASE_H
#define STORAGEBASE_H

#include "utility/Thread.h"
#include "utility/SQliteOrm.h"

using namespace  sqlite_orm;
template<typename T, typename Storage>
class StorageBase {
public:
    typedef std::shared_ptr<T> Ptr;
    typedef QList<T> List;
    typedef struct{} Ref;

protected:
    explicit StorageBase(Storage &storage)
        :m_storage(storage){
    }
public:
    int add(const T& data) {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        return m_storage.insert(data);
    }

    //void add(const List &datas) {
    //    Q_ASSERT(Thread::currentlyOn(Thread::IO));
    //    m_storage.transaction([&] {
    //        m_storage.insert_range(datas.begin(), datas.end());
    //        return true;  //  commit
    //    });
    //}

    void add(const List& datas) {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        m_storage.insert_range(datas.begin(), datas.end());
    }

    template<class... Args>
    List select(Args... args) {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        return m_storage.get_all<T, List>(std::forward<Args>(args)...);
    }

    template<class R, class... Args>
    R select(Args... args) {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        return m_storage.get_all<T, R>(std::forward<Args>(args)...);
    }
    template<class O>
    void update(const O& o) {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        return m_storage.update(o);
    }
    template<class Args, class... Wargs>
    void updateAll(Args set, Wargs... wh) {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        return m_storage.update_all(set, std::forward<Wargs>(wh)...);
    }
    template<class... Ids>
    Ptr get(Ids... ids) {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        return m_storage.get_no_throw<T>(std::forward<Ids>(ids)...);
    }

    template<class... Ids>
    T take(Ids... ids) {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        return m_storage.get<T>(std::forward<Ids>(ids)...);
    }

    template<class... Args>
    void remove(Args&&... args) {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        m_storage.remove_all<T>(std::forward<Args>(args)...);
    }

    bool remove(int row) {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        auto rows = m_storage.get_all_pointer<T>(limit(row,1));
        if (rows.size() > 0) {
            m_storage.remove<T>(rows[0]->id);
        }
        return rows.size() > 0;
    }

    void remove(qint64 rowId) {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        m_storage.remove<T>(rowId);
    }

    void removeAll() {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        m_storage.remove_all<T>();
    }

    auto count() {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        return m_storage.count<T>();
    }

    std::string filename() const {
        return m_storage.filename();
    }

protected:
    Storage &m_storage;
};

#endif // STORAGEBASE_H
