#ifndef CMQTTCLIENTHANDLER_H
#define CMQTTCLIENTHANDLER_H

#include "CMqttConnectionSettings.h"
#include "CMqttMessage.h"

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QQueue>

#include <Mqtt/CMqttTopic.h>
#include <QUrl>

class QTimer;
class QMqttClient;

/** ***********************************************************************
 * @class CMqttClientHandler
 * @brief Класс, реализующий функционал подключения к брокеру и универсальные
 * методы работы
 *
 * !!! Слот threadStartedSlot должен быть обязательно выполнен после создания объекта
 * В данном методе создаются внутренние объекты, без этого вызова объект работать не будет
 *
 * Если необходимо получить retain сообщение, подключение к сигналу messageReceived
 * должно происходить до вызова метода threadStartedSlot
 *
 * Статический метод createNewConnection позволяет получить retain сообщение
 ** ***********************************************************************/
class CMqttClientHandler : public QObject
{
    Q_OBJECT

public:
    enum class ClientState {
        Disconnected = 0,
        Connecting,
        Connected
    };
    enum class SubscriptionState {
        Unsubscribed = 0,
        SubscriptionPending,
        Subscribed,
        UnsubscriptionPending,
        Error
    };
    enum class ClientError {
        // Protocol states
        NoError                = 0,
        InvalidProtocolVersion = 1,
        IdRejected             = 2,
        ServerUnavailable      = 3,
        BadUsernameOrPassword  = 4,
        NotAuthorized          = 5,
        // Qt states
        TransportInvalid       = 256,
        ProtocolViolation,
        UnknownError
    };

private:
    void init(const QString &host, quint16 port, const QString &clientId, const QString &user, const QString &password);

    QMqttClient * m_client = nullptr;
    QTimer * m_connectTimeoutTimer{ nullptr };
    CMqttMessage m_willMessage/* = CMqttMessage{}*/;
    QList<CMqttMessage> m_queue;
//    CMqttConnectionSettings m_settings{};
    QString   m_host;
    quint16   m_port;
    QString   m_clientId;  
    QString m_user;
    QString m_password;

protected slots:
    const QMqttClient * getMqttClient();
    virtual void reconnectSlot();

/*! скрытый функционал для потокобезопасного использования методов */
private slots:
    bool forceDisconnectSlot();
    qint32 publishSlot( const CMqttMessage &message );
    SubscriptionState subscribeSlot( const QString &topicName, quint8 qos );
    void getStateSlot();
    bool unsubscribeSlot(const QString &topicName);
    bool setWillMessageSlot( const CMqttMessage &message );
    void enqueueMessage( const CMqttMessage &message );

signals:
    void forceDisconnectResult( bool, QPrivateSignal );
    void publishResult( qint32, QPrivateSignal );
    void subscribeResult( SubscriptionState, QPrivateSignal );
    void getStateResult( ClientState, QPrivateSignal );
    void unsubscribeResult( bool, QPrivateSignal );
    void setWillMessageResult( bool, QPrivateSignal );

/*! Интерфейсы */
signals:
    void connected();
    void disconnected();
    void messageReceived( const CMqttMessage &);

protected slots:
    void threadStartedSlot();
    void threadStopedSlot();

public slots:
    bool forceReconnect();
    bool forceDisconnect();
    bool safePublish( const CMqttMessage &message );
    SubscriptionState subscribe( const QString &topicName, quint8 qos = 0 );
    bool unsubscribe( const QString &topicName );
    SubscriptionState subscribe( CMqttTopic &topic/*const QString &topicName, quint8 qos = 0*/ );
    bool unsubscribe( const CMqttTopic &topic/*const QString &topicName*/ );
    SubscriptionState subscribe( CTopicValueEntry::ETopicsNamesIndex topicIndex, quint8 qos = 0 );
    bool unsubscribe( CTopicValueEntry::ETopicsNamesIndex topicIndex );
    bool setWillMessage( const CMqttMessage &message );

protected:
//    CMqttClientHandler( QObject * parent = nullptr );
//    CMqttClientHandler( const QString &host, quint16 port, QObject * parent = nullptr );
    CMqttClientHandler( const QString &host, quint16 port, const QString &clientId, QObject * parent = nullptr );
    CMqttClientHandler( const QString &host, quint16 port, const QString &clientId, const QString &user, const QString &password, QObject * parent = nullptr );

public:

    CMqttClientHandler::ClientState getState();
    CMqttClientHandler::ClientError getError() const;

    static CMqttClientHandler * localInstanceOld();
    static void deinitLocalInstanceOld();
    static void startLocalInstanceOld();

    static CMqttClientHandler *createConnection( const QString &host,
                                                 quint16 port,
                                                 const QString &clientId,
                                                 const CMqttMessage &willMessage = CMqttMessage{},
                                                 QObject * parent = nullptr );
    static CMqttClientHandler *createConnection2(const QString &host,
                                                quint16 port,
                                                const QString &clientId,
                                                const QString &user,
                                                const QString &password,
                                                const CMqttMessage &willMessage = CMqttMessage{},
                                                QObject * parent = nullptr);
    static void destroyConnection(CMqttClientHandler *ptr);
};

#endif // CMQTTCLIENTHANDLER_H
