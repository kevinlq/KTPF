#ifndef TOOLPANEL_H
#define TOOLPANEL_H

#include "KTPF_Export.h"

#include <QWidget>

class ToolPanelPrivate;
class KTPF_EXPORT ToolPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ToolPanel(QWidget *parent = nullptr);
    ~ToolPanel();

    bool initPanelData(const QString &strData);

protected:
    virtual void resizeEvent(QResizeEvent* e);

private:
    void updatePanelSize();
    ToolPanelPrivate*   m_pImpl;
};

#endif // TOOLPANEL_H
