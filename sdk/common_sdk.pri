INCLUDEPATH += $$PWD

message("Common_sdk pwd" $$PWD)

include(CDatabaseHandler.pri)
include(CSettingsHandler.pri)
include(CMainHandler.pri)
include(CMqttHandler.pri)
include(CLoggerHandler.pri)
include(CErrorMessageParser.pri)
include(CKeepAliveHandler.pri)
include(CUtils.pri)
include(CMqttTopics.pri)
include(Mqtt.pri)
include(Vehicle.pri)
include(mnt_defs.pri)

#include(CFormalizedMessage.pri) - здесь не включать, включать в своем приложении!
#include(misc.pri) - здесь не включать, включать в своем приложении!

!no_cgos {
    message("--- Bulding with Yocto SDK ---")
    include(CCgosHandler.pri)
}

qmakeSPEC = $$basename(QMAKESPEC)
