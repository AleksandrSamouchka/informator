
QT       += core mqtt

HEADERS += \
    $$PWD/CMqttHandler/CMqttClientHandler.h \
    $$PWD/CMqttHandler/CMqttMessage.h
SOURCES += \
    $$PWD/CMqttHandler/CMqttClientHandler.cpp \
    $$PWD/CMqttHandler/CMqttMessage.cpp

#    $$PWD/CMqttHandler/CMqttConnectionSettings.h 
#    $$PWD/CMqttHandler/CMqttConnectionSettings.cpp 


#include(CMqtt.pri)

DEFINES += USE_MQTT_HANDLER

