#ifndef CMQTTTOPICSHANDLER_H
#define CMQTTTOPICSHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <CMqttTopics/CTopicValueEntry.h>
#include <CMqttTopics/CMqttTopicConnection.h>
#include <CMqttHandler/CMqttClientHandler.h>

#include <QtCore/QMutex>

class QThread;

/** ***********************************************************************
 * @class CMqttTopicsHandler
 * @brief класс, содержит список всех подключений к Mqtt топикам,
 * в свойствах содержит поток для подключений
 ** ***********************************************************************/
class CMqttTopicsHandler : public QObject
{
    Q_OBJECT

    QMutex m_listMutex;
    QMap < QString, QPointer < CMqttTopicConnection > > connectionsList;
    CMqttClientHandler *m_mqttConnect = nullptr;

public:
    virtual ~CMqttTopicsHandler();

/* Скрытый конструктор */
protected:
    CMqttTopicsHandler(CMqttClientHandler *mqttClient, QObject *parent = nullptr);

/* thread-safe методы */
protected slots:
    CMqttTopicConnection * getConnectionSlot( const CTopicValueEntry::ETopicsNamesIndex topicIndex,
                                              const QString &topicName,
                                              const CMqttTopicConnection::EConnectionDirection direction );
    CMqttTopicConnection * createConnectionSlot( const QString &topicName,
                                                 const CMqttTopicConnection::EConnectionDirection direction );
signals:
    void createConnectionResult( CMqttTopicConnection *, QPrivateSignal );
    void ready( QPrivateSignal );

/* внутренний функционал */
protected slots:
    void threadStartedSlot();

public slots:
    const CMqttClientHandler * clientHandler() const;

/* доступные методы */
public:
//    static CMqttTopicsHandler * getInstance();
    static CMqttTopicsHandler * getInstance( CMqttClientHandler * mqttClient/* = nullptr */);
    static void deinitHandler();
/*    static CMqttTopicConnection * getConnection( const CTopicValueEntry::ETopicsNamesIndex topicIndex,
                                                 const CMqttTopicConnection::EConnectionDirection direction
                                                 = CMqttTopicConnection::EConnectionDirection::Connection_fullDuplex );
    static CMqttTopicConnection * getConnection( const QString &topicName,
                                                 const CMqttTopicConnection::EConnectionDirection direction
                                                 = CMqttTopicConnection::EConnectionDirection::Connection_fullDuplex );
//                                                 const CMqttTopicConnection::EConnectionDirection direction );
*/
    static CMqttTopicConnection * createConnection( 
                                        CMqttClientHandler  *mqttClient,
                                        const CTopicValueEntry::ETopicsNamesIndex topicIndex,
                                        const CMqttTopicConnection::EConnectionDirection direction );
    static CMqttTopicConnection * createConnection( 
                                        CMqttClientHandler  *mqttClient,
                                        const QString &topicName,
                                        const CMqttTopicConnection::EConnectionDirection direction );
};

#endif // CMQTTTOPICSHANDLER_H
