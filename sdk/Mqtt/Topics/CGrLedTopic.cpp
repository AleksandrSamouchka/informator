#include "CGrLedTopic.h"

static const char * STM_PREFIX = "LED/";

static const QStringList TARGET_NAMES {
    "FRONT_PANEL",
    "SIDE_PANEL",
    "SALON_PANEL",
    "REAR_PANEL",
    "STOP_PANEL",
};

QString CGrLedTopic::getTargetName(const CGrLedTopic::ETargets value)
{
    return TARGET_NAMES.value( value, "UNCKNOWN" );
}

CGrLedTopic::ETargets CGrLedTopic::getTargetCode(const QString & value)
{
    int index = TARGET_NAMES.indexOf( value );
    if ( index == -1 )
    {
        return ETargets::targetsCount;
    }
    else
    {
        return static_cast< ETargets >( index );
    }
}

CGrLedTopic::CGrLedTopic(const QString &topicName) :
    CMqttTopic( STM_PREFIX + topicName )
{
}

CGrLedTopic::CGrLedTopic( const QString &topicName, const QByteArray & data) :
    CMqttTopic( STM_PREFIX + topicName, data )
{
}

CGrLedTopic::~CGrLedTopic()
{
}
