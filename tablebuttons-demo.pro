QT       += core gui widgets

TARGET = tablebuttons-demo
TEMPLATE = app

include(tablebuttons/tablebuttons.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui
