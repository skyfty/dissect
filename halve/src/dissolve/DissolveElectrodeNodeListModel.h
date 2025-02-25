#ifndef DISSOLVEELECTRODENODELISTMODEL_H
#define DISSOLVEELECTRODENODELISTMODEL_H


#include <catheter/ElectrodeNodeListModel.h>
class Profile;
class ReproduceOptions;

class DissolveElectrodeNodeListModel : public ElectrodeNodeListModel
{
    Q_OBJECT
public:
    explicit DissolveElectrodeNodeListModel(QObject *parent = nullptr);
    void setProfile(Profile* profile) override;

protected:
    void reset() override;

private:
    QPointer<ReproduceOptions> m_reproduceOptions;

};

#endif // DISSOLVEELECTRODENODELISTMODEL_H
