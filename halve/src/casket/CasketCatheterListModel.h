#ifndef CASKETCATHETERLISTMODEL_H
#define CASKETCATHETERLISTMODEL_H

#include <catheter/CatheterListModel.h>

class CasketCatheterListModel : public CatheterListModel
{
    Q_OBJECT
public:
    explicit CasketCatheterListModel(QObject *parent = nullptr);


protected:
    virtual void reset();

    QPointer<Catheter> m_selectCatheter;

};

#endif // CASKETCATHETERLISTMODEL_H
