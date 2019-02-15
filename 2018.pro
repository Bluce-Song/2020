#-------------------------------------------------
#
# Project created by QtCreator 2019-02-14T14:08:33
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2018
TEMPLATE = app


SOURCES += main.cpp\
        sql.cpp

HEADERS  += sql.h

FORMS    += sql.ui
DEFINES  += QT_MESSAGELOGCONTEXT

RESOURCES += \
    image.qrc
