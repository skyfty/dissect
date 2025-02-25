#ifndef CATHETERTRACKPACKAGE_H
#define CATHETERTRACKPACKAGE_H

#include <QObject>
#include <QMap>
#include "combined/CatheterTrack.h"

class CatheterTrackPackage : public QObject
{
    Q_OBJECT
public:
    explicit CatheterTrackPackage(QObject *parent = nullptr);
    QList<CatheterTrack>& getTracks(Catheter *catheter);
    bool exist(Catheter *catheter) const;
    QList<Catheter*> getCatheters();
    bool isEmpty() const;
    bool isTrackEmpty(Catheter *catheter) const;
    std::optional<CatheterTrack> getPant(Catheter* catheter, quint16 port) const;
    std::optional<CatheterTrack> getPant(quint16 port) const;

private:
    QMap<Catheter*, QList<CatheterTrack>> m_map;
};

#endif // CATHETERTRACKPACKAGE_H
