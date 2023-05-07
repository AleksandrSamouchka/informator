#include "CStmRwTopic.h"

static const char * STM_RW_PREFIX = "RW/";

CStmRwTopic::CStmRwTopic( const QString &topicName ) :
    CGrStmTopic( STM_RW_PREFIX + topicName )
{
}

CStmRwTopic::CStmRwTopic( const QString &topicName, const QByteArray & data) :
    CGrStmTopic( STM_RW_PREFIX + topicName, data )
{
}

CStmRwTopic::~CStmRwTopic()
{
}

