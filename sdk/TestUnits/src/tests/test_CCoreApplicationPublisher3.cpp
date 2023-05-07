#include "test_CCoreApplicationPublisher3.h"

/** ************************************************************************
 * @fn Конструктор
 * *****************************************************************************/
test_CCoreApplicationPublisher3::test_CCoreApplicationPublisher3( QStringList &topicsNames,
                                                                  QObject * parent)
    : QObject (parent),
      m_topicsNames{ topicsNames }
{
}

test_CCoreApplicationPublisher3::~test_CCoreApplicationPublisher3()
{
    CMqttClientHandler::destroyConnection( m_clientHandler );
    delete m_clientHandler;
}

/** ************************************************************************
 * @fn Конструктор
 * *****************************************************************************/
void test_CCoreApplicationPublisher3::startTest()
{
    QString host( "127.0.0.1" );
    qint16 port{ 1883 };
    QString clientId = "client3";
    m_clientHandler = CMqttClientHandler::createConnection( thread(), host, port, clientId );

    publishMqttMessage();
}

void test_CCoreApplicationPublisher3::publishMqttMessage()
{
    for( auto & topicName : m_topicsNames )
    {
        QJsonObject jsonObj{};
        jsonObj.insert( "numbMessage", 0 );
        qint64 publishTs = QDateTime::currentMSecsSinceEpoch();
        jsonObj.insert( "publishTs", publishTs );

        CMqttMessage msg(  topicName, QJsonDocument( jsonObj ).toJson( QJsonDocument::Compact ) );

        m_clientHandler->safePublish( msg );
    }
    QTimer::singleShot( 1000, this, SLOT( stopPublish() ));
}

void test_CCoreApplicationPublisher3::stopPublish()
{
    emit stopTest();
}

