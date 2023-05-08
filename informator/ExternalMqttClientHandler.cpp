#include "ExternalMqttClientHandler.h"

#include <QThread>
#include <QMutexLocker>

namespace
{
    const QString EXTERNAL_MQTT_HOST = "EXTERNAL_MQTT_HOST";
    const QString EXTERNAL_MQTT_PORT = "EXTERNAL_MQTT_PORT";
    const QString EXTERNAL_MQTT_KEEP_ALIVE = "EXTERNAL_MQTT_KEEP_ALIVE";
}

MqttClientHandler::MqttClientHandler(QObject *parent)
    : QObject(parent),
      m_host("192.168.85.22"),
      m_port(1883),
      m_keepAlive(60)
{}

QMqttMessage MqttClientHandler::getLastMsg(const QString &topic)
{
    QMqttMessage message;
    QMqttTopicFilter filter(topic);

    QMutexLocker locker(&m_lastMessageMutex);

    if (m_filters.contains(filter) && m_lastMessages.contains(topic)) message = m_lastMessages[filter];

    return message;
}

QList<QMqttMessage> MqttClientHandler::getSubgroupMsgs(const QString &subgroup)
{
    QList<QMqttMessage> result;

    QString subgroupTopic = subgroup;
    subgroupTopic.remove("#");

    QMutexLocker locker(&m_lastMessageMutex);

    for (auto it = m_lastMessages.begin(); it != m_lastMessages.end(); ++it)
    {
        if (it.key().filter().contains(subgroupTopic)) result.append(it.value());
    }

    return result;
}

void MqttClientHandler::subscribe(const QMqttTopicFilter &filter)
{
    if (m_filters.contains(filter)) return;

    m_filters.append(filter);

    QMqttSubscription* subscribtion = m_mqttClient->subscribe(filter);

    connect(subscribtion, &QMqttSubscription::stateChanged, this, [this, subscribtion, filter](QMqttSubscription::SubscriptionState state)
    {
        switch (state)
        {
            case QMqttSubscription::Unsubscribed: qDebug() << "Отписались от топика" << filter.filter(); break;
            case QMqttSubscription::SubscriptionPending: qDebug() << "Подписываемся на топик" << filter.filter(); break;
            case QMqttSubscription::Subscribed:
            {
                qDebug() << "Успешно подписались на топик" << filter.filter();

                connect(subscribtion, &QMqttSubscription::messageReceived, this, [this](QMqttMessage msg)
                {
                    QString topicName = msg.topic().name();

                    QMutexLocker locker(&m_lastMessageMutex);                    

                    m_lastMessages.insert(topicName, msg);

                    emit msgReceived(msg);
                });

//                emit subscribed(filter.filter());
            }
            break;
            case QMqttSubscription::Error: qDebug() << "Не удалось подписаться на топик" << filter.filter(); break;
            default: break;
        }
    });
}

void MqttClientHandler::publish(const QString &topic, const QByteArray &payload, bool retain)
{
    m_mqttClient->publish(topic, payload, 0, retain);
}

void MqttClientHandler::onThreadStarted()
{
    getEnvirenmentVariables();

    m_mqttClient = new QMqttClient(this);
    m_mqttClient->setHostname(m_host);
    m_mqttClient->setPort(m_port);
    m_mqttClient->setKeepAlive(m_keepAlive);

    connect(m_mqttClient, &QMqttClient::stateChanged, this, [this]()
    {
        switch (m_mqttClient->state()) {
            case QMqttClient::Disconnected :
                qDebug() << "MQTT state: Disconnected"; break;
            case QMqttClient::Connecting :
                qDebug() << "MQTT state: Connecting"; break;
            case QMqttClient::Connected :
            {
                qDebug() << "MQTT state: Connected";
                emit connected();
            }
            break;
        }
    });

    connect(m_mqttClient, &QMqttClient::disconnected, this, []()
    {
        qDebug() << "Mqtt соединение разорвано";
    });

    m_mqttClient->connectToHost();
}

void MqttClientHandler::getEnvirenmentVariables()
{
    QProcessEnvironment variables = QProcessEnvironment::systemEnvironment();

    if (!variables.value(EXTERNAL_MQTT_HOST).isEmpty()) m_host = variables.value(EXTERNAL_MQTT_HOST);
    if (!variables.value(EXTERNAL_MQTT_PORT).isEmpty()) m_port = variables.value(EXTERNAL_MQTT_PORT).toInt();
    if (!variables.value(EXTERNAL_MQTT_KEEP_ALIVE).isEmpty()) m_keepAlive = variables.value(EXTERNAL_MQTT_KEEP_ALIVE).toInt();

    qDebug() << QString("Connect mqtt param: host - %1, port - %2, keepAlive - %3")
                .arg(m_host, QString::number(m_port), QString::number(m_keepAlive));
}
