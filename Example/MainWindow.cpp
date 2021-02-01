#include "MainWindow.h"

#include "ToolPanel.h"
#include "ButtonFun.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(800, 600);

    m_pToolPane = new(std::nothrow) ToolPanel(this);
    m_pToolPane->setMinimumSize(30, this->height());
    m_pToolPane->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    m_pToolPane->setMouseTracking(true);

    m_pToolPane->setRunFunObj(new(std::nothrow) ButtonFun);

    QHBoxLayout* pHLayout = new QHBoxLayout;
    pHLayout->addWidget(m_pToolPane);
    pHLayout->addStretch();
    pHLayout->setSpacing(0);
    pHLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pHLayout);

    QFile file(QString("%1/ToolPanelData.json").arg(LIB_DIR_PATH));

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open file fail.." << file.errorString();
    }
    else
    {
        QByteArray baData = file.readAll();
        file.close();

        bool bResult = m_pToolPane->initPanelData(QString(baData));

        qDebug() << "load result " << bResult;
    }
}
