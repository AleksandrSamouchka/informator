#include "CGrLiazTopic.h"

static const char * STM_PREFIX = "LIAZ/";

CGrLiazTopic::CGrLiazTopic(const QString &topicName) :
    CMqttTopic( STM_PREFIX + topicName )
{
}

CGrLiazTopic::CGrLiazTopic( const QString &topicName, const QByteArray & data) :
    CMqttTopic( STM_PREFIX + topicName, data )
{
}

CGrLiazTopic::~CGrLiazTopic()
{
}
