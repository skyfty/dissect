#include "CasketCatheterListModel.h"
#include "catheter/CatheterDb.h"

#include <catheter/Catheter.h>

CasketCatheterListModel::CasketCatheterListModel(QObject *parent)
    : CatheterListModel{parent}
{
    m_selectCatheter = new Catheter(this);
    m_selectCatheter->setName("请选择");
    m_selectCatheter->setId("SELECTED");
}

void CasketCatheterListModel::reset()
{
    m_catheters = m_catheterDb->getDatas();
    m_catheters.insert(0, m_selectCatheter);
}
