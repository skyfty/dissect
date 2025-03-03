#ifndef MAPPINGRESEAUCHECKER_H
#define MAPPINGRESEAUCHECKER_H

#include <QObject>
#include <HalveType.h>
#include <QPointer>

class Profile;
class Mapping;
class MappingPointsDb;
class ReseauDb;
class Reseau;
class MappingPoint;
class MappingSetting;

class MappingReseauChecker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(MappingSetting *mappingSetting READ mappingSetting WRITE setMappingSetting NOTIFY mappingSettingChanged FINAL)
public:
    explicit MappingReseauChecker(QObject *parent = nullptr);

    void setProfile(Profile* profile);
    Profile *profile() const;
    MappingSetting *mappingSetting() const;
    void setMappingSetting(MappingSetting *newMeltSetting);

signals:
    void profileChanged();
    void mappingSettingChanged();

public slots:
    void onReseauChanged(Reseau *reseau);
    void onMappingOptionsThrowChanged();
    void onMappingOptionsChanged();
    void onReseauChangedTimerEvent();
    void onCheckOvercomTimerEvent();

private:
    bool checkMappingPoint(MappingPoint &mp);
    bool checkMappingPoint(Reseau *reseau,double dist, MappingPoint &mp);
    void checkMappingPointOvercome(const MappingPoint &mappingPoint);
    QList<MappingPoint>::iterator getMappingPointCompIterator(QList<MappingPoint> &mappingPointList);

private:
    QPointer<Profile> m_profile;
    MappingSetting *m_mappingSetting = nullptr;
    QPointer<MappingPointsDb> m_mappingPointsDb;
    QPointer<ReseauDb> m_reseauDb;
    QTimer *m_reseauTimer = nullptr;
    QTimer* m_checkOvercomTimer = nullptr;

};

#endif // MAPPINGRESEAUCHECKER_H
