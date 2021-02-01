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
    m_nButtonID = -1;
    m_strButtonName = "";
    m_strButtonTip  = "";
    m_strButtonFun  = "";
    m_strButtonParam = "";
    m_bEnable   = false;

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
        {"buttonFun", m_strButtonFun}, {"param", m_strButtonParam}, {"buttonEnable", m_bEnable},{"second", lschildren}
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
    m_bEnable           = map.value("buttonEnable").toBool();

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

DS_ButtonData *DS_ButtonData::getChildrenPtr(int nButtonID)
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


ToolPanelPrivate::ToolPanelPrivate()
{
    m_vtrpButtonPanel.clear();
}

ToolPanelPrivate::~ToolPanelPrivate()
{
    qDeleteAll(m_vtrpButtonPanel);
    m_vtrpButtonPanel.clear();

    m_pRunFunObj = nullptr;
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

void ToolPanelPrivate::onButtonClick(int nPanelType, int nParentBtnID, int nButtonID)
{
    DS_ButtonData* pButtonPanelData = nullptr;

    if (nPanelType == PanelType_Button)
    {
        pButtonPanelData = getButtonDataPtr(nButtonID);
    }
    else if (nPanelType == PanelType_SecondButton)
    {
        pButtonPanelData = getButtonSecondDataPtr(nParentBtnID, nButtonID);
    }

    if (nullptr == pButtonPanelData)
    {
        qWarning() << "faile find buttonData ... " << nPanelType << nParentBtnID << nButtonID;
        return;
    }

    if (nullptr == m_pRunFunObj)
    {
        qWarning() << "pRunFunObj is null you may forget set runObj";
        return;
    }

    QByteArray baFun = pButtonPanelData->m_strButtonFun.toLocal8Bit();
    bool bResult = false;

    QMetaObject::invokeMethod(m_pRunFunObj,
                              baFun.data(),
                              Qt::DirectConnection,
                              Q_RETURN_ARG(bool, bResult),
                              Q_ARG(const QString, pButtonPanelData->m_strButtonParam));
}

ButtonPanelBase *ToolPanelPrivate::getButtonPanel(PanelType eType)
{
    if (eType < 0 || eType >= m_vtrpButtonPanel.size())
    {
        return nullptr;
    }

    return m_vtrpButtonPanel[eType];
}

QVariantList ToolPanelPrivate::getSecondPanelData(int nButtonID)
{
    DS_ButtonData* pButtonDataPtr = getButtonDataPtr(nButtonID);

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

DS_ButtonData *ToolPanelPrivate::getButtonSecondDataPtr(int nParentBtnID, int nButtonID)
{
    DS_ButtonData* pButtonData = getButtonDataPtr(nParentBtnID);

    if (nullptr == pButtonData)
    {
        return nullptr;
    }

    return pButtonData->getChildrenPtr(nButtonID);
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
