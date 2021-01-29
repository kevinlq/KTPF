#include "ButtonPanelBasePrivate.h"

#include "FancyTab.h"
#include "ButtonPanelBase.h"

ButtonPanelBasePrivate::ButtonPanelBasePrivate(ButtonPanelBase *q)
    :m_pButtonPanel(q)
{
    m_lsTable.clear();
}

ButtonPanelBasePrivate::~ButtonPanelBasePrivate()
{
    qDeleteAll(m_lsTable);
    m_lsTable.clear();

    m_pButtonPanel = nullptr;
}

FancyTab *ButtonPanelBasePrivate::getButtonTabForIndexPtr(int nIndex)
{
    if (!isValid(nIndex))
    {
        return nullptr;
    }

    return m_lsTable[nIndex];
}

FancyTab *ButtonPanelBasePrivate::getButtonTabPtr(int nButtonID)
{
    ButtonTables::iterator iter = m_lsTable.begin();

    for(; iter != m_lsTable.end(); iter++)
    {
        FancyTab* pTable = static_cast<FancyTab*>(*iter);

        if (nullptr != pTable && pTable->m_nButtonID == nButtonID)
        {
            return pTable;
        }
    }

    return nullptr;
}

bool ButtonPanelBasePrivate::hasSecondButton(int nIndex)
{
    FancyTab* pTab = getButtonTabForIndexPtr(nIndex);

    if (nullptr == pTab)
    {
        return false;
    }

    return pTab->hasSecond;
}

bool ButtonPanelBasePrivate::isValid(int nIndex) const
{
    if (nIndex < 0 || nIndex >= m_lsTable.size())
    {
        return false;
    }

    return true;
}

bool ButtonPanelBasePrivate::setTabEnabled(int index, bool enable)
{
    if (isValid(index))
    {
        m_lsTable[index]->enabled   = enable;
        return true;
    }

    return false;
}

bool ButtonPanelBasePrivate::isTabEnabled(int index) const
{
    if (isValid(index))
    {
        return m_lsTable[index]->enabled;
    }

    return false;
}

void ButtonPanelBasePrivate::setTabToolTip(int index, QString toolTip)
{
    if (isValid(index))
    {
        m_lsTable[index]->toolTip = toolTip;
    }
}

QString ButtonPanelBasePrivate::tabToolTip(int index) const
{
    if (isValid(index))
    {
        return m_lsTable[index]->toolTip;
    }

    return "";
}

bool ButtonPanelBasePrivate::updateData(const QVariantList &lsData)
{
    if (lsData.isEmpty())
    {
        return false;
    }

    for (int i = 0; i < lsData.size(); i++)
    {
        QVariantMap map = lsData[i].toMap();

        FancyTab* pTab = new(std::nothrow)FancyTab(m_pButtonPanel);

        if (pTab->fromMap(map))
        {
            pTab->enabled = true;
            m_lsTable.push_back(pTab);
        }
        else
        {
            delete pTab;
            pTab = nullptr;
        }
    }

    return lsData.isEmpty() ? false : true;
}
