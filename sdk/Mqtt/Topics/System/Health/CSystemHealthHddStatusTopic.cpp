#include "CSystemHealthHddStatusTopic.h"

static const char * TOPIC = "HDD_STATUS";

static const char *FIELD_LIST = "List";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CSystemHealthHddStatusTopic::CSystemHealthHddStatusTopic() :
    CGrSystemHealthTopic( TOPIC )
{ }

CSystemHealthHddStatusTopic::~CSystemHealthHddStatusTopic()
{ }

void CSystemHealthHddStatusTopic::clearVolumes()
{
    m_volumes.clear();
}

void CSystemHealthHddStatusTopic::setVolumes(const QStringList & value)
{
    m_volumes = value;
}

/** ***********************************************************************
 * @brief prepareData
 ** ***********************************************************************/
const QByteArray CSystemHealthHddStatusTopic::prepareData() const
{
    QJsonObject jsonMainObj{ { FIELD_LIST, QJsonArray::fromStringList( m_volumes ) } };
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @brief parseData
 ** ***********************************************************************/
bool CSystemHealthHddStatusTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    m_volumes.clear();
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_LIST ) )
        {
            for ( const auto val : jsonMainObj.value( FIELD_LIST ).toArray() )
            {
                m_volumes.append( val.toString() );
            }
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
    return setValidState( res );
}

