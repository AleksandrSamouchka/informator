#ifndef CKEEPALIVEHANDLER_H
#define CKEEPALIVEHANDLER_H

/** ***********************************************************************
 * @name KeepAliveHandler
 * @brief [ Keapalive функционал ]
 * Обработчик сообщений об активности модуля.
 * Для использования необходимо с периодичностью @see KEEPALIVE_TIMEOUT_RECOMMEND
 * вызывать макрос @see SEND_KEEPALIVE_MESSAGE
 * Рекомендуется обработчик создавать в основном потоке
 * @brief [ Diagnostic функционал ]
 * 1. нужно проинициализировать структуру со списком контролируемых полей
 * (список имен) @see initDiagnosticInfo
 * 2. содержимое структуры будет публиковаться в соответствующий топик автоматом
 * при изменении любого из состояний с тем же периодом что и keapalive сообщения
 * 3. для изменения статуса или состояния необходимо пользоваться макросами
 * @see DIAG_CHANGE_STATE и @see DIAG_CHANGE_STATUS. Типы аргументов -
 * @see diagnosticChangeState и @see diagnosticChangeStatus
 * @author Мальнев А
 * @date 06-12-2018
 ** ***********************************************************************/

#include <CMqttHandler/CMqttMessage.h>
#include <CMqttTopics/CTopicDiagnosticInfo.h>
#include <QtCore/QPointer>
#include <QtCore/QReadWriteLock>

#define SEND_KEEPALIVE_MESSAGE_PAYLOADED( moduleName, payload ) \
    CKeepAliveHandler::getInstance()->sendKeepaliveMessage( moduleName, QString( payload ) )

#define SEND_KEEPALIVE_MESSAGE(moduleName) SEND_KEEPALIVE_MESSAGE_PAYLOADED( moduleName, QString{} )

#define DIAG_CHANGE_STATE( subsystem, state ) \
    CKeepAliveHandler::getInstance()->diagnosticChangeState( subsystem, state )

#define DIAG_CHANGE_STATUS( subsystem, status ) \
    CKeepAliveHandler::getInstance()->diagnosticChangeStatus( subsystem, status )

class CMqttClientHandler;

/** ***********************************************************************
 * @class CKeepAliveHandler
 * @brief Класс одиночка, публикует события в keepalive топик модуля
 ** ***********************************************************************/
class CKeepAliveHandler : public QObject
{
    Q_OBJECT

/** ***********************************************************************
 * @brief Keapalive функционал - работает автоматически @see CAbstractController
 ** ***********************************************************************/
public:
    static constexpr int KEEPALIVE_TIMEOUT_MAX{ 5 * 1000 };     /*!< 5 seconds */
    static constexpr int KEEPALIVE_TIMEOUT_RECOMMEND{ 2 * 1000 };     /*!< 2 seconds */

public:
/** ***********************************************************************
 * @fn getInstance [static]
 * @brief метод для создания singleton объекта класса
 * @param none
 * @return CKeepAliveHandler *
 ** ***********************************************************************/
    static CKeepAliveHandler * getInstance( CMqttClientHandler * mqttClient);
    static CKeepAliveHandler * getInstance();
/** ***********************************************************************
 * @fn deinitDatabaseHandler
 * @brief функция для деинициализации singleton объекта
 * @param none
 * @return none
 ** ***********************************************************************/
    static void deinitHandler( );
/** ***********************************************************************
 * @fn getMqttKeepAliveTopicName [static]
 * @brief метод для инициализации и получения топика для keepalive сообщений
 * ( используется в GUI )
 * @param QString       - наименование модуля, для которого нужно получить топик
 * Если аргумент пустой (по умолчанию) - наименование формируется для текущего
 * модуля
 * @return QString      - итоговый топик
 ** ***********************************************************************/
    static QString getMqttKeepAliveTopicName( const QString &moduleName );

public slots:
/** ***********************************************************************
 * @fn sendKeepaliveMessage     [thread safe]
 * @brief потокобезопасный слот для отправки keepalive сообщения
 * @param QString       - пользовательская строка
 * @return none
 ** ***********************************************************************/
    void sendKeepaliveMessage( const QString &moduleName, const QString &payload = QString{} );

/** ***********************************************************************
 * @brief Diagnostic функционал
 ** ***********************************************************************/
public:
/** ***********************************************************************
 * @fn getMqttKeepAliveTopicName [static]
 * @brief метод для инициализации и получения топика для keepalive сообщений
 * ( используется в GUI )
 * @param QString       - наименование модуля, для которого нужно получить топик
 * Если аргумент пустой (по умолчанию) - наименование формируется для текущего
 * модуля
 * @return QString      - итоговый топик
 ** ***********************************************************************/
    static QString getMqttDiagnosticTopicName( const QString &moduleName );

public slots:
/** ***********************************************************************
 * @fn initDiagnosticInfo       [thread safe]
 * @brief слот, инициализирует список полей для диагностики
 * @param QStringList   - список полей. Порядок наименований важен -
 * к нму привязывается порядковый номер для обращения
 * @return none
 ** ***********************************************************************/
    void initDiagnosticInfo( const QStringList &subsystemNames, const QStringList &humanReadableNames );
/** ***********************************************************************
 * @fn diagnosticChangeState, diagnosticChangeStatus    [thread safe]
 * @brief слоты изменения статуса и состояния
 * @param int       - номер подсистемы (см входной список подсистем в конструкторе)
 * @param CDiagnosticEntryInfo::EState или CDiagnosticEntryInfo::EStatus
 * @return bool     - флаг изменения (необходимости отправки данных)
 ** ***********************************************************************/
    bool diagnosticChangeState( const QString &subsystem, const CDiagnosticEntryInfo::EState state );
    bool diagnosticChangeStatus( const QString &subsystem, const CDiagnosticEntryInfo::EStatus status );

signals:
/** ***********************************************************************
 * @fn keepAliveMessageSended
 * @brief сигнал, формируется как признак отправки сообщения
 ** ***********************************************************************/
    void keepAliveMessageSended( QPrivateSignal );

/** ***********************************************************************
 * @brief Прочее
 ** ***********************************************************************/
protected:
    CKeepAliveHandler( CMqttClientHandler * mqttClient);

/* инструменты для обеспечения потокобезопасного доступа к публичным методам */
protected slots:
    void sendMessageSlot(const CMqttMessage &message);
    void sendMessageSlot( const QString &moduleName, const QByteArray &payload );
    void setDiagMessageSlot( const QString &moduleName, const QByteArray &payload );

private:
    QPointer < CMqttClientHandler > m_mqttClient;

    QReadWriteLock m_diagnosticLoker{};
    CTopicDiagnosticInfo m_diagnosticInfo{};
};

#endif // CKEEPALIVEHANDLER_H
