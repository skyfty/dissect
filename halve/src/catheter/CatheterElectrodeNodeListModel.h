#ifndef CATHETERELECTRODENODELISTMODEL_H
#define CATHETERELECTRODENODELISTMODEL_H

#include "ElectrodeNodeListModel.h"

class Catheter;
class Profile;

class CatheterElectrodeNodeListModel : public ElectrodeNodeListModel
{
    Q_OBJECT
    Q_PROPERTY(Catheter *catheter READ catheter WRITE setCatheter FINAL)
    Q_PROPERTY(int electrodeType READ electrodeType WRITE setElectrodeType NOTIFY electrodeTypeChanged FINAL)

public:
    explicit CatheterElectrodeNodeListModel(QObject *parent = nullptr);
    ~CatheterElectrodeNodeListModel() = default;
    void setProfile(Profile* profile) override;

    int electrodeType() const;
    void setElectrodeType(int newElectrodeType);

    Catheter *catheter() const;
    void setCatheter(Catheter *newCatheter);

public slots:
    void reset() override;
    void onEmployChanged();

signals:
    void electrodeTypeChanged();

protected:
    QPointer<Catheter> m_catheter;
    int m_electrodeType = -1;
};

#endif // CATHETERELECTRODENODELISTMODEL_H
