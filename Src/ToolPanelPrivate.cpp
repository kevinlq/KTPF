#include "ToolPanelPrivate.h"

#include <QJsonDocument>

DS_ButtonData::DS_ButtonData()
{
    reset();
}

DS_ButtonData::~DS_ButtonData()
{
    reset();
}

void DS_ButtonData::reset()
{
    qDeleteAll(m_vtrpButtonData);
    m_vtrpButtonData.clear();
}

QVariantMap DS_ButtonData::toMap()
{
    QVariantList lschildren;
    for (int i = 0; i < m_vtrpButtonData.size(); i++)
    {
        lschildren.push_back(m_vtrpButtonData[i]->toMap());
    }

    QVariantMap map = {
        {"buttonID", m_nButtonID}, {"buttonName", m_strButtonName}, {"buttonTip", m_strButtonTip}, {"buttonIcon", m_strButtonIcon},
        {"buttonFun", m_strButtonFun}, {"param", m_strButtonParam}, {"second", lschildren}
    };

    return map;
}

bool DS_ButtonData::fromMap(const QVariantMap &map)
{
    if (map.isEmpty()
            || !map.contains("buttonID")
            || !map.contains("buttonName")
            || !map.contains("buttonTip")
            || !map.contains("buttonFun")
            || !map.contains("param"))
    {
        return false;
    }

    m_nButtonID         = map.value("buttonID").toInt();
    m_strButtonName     = map.value("buttonName").toString();
    m_strButtonTip      = map.value("buttonTip").toString();
    m_strButtonIcon     = map.value("buttonIcon").toString();
    m_strButtonFun      = map.value("buttonFun").toString();
    m_strButtonParam    = map.value("param").toString();

    QVariantList lsChildren = map.value("second").toList();

    if (lsChildren.isEmpty())
    {
        return true;
    }

    for (int i = 0; i < lsChildren.size(); i++)
    {
        QVariantMap subMap = lsChildren[i].toMap();

        if (subMap.isEmpty())
        {
            continue;
        }

        DS_ButtonData* pData = new(std::nothrow) DS_ButtonData;

        if (pData->fromMap(subMap))
        {
            m_vtrpButtonData.push_back(pData);
        }
        else
        {
            delete pData;
            pData = nullptr;
        }
    }

    return true;
}

QVariantList DS_ButtonData::geetChildrenData()
{
    QVariantList lschildren;
    for (int i = 0; i < m_vtrpButtonData.size(); i++)
    {
        lschildren.push_back(m_vtrpButtonData[i]->toMap());
    }

    return lschildren;
}


ToolPanelPrivate::ToolPanelPrivate()
{
    m_vtrpButtonPanel.clear();
}

ToolPanelPrivate::~ToolPanelPrivate()
{
    qDeleteAll(m_vtrpButtonPanel);
    m_vtrpButtonPanel.clear();
}

bool ToolPanelPrivate::initPanelData(const QString &strData)
{
    QJsonDocument doc = QJsonDocument::fromJson(strData.toLocal8Bit());

    if (doc.isEmpty())
    {
        qDebug() << "doc is empty " << doc;
        return false;
    }

    QVariantList lsData = doc.toVariant().toList();

    for (int i = 0; i < lsData.size(); i++)
    {
        QVariantMap map = lsData[i].toMap();

        DS_ButtonData* pButtonDataPtr = new(std::nothrow) DS_ButtonData;

        if (pButtonDataPtr->fromMap(map))
        {
            m_vtrpButtonData.push_back(pButtonDataPtr);
        }
        else
        {
            delete pButtonDataPtr;
            pButtonDataPtr = nullptr;
        }
    }

    return lsData.isEmpty() ? false : true;
}

ButtonPanelBase *ToolPanelPrivate::getButtonPanel(PanelType eType)
{
    if (eType < 0 || eType >= m_vtrpButtonPanel.size())
    {
        return nullptr;
    }

    return m_vtrpButtonPanel[eType];
}

QVariantList ToolPanelPrivate::getSecondPanelData(int nIndex)
{
    // 这里确保 index 和 nButtonID一致
    DS_ButtonData* pButtonDataPtr = getButtonDataPtr(nIndex);

    if (nullptr == pButtonDataPtr)
    {
        return QVariantList();
    }

    return pButtonDataPtr->geetChildrenData();
}

DS_ButtonData *ToolPanelPrivate::getButtonDataPtr(int nButtonID)
{
    vtrpButtonData::iterator iter = m_vtrpButtonData.begin();

    for (; iter != m_vtrpButtonData.end(); iter++)
    {
        DS_ButtonData* pData = static_cast<DS_ButtonData*>(*iter);

        if (nullptr != pData && pData->m_nButtonID == nButtonID)
        {
            return pData;
        }
    }

    return nullptr;
}

QVariantList ToolPanelPrivate::getButtonPanelData()
{
    QVariantList lsButtonData;

    vtrpButtonData::iterator iter = m_vtrpButtonData.begin();

    for (; iter != m_vtrpButtonData.end(); iter++)
    {
        DS_ButtonData* pData = static_cast<DS_ButtonData*>(*iter);

        if (nullptr != pData)
        {
            lsButtonData.push_back(pData->toMap());
        }
    }

    return lsButtonData;
}
