#include "CMqttTopicsHandler.h"
#include <CConsoleLogger/CConsoleLogger.h>

#include <QtCore/QThread>
#include <QtCore/QCoreApplication>
#include <QtCore/QSemaphore>
#include <QtCore/QMap>

namespace
{
    QMutex initHandlerMutex{};
    QMutex instanceMutex{};
    CMqttTopicsHandler * handler = nullptr;
    QThread * handlerThread = nullptr;

    QMutex getConnectionMutex{};

    const int NORM_WAITING_TIMEOUT{ 10000 };
}

/** ***********************************************************************
 * @fn getInstance [static]
 * @brief метод для создания singleton объекта класса
 * @param none
 * @return CMqttTopicsHandler *
 ** ***********************************************************************/
//CMqttTopicsHandler * CMqttTopicsHandler::getInstance()
//{
//    return getInstance(CMqttClientHandler::localInstanceOld());
//}

CMqttTopicsHandler * CMqttTopicsHandler::getInstance(CMqttClientHandler  *mqttClient)
{
    if ( handler == nullptr )
    {
        QMutexLocker locker( & instanceMutex );
        if ( handler == nullptr )
        {
            QMutexLocker locker( & initHandlerMutex );
            if ( mqttClient != nullptr )
            {
                handler = new CMqttTopicsHandler(mqttClient);
                handlerThread = new QThread{};
                handler->moveToThread( handlerThread );
                connect( handlerThread, &QThread::started, handler, &CMqttTopicsHandler::threadStartedSlot );
                QSemaphore sema(0);
                auto conn = connect( handler, & CMqttTopicsHandler::ready, [ &sema ]()
                {
                    sema.release(1);
                });
                handlerThread->start();
                if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
                {
                    PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
                }
                disconnect( conn );
            }
        }
    }
    Q_ASSERT( handler != nullptr );
    return handler;
}

/** ***********************************************************************
 * @fn deinitHandler
 * @brief функция для деинициализации singleton объекта
 * @param none
 * @return none
 ** ***********************************************************************/
void CMqttTopicsHandler::deinitHandler( )
{
    QMutexLocker locker( & initHandlerMutex );
    if ( handler != nullptr )
    {
//        if ( ! QMetaObject::invokeMethod( handler,
//                                          "threadStoppedSlot",
//                                          Qt::BlockingQueuedConnection ) )
//        {
//            PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
//        }
        for ( const auto & i : /*CMqttTopicsHandler::getInstance()*/handler->connectionsList )
        {
            i.data()->deleteLater();
        }
        handlerThread->quit();
        handlerThread->wait();
        delete handlerThread;
        handlerThread = nullptr;
        delete handler;
        handler = nullptr;
    }
}

/** ***********************************************************************
 * @fn getConnection [static]
 * @brief метод, возвращает указатель на объект подключения к топику
 *      Если такого подключения еще нет - создается, иначе - нет
 * @param CTopicValueEntry::ETopicsNamesIndex
 * @return CMqttTopicConnection *
 ** ***********************************************************************/
/*
CMqttTopicConnection *CMqttTopicsHandler::getConnection(
        const CTopicValueEntry::ETopicsNamesIndex topicIndex,
        const CMqttTopicConnection::EConnectionDirection direction )
{
    return createConnection( / * CMqttTopicsHandler::getInstance()->clientHandler(), * /
                             topicIndex,
                             direction);
}

CMqttTopicConnection * CMqttTopicsHandler::getConnection(
        const QString &topicName,
        const CMqttTopicConnection::EConnectionDirection direction)
{
    return createConnection( / *CMqttClientHandler::localInstance(), * /
                             topicName,
                             direction);
}

*/

CMqttTopicConnection *CMqttTopicsHandler::createConnection( 
                                CMqttClientHandler  *mqttClient,
                                const CTopicValueEntry::ETopicsNamesIndex topicIndex,
                                const CMqttTopicConnection::EConnectionDirection direction )
{
    QMutexLocker locker( & getConnectionMutex );
    CMqttTopicsHandler * handler = CMqttTopicsHandler::getInstance(mqttClient);
//    CMqttTopicConnection * connection = nullptr;
    QString topicName = CTopicValueEntry::getTopicName(topicIndex);
    return handler->getConnectionSlot( topicIndex, topicName, direction );
//    return connection;
}

CMqttTopicConnection * CMqttTopicsHandler::createConnection( 
                                CMqttClientHandler  *mqttClient,
                                const QString &topicName,
                                const CMqttTopicConnection::EConnectionDirection direction)
{
    QMutexLocker locker( & getConnectionMutex );
    CMqttTopicsHandler * handler = CMqttTopicsHandler::getInstance(mqttClient);
//    CMqttTopicConnection * connection = nullptr;
    CTopicValueEntry::ETopicsNamesIndex topicIndex = CTopicValueEntry::getTopicNameIndex(topicName);    
    return handler->getConnectionSlot( topicIndex, topicName, direction );
//    return connection;
}

/** ***********************************************************************
 * @fn CMqttTopicsHandler       [конструктор]
 ** ***********************************************************************/
