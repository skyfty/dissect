#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "Notebook.h"


class NotebookSnapshot : public Notebook
{
    Q_OBJECT
    Q_PROPERTY(qint64 beginTime READ beginTime WRITE setBeginTime NOTIFY beginTimeChanged FINAL)
    Q_PROPERTY(qint64 endTime READ endTime WRITE setEndTime NOTIFY endTimeChanged FINAL)

public:
    explicit NotebookSnapshot(const QString &path,QObject *parent = nullptr);
    Halve::NotebookType type() const override;

    QString snapshotPath();
    void fromJson(const QJsonObject &v) override;
    void toJson(QJsonObject &json) const override;
    void clean() override;

    qint64 beginTime() const;
    void setBeginTime(qint64 newBeginTime);

    qint64 endTime() const;
    void setEndTime(qint64 newEndTime);

signals:

    void beginTimeChanged();

    void endTimeChanged();

private:
    QString m_snapshotPath;
    qint64 m_beginTime;
    qint64 m_endTime;
};

#endif // SNAPSHOT_H
