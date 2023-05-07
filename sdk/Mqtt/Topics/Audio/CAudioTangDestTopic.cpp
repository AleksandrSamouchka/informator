#include "CAudioTangDestTopic.h"

CAudioTangDestTopic::CAudioTangDestTopic() :
    CAudioTopic("TANGDEST")
{
}

CAudioTangDestTopic::~CAudioTangDestTopic()
{
}

bool CAudioTangDestTopic::parseData( const QByteArray & data )
{
    bool res = false;

    QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    QStringList keys = {
        CAudioTopic::FIELD_SENDER,
        CAudioTopic::FIELD_STATUS,
        CAudioTopic::FIELD_TARGET
    };

    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.keys() == keys)
        {
// TODO Need to make loop for keys list
            CAudioTopic::setField(CAudioTopic::FIELD_SENDER, jsonMainObj.value( CAudioTopic::FIELD_SENDER ).toString());
// TODO Need to make enum  TO_LTE, TO_KONGA
            CAudioTopic::setField(CAudioTopic::FIELD_STATUS, jsonMainObj.value( CAudioTopic::FIELD_STATUS ).toString());
            CAudioTopic::setField(CAudioTopic::FIELD_TARGET, jsonMainObj.value( CAudioTopic::FIELD_TARGET ).toString());
            res = true;
        }
        else
        {
            printUnknownFieldsListMessage(jsonMainObj.keys());
        }
    }
    else
    {
        printEmptyJsonMessage();
    }
    setValidState( res );
    return res;
}

const QByteArray CAudioTangDestTopic::prepareData() const 
{
    QJsonObject jsonMainObj{};
    foreach(const QString key, m_fields.keys() )
    {
// TODO Need to make enum For Status
        jsonMainObj.insert(key, m_fields[key].toString());
    }
    
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}


