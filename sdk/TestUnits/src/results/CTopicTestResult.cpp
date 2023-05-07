#include "CTopicTestResult.h"

CTopicTestResult::CTopicTestResult( QString topicName )
    : m_topicName{ topicName }
{
}

void CTopicTestResult::addMessage( int numbMessage, qint64 publishTs, qint64 receiveTs, int receivingDelay )
{
    m_receivedMessages.append( CReceivedMessage( numbMessage, publishTs, receiveTs, receivingDelay ) );
}

void CTopicTestResult::calcMaxDelay()
{
    if ( m_receivedMessages.size() == 0 )
    {
        return;
    }
    m_numbMessageMaxDelay = m_receivedMessages.begin()->m_numbMessage;
    m_maxDelay = m_receivedMessages.begin()->m_receivingDelay;
    for( const auto & message : m_receivedMessages)
    {
        if( message.m_receivingDelay > m_maxDelay )
        {
            m_maxDelay = message.m_receivingDelay;
            m_numbMessageMaxDelay = message.m_numbMessage;
        }
    }
}

