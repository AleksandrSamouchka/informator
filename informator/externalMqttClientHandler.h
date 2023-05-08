#pragma once

#include <QObject>
#include <QMqttClient>
#include <QMqttSubscription>
#include <QMqttMessage>
#include <QProcessEnvironment>
#include <QMutex>

#include <Mqtt/Topics/Informator/InformatorDefines.h>

class MqttClientHandler : public QObject
{
    Q_OBJECT
public:
    MqttClientHandler(QObject* parent = nullptr);
public slots:
    QMqttMessage getLastMsg(const QString& topic);
    QList<QMqttMessage> getSubgroupMsgs(const QString& subgroup);
    void subscribe(const QMqttTopicFilter& filter);
    void publish(const QString& topic, const QByteArray& payload, bool retain = false);
signals:
    void connected();
    void msgReceived(QMqttMessage msg);
    void subscribed(QString topicName);
public slots:
    void onThreadStarted();
private:
    void getEnvirenmentVariables();
private:
    QString                                 m_host;
    quint16                                 m_port;
    quint16                                 m_keepAlive;
    QMqttClient*                            m_mqttClient;
    QList<QMqttTopicFilter>                 m_filters;
    QHash<QMqttTopicFilter, QMqttMessage>   m_lastMessages;
    QMutex                                  m_lastMessageMutex;
};

