#include "CGrStmRoTopic.h"

static const char * STM_PREFIX = "RO/";

CGrStmRoTopic::CGrStmRoTopic( const QString &topicName ) :
    CGrStmTopic( STM_PREFIX + topicName )
{
}

CGrStmRoTopic::CGrStmRoTopic( const QString &topicName, const QByteArray & data) :
    CGrStmTopic( STM_PREFIX + topicName, data )
{
}

CGrStmRoTopic::~CGrStmRoTopic()
{
}
