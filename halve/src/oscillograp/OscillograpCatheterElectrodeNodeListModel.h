#ifndef OSCILLOGRAPCATHETERELECTRODENODELISTMODEL_H
#define OSCILLOGRAPCATHETERELECTRODENODELISTMODEL_H

#include "catheter/CatheterElectrodeNodeListModel.h"

class OscillograpElectrode;

class OscillograpCatheterElectrodeNodeListModel : public CatheterElectrodeNodeListModel
{
    Q_OBJECT
public:
    explicit OscillograpCatheterElectrodeNodeListModel(QObject *parent = nullptr);
    void setProfile(Profile* profile) override;
public slots:
    void reset() override;

signals:
    void oscillograpElectrodeChanged();

private:
    QPointer<OscillograpElectrode> m_oscillograpElectrode;
};

#endif // OSCILLOGRAPCATHETERELECTRODENODELISTMODEL_H
