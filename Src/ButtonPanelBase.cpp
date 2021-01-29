#include "ButtonPanelBase.h"

#include "ButtonPanelBasePrivate.h"
#include "FancyTab.h"

#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>
#include <QStyleOption>
#include <QPixmapCache>
#include <QApplication>
#include <QtMath>

const int ButtonPanelBase::m_rounding = 22;
const int ButtonPanelBase::m_textPadding = 4;
static const int kMenuButtonWidth = 16;


// Note, this is exported but in a private header as qtopengl depends on it.
// We should consider adding this as a public helper function.
void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);

ButtonPanelBase::ButtonPanelBase(QWidget *parent)
    : QWidget(parent), m_pImpl(new(std::nothrow) ButtonPanelBasePrivate(this))
{
}


ButtonPanelBase::ButtonPanelBase(ButtonPanelBasePrivate &d,QWidget *parent)
    : QWidget(parent),m_pImpl(&d)
{
}


ButtonPanelBase::~ButtonPanelBase()
{
    if (nullptr != m_pImpl)
    {
        delete m_pImpl;
        m_pImpl = nullptr;
    }
}

bool ButtonPanelBase::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip)
    {
        if (validIndex(m_nHoverIndex))
        {
            QString tt = tabToolTip(m_nHoverIndex);
            if (!tt.isEmpty())
            {
                QToolTip::showText(static_cast<QHelpEvent*>(event)->globalPos(), tt, this);
                return true;
            }
        }
    }
    return QWidget::event(event);
}

void ButtonPanelBase::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    p.fillRect(event->rect(), QColor(255,200,200, 200));

    for (int i = 0; i < count(); ++i)
    {
        if (i != currentIndex())
        {
            drawButtonTab(&p, i);
        }
    }

    // paint active tab last, since it overlaps the neighbors
    if (currentIndex() != -1)
    {
        drawButtonTab(&p, currentIndex());
    }
}

void ButtonPanelBase::mousePressEvent(QMouseEvent *e)
{
    e->accept();

    for (int index = 0; index < count(); ++index)
    {
        const QRect rect = tabRect(index);
        if (rect.contains(e->pos()))
        {
            if (isTabEnabled(index))
            {
                if (m_pImpl->hasSecondButton(index))
                {
                    // show secondpanel
                    emit secondTriggered(index);
                }
                else
                {
                    m_nCurrentIndex = index;
                    update();

                    // update tab bar before showing widget
                    QTimer::singleShot(0, this, [this]() { emit currentChanged(m_nCurrentIndex); });
                }
            }
            break;
        }
    }
}

void ButtonPanelBase::mouseMoveEvent(QMouseEvent *e)
{
    int newHover = -1;
    for (int i = 0; i < count(); ++i)
    {
        QRect area = tabRect(i);
        if (area.contains(e->pos()))
        {
            newHover = i;
            break;
        }
    }

    if (newHover == m_nHoverIndex)
        return;

    if (validIndex(m_nHoverIndex))
    {
        m_pImpl->getButtonTabForIndexPtr(m_nHoverIndex)->fadeOut();
    }

    m_nHoverIndex = newHover;

    if (validIndex(m_nHoverIndex))
    {
        m_pImpl->getButtonTabForIndexPtr(m_nHoverIndex)->fadeIn();

        m_hoverRect = tabRect(m_nHoverIndex);
    }
}

void ButtonPanelBase::enterEvent(QEvent *e)
{
    Q_UNUSED(e)
    m_hoverRect = QRect();
    m_nHoverIndex = -1;
}

void ButtonPanelBase::leaveEvent(QEvent *e)
{
    Q_UNUSED(e)

    m_nHoverIndex = -1;
    m_hoverRect = QRect();

    for (int i = 0 ; i < count() ; ++i)
    {
        m_pImpl->getButtonTabForIndexPtr(i)->fadeOut();
    }
}

