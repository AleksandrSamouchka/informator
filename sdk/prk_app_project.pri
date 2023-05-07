## Project

lessThan(QT_MINOR_VERSION, 9) {
    error(Used unsupported Qt version[$$QT_MINOR_VERSION]! Need to use Qt no less than 5.9)
}

QT += core
QT -= gui
QT -= network
QT -= multimedia

CONFIG += c++14

TEMPLATE = app
TARGET = mntconti_$$TARGET

target.path = $$BIN_DIR
INSTALLS += target


## Build

OBJECTS_DIR = .obj
MOC_DIR     = .moc

qmakeSPEC = $$basename(QMAKESPEC)


## Useful defines

isEmpty(VERSION):VERSION = 1.0.0
BUILD_NUM = $$system(cd $$_PRO_FILE_PWD_ && git rev-parse --short HEAD)
DEFINES += APP_VERSION=\\\"$${VERSION}\\\"
DEFINES += APP_REVISION=\\\"$${BUILD_NUM}\\\"


## Legacy defines

DEFINES += USE_MQTT_HANDLER
