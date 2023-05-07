#include "CTopicNavigationValueGnssState.h"

namespace
{
    const QStringList FIELDS
    {
        "Alt",
        "Dir",
        "Fix_type",      /*!< 2D / 3D */
        "HDOP",
        "Is_valid",      /*!< 0 / 1 */
        "Lat",
        "Lon",
        "PDOP",
        "Sat_used",
        "Sat_visible",
        "Speed",
        "Timestamp",
        "VDOP"
    };

    enum EFieldsIndexes
    {
        Field_altitude = 0,
        Field_direction,
        Field_fixType,
        Field_hdop,
        Field_isValid,
        Field_latitude,
        Field_longitude,
        Field_pdop,
        Field_satUsed,
        Field_satVisible,
        Field_speed,
        Field_ts,
        Field_vdop,
    };

    QString getFieldName( int index )
    {
        return FIELDS.value(  index, "" );
    }
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicNavigationValueGnssState::CTopicNavigationValueGnssState( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Navigation_gnssState )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicNavigationValueGnssState::initFromData( const CTopicValueEntry &value )
{
    bool res{ false };
    if ( value.isValid() )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
        if ( ! jsonMainObj.isEmpty() )
        {
            if ( jsonMainObj.keys() == FIELDS )
            {
                m_time = static_cast< uint32_t >( jsonMainObj.value( getFieldName( EFieldsIndexes::Field_ts ) ).toDouble() );
                m_isValid = jsonMainObj.value( getFieldName( EFieldsIndexes::Field_isValid ) ).toBool();
                m_fixType = jsonMainObj.value( getFieldName( EFieldsIndexes::Field_fixType ) ).toString();
                m_latitude = jsonMainObj.value( getFieldName( EFieldsIndexes::Field_latitude ) ).toDouble();
                m_longitude = jsonMainObj.value( getFieldName( EFieldsIndexes::Field_longitude ) ).toDouble();
                m_speed = jsonMainObj.value( getFieldName( EFieldsIndexes::Field_speed ) ).toDouble();
                m_altitude = qRound( jsonMainObj.value( getFieldName( EFieldsIndexes::Field_altitude ) ).toDouble() );
                m_dir = qRound( jsonMainObj.value( getFieldName( EFieldsIndexes::Field_direction ) ).toDouble() );
                m_satVisible = jsonMainObj.value( getFieldName( EFieldsIndexes::Field_satVisible ) ).toInt();
                m_satUsed = jsonMainObj.value( getFieldName( EFieldsIndexes::Field_satUsed ) ).toInt();
                m_hdop = qRound( jsonMainObj.value( getFieldName( EFieldsIndexes::Field_hdop ) ).toDouble() );
                m_vdop = qRound( jsonMainObj.value( getFieldName( EFieldsIndexes::Field_vdop ) ).toDouble() );
                m_pdop = qRound( jsonMainObj.value( getFieldName( EFieldsIndexes::Field_pdop ) ).toDouble() );
                res = true;
            }
            else
            {
//                PRINT_CONSOLE_MESSAGE_WARN( "JSON объект содержит неизвестный список полей: " + jsonMainObj.keys().join( "," ) );
                printUnknownFieldsListMessage(jsonMainObj.keys());
            }
        }
        else
        {
            printEmptyJsonMessage();
        }
        setValidState( res );
    }
    return res;
}

QByteArray CTopicNavigationValueGnssState::prepareData() const
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_ts ), static_cast< double >( m_time ) );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_isValid ), m_isValid );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_fixType ), m_fixType );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_latitude ), m_latitude );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_longitude ), m_longitude );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_speed ), m_speed );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_altitude ), m_altitude );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_direction ), m_dir );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_satVisible ), m_satVisible );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_satUsed ), m_satUsed );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_hdop ), m_hdop );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_vdop ), m_vdop );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_pdop ), m_pdop );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}




