#include "CKeepAliveHandler.h"
#include <CConsoleLogger/CConsoleLogger.h>
//#include <CLoggerHandler/CLoggerHandler.h>
#include <CUtils/CUtils.h>
#include <CKeepAliveHandler/CKeepAliveTopicMessage.h>
#include <CMqttHandler/CMqttClientHandler.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QTimer>

namespace
{
    QMutex initHandlerMutex{};
    QMutex instanceMutex{};
    QPointer<CKeepAliveHandler> handler( nullptr );
}

/** ***********************************************************************
 * @fn deinitDatabaseHandler
 ** ***********************************************************************/
void CKeepAliveHandler::deinitHandler( )
{
    QMutexLocker locker( & initHandlerMutex );
    if ( handler.isNull() )
    {
        return;
    }
    delete handler;
    handler.clear();
}

/** ***********************************************************************
 * @fn CMqttClientHandler  [конструктор]
 ** ***********************************************************************/
CKeepAliveHandler::CKeepAliveHandler( CMqttClientHandler * mqttClient ) :
    QObject(),
    m_mqttClient(mqttClient)
{
    Q_ASSERT( mqttClient != nullptr );
    QMutexLocker locker( & initHandlerMutex );
    PRINT_CONSOLE_MESSAGE_INFO( "DEBUG: CKeepAliveHandler клиент создан" );
}

/** ***********************************************************************
 * @fn getInstance [static]
 ** ***********************************************************************/
CKeepAliveHandler * CKeepAliveHandler::getInstance(CMqttClientHandler * mqttClient)
{
    if ( handler == nullptr )
    {
        QMutexLocker locker( & instanceMutex );
        if ( handler == nullptr )
        {
            handler = new CKeepAliveHandler( mqttClient );
        }
    }
    Q_ASSERT( handler != nullptr );
    return handler;
}

CKeepAliveHandler * CKeepAliveHandler::getInstance()
{
    if ( handler == nullptr )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Объект CKeepAliveHandler не проинициализирован!" );
    }
    Q_ASSERT( handler != nullptr );
    return handler;
}

/** ***********************************************************************
 * @fn getMqttKeepAliveTopicName [static]
 ** ***********************************************************************/
QString CKeepAliveHandler::getMqttKeepAliveTopicName( const QString &moduleName )
{
    return QString( "WATCHDOG/%1_KEEPALIVE" ).arg( moduleName.toUpper());
}

/** ***********************************************************************
 * @fn sendMessageSlot
 ** ***********************************************************************/
void CKeepAliveHandler::sendMessageSlot(const CMqttMessage &message)
{
    if ( m_mqttClient.isNull() )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Объект MqttClient не проинициализирован!" );
        return;
    }
    m_mqttClient->safePublish( message );
}

void CKeepAliveHandler::sendMessageSlot(const QString &moduleName, const QByteArray &payload)
{
    sendMessageSlot(CMqttMessage( getMqttKeepAliveTopicName(moduleName), payload ));
}

/** ***********************************************************************
 * @fn setDiagMessageSlot
 ** ***********************************************************************/
void CKeepAliveHandler::setDiagMessageSlot(const QString &moduleName, const QByteArray &payload)
{
    sendMessageSlot(CMqttMessage( getMqttDiagnosticTopicName(moduleName), payload, true ));
}

/** ***********************************************************************
 * @fn sendKeepaliveMessage         [thread safe]
 ** ***********************************************************************/
void CKeepAliveHandler::sendKeepaliveMessage(const QString &moduleName, const QString &payload)
{
    const CKeepAliveTopicMessage message( payload );
    QByteArray preparedData( message.prepareData() );
    if ( ! QMetaObject::invokeMethod( this,
                                      "sendMessageSlot",
                                      Qt::QueuedConnection,
                                      Q_ARG( QString, moduleName ),
                                      Q_ARG( QByteArray, preparedData ) ) )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
    }
    if ( m_diagnosticInfo.isChanges() )
    {
        preparedData = m_diagnosticInfo.prepareData();
        if ( ! QMetaObject::invokeMethod( this,
                                          "setDiagMessageSlot",
                                          Qt::QueuedConnection,
                                          Q_ARG( QString, moduleName ),
                                          Q_ARG( QByteArray, preparedData ) ) )
        {
            PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
        }
    }
    emit keepAliveMessageSended( QPrivateSignal{} );
}

/** ***********************************************************************
 * @fn getMqttDiagnosticTopicName   [static]
 ** ***********************************************************************/
QString CKeepAliveHandler::getMqttDiagnosticTopicName(const QString &moduleName)
{
    return QString( "DIAGNOSTIC/%1" ).arg( moduleName.toUpper() );
}

/** ***********************************************************************
 * @fn initDiagnosticInfo           [thread safe]
 ** ***********************************************************************/
void CKeepAliveHandler::initDiagnosticInfo( const QStringList &subsystemNames, 
                                            const QStringList &humanReadableNames )
{
    m_diagnosticLoker.lockForWrite();
    m_diagnosticInfo = CTopicDiagnosticInfo( subsystemNames, humanReadableNames );
    m_diagnosticLoker.unlock();
}

/** ***********************************************************************
 * @fn diagnosticChangeState           [thread safe]
 ** ***********************************************************************/
bool CKeepAliveHandler::diagnosticChangeState( const QString &subsystem,
                                               const CDiagnosticEntryInfo::EState state )
{
    m_diagnosticLoker.lockForRead();
    const bool res{ m_diagnosticInfo.changeState( subsystem, state ) };
    m_diagnosticLoker.unlock();
    return res;
}

/** ***********************************************************************
 * @fn diagnosticChangeStatus           [thread safe]
 ** ***********************************************************************/
bool CKeepAliveHandler::diagnosticChangeStatus( const QString &subsystem,
                                                const CDiagnosticEntryInfo::EStatus status )
{
    m_diagnosticLoker.lockForRead();
    const bool res{ m_diagnosticInfo.changeStatus( subsystem, status ) };
    m_diagnosticLoker.unlock();
    return res;
}
