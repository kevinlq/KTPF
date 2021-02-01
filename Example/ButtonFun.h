#ifndef BUTTONFUN_H
#define BUTTONFUN_H

#include <QObject>

class ButtonFun : public QObject
{
    Q_OBJECT
public:
    explicit ButtonFun(QObject *parent = nullptr);

public Q_SLOTS:
    bool newFile(const QString &strParam);
    bool saveFile(const QString &strParam);

};

#endif // BUTTONFUN_H
