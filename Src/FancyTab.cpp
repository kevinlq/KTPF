#include "FancyTab.h"

#include <QWidget>

void FancyTab::fadeIn()
{
    animator.stop();
    animator.setDuration(80);
    animator.setEndValue(1);
    animator.start();
}

void FancyTab::fadeOut()
{
    animator.stop();
    animator.setDuration(160);
    animator.setEndValue(0);
    animator.start();
}

bool FancyTab::fromMap(const QVariantMap &map)
{
    if (map.isEmpty()
            || !map.contains("buttonID")
            || !map.contains("buttonName")
            || !map.contains("buttonTip")
            || !map.contains("buttonFun"))
    {
        return false;
    }

    m_nButtonID     = map.value("buttonID").toInt();
    icon            = QIcon(map.value("buttonIcon").toString());
    text            = map.value("buttonName").toString();
    toolTip         = map.value("buttonTip").toString();
    m_strButtonFun  = map.value("buttonFun").toString();

    return true;
}

void FancyTab::setFader(float value)
{
    m_fader = value;
    tabbar->update();
}
