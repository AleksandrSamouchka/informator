#ifndef CMQTTMESSAGE_H
#define CMQTTMESSAGE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>

#include <CMqttTopics/CTopicValueEntry.h>



class CMqttMessage
{
    QString m_topicName;
    CTopicValueEntry::ETopicsNamesIndex m_topicIndex;
    QByteArray m_message;
    quint8 m_qos;
    bool m_retain;

public:
    static const int MQTT_QOS_MIN{ 0 };
    static const int MQTT_QOS_MAX{ 2 };

public slots:
//    void setTopicName( const QString &topicName );
//    void setTopicName( const QString &topicName );
 //  void setMessage( const QByteArray &message );
//    void setQos( quint8 qos );
//    void setRetain( bool retain );

public:
    explicit CMqttMessage();

/*    explicit CMqttMessage(CTopicValueEntry::ETopicsNamesIndex index, const QJsonObject &obj);
    explicit CMqttMessage(CTopicValueEntry::ETopicsNamesIndex index, const QByteArray &message);
    explicit CMqttMessage(CTopicValueEntry::ETopicsNamesIndex index, const QString &message);
*/
    explicit CMqttMessage(CTopicValueEntry::ETopicsNamesIndex index, const QJsonObject &obj, 
                          bool retain = false/*, 
                          quint8 qos = 0*/);
    explicit CMqttMessage(CTopicValueEntry::ETopicsNamesIndex index, const QByteArray &message, 
                          bool retain = false/*, 
                          quint8 qos = 0*/);
    explicit CMqttMessage(CTopicValueEntry::ETopicsNamesIndex index, const QString &message, 
                          bool retain = false/*, 
                          quint8 qos = 0*/);

    explicit CMqttMessage(const QString &topicName, const QJsonObject &obj, 
                          bool retain = false/*, 
                          quint8 qos = 0*/);
    explicit CMqttMessage( const QString &topicName, const QByteArray &message, 
                          bool retain = false/*, 
                          quint8 qos = 0*/);
    explicit CMqttMessage(const QString &topicName, const QString &message,
                           bool retain = false/*,
                           quint8 qos = 0*/);

    QString getTopicName() const;
    CTopicValueEntry::ETopicsNamesIndex getTopicIndex() const;
    const QByteArray &getMessage() const;
    quint8 getQos() const;
    bool getRetain() const;

    bool isValid() const;

private:
    void init(const QString &topicName,
                           const QByteArray &message,
                           quint8 qos,
                           bool retain);
    void init(CTopicValueEntry::ETopicsNamesIndex index,
                           const QByteArray &message,
                           quint8 qos,
                           bool retain);
    void init(const QByteArray &message,
                           quint8 qos,
                           bool retain);
};

#endif // CMQTTMESSAGE_H
