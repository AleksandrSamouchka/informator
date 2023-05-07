#ifndef CMQTTTOPICCONNECTION_H
#define CMQTTTOPICCONNECTION_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QMutex>

#include <CMqttTopics/CTopicValueEntry.h>
#include <CMqttHandler/CMqttMessage.h>

/** ***********************************************************************
 * @class CMqttTopicConnection
 * @brief Класс, реализующий подключение к торику и принятие / отправку данных
 *  с какими-то параметрами
 *      После создания оюъекта нужно проверить возможное принятое retain сообщение
 *  (если это нужно) с помощью метода getLastValue
 ** ***********************************************************************/
class CMqttTopicConnection : public QObject
{
    Q_OBJECT

public:
    enum class EConnectionDirection
    {
        Connection_subscribeOnly = 0,
        Connection_publishOnly,
        Connection_fullDuplex,
    };

private:
    QString m_topic = QString{};
//    CTopicValueEntry::ETopicsNamesIndex m_topicIndex{ CTopicValueEntry::ETopicsNamesIndex::Topic_unknown };
    CTopicValueEntry m_lastTopicValue = CTopicValueEntry{};
    EConnectionDirection m_direction = EConnectionDirection::Connection_subscribeOnly;

    QMutex m_lastValueMutex{};

/* скрытый интерфейс */
public slots:
    void messageReceived( const CMqttMessage &message );
signals:
    void needToPublishMessage( const CMqttMessage &);


/* доступные интерфейсы */
signals:
    void valueUpdated( const CTopicValueEntry & );

public slots:
    CTopicValueEntry getLastValue();
    bool publishValue( const CTopicValueEntry &message );

public:
    explicit CMqttTopicConnection( const CTopicValueEntry::ETopicsNamesIndex topic,
                                   const EConnectionDirection direction,
                                   QObject * parent = nullptr );
    explicit CMqttTopicConnection( const QString &topic,
                                   const EConnectionDirection direction,
                                   QObject * parent = nullptr );

    EConnectionDirection getConnectionDirection() const;
    QString getTopic() const;
};

#endif // CMQTTTOPICCONNECTION_H
