TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    square.cpp \
    figure.cpp \
    circle.cpp \
    figurecontainer.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    square.h \
    figure.h \
    circle.h \
    figurecontainer.h

