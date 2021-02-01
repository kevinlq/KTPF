#include "ButtonFun.h"

#include <QDebug>

ButtonFun::ButtonFun(QObject *parent) : QObject(parent)
{
}

bool ButtonFun::newFile(const QString &strParam)
{
    qDebug() << "newFile " << strParam;
    return true;
}

bool ButtonFun::saveFile(const QString &strParam)
{
    qDebug() << "saveFile " << strParam;
    return true;
}
