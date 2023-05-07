QT -= gui
QT += network mqtt

CONFIG += console
CONFIG += c++14

DEFINES += USE_MQTT_HANDLER

CONFIG += c++11 console
CONFIG -= app_bundle

unix {
    OBJECTS_DIR = .obj
    MOC_DIR     = .moc
}

CONFIG(yocto) {
    message("Build for yocto")
    SDK_DIR = $$PWD/../sdk
} else {
    message("Build for Debian")
    CONFIG += link_pkgconfig
    PKGCONFIG += sdk
    SDK_DIR = /usr/share/libsdk
    target.path=/usr/bin
    INSTALLS += target
}

include($$SDK_DIR/app.pri)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        informatorhandler.cpp \
        main.cpp \
        misc/cinformatormisc.cpp \
        #Mqtt.cpp

TRANSLATIONS += \
    informator_en_VG.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    informatorhandler.h \
    misc/cinformatormisc.h \
    #Mqtt.h
