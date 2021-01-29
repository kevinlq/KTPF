#ifndef BUTTONPANELBASE_H
#define BUTTONPANELBASE_H

#include <QWidget>
#include <QIcon>
#include <QVariantList>
#include <QDebug>

class ButtonPanelBasePrivate;

class ButtonPanelBase : public QWidget
{
    Q_OBJECT
public:
    explicit ButtonPanelBase(QWidget *parent = nullptr);
    virtual ~ButtonPanelBase();

protected:
    virtual bool event(QEvent *event);
    virtual void paintEvent(QPaintEvent*);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

    void drawButtonTab(QPainter* painter, int tabIndex);

public:
    // 从数组中加载数据
    bool updateData(const QVariantList &lsData);


    int count() const ;
    QRect tabRect(int index) const;
    QSize tabSizeHint(bool minimum = false) const;

    void setCurrentIndex(int index);
    int currentIndex() const;

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void setTabEnabled(int index, bool enable);
    bool isTabEnabled(int index) const;

    bool validIndex(int index) const;

    void setTabToolTip(int index, QString toolTip);
    QString tabToolTip(int index) const;

private:
    void drawIconWithShadow(const QIcon &icon, const QRect &rect, QPainter *p, QIcon::Mode iconMode,
                            int dipRadius = 3, const QColor &color = QColor(0, 0, 0, 130),
                            const QPoint &dipOffset = QPoint(1, -2));
    QPixmap disabledSideBarIcon(const QPixmap &enabledicon);

Q_SIGNALS:
    void currentChanged(int index);
    void secondTriggered(int index);

protected:
    ButtonPanelBase(ButtonPanelBasePrivate& d,QWidget *parent = nullptr);
    ButtonPanelBasePrivate* m_pImpl;

    static const int m_rounding;
    static const int m_textPadding;

    QRect m_hoverRect;
    int m_nHoverIndex = -1;
    int m_nCurrentIndex = -1;
};

/*================================= ButtonPanel=============================================*/

class ButtonPanel : public ButtonPanelBase
{
    Q_OBJECT
public:
    explicit ButtonPanel(QWidget *parent = nullptr);
    ~ButtonPanel();
};


/*================================= ButtonPanel=============================================*/
class ButtonSecondPanel : public ButtonPanelBase
{
    Q_OBJECT
public:
    explicit ButtonSecondPanel(QWidget *parent = nullptr);
    ~ButtonSecondPanel();

protected:
    virtual void paintEvent(QPaintEvent*);

    virtual void enterEvent(QEvent*);
    virtual void leaveEvent(QEvent*);
};

#endif // BUTTONPANELBASE_H
