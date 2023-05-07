#include "CTopicConnectionState.h"

namespace
{
    const char *FIELD_USEDNETTYPE = "Used_net_type";
    const char *FIELD_CELLULARNETTYPE = "Cellular_net_type";
    const char *FIELD_CELLULARQUALITY = "Cellular_quality_percentage";
    const char *FIELD_WIFIQUALITY = "Wi_fi_quality_percentage";
    const QStringList FIELDS{
        FIELD_CELLULARNETTYPE,
        FIELD_CELLULARQUALITY,
        FIELD_USEDNETTYPE,
        FIELD_WIFIQUALITY
    };

    const QStringList NETTYPES{
        "cellular",
        "wi_fi",
        "ethernet"
    };
    const QStringList CELLULARTYPES{
        "4G",
        "3G",
        "2G",
        "No_coverage"
    };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicConnectionState::CTopicConnectionState(const QByteArray &json) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Connection_connectionState )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicConnectionState::initFromData( const CTopicValueEntry &value )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.keys() == FIELDS )
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
//            PRINT_CONSOLE_MESSAGE_WARN( "JSON объект содержит неизвестный список полей: " + jsonMainObj.keys().join( "," ) );
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
