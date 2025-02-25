#ifndef CATHETERREPERTORY_H
#define CATHETERREPERTORY_H



#include "CatheterTempletDb.h"
#include "CatheterTypeDb.h"

#include <QObject>
#include <QPointer>

class Catheter;
class CatheterDb;
class CatheterTypeDb;
class CatheterTempletDb;

class CatheterRepertory : public QObject
{
    Q_OBJECT
    Q_PROPERTY(CatheterDb *catheterDb READ catheterDb CONSTANT FINAL)
    Q_PROPERTY(CatheterTypeDb *catheterTypeDb READ catheterTypeDb CONSTANT FINAL)
    Q_PROPERTY(CatheterTempletDb *catheterTempletDb READ catheterTempletDb CONSTANT FINAL)

public:
    explicit CatheterRepertory(const QString& filePath,QObject *parent = nullptr);
    void initDefault(const QString& path);

    CatheterTypeDb *catheterTypeDb();
    CatheterTempletDb *catheterTempletDb();

    CatheterDb *catheterDb() const;
    Q_INVOKABLE void save();

private:
    QString m_path;
    QPointer<CatheterDb> m_catheterDb;
    QPointer<CatheterTypeDb> m_catheterTypeDb;
    QPointer<CatheterTempletDb> m_catheterTempletDb;
};

#endif // CATHETERREPERTORY_H
