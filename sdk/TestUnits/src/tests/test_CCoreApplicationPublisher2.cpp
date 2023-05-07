#include "test_CCoreApplicationPublisher2.h"

/** **********************************************************
 * @fn Конструктор
 * **********************************************************/
test_CCoreApplicationPublisher2::test_CCoreApplicationPublisher2( const QString & topicName,
                                                                  QObject * parent )
    : QObject ( parent ),
      m_topicName( topicName )
{
}

test_CCoreApplicationPublisher2::~test_CCoreApplicationPublisher2()
{
    CMqttClientHandler::destroyConnection( m_clientHandler );
    delete m_clientHandler;
    delete m_periodTimer;
}

/** ***************************************************************
 * @fn startTest
 * @brief Создение коннекта и запуск таймера
 * @param const int period - интервал между отправками
 * @param const int repetitions - количество публикаций
 * *****************************************************************/
void test_CCoreApplicationPublisher2::startTest( const int period,
                                                 const int repetitions )
{
    m_period = period;
    m_repetitions = repetitions;

    QString host{ "127.0.0.1" };
    qint16 port{ 1883 };

    QString clientId = "client2";
    m_clientHandler = CMqttClientHandler::createConnection( thread(), host, port, clientId );

    m_periodTimer = new QTimer( this );
    m_periodTimer->setSingleShot( false );
    m_periodTimer->setInterval( m_period );

    connect( m_periodTimer, & QTimer::timeout, this, & test_CCoreApplicationPublisher2::publishMqttMessage );
    m_periodTimer->start();

}

void test_CCoreApplicationPublisher2::publishMqttMessage()
{
    m_numbMessage++;
    if( m_numbMessage >= m_repetitions )
    {
        emit stopTest();
    }
    else
    {
        QJsonObject jsonObj{};
        jsonObj.insert( "numbMessage", m_numbMessage );
        qint64 publishTs = QDateTime::currentMSecsSinceEpoch();
        jsonObj.insert( "publishTs", publishTs );

        CMqttMessage msg( m_topicName, QJsonDocument( jsonObj ).toJson( QJsonDocument::Compact ), false );

        m_clientHandler->safePublish( msg );
    }

}
