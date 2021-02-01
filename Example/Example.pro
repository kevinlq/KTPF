include($$PWD/../KTPF.pri)
include($$PWD/Example.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = Example$${FILE_POSTFIX}

INCLUDEPATH +=$$PWD/
INCLUDEPATH +=$$PWD/../Src/
INCLUDEPATH +=$$PWD/../Src/Include

SOURCES += \
    ButtonFun.cpp \
    MainWindow.cpp \
    main.cpp

HEADERS += \
    ButtonFun.h \
    MainWindow.h

RESOURCES += \
    images.qrc
