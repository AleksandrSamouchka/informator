#include "test_CCoreApplication3.h"

#include <QtCore/QtAlgorithms>

/** **********************************************************
 * @fn Конструктор
 * **********************************************************/
test_CCoreApplication3::test_CCoreApplication3( const QList < CTestTopic > & topics,
                                                QObject * parent)
    : CCoreApplication ( parent ),
      m_mqttTopics{ topics }
{
    for ( auto & topic : topics )
    {
        m_topicTestResults.append( CTopicTestResult( topic.topicName() ) );
    }
}

test_CCoreApplication3::~test_CCoreApplication3()
{
}

void test_CCoreApplication3::calcMaxDelay()
{
    if ( m_topicTestResults.size() == 0 )
    {
        return;
    }

    m_topicMaxDelay = m_topicTestResults[0].getTopicName();
    m_maxDelay = m_topicTestResults[0].getMaxReceivingDelay();
    for( auto & topic : m_topicTestResults)
    {
        topic.calcMaxDelay();
        if ( topic.getMaxReceivingDelay() > m_maxDelay )
        {
            m_maxDelay = topic.getMaxReceivingDelay();
            m_topicMaxDelay = topic.getTopicName();
        }

        if ( topic.getNumbOfMessages() == 0 )
        {
            m_numbOfMissedMessages ++;
        }
    }
}

void test_CCoreApplication3::stopTest()
{
    unsubscribeFromTopics();
    QCoreApplication::exit(0);
}

int test_CCoreApplication3::onAttach()
{
    subscribeToTopics();
    emit startTest();
    return 0;
}

int test_CCoreApplication3::onDetach()
{
    return 0;
}

void test_CCoreApplication3::subscribeToTopics()
{
    for( auto & topic : m_mqttTopics )
    {
        connect( & topic, & CTestTopic::messageReceived,
                 this, & test_CCoreApplication3::topicHandler,
                 Qt::QueuedConnection);
        subscribeToMqttTopic( & topic );
    }
}

void test_CCoreApplication3::unsubscribeFromTopics()
{
    for( auto & topic : m_mqttTopics )
    {
        disconnect( & topic, & CTestTopic::messageReceived,
                    this, & test_CCoreApplication3::topicHandler);
        unsubscribeFromMqttTopic( & topic );
    }
}

void test_CCoreApplication3::topicHandler()
{
    m_receiveTs = QDateTime::currentMSecsSinceEpoch();

    QObject * obj = QObject::sender();
    CTestTopic * currentTopic = qobject_cast < CTestTopic * >( obj );

    m_numbMessage = currentTopic->numbMessage();
    m_publishTs = currentTopic->publishTs();

    for ( auto & topicTestResult : m_topicTestResults )
    {
        if ( topicTestResult.getTopicName() == currentTopic->topicName() )
        {
            topicTestResult.addMessage( m_numbMessage,
                                         m_publishTs,
                                         m_receiveTs,
                                         m_receiveTs - m_publishTs );
        }
    }
}
