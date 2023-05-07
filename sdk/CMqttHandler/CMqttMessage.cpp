#include <QJsonDocument>

#include "CMqttMessage.h"


/** ***********************************************************************
 * @fn CMqttMessage  [конструктор]
 ** ***********************************************************************/

CMqttMessage::CMqttMessage()
{
    init("", {}, 0, false);
}

CMqttMessage::CMqttMessage(CTopicValueEntry::ETopicsNamesIndex index, 
                           const QJsonObject &obj, 
                           bool retain/*,
                           quint8 qos*/)
{
//    QString topicName = CTopicValueEntry::getTopicName( index );
    QJsonDocument doc = QJsonDocument(obj);
    init(index, doc.toJson(), 0/*qos*/, retain);
}

CMqttMessage::CMqttMessage(CTopicValueEntry::ETopicsNamesIndex index, 
                            const QByteArray &message,
                            bool retain/*,
                            quint8 qos*/)
{
//    QString topicName = CTopicValueEntry::getTopicName( index );
    init(index, message, 0/*qos*/, retain);
}

CMqttMessage::CMqttMessage(CTopicValueEntry::ETopicsNamesIndex index, 
                           const QString &message,
                           bool retain/*,
                           quint8 qos*/)
{
//    QString topicName = CTopicValueEntry::getTopicName( index );
    init(index, message.toLocal8Bit(), 0/*qos*/, retain);
}

CMqttMessage::CMqttMessage(const QString &topicName, const QJsonObject &obj, bool retain/*, quint8 qos*/)
{
    QJsonDocument doc = QJsonDocument(obj);
    init(topicName, doc.toJson(), 0/*qos*/, retain);
}


CMqttMessage::CMqttMessage(const QString &topicName, const QString &message, bool retain/*, quint8 qos*/)
{
    init(topicName, message.toLocal8Bit(), 0/*qos*/, retain);
}

CMqttMessage::CMqttMessage(const QString &topicName, const QByteArray &message, bool retain/*, quint8 qos*/) 
{
    init(topicName, message, 0/*qos*/, retain);
}

/*CMqttMessage::CMqttMessage(const QString &topicName, const QString &message, const quint8 qos, const bool retain) :
    CMqttMessage( topicName, message.toUtf8(), qos, retain )
{ }
*/

void CMqttMessage::init(const QString &topicName, const QByteArray &message,
                           quint8 qos,
                           bool retain)
{
//    /*static auto id1 = */qRegisterMetaType<CMqttMessage>("CMqttMessage");
////    Q_UNUSED(id1);
    m_topicIndex = CTopicValueEntry::getTopicNameIndex(topicName);
    m_topicName = topicName;
//    m_message = message;
//    m_qos = qos;
//    m_retain = retain;
    init(message, qos, retain);
}

void CMqttMessage::init(CTopicValueEntry::ETopicsNamesIndex index,
                           const QByteArray &message/* = QByteArray{}*/,
                           quint8 qos,
                           bool retain)
{
//    /*static auto id1 = */qRegisterMetaType<CMqttMessage>("CMqttMessage");
//    Q_UNUSED(id1);
    m_topicIndex = index;
    m_topicName = CTopicValueEntry::getTopicName( index );
//    m_message = message;
//    m_qos = qos;
//    m_retain = retain;
    init(message, qos, retain);
}

void CMqttMessage::init(const QByteArray &message/* = QByteArray{}*/,
                           quint8 qos,
                           bool retain)
{
    qRegisterMetaType<CMqttMessage>("CMqttMessage");
//    m_topicIndex = index;
//    m_topicName = CTopicValueEntry::getTopicName( index );
    m_message = message;
    m_qos = qos;
    m_retain = retain;
}



/** ***********************************************************************
 * @fn setters
 * @brief слоты для задания свойств класса
 ** ***********************************************************************/
/*
void CMqttMessage::setTopicName(const QString &topicName)
{
    m_topicName = topicName;
}

void CMqttMessage::setMessage(const QByteArray &message)
{
    m_message = message;
}

void CMqttMessage::setQos(quint8 qos)
{
    m_qos = qos;
}

void CMqttMessage::setRetain(bool retain)
{
    m_retain = retain;
}
*/

/** ***********************************************************************
 * @fn getters
 * @brief методы для чтения свойств класса
 ** ***********************************************************************/
CTopicValueEntry::ETopicsNamesIndex CMqttMessage::getTopicIndex() const
{
    return m_topicIndex;
}

QString CMqttMessage::getTopicName() const
{
    return m_topicName;
}

const QByteArray &CMqttMessage::getMessage() const
{
    return m_message;
}

quint8 CMqttMessage::getQos() const
{
    return m_qos;
}

bool CMqttMessage::getRetain() const
{
    return m_retain;
}

/** ***********************************************************************
 * @fn isValid
 * @brief метод, проверяет поля на корректность заполнения:
 *  topicName не должен быть пустым
 *  qos должен быть в диапазоне от 0 до 2 включительно
 * @param none
 * @return none
 ** ***********************************************************************/
bool CMqttMessage::isValid() const
{
    bool res{ false };
    if ( ! m_topicName.isEmpty() && m_qos <= MQTT_QOS_MAX )
    {
        res = true;
    }
    return res;
}

