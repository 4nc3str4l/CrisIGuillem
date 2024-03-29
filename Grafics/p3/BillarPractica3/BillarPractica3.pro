#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T16:04:09
#
#-------------------------------------------------

QT       += core gui opengl
QT       += widgets

TARGET = BillarPractica1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    Common.cpp \
    escena.cpp \
    objecte.cpp \
    taulabillar.cpp \
    cara.cpp \
    readfile.cpp \
    plabase.cpp \
    bola.cpp \
    conjuntboles.cpp \
    camera.cpp \
    llum.cpp \
    material.cpp

HEADERS  += mainwindow.h \
    vec.h \
    mat.h \
    glwidget.h \
    Common.h \
    escena.h \
    objecte.h \
    taulabillar.h \
    cara.h \
    readfile.h \
    plabase.h \
    bola.h \
    conjuntboles.h \
    camera.h \
    llum.h \
    material.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    vshader1.glsl \
    fshader1.glsl

RESOURCES += \
    resources.qrc

DISTFILES += \
    phong_fshader.glsl \
    phong_vshader.glsl \
    toon_fshader.glsl \
    toon_vshader.glsl







