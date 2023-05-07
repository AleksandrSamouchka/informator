# @author Мальнев А

HEADERS += \
#        $$PWD/CMqttTopics/CTopicAudioTrackToPlay.h \
#        $$PWD/CMqttTopics/CTopicAudioTrackToPlayStatus.h \
        $$PWD/CMqttTopics/CTopicIntegrationInformator.h \
    $$PWD/CMqttTopics/CTopicStopPanel.h \
        $$PWD/CMqttTopics/CTopicStringValue.h \
        $$PWD/CMqttTopics/CTopicSystemHealthHddPrepare.h \
    $$PWD/CMqttTopics/CTopicTextForLed.h \
        $$PWD/CMqttTopics/CTopicValidatorsPassangerCounter.h \
        $$PWD/CMqttTopics/CTopicValidatorsStatus.h \
        $$PWD/CMqttTopics/CTopicValueEntry.h \
        $$PWD/CMqttTopics/CTopicInformatorSelectedMode.h \
        $$PWD/CMqttTopics/CTopicInformatorSelectedRouteCmd.h \
        $$PWD/CMqttTopics/CTopicNavigationValueGnssState.h \
        $$PWD/CMqttTopics/CTopicInformatorChangeModeCmd.h \
        $$PWD/CMqttTopics/CMqttTopicsHandler.h \
        $$PWD/CMqttTopics/CMqttTopicConnection.h \
        $$PWD/CMqttTopics/CTopicRecorderCameras.h \
        $$PWD/CMqttTopics/CTopicInformatorBusesList.h \
        $$PWD/CMqttTopics/CTopicLoggerGuiMessage.h \
        $$PWD/CMqttTopics/CTopicInformatorRouteState.h \
        $$PWD/CMqttTopics/CInformatorStationInfo.h \
        $$PWD/CMqttTopics/CInformatorSubrouteInfo.h \
#        $$PWD/CMqttTopics/CTopicInformatorSelectedSubrouteCmd.h \
        $$PWD/CMqttTopics/CTopicInformatorPlayStationCmd.h \
        $$PWD/CMqttTopics/CTopicInformatorPlayDoorsCmd.h \
        $$PWD/CMqttTopics/CTopicRecorderRecords.h \
        $$PWD/CMqttTopics/CTopicRecorderUploadCmd.h \
        $$PWD/CMqttTopics/CTopicSettingsSettingChangeCmd.h \
        $$PWD/CMqttTopics/CTopicRecorderUploadStatus.h \
        $$PWD/CMqttTopics/CRecorderUploadStatus.h \
        $$PWD/CMqttTopics/CTopicStmRoMfi.h \
        $$PWD/CMqttTopics/CSensorState.h \
        $$PWD/CMqttTopics/CTopicStmDiscretValue.h \
        $$PWD/CMqttTopics/CTopicStmDoubleValue.h \
        $$PWD/CMqttTopics/CTopicStmRoOneWire.h \
        $$PWD/CMqttTopics/CTermoState.h \
        $$PWD/CMqttTopics/CTopicConnectionState.h \
        $$PWD/CMqttTopics/CTopicConnectionVoiceDialCmd.h \
        $$PWD/CMqttTopics/CTopicConnectionVoiceDialState.h \
        $$PWD/CMqttTopics/CCameraState.h \
        $$PWD/CMqttTopics/CTopicLiazKneeling.h \
        $$PWD/CMqttTopics/CTopicLiazErrors.h \
        $$PWD/CMqttTopics/CTopicTrackerServerIps.h \
        $$PWD/CMqttTopics/CTopicIntegrationValidators.h \
        $$PWD/CMqttTopics/CTopicDiagnosticInfo.h \
        $$PWD/CMqttTopics/modules/CDiagnosticEntryInfo.h \
        $$PWD/CMqttTopics/CTopicStmRoFwWersion.h \
        $$PWD/CMqttTopics/CTopicAudioVolume.h \
        $$PWD/CMqttTopics/CTopicSystemRunasroot.h \
        $$PWD/CMqttTopics/CTopicOnvifFoundCameras.h \
        $$PWD/CMqttTopics/CTopicSystemHealthHddStatus.h \
        $$PWD/CMqttTopics/CTopicSystemHealthHddStatusVolume.h \
        $$PWD/CMqttTopics/CTopicSystemHealthHddStatusVolumePart.h \
        $$PWD/CMqttTopics/COnvifCameraParams.h \
        $$PWD/CMqttTopics/CTopicASOTPState.h
		

SOURCES += \
        $$PWD/CMqttTopics/COnvifCameraParams.cpp \
