#include "CConnectionConnectionStateTopic.h"

static const char * TOPIC = "CONNECTION_STATE";

static const char *FIELD_USEDNETTYPE = "Used_net_type";
static const char *FIELD_CELLULARNETTYPE = "Cellular_net_type";
static const char *FIELD_CELLULARQUALITY = "Cellular_quality_percentage";
static const char *FIELD_WIFIQUALITY = "Wi_fi_quality_percentage";

static const QStringList NETTYPES{
    "cellular",
    "wi_fi",
    "ethernet"
};
static const QStringList CELLULARTYPES{
    "4G",
    "3G",
    "2G",
    "No_coverage"
};

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CConnectionConnectionStateTopic::CConnectionConnectionStateTopic() :
    CGrConnectionTopic( TOPIC )
{ }

CConnectionConnectionStateTopic::CConnectionConnectionStateTopic( const QByteArray & data ) :
    CGrConnectionTopic( TOPIC, data )
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CConnectionConnectionStateTopic::getEtalonJson()
{
    QJsonObject jsonMainObj( { { FIELD_USEDNETTYPE, NETTYPES.value( static_cast< int >( EUsedNetType::NetType_wiFi ) ) },
                                     { FIELD_CELLULARNETTYPE, CELLULARTYPES.value( static_cast< int >( ECellurarNetType::CellurarType_3G ) ) },
                                     { FIELD_CELLULARQUALITY, 10 },
                                     { FIELD_WIFIQUALITY, 20 } } );
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

void CConnectionConnectionStateTopic::setUsedNetType(const CConnectionConnectionStateTopic::EUsedNetType value)
{
    m_usedNetType = value;
}

void CConnectionConnectionStateTopic::setCellurarNetType(const CConnectionConnectionStateTopic::ECellurarNetType value)
{
    m_cellurarNetType = value;
}

void CConnectionConnectionStateTopic::setCellurarQuality(const int value)
{
    m_cellurarQuality = value;
}

void CConnectionConnectionStateTopic::setWiFiQuality(const int value)
{
    m_wiFiQuality = value;
}


const QByteArray CConnectionConnectionStateTopic::prepareData() const
{
    QJsonObject jsonMainObj( { { FIELD_USEDNETTYPE, NETTYPES.value( static_cast< int >( m_usedNetType ) ) },
                                     { FIELD_CELLULARNETTYPE, CELLULARTYPES.value( static_cast< int >( m_cellurarNetType ) ) },
                                     { FIELD_CELLULARQUALITY, m_cellurarQuality },
                                     { FIELD_WIFIQUALITY, m_wiFiQuality } } );
//    QByteArray jsonData{ 
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
//};
//    return setData( /*jsonData*/jsonMainObj );
}

bool CConnectionConnectionStateTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_USEDNETTYPE ) &&
             jsonMainObj.contains( FIELD_CELLULARNETTYPE ) &&
             jsonMainObj.contains( FIELD_CELLULARQUALITY ) &&
             jsonMainObj.contains( FIELD_WIFIQUALITY ) )
        {
            QString indexStr( jsonMainObj.value( FIELD_USEDNETTYPE ).toString() );
            int index{ NETTYPES.indexOf( indexStr ) };
            if ( index >= static_cast< int >( EUsedNetType::NetType_cellular ) &&
                 index <= static_cast< int >( EUsedNetType::NetType_ethernet ) )
            {
                m_usedNetType = static_cast< EUsedNetType >( index );
            }
            indexStr = jsonMainObj.value( FIELD_CELLULARNETTYPE ).toString();
            index = CELLULARTYPES.indexOf( indexStr );
            if ( index >= static_cast< int >( ECellurarNetType::CellurarType_4G ) &&
                 index <= static_cast< int >( ECellurarNetType::CellurarType_noCoverage ) )
            {
                m_cellurarNetType = static_cast< ECellurarNetType >( index );
            }
            m_cellurarQuality = qRound( jsonMainObj.value( FIELD_CELLULARQUALITY ).toDouble() );
            m_wiFiQuality = qRound( jsonMainObj.value( FIELD_WIFIQUALITY ).toDouble() );
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
