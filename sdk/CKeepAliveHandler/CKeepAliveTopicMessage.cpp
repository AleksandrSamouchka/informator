#include "CKeepAliveTopicMessage.h"
#include <CLoggerMessage/CLoggerMessage.h>

#include <QtCore/QVariant>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>

namespace
{
    const char *FIELD_VERSION = "Version";
//    const char *FIELD_REVISION = "Revision";
    const char *FIELD_PAYLOAD = "Payload";
}

/** ***********************************************************************
 * @fn CKeepAliveTopicMessage  [конструктор]
 ** ***********************************************************************/
CKeepAliveTopicMessage::CKeepAliveTopicMessage( const QString &payload ) :
    mPayload( payload )
{ }

/** ***********************************************************************
* @fn prepareData
* @brief метод формирования значения топика из полей структуры
* @param none
* @return QByteArray
** ***********************************************************************/
QByteArray CKeepAliveTopicMessage::prepareData() const
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_VERSION, APP_VERSION_STRING );
    if ( ! mPayload.isEmpty() )
    {
        jsonMainObj.insert( FIELD_PAYLOAD, mPayload );
    }
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
