#pragma once

#include <QObject>
#include <QThread>
#include <mutex>
#include <QSemaphore>

#include "externalMqttClientHandler.h"

class Mqtt : public QObject
{
    Q_OBJECT
public:
    static Mqtt* getInstance();
    /*!
     * \brief getLastMsg - возвращает последнее сообщение из топика, если таковое имеется
     * \param topic - имя топика
     */
    QMqttMessage getLastMsg(const QString& topic) {return m_mqttHandler->getLastMsg(topic);}
    /*!
     * \brief getSubgroupMsgs - возвращает список сообщений из подгруппы.
     * \param subgroup - подгруппа, к примеру TOPIC/#
     */
    QList<QMqttMessage> getSubgroupMsgs(const QString& subgroup) {return m_mqttHandler->getSubgroupMsgs(subgroup);}
    void subscribe(const QMqttTopicFilter& filter);
    void publish(const QString& topic, const QByteArray& payload, bool retain = false);
signals:
    void subscribed(QString topicName);
    void connected();
    void msgReceived(QMqttMessage msg);
private:
    explicit Mqtt(QObject *parent = nullptr);
    Mqtt(const Mqtt& other) = delete;
    Mqtt& operator = (const Mqtt& other) = delete;
private:
    static std::once_flag               m_flag;
    static QScopedPointer<Mqtt>         m_instance;
    QThread*                            m_mqttWorkerThread;
    QScopedPointer<MqttClientHandler>   m_mqttHandler;    
};

