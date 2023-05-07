TEMPLATE = lib

TARGET = sdk

QT -= gui

!yocto {
    CONFIG += create_pc create_prl no_install_prl
}

include(prk_paths.pri)
include(prk_sdk_version.pri)
include(prk_app_stack_trace.pri)
include(common_sdk.pri)
include(CFormalizedMessage.pri)

DEFINES += USE_MQTT_HANDLER

unix {
    DATA_DIR	= $$DATA_DIR/lib$${TARGET}
    !yocto {

        message("Build without yocto for debian")
        INCLUDE_DIR	= $$INCLUDE_DIR/lib$${TARGET}
        ## Adding headers keeping folder hierarchy

            inc_CFormalizedMessage.path  = $${INCLUDE_DIR}/CFormalizedMessage
            inc_CFormalizedMessage.files = CFormalizedMessage/*.h
            INSTALLS += inc_CFormalizedMessage

            inc_CLoggerHandler.path  = $${INCLUDE_DIR}/CLoggerHandler
            inc_CLoggerHandler.files = CLoggerHandler/*.h
            INSTALLS += inc_CLoggerHandler

            inc_CMqttHandler.path = $${INCLUDE_DIR}/CMqttHandler
            inc_CMqttHandler.files = CMqttHandler/*.h
            INSTALLS += inc_CMqttHandler

            inc_CDatabaseHandler.path  = $${INCLUDE_DIR}/CDatabaseHandler
            inc_CDatabaseHandler.files = CDatabaseHandler/*.h
            INSTALLS += inc_CDatabaseHandler

            inc_CErrorMessageParser.path  = $${INCLUDE_DIR}/CErrorMessageParser
            inc_CErrorMessageParser.files = CErrorMessageParser/*.h
            INSTALLS += inc_CErrorMessageParser

            inc_CKeepAliveHandler.path  = $${INCLUDE_DIR}/CKeepAliveHandler
            inc_CKeepAliveHandler.files = CKeepAliveHandler/*.h
            INSTALLS += inc_CKeepAliveHandler

            inc_CConsoleLogger.path = $${INCLUDE_DIR}/CConsoleLogger
            inc_CConsoleLogger.files = CConsoleLogger/*.h
            INSTALLS += inc_CConsoleLogger

            inc_CLoggerMessage.path = $${INCLUDE_DIR}/CLoggerMessage
            inc_CLoggerMessage.files = CLoggerMessage/*.h
            INSTALLS += inc_CLoggerMessage

            inc_CMainHandler.path = $${INCLUDE_DIR}/CMainHandler
            inc_CMainHandler.files = CMainHandler/*.h
            INSTALLS += inc_CMainHandler

            inc_CMqttTopics.path = $${INCLUDE_DIR}/CMqttTopics
            inc_CMqttTopics.files = CMqttTopics/*.h
            INSTALLS += inc_CMqttTopics

            inc_CMqttTopics_modules.path = $${INCLUDE_DIR}/CMqttTopics/modules
            inc_CMqttTopics_modules.files = CMqttTopics/modules/*.h
            INSTALLS += inc_CMqttTopics_modules

            inc_CSerialDetector.path = $${INCLUDE_DIR}/CSerialDetector
            inc_CSerialDetector.files = CSerialDetector/*.h
            INSTALLS += inc_CSerialDetector

            inc_CSettings.path = $${INCLUDE_DIR}/CSettings
            inc_CSettings.files = CSettings/*.h
            INSTALLS += inc_CSettings

            inc_CSettingsHandler.path = $${INCLUDE_DIR}/CSettingsHandler
            inc_CSettingsHandler.files = CSettingsHandler/*.h
            INSTALLS += inc_CSettingsHandler

            inc_CSettingsHandler_CSettingsEntry.path = $${INCLUDE_DIR}/CSettingsHandler/CSettingsEntry
            inc_CSettingsHandler_CSettingsEntry.files = CSettingsHandler/CSettingsEntry/*.h
            INSTALLS += inc_CSettingsHandler_CSettingsEntry

            inc_CUtils.path = $${INCLUDE_DIR}/CUtils
            inc_CUtils.files = CUtils/*.h
            INSTALLS += inc_CUtils

            inc_mnt_defs.path = $${INCLUDE_DIR}/mnt_defs
            inc_mnt_defs.files = mnt_defs/*.h
            INSTALLS += inc_mnt_defs

            inc_Mqtt.path = $${INCLUDE_DIR}/Mqtt
            inc_Mqtt.files = Mqtt/*.h
            INSTALLS += inc_Mqtt

            inc_Mqtt_Topics.path = $${INCLUDE_DIR}/Mqtt/Topics
            inc_Mqtt_Topics.files = Mqtt/Topics/*.h
            INSTALLS += inc_Mqtt_Topics

            inc_Mqtt_Topics_Audio.path = $${INCLUDE_DIR}/Mqtt/Topics/Audio
            inc_Mqtt_Topics_Audio.files = Mqtt/Topics/Audio/*.h
            INSTALLS += inc_Mqtt_Topics_Audio

            inc_Mqtt_Topics_Audio_Mute.path = $${INCLUDE_DIR}/Mqtt/Topics/Audio/Mute
            inc_Mqtt_Topics_Audio_Mute.files = Mqtt/Topics/Audio/Mute/*.h
            INSTALLS += inc_Mqtt_Topics_Audio_Mute

            inc_Mqtt_Topics_Audio_TrackTo.path = $${INCLUDE_DIR}/Mqtt/Topics/Audio/TrackTo
            inc_Mqtt_Topics_Audio_TrackTo.files = Mqtt/Topics/Audio/TrackTo/*.h
            INSTALLS += inc_Mqtt_Topics_Audio_TrackTo

            inc_Mqtt_Topics_Audio_Volume.path = $${INCLUDE_DIR}/Mqtt/Topics/Audio/Volume
            inc_Mqtt_Topics_Audio_Volume.files = Mqtt/Topics/Audio/Volume/*.h
            INSTALLS += inc_Mqtt_Topics_Audio_Volume

            inc_Mqtt_Topics_Bus.path = $${INCLUDE_DIR}/Mqtt/Topics/Bus
            inc_Mqtt_Topics_Bus.files = Mqtt/Topics/Bus/*.h
            INSTALLS += inc_Mqtt_Topics_Bus

            inc_Mqtt_Topics_Connection.path = $${INCLUDE_DIR}/Mqtt/Topics/Connection
            inc_Mqtt_Topics_Connection.files = Mqtt/Topics/Connection/*.h
            INSTALLS += inc_Mqtt_Topics_Connection

            inc_Mqtt_Topics_Gui.path = $${INCLUDE_DIR}/Mqtt/Topics/Gui
            inc_Mqtt_Topics_Gui.files = Mqtt/Topics/Gui/*.h
            INSTALLS += inc_Mqtt_Topics_Gui

            inc_Mqtt_Topics_IncarnetInspector.path = $${INCLUDE_DIR}/Mqtt/Topics/IncarnetInspector
            inc_Mqtt_Topics_IncarnetInspector.files = Mqtt/Topics/IncarnetInspector/*.h
            INSTALLS += inc_Mqtt_Topics_IncarnetInspector

            inc_Mqtt_Topics_Informator.path = $${INCLUDE_DIR}/Mqtt/Topics/Informator
            inc_Mqtt_Topics_Informator.files = Mqtt/Topics/Informator/*.h
            INSTALLS += inc_Mqtt_Topics_Informator

            inc_Mqtt_Topics_Integration.path = $${INCLUDE_DIR}/Mqtt/Topics/Integration
            inc_Mqtt_Topics_Integration.files = Mqtt/Topics/Integration/*.h
            INSTALLS += inc_Mqtt_Topics_Integration

            inc_Mqtt_Topics_Led.path = $${INCLUDE_DIR}/Mqtt/Topics/Led
            inc_Mqtt_Topics_Led.files = Mqtt/Topics/Led/*.h
            INSTALLS += inc_Mqtt_Topics_Led

            inc_Mqtt_Topics_Liaz.path = $${INCLUDE_DIR}/Mqtt/Topics/Liaz
            inc_Mqtt_Topics_Liaz.files = Mqtt/Topics/Liaz/*.h
            INSTALLS += inc_Mqtt_Topics_Liaz

            inc_Mqtt_Topics_Navigation.path = $${INCLUDE_DIR}/Mqtt/Topics/Navigation
            inc_Mqtt_Topics_Navigation.files = Mqtt/Topics/Navigation/*.h
            INSTALLS += inc_Mqtt_Topics_Navigation

            inc_Mqtt_Topics_Onvif.path = $${INCLUDE_DIR}/Mqtt/Topics/Onvif
            inc_Mqtt_Topics_Onvif.files = Mqtt/Topics/Onvif/*.h
            INSTALLS += inc_Mqtt_Topics_Onvif

            inc_Mqtt_Topics_Recorder.path = $${INCLUDE_DIR}/Mqtt/Topics/Recorder
            inc_Mqtt_Topics_Recorder.files = Mqtt/Topics/Recorder/*.h
            INSTALLS += inc_Mqtt_Topics_Recorder

            inc_Mqtt_Topics_Settings.path = $${INCLUDE_DIR}/Mqtt/Topics/Settings
            inc_Mqtt_Topics_Settings.files = Mqtt/Topics/Settings/*.h
            INSTALLS += inc_Mqtt_Topics_Settings

            inc_Mqtt_Topics_Simulator.path = $${INCLUDE_DIR}/Mqtt/Topics/Simulator
            inc_Mqtt_Topics_Simulator.files = Mqtt/Topics/Simulator/*.h
            INSTALLS += inc_Mqtt_Topics_Simulator

            inc_Mqtt_Topics_Stm.path = $${INCLUDE_DIR}/Mqtt/Topics/Stm
            inc_Mqtt_Topics_Stm.files = Mqtt/Topics/Stm/*.h
            INSTALLS += inc_Mqtt_Topics_Stm

            inc_Mqtt_Topics_Stm_Ifc.path = $${INCLUDE_DIR}/Mqtt/Topics/Stm/Ifc
            inc_Mqtt_Topics_Stm_Ifc.files = Mqtt/Topics/Stm/Ifc/*.h
            INSTALLS += inc_Mqtt_Topics_Stm_Ifc

            inc_Mqtt_Topics_Stm_Ro.path = $${INCLUDE_DIR}/Mqtt/Topics/Stm/Ro
            inc_Mqtt_Topics_Stm_Ro.files = Mqtt/Topics/Stm/Ro/*.h
            INSTALLS += inc_Mqtt_Topics_Stm_Ro

            inc_Mqtt_Topics_Stm_Rw.path = $${INCLUDE_DIR}/Mqtt/Topics/Stm/Rw
            inc_Mqtt_Topics_Stm_Rw.files = Mqtt/Topics/Stm/Rw/*.h
            INSTALLS += inc_Mqtt_Topics_Stm_Rw

            inc_Mqtt_Topics_Stm_Rw_Gnss.path = $${INCLUDE_DIR}/Mqtt/Topics/Stm/Rw/Gnss
            inc_Mqtt_Topics_Stm_Rw_Gnss.files = Mqtt/Topics/Stm/Rw/Gnss/*.h
            INSTALLS += inc_Mqtt_Topics_Stm_Rw_Gnss

            inc_Mqtt_Topics_Stm_Rw_Lte.path = $${INCLUDE_DIR}/Mqtt/Topics/Stm/Rw/Lte
            inc_Mqtt_Topics_Stm_Rw_Lte.files = Mqtt/Topics/Stm/Rw/Lte/*.h
            INSTALLS += inc_Mqtt_Topics_Stm_Rw_Lte

            inc_Mqtt_Topics_System.path = $${INCLUDE_DIR}/Mqtt/Topics/System
            inc_Mqtt_Topics_System.files = Mqtt/Topics/System/*.h
            INSTALLS += inc_Mqtt_Topics_System

            inc_Mqtt_Topics_System_Configuration.path = $${INCLUDE_DIR}/Mqtt/Topics/System/Configuration
            inc_Mqtt_Topics_System_Configuration.files = Mqtt/Topics/System/Configuration/*.h
            INSTALLS += inc_Mqtt_Topics_System_Configuration

            inc_Mqtt_Topics_System_Health.path = $${INCLUDE_DIR}/Mqtt/Topics/System/Health
            inc_Mqtt_Topics_System_Health.files = Mqtt/Topics/System/Health/*.h
            INSTALLS += inc_Mqtt_Topics_System_Health

            inc_Mqtt_Topics_Validators.path = $${INCLUDE_DIR}/Mqtt/Topics/Validators
            inc_Mqtt_Topics_Validators.files = Mqtt/Topics/Validators/*.h
            INSTALLS += inc_Mqtt_Topics_Validators

            inc_Vehicle.path = $${INCLUDE_DIR}/Vehicle
            inc_Vehicle.files = Vehicle/*.h
            INSTALLS += inc_Vehicle


            CONFIG(no_cgos) {
                message( "Building without CGOS " )
            } else {
                inc_cgos_sdk.path = $${INCLUDE_DIR}/CCgosHandler
                inc_cgos_sdk.files += CCgosHandler/*.h

                INSTALLS += inc_cgos_sdk
            }



            ## Adding app.pri to dev package
            inc_pri.path = $$DATA_DIR
            inc_pri.files = app.pri common_sdk.pri prk_paths.pri prk_app_project.pri prk_app_sdk.pri prk_sdk_version.pri prk_app_compile.pri prk_app_stack_trace.pri
            INSTALLS += inc_pri


            ## This is the configuration for generating the pkg-config file
            QMAKE_PKGCONFIG_NAME = lib$$TARGET
            QMAKE_PKGCONFIG_DESCRIPTION = Mntconti SDK library
            QMAKE_PKGCONFIG_LIBDIR = $$TARGET.path

            # This is where our API specific headers are
            QMAKE_PKGCONFIG_INCDIR = $$INCLUDE_DIR
            QMAKE_PKGCONFIG_DESTDIR = pkgconfig
            QMAKE_PKGCONFIG_PREFIX = $$PREFIX
            QMAKE_PKGCONFIG_VERSION = $$VERSION

            QMAKE_CLEAN += *.pc
    }

    target.path = $$LIB_DIR
    INSTALLS   += target

    OBJECTS_DIR = .obj
    MOC_DIR     = .moc

    QMAKE_CLEAN += *.so*
    QMAKE_CLEAN += $(TARGET) $(QMAKE_TARGET)
}


