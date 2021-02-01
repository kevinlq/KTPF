#ifndef BUTTONPANELBASEPRIVATE_H
#define BUTTONPANELBASEPRIVATE_H

#include <QObject>

class FancyTab;
class ButtonPanelBase;

class ButtonPanelBasePrivate : public QObject
{
    Q_OBJECT
public:
    explicit ButtonPanelBasePrivate(ButtonPanelBase*q);
    ~ButtonPanelBasePrivate();

    FancyTab* getButtonTabForIndexPtr(int nIndex);
    FancyTab* getButtonTabPtr(int nButtonID);

    int       getButtonID(int nIndex);

    bool hasSecondButton(int nIndex);

    bool isValid(int nIndex) const;

    bool setTabEnabled(int index, bool enable);
    bool isTabEnabled(int index) const;

    void setTabToolTip(int index, QString toolTip);
    QString tabToolTip(int index) const;

    bool updateData(const QVariantList &lsData);

    bool resetTabData();

public:
    using ButtonTables = QList<FancyTab*>;
    ButtonTables m_lsTable;
    ButtonPanelBase*  m_pButtonPanel;
};

#endif // BUTTONPANELBASEPRIVATE_H
