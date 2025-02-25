#ifndef SCALARMODEL_H
#define SCALARMODEL_H

#include <QAbstractListModel>
#include <QPointer>
#include "profile/Profile.h"


class ScalarDb;
class Scalar;
class ScalarOptions;
class Profile;
class ScalarColor;
class MeltOptions;

class ScalarModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(double max READ max WRITE setMax NOTIFY maxChanged FINAL)
    Q_PROPERTY(double min READ min WRITE setMin NOTIFY minChanged FINAL)
    Q_PROPERTY(double pos READ pos WRITE setPos NOTIFY posChanged FINAL)
    Q_PROPERTY(double height READ height WRITE setHeight NOTIFY heightChanged FINAL)
    Q_PROPERTY(double top READ top NOTIFY posChanged FINAL)
    Q_PROPERTY(double bottom READ bottom NOTIFY posChanged FINAL)
    Q_PROPERTY(MeltSetting *meltSetting READ meltSetting WRITE setMeltSetting NOTIFY meltSettingChanged FINAL)
    Q_PROPERTY(QList<ScalarColor *> linearGradient READ getLinearGradient CONSTANT FINAL)

public:
    explicit ScalarModel(QObject *parent = nullptr);
    void setProfile(Profile* profile);
    Profile *profile() const;
    Q_INVOKABLE Scalar* getData() const;
    Q_INVOKABLE void increaseMax();
    Q_INVOKABLE void decreaseMax();
    Q_INVOKABLE void increaseMin();
    Q_INVOKABLE void decreaseMin();
    Q_INVOKABLE void reset();

    double max() const;
    void setMax(double newMax);

    double min() const;
    void setMin(double newMin);

    double pos() const;
    void setPos(double newMax);

    double height() const;
    void setHeight(double newMin);
    double top() const;
    double bottom() const;

    QList<ScalarColor*> getLinearGradient();

    MeltSetting *meltSetting() const;
    void setMeltSetting(MeltSetting *newMeltSetting);

public slots:
    void onMappingTypeChanged();

signals:
    void profileChanged();
    void maxChanged();
    void minChanged();

    void posChanged();
    void heightChanged();

    void changed();
    void meltSettingChanged();

private:
    void loadScalarData(Halve::MappingType type);
    double getScalarStep();
    double getMaxScope();

protected:
    QPointer<Profile> m_profile;
    QPointer<ScalarDb> m_scalarDb;
    QPointer<ScalarOptions> m_scalarOptions;
    QPointer<Scalar> m_scalar;
    QPointer<MeltSetting> m_meltSetting;
};

#endif // SCALARMODEL_H