CMqttTopicsHandler::CMqttTopicsHandler(CMqttClientHandler *mqttClient, QObject *parent) :
    QObject(parent)
{
    static auto id1 = qRegisterMetaType<CMqttTopicConnection *>("CMqttTopicConnection *");
    static auto id2 = qRegisterMetaType<CTopicValueEntry>("CTopicValueEntry");
    static auto id3 = qRegisterMetaType<CTopicValueEntry::ETopicsNamesIndex>("CTopicValueEntry::ETopicsNamesIndex");
    static auto id4 = qRegisterMetaType<CMqttTopicConnection::EConnectionDirection>("CMqttTopicConnection::EConnectionDirection");
    Q_UNUSED(id1)
    Q_UNUSED(id2)
    Q_UNUSED(id3)
    Q_UNUSED(id4)

    m_mqttConnect = mqttClient; //CMqttClientHandler::localInstance();
}

CMqttTopicsHandler::~CMqttTopicsHandler()
{
}

/** ***********************************************************************
 * @fn getConnectionSlot
 * @brief слот, формирует новое подключение к топику
 * @param CTopicValueEntry::ETopicsNamesIndex
 * @param CMqttTopicConnection::EConnectionDirection
 * @return CMqttTopicsHandler *
 ** ***********************************************************************/
CMqttTopicConnection *CMqttTopicsHandler::getConnectionSlot( const CTopicValueEntry::ETopicsNamesIndex topicIndex,
                                                             const QString &topicName_,
                                                             const CMqttTopicConnection::EConnectionDirection direction )
{
    QString topicName = topicName_;
    CMqttTopicConnection * connection{ nullptr };
    if ( topicName.isEmpty() )
    {
        topicName = CTopicValueEntry::getTopicName( topicIndex );
    }
//    QMutexLocker locker( & m_listMutex );
    if ( connectionsList.contains( topicName ) )
    {
        connection = connectionsList.value( topicName ).data();
    }
    else
    {
        if ( QThread::currentThread() == handlerThread )
        {
            connection = createConnectionSlot( topicName, direction );
        }
        else
        {
            QSemaphore sema(0);
            QMetaObject::Connection conn = connect( this, &CMqttTopicsHandler::createConnectionResult,
                                                    [ &sema, &connection ]( CMqttTopicConnection * newConnection )
            {
                connection = newConnection;
                sema.release(1);
            });
            if ( ! QMetaObject::invokeMethod( this,
                                              "createConnectionSlot",
                                              Qt::QueuedConnection,
                                              Q_ARG( QString, topicName ),
                                              Q_ARG( CMqttTopicConnection::EConnectionDirection, direction ) ) )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
            }
            else
            {
                if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
                {
                    PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
                }
            }
            disconnect( conn );
        }
        if ( connection == nullptr )
        {
            PRINT_CONSOLE_MESSAGE_ERROR( "Внимание! Ошибка создания объекта!" );
        }
        else
        {
            connectionsList.insert( topicName, connection );
        }
    }
    return connection;
}

/** ***********************************************************************
 * @fn getConnectionSlot
 * @brief слот, формирует новое подключение к топику
 * @param CTopicValueEntry::ETopicsNamesIndex
 * @param CMqttTopicConnection::EConnectionDirection
 * @return CMqttTopicsHandler *
 ** ***********************************************************************/
CMqttTopicConnection * CMqttTopicsHandler::createConnectionSlot(const QString &topicName,
                                                                 const CMqttTopicConnection::EConnectionDirection direction )
{
    CMqttTopicConnection * connection{ new CMqttTopicConnection( topicName, direction, this ) };
    if ( connection->getConnectionDirection() == CMqttTopicConnection::EConnectionDirection::Connection_fullDuplex ||
         connection->getConnectionDirection() == CMqttTopicConnection::EConnectionDirection::Connection_subscribeOnly )
    {
        if ( m_mqttConnect !=  nullptr/*.isNull()*/ )
        {
            m_mqttConnect->subscribe( connection->getTopic() );
        }
    }
    connect( connection, &CMqttTopicConnection::needToPublishMessage, this, [ this ]( const CMqttMessage message )
    {
        if ( m_mqttConnect != nullptr /*.isNull()*/ )
        {
            m_mqttConnect->safePublish( message );
        }
    }, Qt::QueuedConnection );
    emit createConnectionResult( connection, QPrivateSignal{} );
    return connection;
}

/** ***********************************************************************
 * @fn threadStartedSlot
 * @brief слот, вызывается при запуске потока
 * @param none
 * @return none
 ** ***********************************************************************/
void CMqttTopicsHandler::threadStartedSlot()
{
    if ( m_mqttConnect == nullptr/*.isNull()*/ )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Внимание! Ошибка создания объекта!" );
    }
    else
    {
        connect( m_mqttConnect/*.data()*/, &CMqttClientHandler::messageReceived, this, [ this ]( const CMqttMessage message )
        {
            if ( message.isValid() )
            {
                if ( connectionsList.contains( message.getTopicName() ) )
                {
                    if ( ! QMetaObject::invokeMethod( connectionsList.value( message.getTopicName() ).data(),
                                                      "messageReceived",
                                                      Qt::QueuedConnection,
                                                      Q_ARG( CMqttMessage, message ) ) )
                    {
                        PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
                    }
                }
            }
            else
            {
                PRINT_CONSOLE_MESSAGE_WARN( QString( "Из топика [%1] принято невалидное сообщение" )
                                            .arg( message.getTopicName() ) );
            }
        }, Qt::QueuedConnection );
    }
    emit ready( QPrivateSignal{} );
}

/** ***********************************************************************
 * @fn clientHandler
 * @brief слот, возвращает
 * @return none
 ** ***********************************************************************/
const CMqttClientHandler * CMqttTopicsHandler::clientHandler() const
{
    return m_mqttConnect/*.data()*/;
}
