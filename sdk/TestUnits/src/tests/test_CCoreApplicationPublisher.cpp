#include "test_CCoreApplicationPublisher.h"

#ifdef USE_KEEP_ALIVE_HANDLER
    #include <CKeepAliveHandler/CKeepAliveHandler.h>
#endif
#ifdef USE_MQTT_TOPICS
    #include <CMqttTopics/CMqttTopicsHandler.h>
#endif

#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtCore/QJsonObject>
#include <QtCore/QEventLoop>
#include <QDebug>

/** **********************************************************
 * @fn Конструктор
 * **********************************************************/
test_CCoreApplicationPublisher::test_CCoreApplicationPublisher( const QString & topic,
                                                                QObject * parent ) :
    QObject( parent ),
    m_topic( topic )
{
}

test_CCoreApplicationPublisher::~test_CCoreApplicationPublisher()
{
    CMqttClientHandler::destroyConnection( m_ptr );
    delete m_ptr;
    delete m_periodTimer;
}

/** **********************************************************
 * @fn startTestRetain
 * @brief Начало цикла отправки сообщений
 * @param const int period - период между отправками
 * @param const int repeatitions - количество повторов
 * **********************************************************/
void test_CCoreApplicationPublisher::startTestRetain( const int period,
                                                      const int repetitions )
{
    QString host{ "127.0.0.1" };
    qint16 port{ 1883 };

    QString clientId = "client1";
    m_ptr = CMqttClientHandler::createConnection( thread(), host, port, clientId );

    m_period = period;
    m_repetitions = repetitions;

    m_periodTimer = new QTimer( this );
    m_periodTimer->setSingleShot( true );
    m_periodTimer->setInterval( m_period );
    connect( m_periodTimer, & QTimer::timeout, this, &test_CCoreApplicationPublisher::publishMqttMessage);

    publishMqttMessage();
}

void test_CCoreApplicationPublisher::endIteration()
{
    m_periodTimer->start();
}


void test_CCoreApplicationPublisher::publishMqttMessage()
{
    m_numbMessage++;
    if( m_numbMessage >= m_repetitions )
    {
        emit stopTestRetain();
    }
    else
    {
        QJsonObject jsonObj{};
        jsonObj.insert( "numbMessage", m_numbMessage );
        qint64 publishTs = QDateTime::currentMSecsSinceEpoch();
        jsonObj.insert( "publishTs", publishTs );

        CMqttMessage msg( m_topic, QJsonDocument( jsonObj ).toJson( QJsonDocument::Compact ), true);

        m_ptr->safePublish( msg );
        emit messagePublished( m_numbMessage );

    }
}
