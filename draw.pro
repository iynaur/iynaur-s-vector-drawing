#-------------------------------------------------
#
# Project created by QtCreator 2015-08-22T13:03:16
#
#-------------------------------------------------

QT       += core gui
QT += xml


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = draw
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    abstractshape.cpp \
    curve.cpp \
    drawareawidget.cpp \
    Polyline.cpp \
    Polygon.cpp \
    circle.cpp \
    mymath.cpp \
    rect.cpp \
    ellipse.cpp \
    text.cpp \
    gettextdialog.cpp \
    closecurve.cpp \
    generalshape.cpp \
    combo.cpp \
    scroll.cpp \
    abstractaction.cpp \
    addaction.cpp \
    deleteaction.cpp \
    editaction.cpp \
    combineaction.cpp \
    divideaction.cpp \
    topaction.cpp \
    bottomaction.cpp

HEADERS  += mainwindow.h \
    abstractshape.h \
    curve.h \
    drawareawidget.h \
    circle.h \
    mymath.h \
    rect.h \
    ellipse.h \
    text.h \
    gettextdialog.h \
    closecurve.h \
    generalshape.h \
    combo.h \
    scroll.h \
    abstractaction.h \
    addaction.h \
    deleteaction.h \
    editaction.h \
    combineaction.h \
    divideaction.h \
    topaction.h \
    bottomaction.h \
    polygon.h \
    polyline.h

FORMS    += mainwindow.ui \
    gettextdialog.ui

DISTFILES += \
    log.txt

RESOURCES+=res.qrc\

