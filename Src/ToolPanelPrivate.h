#ifndef TOOLPANELPRIVATE_H
#define TOOLPANELPRIVATE_H

#include <QObject>
#include "ButtonPanelBase.h"

enum PanelType{
    PanelType_Button = 0,
    PanelType_SecondButton,
    PanelType_Size
};

struct DS_ButtonData;

using vtrpButtonPanel = QVector<ButtonPanelBase*>;
using vtrpButtonData = QVector<DS_ButtonData*>;

struct DS_ButtonData
{
    int         m_nButtonID;
    QString     m_strButtonName;
    QString     m_strButtonTip;
    QString     m_strButtonIcon;
    QString     m_strButtonFun;
    QString     m_strButtonParam;
    bool        m_bEnable;

    DS_ButtonData();
    ~DS_ButtonData();

    void reset();

    QVariantMap toMap();
    bool        fromMap(const QVariantMap &map);
    QVariantList geetChildrenData();
    DS_ButtonData* getChildrenPtr(int nButtonID);

    vtrpButtonData  m_vtrpButtonData;
};

class ToolPanelPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ToolPanelPrivate();
    ~ToolPanelPrivate();

    bool initPanelData(const QString &strData);

    void onButtonClick(int nPanelType, int nParentBtnID,int nButtonID);

public:
    ButtonPanelBase* getButtonPanel(PanelType eType);
    QVariantList getSecondPanelData(int nButtonID);

    DS_ButtonData* getButtonDataPtr(int nButtonID);
    DS_ButtonData* getButtonSecondDataPtr(int nParentBtnID,int nButtonID);
    QVariantList getButtonPanelData();

    vtrpButtonPanel   m_vtrpButtonPanel;
    vtrpButtonData  m_vtrpButtonData;

    QObject*        m_pRunFunObj = nullptr;
};

#endif // TOOLPANELPRIVATE_H
