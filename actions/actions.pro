# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = lib
TARGET = actions
CONFIG += c++11

QT += core xml widgets gui

DEFINES +=  QT_WIDGETS_LIB QT_XML_LIB ACTIONS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./../shapes \
    ./GeneratedFiles/Debug
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(actions.pri)
TRANSLATIONS += actions_zh.ts

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../shapes/release/ -lshapes
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../shapes/debug/ -lshapes
else:unix: LIBS += -L$$OUT_PWD/../shapes/ -lshapes

INCLUDEPATH += $$PWD/../shapes
DEPENDPATH += $$PWD/../shapes