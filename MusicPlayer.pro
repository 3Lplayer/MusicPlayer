QT       += core gui multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    button.cpp \
    common.cpp \
    form.cpp \
    item.cpp \
    listitem.cpp \
    lyrics.cpp \
    main.cpp \
    music.cpp \
    musiclist.cpp \
    musicplayer.cpp \
    playprogressbar.cpp \
    volumecontrol.cpp

HEADERS += \
    button.h \
    common.h \
    form.h \
    item.h \
    listitem.h \
    lyrics.h \
    music.h \
    musiclist.h \
    musicplayer.h \
    playprogressbar.h \
    volumecontrol.h

FORMS += \
    button.ui \
    common.ui \
    form.ui \
    item.ui \
    listitem.ui \
    lyrics.ui \
    musicplayer.ui \
    playprogressbar.ui \
    volumecontrol.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
