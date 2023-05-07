#include "CAudioTopic.h"

const char *CAudioTopic::FIELD_SENDER = "Sender";
const char *CAudioTopic::FIELD_VALUE = "Value";
const char *CAudioTopic::FIELD_NEED_TO_SAVE = "NeedToSave";
const char *CAudioTopic::FIELD_ACTION = "Action";
const char *CAudioTopic::FIELD_TRACK = "Track";
const char *CAudioTopic::FIELD_UUID = "Uuid";
const char *CAudioTopic::FIELD_TARGET = "Target";
const char *CAudioTopic::FIELD_STATUS = "Status";
const char *CAudioTopic::FIELD_PRIORITY = "Priority";
const char *CAudioTopic::FIELD_DELAY = "Delay";

CAudioTopic::CAudioTopic(const QString &subTopicName) :
    CMqttTopic("AUDIO/" + subTopicName)
{
}

CAudioTopic::~CAudioTopic()
{
}

void CAudioTopic::setField(const QString &name, const QVariant &value)
{
//    setField(name, QVariant(value));
    m_fields[name] = value;
}

void CAudioTopic::setField(const QString &name, const QString &value)
{
    setField(name, QVariant(value));
}

void CAudioTopic::setField(const QString &name, int value)
{
    setField(name, QVariant(value));
}

void CAudioTopic::setField(const QString &name, double value)
{
    setField(name, QVariant(value));
}

void CAudioTopic::setField(const char *name, const char *value)
{
    setField(QString(name), QVariant(value));
}

void CAudioTopic::setField(const char *name, const QString &value)
{
    setField(QString(name), QVariant(value));
}

const QVariant CAudioTopic::getField(const QString &name) const
{
    if(m_fields.contains(name))
    {
        return m_fields[name];
    }
    return QVariant("");
}

const QByteArray CAudioTopic::prepareData() const
{
    QJsonObject jsonMainObj;

    foreach(const QString key, m_fields.keys() )
    {
        //jsonMainObj.insert(key, m_fields[key].toString());
        jsonMainObj.insert( key, QJsonValue::fromVariant( QVariant( m_fields[key])) );
    }

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

bool CAudioTopic::parseData( const QByteArray &data )
{
    bool res = false;

    QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );

    if ( ! jsonMainObj.isEmpty() )
    {
        foreach(const QString& key, jsonMainObj.keys())
        {
            QVariant tmp = jsonMainObj.value(key).toVariant();
            //QString tmp = jsonMainObj.value(key).toString();
            setField(key, tmp);
        }
    }
    else
    {
        printEmptyJsonMessage();
    }
    setValidState( res );
    return res;
}


