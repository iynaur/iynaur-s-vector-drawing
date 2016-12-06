#-------------------------------------------------
#
# Project created by QtCreator 2015-08-22T13:03:16
#
#-------------------------------------------------

QT       += core gui
QT += xml
QMAKE_CXXFLAGS+= -std=gnu++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = draw
TEMPLATE = app
TRANSLATIONS = draw_zh_cn.ts

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
    bottomaction.cpp \
    setbrushaction.cpp \
    slightmoveaction.cpp \
    texteditaction.cpp \
    tabwidget.cpp \
    edittextdialog.cpp \
    mycolordialog.cpp \
    myfontdialog.cpp \
    Label.cpp \
    LineEdit.cpp

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
    polyline.h \
    setbrushaction.h \
    slightmoveaction.h \
    texteditaction.h \
    tabwidget.h \
    edittextdialog.h \
    mycolordialog.h \
    myfontdialog.h \
    Label.h \
    LineEdit.h \
    ruler.h

FORMS    += mainwindow.ui \
    gettextdialog.ui \
    edittextdialog.ui

DISTFILES += log.txt

RESOURCES+=res.qrc

