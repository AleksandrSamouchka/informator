#ifndef CMQTTCLIENTHANDLER_H
#define CMQTTCLIENTHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QMutex>

#include "CMqttConnectionSettings.h"
#include "CMqttMessage.h"

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
class CMqttClientHandler_old : public QObject
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
    bool m_passReconnect = false;
    QMqttClient * m_client = nullptr;
    QTimer * m_connectTimeoutTimer;
    CMqttMessage m_willMessage = CMqttMessage{};
    QList<CMqttMessage> m_queue;
    CMqttConnectionSettings m_settings{};

protected slots:
    const QMqttClient * getMqttClient();
    virtual void reconnectSlot();

/*! скрытый функционал для потокобезопасного использования методов */
private slots:
    bool forceDisconnectSlot();
    qint32 publishSlot( const CMqttMessage message );
    SubscriptionState subscribeSlot( const QString topicName, const quint8 qos );
    void getStateSlot();
    bool unsubscribeSlot(const QString topicName);
    bool setWillMessageSlot( const CMqttMessage message );
    void enqueueMessage( const CMqttMessage message );

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
    void messageReceived( CMqttMessage );

public slots:
    void threadStartedSlot();
    void threadStopedSlot();

    bool forceReconnect();
    bool forceDisconnect();
    void safePublish( const CMqttMessage message );
    qint32 publish( const CMqttMessage message );
    SubscriptionState subscribe( const QString topicName, const quint8 qos = 0 );
    bool unsubscribe( const QString topicName );
    bool setWillMessage( const CMqttMessage message );

public:
    CMqttClientHandler_old( QObject * parent = nullptr );
    CMqttClientHandler_old( const CMqttConnectionSettings settings, QObject * parent = nullptr );

    CMqttClientHandler_old::ClientState getState();
    CMqttClientHandler_old::ClientError getError() const;
    static CMqttClientHandler_old * createNewConnection( QThread * thread,
                                                     const CMqttMessage willMessage = CMqttMessage{},
                                                     QObject * parent = nullptr );
    static CMqttClientHandler_old * createNewConnectionWithSettings( QThread * thread,
                                                                 const CMqttConnectionSettings settings,
                                                                 const CMqttMessage willMessage = CMqttMessage{},
                                                                 QObject * parent = nullptr );
};

#endif // CMQTTCLIENTHANDLER_H
