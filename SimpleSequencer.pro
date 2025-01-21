QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    worker.cpp

HEADERS += \
    audio.h \
    mainwindow.h \
    worker.h

FORMS += \
    mainwindow.ui

# Use pkg-config to find include paths and libraries for SDL2 and SDL2_mixer
INCLUDEPATH += $$system(pkg-config --cflags-only-I sdl2 SDL2_mixer)
LIBS += $$system(pkg-config --libs sdl2 SDL2_mixer)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    808-hihat.wav

RESOURCES += \
    resource.qrc
