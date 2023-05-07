#include "test_CCoreApplication.h"
#include <QtCore/QTimer>

#include <QDebug>


static const int TIMEOUT_PERIOD{ 100 };

namespace
{
    const char *FIELD_NUMBMESSAGE = "numbMessage";
    const char *FIELD_PUBLISHTS = "publishTs";
    const QStringList FIELDS{
        FIELD_NUMBMESSAGE,
        FIELD_PUBLISHTS
    };
}

/** **********************************************************
 * @fn Конструктор
 * **********************************************************/
test_CCoreApplication::test_CCoreApplication( const QString &topicName, QObject *parent )
    : CCoreApplication( parent )
{
    m_mqttTopic = new CMqttTopic( topicName );
}

test_CCoreApplication::~test_CCoreApplication()
{
    delete m_mqttTopic;
}

void test_CCoreApplication::calcDelays()
{
    if ( m_retainTestResults.isEmpty() )
    {
        PRINT_CONSOLE_MESSAGE_WARN( "Не принято ни одного сообщения!" );
        return;
    }

    m_pubSubMaxDelay = m_retainTestResults.begin()->m_pubSubDelay;
    m_subReceiveMaxDelay = m_retainTestResults.begin()->m_subReceiveDelay;

    for( const auto & message : m_retainTestResults )
    {
        if ( message.m_pubSubDelay > m_pubSubMaxDelay )
        {
            m_numbMsgPubSubDelay = message.m_numbMessage;
            m_pubSubMaxDelay = message.m_pubSubDelay;
        }

        if ( message.m_subReceiveDelay > m_subReceiveMaxDelay )
        {
            m_numbMsgSubReceiveDelay = message.m_numbMessage;
            m_subReceiveMaxDelay = message.m_subReceiveDelay;
        }
    }
}

void test_CCoreApplication::messagePublishedHandler(int index)
{
    m_currentIndex = index;

    subscribeToTopic();
    m_timeoutTimer->start();
}

void test_CCoreApplication::stopTestRetain()
{
    QCoreApplication::exit(0);
}

int test_CCoreApplication::onAttach()
{
    m_timeoutTimer = new QTimer( this );
    m_timeoutTimer->setSingleShot( true );
    m_timeoutTimer->setInterval( TIMEOUT_PERIOD );
    connect( m_timeoutTimer, & QTimer::timeout, this, & test_CCoreApplication::timeout );
    emit startTestRetain();
    return 0;
}

int test_CCoreApplication::onDetach()
{
    return 0;
}

void test_CCoreApplication::subscribeToTopic()
{
    m_connectSubscribe = connect( m_mqttTopic, &CMqttTopic::messageReceived,
                                  this, &test_CCoreApplication::topicHandler,
                                  Qt::UniqueConnection);
    if ( ! m_connectSubscribe )
    {
        PRINT_CONSOLE_MESSAGE_WARN( "Не удалось создать подключение!" );
    }

    m_subscribeTs = QDateTime::currentMSecsSinceEpoch();
    subscribeToMqttTopic(m_mqttTopic);
}

void test_CCoreApplication::topicHandler()
{
    m_receiveTs = QDateTime::currentMSecsSinceEpoch();
    const QJsonObject jsonObj( QJsonDocument::fromJson( m_mqttTopic->data() ).object() );
    if( ! jsonObj.isEmpty() )
    {
        if( jsonObj.keys() == FIELDS )
        {
            m_numbMessage = jsonObj.value( FIELD_NUMBMESSAGE ).toInt();
            if( m_numbMessage == m_currentIndex )
            {
                m_numbOfMessages++;
                m_publishTs = static_cast < qint64 > ( jsonObj.value( FIELD_PUBLISHTS ).toDouble() );
            }
            else
            {
                return;
            }
        }
        else
        {
            PRINT_CONSOLE_MESSAGE_WARN( "Принятое сообщение содержит неизвестные поля!" );
        }
    }
    else
    {
        PRINT_CONSOLE_MESSAGE_WARN( "Принято пустое сообщение!" );
    }

    m_timeoutTimer->stop();
    unsubscribeFromTopic();

    m_retainTestResults.append( CRetainTestResult( m_numbMessage, m_publishTs,
                                                   m_subscribeTs, m_receiveTs,
                                                   m_subscribeTs - m_publishTs,
                                                   m_receiveTs - m_subscribeTs ));

    if ( ! QMetaObject::invokeMethod( this,
                                      "stopCurrentIteration",
                                      Qt::QueuedConnection,
                                      Q_ARG( bool, true ) ) )
    {
        PRINT_CONSOLE_MESSAGE_WARN( "Невозможно создать invokeMethod!" );
    }

}

void test_CCoreApplication::unsubscribeFromTopic()
{
    disconnect( m_connectSubscribe );
    unsubscribeFromMqttTopic( m_mqttTopic );
}

void test_CCoreApplication::timeout()
{
    unsubscribeFromTopic();
    stopCurrentIteration( false );
}

void test_CCoreApplication::stopCurrentIteration( bool success )
{
    Q_UNUSED(success);
    emit endIteration();
}
