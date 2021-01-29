#include "ToolPanel.h"

#include "ButtonPanelBase.h"
#include "ToolPanelPrivate.h"

#include <QVBoxLayout>

#define SecondPanelMarginWith  20

ToolPanel::ToolPanel(QWidget *parent)
    : QWidget(parent)
    , m_pImpl(new(std::nothrow) ToolPanelPrivate)
{
    setMinimumSize(30,100);

    m_pImpl->m_vtrpButtonPanel.fill(nullptr, PanelType_Size);

    ButtonPanelBase* pButtonPanel = new ButtonPanel(this);
    pButtonPanel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    pButtonPanel->setMinimumSize(this->width(), this->height());
    m_pImpl->m_vtrpButtonPanel[PanelType_Button] = pButtonPanel;

    ButtonPanelBase* pSecondButtonPanel = new ButtonSecondPanel(parent);
    pSecondButtonPanel->setMinimumSize(pButtonPanel->width() + SecondPanelMarginWith, pButtonPanel->height());
    m_pImpl->m_vtrpButtonPanel[PanelType_SecondButton] = pSecondButtonPanel;


    QVBoxLayout* pVLayout = new(std::nothrow) QVBoxLayout;
    pVLayout->addWidget(pButtonPanel);

    pVLayout->setSpacing(0);
    pVLayout->setContentsMargins(0, 0, 0 ,0);
    setLayout(pVLayout);


    connect(pButtonPanel, &ButtonPanelBase::currentChanged, [](int nIndex){
        qDebug() <<"currentChanged" << nIndex;
    });
    connect(pButtonPanel, &ButtonPanelBase::secondTriggered, [this](int nIndex){

        ButtonPanelBase* pButtonPanel = m_pImpl->getButtonPanel(PanelType_Button);
        ButtonPanelBase* pSecondButtonPanel = m_pImpl->getButtonPanel(PanelType_SecondButton);

        updatePanelSize();

        pSecondButtonPanel->updateData(m_pImpl->getSecondPanelData(nIndex));

        QPoint ptBase = pButtonPanel->pos();
        ptBase = mapToGlobal(ptBase);

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
