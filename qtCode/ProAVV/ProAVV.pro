QT       += core gui multimediawidgets multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aav_lineeditsearch.cpp \
    aav_main_view.cpp \
    aav_networkmanager.cpp \
    aav_networkthread.cpp \
    aav_uploadfile.cpp \
    aav_uploadselect.cpp \
    aav_userlogin.cpp \
    aav_userloginandregis.cpp \
    aav_usermanager.cpp \
    aav_userregister.cpp \
    aav_usrpage.cpp \
    aav_videocoverwidget.cpp \
    aav_videocoverwidlistloop.cpp \
    aav_videodisplay.cpp \
    aav_videolist.cpp \
    aav_volumebutton.cpp \
    aav_widgetvlume.cpp \
    main.cpp

HEADERS += \
    aav_lineeditsearch.h \
    aav_main_view.h \
    aav_networkmanager.h \
    aav_networkthread.h \
    aav_uploadfile.h \
    aav_uploadselect.h \
    aav_userlogin.h \
    aav_userloginandregis.h \
    aav_usermanager.h \
    aav_userregister.h \
    aav_usrpage.h \
    aav_videocoverwidget.h \
    aav_videocoverwidlistloop.h \
    aav_videodisplay.h \
    aav_videolist.h \
    aav_volumebutton.h \
    aav_widgetvlume.h

FORMS += \
    aav_main_view.ui \
    aav_uploadfile.ui \
    aav_uploadselect.ui \
    aav_userlogin.ui \
    aav_userloginandregis.ui \
    aav_userregister.ui \
    aav_usrpage.ui \
    aav_videocoverwidget.ui \
    aav_videocoverwidlistloop.ui \
    aav_videodisplay.ui \
    aav_videolist.ui

INCLUDEPATH +=/usr/local/include

LIBS +=-L /usr/local/lib  -lavformat -lavcodec -lavutil -lswscale
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    src.qrc
