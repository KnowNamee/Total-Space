#-------------------------------------------------
#
# Project created by QtCreator 2020-03-21T19:53:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TotalSpace
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
        building.cpp \
        drawer.cpp \
        eventhandling.cpp \
        gamescene.cpp \
        gameview.cpp \
        imageitem.cpp \
        loader.cpp \
        main.cpp \
        mainwindow.cpp \
        menu.cpp \
        planet.cpp \
        planetgraphics.cpp \
        player.cpp \
        playerbase.cpp \
        statemachine.cpp \
        unit.cpp

HEADERS += \
        building.h \
        drawer.h \
        eventhandling.h \
        gamescene.h \
        gameview.h \
        imageitem.h \
        loader.h \
        mainwindow.h \
        menu.h \
        planet.h \
        planetgraphics.h \
        player.h \
        playerbase.h \
        resources.h \
        unit.h \
        statemachine.h \
        typeoffset.h
        
FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
