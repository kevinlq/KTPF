#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class ToolPanel;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

private:
    ToolPanel*  m_pToolPane;
};

#endif // MAINWINDOW_H
