#ifndef FANCYTAB_H
#define FANCYTAB_H

#include <QObject>

#include <QTimer>
#include <QIcon>
#include <QPropertyAnimation>

class FancyTab : public QObject
{
    Q_OBJECT

    Q_PROPERTY(float fader READ fader WRITE setFader)
public:
    FancyTab(QWidget *tabbar) : tabbar(tabbar){
        animator.setPropertyName("fader");
        animator.setTargetObject(this);
    }
    float fader() { return m_fader; }
    void setFader(float value);

    void fadeIn();
    void fadeOut();

    bool fromMap(const QVariantMap& map);

    int   m_nButtonID = -1;
    QIcon icon;
    QString text;
    QString toolTip;
    QString m_strButtonFun = "";
    bool enabled = false;
    bool hasSecond = false;

private:
    QPropertyAnimation animator;
    QWidget *tabbar;
    float m_fader = 0;
};

#endif // FANCYTAB_H
