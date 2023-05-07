#include "Mqtt.h"

QScopedPointer<Mqtt>    Mqtt::m_instance;
std::once_flag          Mqtt::m_flag;

Mqtt *Mqtt::getInstance()
{
    std::call_once(Mqtt::m_flag, [&]()
    {
        m_instance.reset(new Mqtt());
    });

    return m_instance.data();
}

void Mqtt::subscribe(const QMqttTopicFilter &filter)
{
    if (!QMetaObject::invokeMethod(m_mqttHandler.data(), "subscribe",
                                   Q_ARG(QMqttTopicFilter, filter)))
    {
        qWarning() << "Невозможно вызвать invokeMethod mqtt subscribe для топика" << filter.filter();
    }    
}

void Mqtt::publish(const QString &topic, const QByteArray &payload, bool retain)
{
    if (!QMetaObject::invokeMethod(m_mqttHandler.data(), "publish", Q_ARG(QString, topic),
                                   Q_ARG(QByteArray, payload), Q_ARG(bool, retain)))
    {
        qWarning() << "Can't invoke publish method of mqttHandler";
    }
}

Mqtt::Mqtt(QObject *parent)
    : QObject(parent),
      m_mqttWorkerThread(new QThread(this)),
      m_mqttHandler(new MqttClientHandler())
{
    connect(m_mqttWorkerThread, &QThread::started, m_mqttHandler.data(), &MqttClientHandler::onThreadStarted);
    connect(m_mqttWorkerThread, &QThread::finished, m_mqttHandler.data(), &MqttClientHandler::deleteLater);
    connect(m_mqttHandler.data(), &MqttClientHandler::connected, this, &Mqtt::connected);
    connect(m_mqttHandler.data(), &MqttClientHandler::msgReceived, this, &Mqtt::msgReceived);
    connect(m_mqttHandler.data(), &MqttClientHandler::subscribed, this, &Mqtt::subscribed);

    m_mqttHandler->moveToThread(m_mqttWorkerThread);
    m_mqttWorkerThread->start();
}