#        $$PWD/CMqttTopics/CTopicAudioTrackToPlay.cpp \
#        $$PWD/CMqttTopics/CTopicAudioTrackToPlayStatus.cpp \
        $$PWD/CMqttTopics/CTopicIntegrationInformator.cpp \
    $$PWD/CMqttTopics/CTopicStopPanel.cpp \
        $$PWD/CMqttTopics/CTopicStringValue.cpp \
        $$PWD/CMqttTopics/CTopicSystemHealthHddPrepare.cpp \
    $$PWD/CMqttTopics/CTopicTextForLed.cpp \
        $$PWD/CMqttTopics/CTopicValidatorsPassangerCounter.cpp \
        $$PWD/CMqttTopics/CTopicValidatorsStatus.cpp \
        $$PWD/CMqttTopics/CTopicValueEntry.cpp \
        $$PWD/CMqttTopics/CTopicInformatorSelectedMode.cpp \
        $$PWD/CMqttTopics/CTopicInformatorSelectedRouteCmd.cpp \
        $$PWD/CMqttTopics/CTopicNavigationValueGnssState.cpp \
        $$PWD/CMqttTopics/CTopicInformatorChangeModeCmd.cpp \
        $$PWD/CMqttTopics/CMqttTopicsHandler.cpp \
        $$PWD/CMqttTopics/CMqttTopicConnection.cpp \
        $$PWD/CMqttTopics/CTopicRecorderCameras.cpp \
        $$PWD/CMqttTopics/CTopicInformatorBusesList.cpp \
        $$PWD/CMqttTopics/CTopicLoggerGuiMessage.cpp \
        $$PWD/CMqttTopics/CTopicInformatorRouteState.cpp \
        $$PWD/CMqttTopics/CInformatorStationInfo.cpp \
        $$PWD/CMqttTopics/CInformatorSubrouteInfo.cpp \
#        $$PWD/CMqttTopics/CTopicInformatorSelectedSubrouteCmd.cpp \
        $$PWD/CMqttTopics/CTopicInformatorPlayStationCmd.cpp \
        $$PWD/CMqttTopics/CTopicInformatorPlayDoorsCmd.cpp \
        $$PWD/CMqttTopics/CTopicRecorderRecords.cpp \
        $$PWD/CMqttTopics/CTopicRecorderUploadCmd.cpp \
        $$PWD/CMqttTopics/CTopicSettingsSettingChangeCmd.cpp \
        $$PWD/CMqttTopics/CTopicRecorderUploadStatus.cpp \
        $$PWD/CMqttTopics/CRecorderUploadStatus.cpp \
        $$PWD/CMqttTopics/CTopicStmRoMfi.cpp \
        $$PWD/CMqttTopics/CSensorState.cpp \
        $$PWD/CMqttTopics/CTopicStmDiscretValue.cpp \
        $$PWD/CMqttTopics/CTopicStmDoubleValue.cpp \
        $$PWD/CMqttTopics/CTopicStmRoOneWire.cpp \
        $$PWD/CMqttTopics/CTermoState.cpp \
        $$PWD/CMqttTopics/CTopicConnectionState.cpp \
        $$PWD/CMqttTopics/CTopicConnectionVoiceDialCmd.cpp \
        $$PWD/CMqttTopics/CTopicConnectionVoiceDialState.cpp \
        $$PWD/CMqttTopics/CCameraState.cpp \
        $$PWD/CMqttTopics/CTopicLiazKneeling.cpp \
        $$PWD/CMqttTopics/CTopicLiazErrors.cpp \
        $$PWD/CMqttTopics/CTopicTrackerServerIps.cpp \
        $$PWD/CMqttTopics/CTopicIntegrationValidators.cpp \
        $$PWD/CMqttTopics/CTopicDiagnosticInfo.cpp \
        $$PWD/CMqttTopics/modules/CDiagnosticEntryInfo.cpp \
        $$PWD/CMqttTopics/CTopicStmRoFwWersion.cpp \
        $$PWD/CMqttTopics/CTopicAudioVolume.cpp \
        $$PWD/CMqttTopics/CTopicSystemRunasroot.cpp \
        $$PWD/CMqttTopics/CTopicSystemHealthHddStatus.cpp \
        $$PWD/CMqttTopics/CTopicSystemHealthHddStatusVolume.cpp \
        $$PWD/CMqttTopics/CTopicSystemHealthHddStatusVolumePart.cpp \
        $$PWD/CMqttTopics/CTopicOnvifFoundCameras.cpp \
        $$PWD/CMqttTopics/CTopicASOTPState.cpp


DEFINES += USE_MQTT_TOPICS
