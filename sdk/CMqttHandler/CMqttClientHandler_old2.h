#ifndef CMqttClientHandler_H
#define CMqttClientHandler_H

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QQueue>
#include <QMap>
#include "CLoggerHandler.h"
//#include "qmqttclient.h"


#include "CMqttConnectionSettings.h"
#include "CMqttMessage.h"

class QTimer;
class QMqttClient;


/*class CMqttClientErrorSignalConverter: public QObject
{
    Q_OBJECT
public:
    CMqttClientErrorSignalConverter(QObject *parent = nullptr) :
            QObject(parent)
    {

    }
signals:
    void errorChanged();
public slots:
    void onErrorChanged(QMqttClient::ClientError err)
    {
        Q_UNUSED(err);
        emit errorChanged();
    }
};*/

/** ***********************************************************************
 * @class CMqttClientHandlerNew
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
    bool m_passReconnect = false;
    QMqttClient * m_client = nullptr;
    QTimer * m_connectTimeoutTimer = nullptr;
    CMqttMessage m_willMessage = CMqttMessage{};
    QQueue<CMqttMessage> m_queue; // Сюда сохраняем не переданные сообщения из-за обрыва
                                  // связи для дальнейшей публикации после восстановления
                                  // связи.
    QMap<QString, CMqttMessage> m_retainMessages; // Сюда сохраняем ранее опубликованные
                                        // retain сообщения для передачи их после реконнекта
    QMap<int, CMqttMessage> m_sendingMessages; // Отправляемые в текущий момент сообщения,
                                            // ключ - id сообщения
    CMqttConnectionSettings m_settings;
    bool m_publishInited{false};
    QMap<QString, quint8> m_subscrubedTopics;
    QString m_clientName;
    QTimer *txMessageTimeoutTimer {nullptr};
    bool m_notifyPublished {false};
    bool m_printLogMessages{true};
    static void printMessage(const QString &message, CLoggerMessage::ELogCriteria criteria, bool printLogMessages);
    //QAbstractSocket *abstractSocket{nullptr};
    //setDisablePrintDurringInit( QObject * parent = nullptr,
    //                    const QString &clientName="");
protected slots:
    virtual void reconnectSlot();

/*! скрытый функционал для потокобезопасного использования методов */
private slots:
    bool forceDisconnectSlot();
    SubscriptionState subscribeSlot( const QString topicName, const quint8 qos );
    void getStateSlot();
    bool unsubscribeSlot(const QString topicName);
    bool setWillMessageSlot( const CMqttMessage message );
    void enqueueMessage( const CMqttMessage message );

    void tryToSendMessage();
    void sendMessageTimeout();
    void onMessageSent(qint32 mesId);
signals:
    void forceDisconnectResult( bool, QPrivateSignal );
    void publishResult( qint32, QPrivateSignal );
    void subscribeResult( SubscriptionState, QPrivateSignal );
    void getStateResult( ClientState, QPrivateSignal );
    void unsubscribeResult( bool, QPrivateSignal );
    void setWillMessageResult( bool, QPrivateSignal );
    void errorChanged();
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
    qint32 safePublish( const CMqttMessage message );
    qint32 publish( const CMqttMessage message );
    SubscriptionState subscribe( const QString topicName, const quint8 qos = 0 );
    bool unsubscribe( const QString topicName );
    bool setWillMessage( const CMqttMessage message );
public:
    CMqttClientHandler( QObject * parent = nullptr,
                        const QString &clientName="");
    CMqttClientHandler(const CMqttConnectionSettings &settings,
                       QObject * parent = nullptr ,
                       const QString &clientName ="");
    virtual ~CMqttClientHandler();
    inline const QString &clientName() const
    {
        return m_clientName;
    }
    inline void setNotifyPublished(bool value) {
        m_notifyPublished = value;
    }
    inline bool getNotifyPublished() const {
        return m_notifyPublished;
    }
    inline void setPrintLogMessages(bool value)
    {
        m_printLogMessages = value;
    }
    inline bool getPrintLogMessages()
    {
        return m_printLogMessages;
    }

    CMqttClientHandler::ClientState getState();
    CMqttClientHandler::ClientError getError() const;
    static CMqttClientHandler * createNewConnection(QThread * thread,
            const CMqttMessage &willMessage = CMqttMessage{},
            QObject * parent = nullptr,
            const QString &clientName = "",
            bool printLogMessages = true);
    static CMqttClientHandler * createNewConnection(QThread * thread,
            const CMqttConnectionSettings &settings,
            const CMqttMessage &willMessage = CMqttMessage{},
            QObject * parent = nullptr,
            const QString &clientName = "",
            bool printLogMessages = true);

protected:
    inline const QMqttClient * getMqttClient() const
    {
        return m_client;
    }

private:
    void init(/*const CMqttConnectionSettings &settings, */const QString &clientName);
};

#endif // CMqttClientHandlerNew_H