void ButtonPanelBase::drawButtonTab(QPainter *painter, int tabIndex)
{
    if (!validIndex(tabIndex))
    {
        qWarning("invalid index");
        return;
    }

    painter->save();

    FancyTab *tab = m_pImpl->getButtonTabForIndexPtr(tabIndex);
    QRect rect = tabRect(tabIndex);
    bool selected = (tabIndex == m_nCurrentIndex);
    bool enabled = isTabEnabled(tabIndex);

    if (selected)
    {
        painter->fillRect(rect, QColor(34,34,34));
    }

    QString tabText(tab->text);
    QRect tabTextRect(rect);
    const bool drawIcon = rect.height() > 36;
    QRect tabIconRect(tabTextRect);
    tabTextRect.translate(0, drawIcon ? -2 : 1);

    QFont boldFont(painter->font());
    boldFont.setPointSizeF(8);
    boldFont.setBold(true);
    painter->setFont(boldFont);
    painter->setPen(selected ? QColor(255, 255, 255, 160) : QColor(0, 0, 0, 110));
    const int textFlags = Qt::AlignCenter | (drawIcon ? Qt::AlignBottom : Qt::AlignVCenter) | Qt::TextWordWrap;

    const float fader = tab->fader();

    if (fader > 0 /*&& !HostOsInfo::isMacHost()*/ && !selected && enabled)
    {
        painter->save();

        painter->setOpacity(fader);

        painter->fillRect(rect, QColor("28ffffff"));

        painter->restore();
    }

    if (!enabled)
    {
        painter->setOpacity(0.7);
    }

    if (drawIcon)
    {
        int textHeight = painter->fontMetrics().boundingRect(QRect(0, 0, width(), height()), Qt::TextWordWrap, tabText).height();
        tabIconRect.adjust(0, 4, 0, -textHeight);
        const QIcon::Mode iconMode = enabled ? (selected ? QIcon::Active : QIcon::Normal)
                                             : QIcon::Disabled;
        drawIconWithShadow(tab->icon, tabIconRect, painter, iconMode);
    }

    painter->setOpacity(1.0); //FIXME: was 0.7 before?

    if (enabled)
    {
        painter->setPen(QColor("b6fbfdff"));
    }
    else
    {
        painter->setPen(QColor("56a5a6a7"));
    }

    painter->translate(0, -1);
    painter->drawText(tabTextRect, textFlags, tabText);

    // menu arrow
    if (tab->hasSecond)
    {
        QStyleOption opt;
        opt.initFrom(this);
        opt.rect = rect.adjusted(rect.width() - kMenuButtonWidth, 0, -8, 0);

        //StyleHelper::drawArrow(QStyle::PE_IndicatorArrowRight, painter, &opt);
    }

    painter->restore();
}

bool ButtonPanelBase::updateData(const QVariantList &lsData)
{
    return m_pImpl->updateData(lsData);
}

int ButtonPanelBase::count() const
{
    return m_pImpl->m_lsTable.count();
}

QRect ButtonPanelBase::tabRect(int index) const
{
    QSize sh = tabSizeHint();

    if (sh.height() * count() > height())
    {
        sh.setHeight(height() / count());
    }

    return QRect(0, index * sh.height(), sh.width(), sh.height());
}

QSize ButtonPanelBase::tabSizeHint(bool minimum) const
{
    QFont boldFont(font());
    boldFont.setPointSizeF(8);
    boldFont.setBold(true);

    QFontMetrics fm(boldFont);
    int spacing = 8;
    int width = 60 + spacing + 2;
    int maxLabelwidth = 0;

    for (int tab = 0 ; tab < count() ;++tab)
    {
        int width = fm.width(m_pImpl->getButtonTabForIndexPtr(tab)->text);
        if (width > maxLabelwidth)
        {
            maxLabelwidth = width;
        }
    }

    int iconHeight = minimum ? 0 : 32;
    return QSize(qMax(width, maxLabelwidth + 4), iconHeight + spacing + fm.height());
}

void ButtonPanelBase::setCurrentIndex(int index)
{
    m_nCurrentIndex = index;
}

int ButtonPanelBase::currentIndex() const
{
    return m_nCurrentIndex;
}

QSize ButtonPanelBase::sizeHint() const
{
    QSize sh = tabSizeHint();

    return QSize(sh.width(), sh.height() * count());
}

QSize ButtonPanelBase::minimumSizeHint() const
{
    QSize sh = tabSizeHint();

    return QSize(sh.width(), sh.height() * count());
}

void ButtonPanelBase::setTabEnabled(int index, bool enable)
{
    if (m_pImpl->setTabEnabled(index, enable))
    {
        update(tabRect(index));
    }
}

bool ButtonPanelBase::isTabEnabled(int index) const
{
    return m_pImpl->isTabEnabled(index);
}

bool ButtonPanelBase::validIndex(int index) const
{
    return m_pImpl->isValid(index);
}

void ButtonPanelBase::setTabToolTip(int index, QString toolTip)
{
    m_pImpl->setTabToolTip(index, toolTip);
}

QString ButtonPanelBase::tabToolTip(int index) const
{
    return m_pImpl->tabToolTip(index);
}

