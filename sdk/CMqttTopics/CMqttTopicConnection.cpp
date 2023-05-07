#include "CMqttTopicConnection.h"

#include <QtCore/QThread>
#include <QtCore/QSemaphore>

/** ***********************************************************************
 * @fn CMqttTopicConnection  [конструктор]
 ** ***********************************************************************/
CMqttTopicConnection::CMqttTopicConnection( const CTopicValueEntry::ETopicsNamesIndex topic,
                                            const EConnectionDirection direction,
                                            QObject * parent ) :
    QObject( parent ),
    m_topic{ CTopicValueEntry::getTopicName( topic ) },
//    m_topicIndex{ topic },
    m_direction{ direction }
{
    static auto id{ qRegisterMetaType<CTopicValueEntry>("CTopicValueEntry") };
    Q_UNUSED( id )
}

CMqttTopicConnection::CMqttTopicConnection( const QString &topic,
                                            const CMqttTopicConnection::EConnectionDirection direction,
                                            QObject * parent ) :
    QObject( parent ),
    m_topic{ topic },
//    m_topicIndex{  },
    m_direction{ direction }
{
    static auto id{ qRegisterMetaType<CTopicValueEntry>("CTopicValueEntry") };
    Q_UNUSED( id )
}


CMqttTopicConnection::EConnectionDirection CMqttTopicConnection::getConnectionDirection() const
{
    return m_direction;
}

QString CMqttTopicConnection::getTopic() const
{
    return m_topic;
}

/** ***********************************************************************
 * @fn messageReceived
 * @brief слот, сообщение получение от брокера
 * @param CMqttMessage
 * @return none
 ** ***********************************************************************/
void CMqttTopicConnection::messageReceived(const CMqttMessage &message)
{
    if ( message.isValid() )
    {
        {
            QMutexLocker locker( & m_lastValueMutex );
            m_lastTopicValue = CTopicValueEntry( message.getMessage(), m_topic );
        }
        emit valueUpdated( m_lastTopicValue );
    }
    else
    {
        PRINT_CONSOLE_MESSAGE_WARN( QString( "Из топика [%1] принято невалидное сообщение" )
                                    .arg( m_topic ) );
    }
}

/** ***********************************************************************
 * @fn getLastTopicValue
 * @brief слот, возвращает последнее принятое сообщение в топике.
 *      Если сообщение не было еще принято - вернет невалидное сообщение
 * @param none
 * @return CTopicValueEntry
 ** ***********************************************************************/
CTopicValueEntry CMqttTopicConnection::getLastValue()
{
    QMutexLocker locker( & m_lastValueMutex );
    const CTopicValueEntry value( m_lastTopicValue );
    return value;
}

/** ***********************************************************************
 * @fn publishValue         [thread safe]
 * @brief слот, публикует сообщение в текущем топике
 * @param CTopicValueEntry
 * @return bool
 ** ***********************************************************************/
bool CMqttTopicConnection::publishValue( const CTopicValueEntry &message )
{
    bool res{ true };
    if ( message.isValid() )
    {
        emit needToPublishMessage( CMqttMessage( m_topic, message.getData(), /*0,*/ message.isRetain() ) );
    }
    return res;
}
