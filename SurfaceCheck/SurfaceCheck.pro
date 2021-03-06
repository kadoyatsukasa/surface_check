# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS    += "$$(HALCONROOT)/lib/x64-win64/halconcpp.lib" \
           "$$(HALCONROOT)/lib/x64-win64/halcon.lib"

INCLUDEPATH += "$$(HALCONROOT)/include" \
               "$$(HALCONROOT)/include/halconcpp"

TEMPLATE = app
TARGET = SurfaceCheck
DESTDIR = ./debug
CONFIG += debug
OBJECTS_DIR += debug

HEADERS += ./AlgTemplate.h \
    ./AlgCheckModel.h \
    ./AlgParameters.h \
    ./SurfaceCheck.h \
    ./HImageWidget.h
SOURCES += ./AlgCheckModel.cpp \
    ./AlgTemplate.cpp \
    ./AlgParameters.cpp \
    ./HImageWidget.cpp \
    ./main.cpp \
    ./SurfaceCheck.cpp
FORMS += ./SurfaceCheck.ui
RESOURCES += SurfaceCheck.qrc


