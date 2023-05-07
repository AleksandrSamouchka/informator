#include "test_CCoreApplication2.h"

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
test_CCoreApplication2::test_CCoreApplication2( const QString & topicName,
                                                QObject * parent)
    : CCoreApplication( parent )

{
    m_mqttTopic = new CMqttTopic( topicName );
    m_testResults = new CTopicTestResult( topicName );
}

test_CCoreApplication2::~test_CCoreApplication2()
{
    delete m_mqttTopic;
    delete m_testResults;
}

/** *****************************************************
 * @fn calcDelays
 * @brief Подсчет максимальной задержки и количества
 * принятых сообщений
 * @param none
 * *****************************************************/
void test_CCoreApplication2::calcDelays()
{
    m_testResults->calcMaxDelay();
    m_numbMessageMaxDelay = m_testResults->getNumbMessageMaxDelay();
    m_maxDelay = m_testResults->getMaxReceivingDelay();
    m_numbOfMessages = m_testResults->getNumbOfMessages();
}

/** ********************************************************
 * @fn stopTest
 * @brief Останавливает основной цикл обработки событий.
 * Вызывается после того, как все сообщения опубликованы
 * @param none
 **********************************************************/
void test_CCoreApplication2::stopTest()
{
    unsubscribeFromTopic();
    QCoreApplication::exit( 0 );
}

int test_CCoreApplication2::onAttach()
{
    subscribeToTopic();
    emit startTest();
    return 0;
}

int test_CCoreApplication2::onDetach()
{
    return  0;
}

void test_CCoreApplication2::subscribeToTopic()
{
    m_connectSubscribe = connect( m_mqttTopic, & CMqttTopic::messageReceived,
             this, & test_CCoreApplication2::topicHandler,
                                  Qt::QueuedConnection);
    if ( ! m_connectSubscribe )
    {
        PRINT_CONSOLE_MESSAGE_WARN( "Не удалось создать подключение!" );
        return;
    }

    subscribeToMqttTopic( m_mqttTopic );
}

void test_CCoreApplication2::unsubscribeFromTopic()
{
    disconnect( m_connectSubscribe );
    unsubscribeFromMqttTopic( m_mqttTopic );
}

void test_CCoreApplication2::topicHandler()
{
    m_receiveTs = QDateTime::currentMSecsSinceEpoch();
    const QJsonObject jsonObj( QJsonDocument::fromJson( m_mqttTopic->data() ).object() );
    if ( ! jsonObj.isEmpty() )
    {
        if ( jsonObj.keys() == FIELDS )
        {
            m_numbMessage = jsonObj.value( FIELD_NUMBMESSAGE ).toInt();
            m_publishTs = static_cast < qint64 > ( jsonObj.value( FIELD_PUBLISHTS ).toDouble() );
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

    m_testResults->addMessage( m_numbMessage, m_publishTs, m_receiveTs, m_receiveTs - m_publishTs );

}


