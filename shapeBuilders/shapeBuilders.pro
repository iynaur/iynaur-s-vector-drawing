# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = lib
TARGET = shapeBuilders
CONFIG += c++11

QT += core xml widgets gui printsupport

DEFINES +=  QT_PRINTSUPPORT_LIB QT_UITOOLS_LIB QT_WIDGETS_LIB QT_XML_LIB SHAPEBUILDERS2_LIB
INCLUDEPATH += ./../draw \
    ./../ICallback \
    ./../actions \
    ./../Dialog \
    ./../shapes \
    ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(shapeBuilders.pri)
TRANSLATIONS += shapebuilders_zh.ts

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Dialog/release/ -lDialog
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Dialog/debug/ -lDialog
else:unix: LIBS += -L$$OUT_PWD/../Dialog/ -lDialog

INCLUDEPATH += $$PWD/../Dialog
DEPENDPATH += $$PWD/../Dialog

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ICallback/release/ -lICallback
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ICallback/debug/ -lICallback
else:unix: LIBS += -L$$OUT_PWD/../ICallback/ -lICallback

INCLUDEPATH += $$PWD/../ICallback
DEPENDPATH += $$PWD/../ICallback

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../shapes/release/ -lshapes
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../shapes/debug/ -lshapes
else:unix: LIBS += -L$$OUT_PWD/../shapes/ -lshapes

INCLUDEPATH += $$PWD/../shapes
DEPENDPATH += $$PWD/../shapes
