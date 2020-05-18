#-------------------------------------------------
#
# Project created by QtCreator 2020-03-21T19:53:25
#
#-------------------------------------------------

QT       += core gui multimedia

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
        core/eventhandling.cpp \
        core/keyhandler.cpp \
        core/planetsgraph.cpp \
        core/statemachine.cpp \
        data/loader.cpp \
        data/objectsloader.cpp \
        data/objectsstorage.cpp \
        graphics/attackresultwindow.cpp \
        graphics/buttonitem.cpp \
        graphics/drawer.cpp \
        graphics/fullplanetinfo.cpp \
        graphics/imageitem.cpp \
        graphics/keyfield.cpp \
        graphics/planetgraphics.cpp \
        graphics/planetinfographics.cpp \
        graphics/shopplanetinfo.cpp \
        graphics/shopwidget.cpp \
        graphics/statusbar.cpp \
        graphics/unitwidget.cpp \
        main.cpp \
        mainwindow.cpp \
        core/menu.cpp \
        core/menugraph.cpp \
        objects/bot.cpp \
        objects/building.cpp \
        objects/planet.cpp \
        objects/player.cpp \
        objects/playerbase.cpp \
        objects/unit.cpp \
        scene/gamescene.cpp \
        scene/gameview.cpp \
        scene/loadscreen.cpp

HEADERS += \
    core/eventhandling.h \
  core/keyhandler.h \
    core/planetsgraph.h \
    core/statemachine.h \
    data/loader.h \
    data/objectsloader.h \
    data/objectsstorage.h \
    graphics/attackresultwindow.h \
    graphics/buttonitem.h \
    graphics/drawer.h \
    graphics/fullplanetinfo.h \
    graphics/imageitem.h \
  graphics/keyfield.h \
    graphics/planetgraphics.h \
    graphics/planetinfographics.h \
    graphics/shopplanetinfo.h \
    graphics/shopwidget.h \
    graphics/statusbar.h \
    graphics/unitwidget.h \
        mainwindow.h \
        core/menu.h \
  core/menugraph.h \
  objects/bot.h \
    objects/building.h \
    objects/planet.h \
    objects/player.h \
    objects/playerbase.h \
    objects/unit.h \
    scene/gamescene.h \
    scene/gameview.h \
         \
  scene/loadscreen.h \
    util/typeoffset.h \
        util/utility.h
FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../data/objectsdata.qrc \
    ../img/img.qrc \
