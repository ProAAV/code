QT       += core gui multimediawidgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aav_main_view.cpp \
    aav_usrpage.cpp \
    aav_videodisplay.cpp \
    aav_videolist.cpp \
    main.cpp

HEADERS += \
    aav_main_view.h \
    aav_usrpage.h \
    aav_videodisplay.h \
    aav_videolist.h

FORMS += \
    aav_main_view.ui \
    aav_usrpage.ui \
    aav_videodisplay.ui \
    aav_videolist.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
