#include "CGrNavigationTopic.h"

static const char * PREFIX = "NAVIGATION/";

CGrNavigationTopic::CGrNavigationTopic( const QString &topicName ) :
    CMqttTopic( PREFIX + topicName )
{
}

CGrNavigationTopic::CGrNavigationTopic( const QString &topicName, const QByteArray & data) :
    CMqttTopic( PREFIX + topicName, data )
{
}

CGrNavigationTopic::~CGrNavigationTopic()
{
}
