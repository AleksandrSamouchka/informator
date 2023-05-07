#include "CGrStmTopic.h"

static const char * STM_PREFIX = "STM/";

CGrStmTopic::CGrStmTopic(const QString &topicName) :
    CMqttTopic( STM_PREFIX + topicName )
{
}

CGrStmTopic::CGrStmTopic( const QString &topicName, const QByteArray & data) :
    CMqttTopic( STM_PREFIX + topicName, data )
{
}

CGrStmTopic::~CGrStmTopic()
{
}