void ButtonPanelBase::drawIconWithShadow(const QIcon &icon, const QRect &rect, QPainter *p
                                         , QIcon::Mode iconMode, int dipRadius
                                         , const QColor &color, const QPoint &dipOffset)
{
    QPixmap cache;
    QString pixmapName = QString::fromLatin1("icon %0 %1 %2").arg(icon.cacheKey()).arg(iconMode).arg(rect.height());

    if (!QPixmapCache::find(pixmapName, cache))
    {
        // High-dpi support: The in parameters (rect, radius, offset) are in
        // device-independent pixels. The call to QIcon::pixmap() below might
        // return a high-dpi pixmap, which will in that case have a devicePixelRatio
        // different than 1. The shadow drawing caluculations are done in device
        // pixels.
        QWindow *window = QApplication::allWidgets().first()->windowHandle();
        QPixmap px = icon.pixmap(window, rect.size(), iconMode);
        int devicePixelRatio = qCeil(px.devicePixelRatio());
        int radius = dipRadius * devicePixelRatio;
        QPoint offset = dipOffset * devicePixelRatio;
        cache = QPixmap(px.size() + QSize(radius * 2, radius * 2));
        cache.fill(Qt::transparent);

        QPainter cachePainter(&cache);
        if (iconMode == QIcon::Disabled)
        {
            const bool hasDisabledState = icon.availableSizes(QIcon::Disabled).contains(px.size());
            if (!hasDisabledState)
                px = disabledSideBarIcon(icon.pixmap(window, rect.size()));
        }
        else if (false/*creatorTheme()->flag(Theme::ToolBarIconShadow)*/)
        {
            // Draw shadow
            QImage tmp(px.size() + QSize(radius * 2, radius * 2 + 1), QImage::Format_ARGB32_Premultiplied);
            tmp.fill(Qt::transparent);

            QPainter tmpPainter(&tmp);
            tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
            tmpPainter.drawPixmap(QRect(radius, radius, px.width(), px.height()), px);
            tmpPainter.end();

            // blur the alpha channel
            QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
            blurred.fill(Qt::transparent);
            QPainter blurPainter(&blurred);
            qt_blurImage(&blurPainter, tmp, radius, false, true);
            blurPainter.end();

            tmp = blurred;

            // blacken the image...
            tmpPainter.begin(&tmp);
            tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            tmpPainter.fillRect(tmp.rect(), color);
            tmpPainter.end();

            tmpPainter.begin(&tmp);
            tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            tmpPainter.fillRect(tmp.rect(), color);
            tmpPainter.end();

            // draw the blurred drop shadow...
            cachePainter.drawImage(QRect(0, 0, cache.rect().width(), cache.rect().height()), tmp);
        }

        // Draw the actual pixmap...
        cachePainter.drawPixmap(QRect(QPoint(radius, radius) + offset, QSize(px.width(), px.height())), px);
        cachePainter.end();
        cache.setDevicePixelRatio(devicePixelRatio);

        QPixmapCache::insert(pixmapName, cache);
    }

    QRect targetRect = cache.rect();
    targetRect.setSize(targetRect.size() / cache.devicePixelRatio());
    targetRect.moveCenter(rect.center() - dipOffset);

    p->drawPixmap(targetRect, cache);
}

QPixmap ButtonPanelBase::disabledSideBarIcon(const QPixmap &enabledicon)
{
    QImage im = enabledicon.toImage().convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < im.height(); ++y)
    {
        QRgb *scanLine = reinterpret_cast<QRgb*>(im.scanLine(y));
        for (int x = 0; x < im.width(); ++x)
        {
            QRgb pixel = *scanLine;
            char intensity = char(qGray(pixel));
            *scanLine = qRgba(intensity, intensity, intensity, qAlpha(pixel));
            ++scanLine;
        }
    }
    return QPixmap::fromImage(im);
}

ButtonPanel::ButtonPanel(QWidget *parent)
    :ButtonPanelBase(parent)
{
}

ButtonPanel::~ButtonPanel()
{
}

ButtonSecondPanel::ButtonSecondPanel(QWidget *parent)
    :ButtonPanelBase(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);

    setMouseTracking(true);
}

ButtonSecondPanel::~ButtonSecondPanel()
{
}

void ButtonSecondPanel::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    //QPainter painter(this);

    //painter.fillRect(this->rect(), QColor(64,65,66, 80));
}

void ButtonSecondPanel::enterEvent(QEvent *)
{
}

void ButtonSecondPanel::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->setVisible(false);
}
