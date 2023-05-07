#include "CNavigationGnssStateTopic.h"

static const char * TOPIC = "GNSS_STATE";

static const QStringList FIELDS
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

static QString getFieldName( int index )
{
    return FIELDS.value(  index, "" );
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CNavigationGnssStateTopic::CNavigationGnssStateTopic() :
    CGrNavigationTopic( TOPIC )
{

}

CNavigationGnssStateTopic::CNavigationGnssStateTopic(const QByteArray & data) :
    CGrNavigationTopic( TOPIC, data )
{

}

CNavigationGnssStateTopic::~CNavigationGnssStateTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CNavigationGnssStateTopic::getEtalonJson()
{
    QJsonObject jsonMainObj{};

    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_ts ), static_cast< double >( 1570154 ) );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_isValid ), true );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_fixType ), "3D" );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_latitude ), 55.01401901245117 );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_longitude ), 82.95474243164062 );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_speed ), 0.8908120393753052 );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_altitude ), 161 );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_direction ), 0 );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_satVisible ), 22 );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_satUsed ), 20 );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_hdop ), 1 );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_vdop ), 2 );
    jsonMainObj.insert( getFieldName( EFieldsIndexes::Field_pdop ), 3 );

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

void CNavigationGnssStateTopic::setTime(const uint32_t value)
{
    m_time = value;
}

void CNavigationGnssStateTopic::setIsValid(const bool value)
{
    m_isValid = value;
}

void CNavigationGnssStateTopic::setFixType(const QString & value)
{
    m_fixType = value;
}

void CNavigationGnssStateTopic::setLatitude(const double value)
{
    m_latitude = value;
}

void CNavigationGnssStateTopic::setLongitude(const double value)
{
    m_longitude = value;
}

void CNavigationGnssStateTopic::setSpeed(const double value)
{
    m_speed = value;
}

void CNavigationGnssStateTopic::setAltitude(const int value)
{
    m_altitude = value;
}

void CNavigationGnssStateTopic::setDir(const int value)
{
    m_dir = value;
}

void CNavigationGnssStateTopic::setSatVisible(const int value)
{
    m_satVisible = value;
}

void CNavigationGnssStateTopic::setSatUsed(const int value)
{
    m_satUsed = value;
}

void CNavigationGnssStateTopic::setHDop(const int value)
{
    m_hdop = value;
}

void CNavigationGnssStateTopic::setVDop(const int value)
{
    m_vdop = value;
}

void CNavigationGnssStateTopic::setPDop(const int value)
{
    m_pdop = value;
}

const QByteArray CNavigationGnssStateTopic::prepareData() const
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
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

bool CNavigationGnssStateTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
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
            printUnknownFieldsListMessage(jsonMainObj.keys());
        }
    }
    else
    {
        printEmptyJsonMessage();
    }
    return setValidState( res );
}
