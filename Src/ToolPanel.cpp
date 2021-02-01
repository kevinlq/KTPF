#include "ToolPanel.h"

#include "ButtonPanelBase.h"
#include "ToolPanelPrivate.h"

#include <QVBoxLayout>

#define SecondPanelMarginWith  10

ToolPanel::ToolPanel(QWidget *parent)
    : QWidget(parent)
    , m_pImpl(new(std::nothrow) ToolPanelPrivate)
{
    setMinimumSize(30,100);
    setMaximumWidth(70);

    m_pImpl->m_vtrpButtonPanel.fill(nullptr, PanelType_Size);

    ButtonPanelBase* pButtonPanel = new ButtonPanel(this);
    pButtonPanel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    pButtonPanel->setMinimumSize(this->width(), this->height());
    m_pImpl->m_vtrpButtonPanel[PanelType_Button] = pButtonPanel;

    ButtonPanelBase* pSecondButtonPanel = new ButtonSecondPanel(parent);
    pSecondButtonPanel->setMinimumSize(pButtonPanel->width() + SecondPanelMarginWith, pButtonPanel->height());
    m_pImpl->m_vtrpButtonPanel[PanelType_SecondButton] = pSecondButtonPanel;
    pSecondButtonPanel->setVisible(false);


    QVBoxLayout* pVLayout = new(std::nothrow) QVBoxLayout;
    pVLayout->addWidget(pButtonPanel);

    pVLayout->setSpacing(0);
    pVLayout->setContentsMargins(0, 0, 0 ,0);
    setLayout(pVLayout);


    connect(pButtonPanel, &ButtonPanelBase::currentChanged, [this](int nButtonID){
        ButtonPanelBase* pSecondButtonPanel = m_pImpl->getButtonPanel(PanelType_SecondButton);
        pSecondButtonPanel->setVisible(false);
        m_pImpl->onButtonClick(PanelType_Button, -1, nButtonID);
    });
    connect(pSecondButtonPanel, &ButtonPanelBase::currentChanged, [this](int nButtonID){
        ButtonPanelBase* pSecondButtonPanel = m_pImpl->getButtonPanel(PanelType_SecondButton);

        m_pImpl->onButtonClick(PanelType_SecondButton, pSecondButtonPanel->getParentButtonID(),nButtonID);
    });

    connect(pButtonPanel, &ButtonPanelBase::secondTriggered, [this](int nButtonID){

        ButtonPanelBase* pButtonPanel = m_pImpl->getButtonPanel(PanelType_Button);
        ButtonPanelBase* pSecondButtonPanel = m_pImpl->getButtonPanel(PanelType_SecondButton);
        pSecondButtonPanel->setParentButtonID(nButtonID);

        updatePanelSize();

        pSecondButtonPanel->updateData(m_pImpl->getSecondPanelData(nButtonID));

        QPoint ptBase = pButtonPanel->pos();
        //ptBase = mapToGlobal(ptBase);

        QPoint ptMove(ptBase.x() + pButtonPanel->width(), ptBase.y());
        pSecondButtonPanel->move(ptMove);

        pSecondButtonPanel->setVisible(true);
    });
}

ToolPanel::~ToolPanel()
{
    if (nullptr != m_pImpl)
    {
        delete m_pImpl;
        m_pImpl = nullptr;
    }
}

bool ToolPanel::initPanelData(const QString &strData)
{
    bool bResult = m_pImpl->initPanelData(strData);

    if (bResult)
    {
        // 数据加载成功，刷新面板
        ButtonPanelBase* pButtonPanel = m_pImpl->getButtonPanel(PanelType_Button);

        bResult = pButtonPanel->updateData(m_pImpl->getButtonPanelData());
    }

    return bResult;
}

void ToolPanel::setRunFunObj(QObject *pObj)
{
    m_pImpl->m_pRunFunObj = pObj;
}

void ToolPanel::resizeEvent(QResizeEvent *e)
{
    updatePanelSize();

    QWidget::resizeEvent(e);
}

void ToolPanel::updatePanelSize()
{
    ButtonPanelBase* pButtonPanel = m_pImpl->getButtonPanel(PanelType_Button);
    ButtonPanelBase* pSecondButtonPanel = m_pImpl->getButtonPanel(PanelType_SecondButton);

    pSecondButtonPanel->setFixedSize(pButtonPanel->width() + SecondPanelMarginWith, pButtonPanel->height());
}
