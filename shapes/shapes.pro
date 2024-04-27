#-------------------------------------------------
#
# Project created by QtCreator 2018-04-09T22:38:47
#
#-------------------------------------------------

QT       += core xml  widgets gui

CONFIG += c++11

TARGET = shapes
TEMPLATE = lib

DEFINES +=  QT_WIDGETS_LIB QT_XML_LIB  SHAPES_LIB



include(shapes.pri)
TRANSLATIONS += shapes_zh.ts

HEADERS += \
    arrow.h

SOURCES += \
    arrow.cpp
