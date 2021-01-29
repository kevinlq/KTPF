include($$PWD/../KTPF.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
TARGET = KTPF$${FILE_POSTFIX}

DEFINES += KTPF_LIBRARY

INCLUDEPATH +=$$PWD/
INCLUDEPATH +=$$PWD/Include

HEADERS += \
    ButtonPanelBase.h \
    ButtonPanelBasePrivate.h \
    FancyTab.h \
    Include/KTPF_Export.h \
    ToolPanel.h \
    ToolPanelPrivate.h

SOURCES += \
    ButtonPanelBase.cpp \
    ButtonPanelBasePrivate.cpp \
    FancyTab.cpp \
    ToolPanel.cpp \
    ToolPanelPrivate.cpp

