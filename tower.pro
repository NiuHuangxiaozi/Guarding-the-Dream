QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    affixlabel.cpp \
    card.cpp \
    enemy.cpp \
    main.cpp \
    messagetips.cpp \
    mymap.cpp \
    mypushbutton.cpp \
    showtable.cpp \
    start.cpp \
    tower.cpp \
    weapon.cpp

HEADERS += \
    affixlabel.h \
    card.h \
    config.h \
    enemy.h \
    messagetips.h \
    mymap.h \
    mypushbutton.h \
    showtable.h \
    start.h \
    tower.h \
    weapon.h

FORMS += \
    showtable.ui \
    start.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    Photo/Mainwindow/name.png \
    Photo/Playtable/final.png \
    Photo/Playtable/grass2.png \
    Photo/Playtable/isconnected.png \
    Photo/Playtable/music/back.wav \
    Photo/Playtable/origin.png \
    Photo/Playtable/platform2.png \
    Photo/Playtable/playgamelabel/gameback.png \
    Photo/Playtable/playgamelabel/gamecontinue.png \
    Photo/Playtable/road2.png \
    Photo/Playtable/tower/army1.gif \
    Photo/Playtable/tower/army2.gif \
    Photo/Playtable/tower/dismantle.png \
    Photo/Playtable/tower/set.png \
    Photo/Playtable/tower/tower0.gif
