#include "CGrConnectionTopic.h"

static const char * PREFIX = "CONNECTION/";

CGrConnectionTopic::CGrConnectionTopic( const QString &topicName ) :
    CMqttTopic( PREFIX + topicName )
{
}

CGrConnectionTopic::CGrConnectionTopic( const QString &topicName, const QByteArray & data) :
    CMqttTopic( PREFIX + topicName, data )
{
}

CGrConnectionTopic::~CGrConnectionTopic()
{
}
