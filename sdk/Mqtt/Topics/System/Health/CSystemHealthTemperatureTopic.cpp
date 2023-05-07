#include "CSystemHealthTemperatureTopic.h"

static const char * TOPIC = "TEMPERATURE";

/*!
"Amplifer": 0,
"Congatec": {
    "Board temperature": 46,
    "CPU temperature": 45
},
"Disks": {
    "sda": {
        "curr_max_temp": "48",
        "curr_min_temp": "30",
        "curr_temp": "47",
        "spec_max_temp": "100",
        "spec_min_temp": "0",
        "type": "SSD"
    },
    "sdb": {
        "curr_max_temp": "unknown",
        "curr_min_temp": "unknown",
        "curr_temp": "47",
        "spec_max_temp": "unknown",
        "spec_min_temp": "unknown",
        "type": "M.2"
    }
}
*/

static const char *FIELD_AMPLIFER = "Amplifer";
static const char *FIELD_CONGATEC = "Congatec";
static const char *FIELD_DISKS = "Disks";

static const char *FIELD_CONGA_BOARDTEMP = "Board temperature";
static const char *FIELD_CONGA_CPUTEMP = "CPU temperature";

static const char *FIELD_DISK_CURRMAXTEMP = "curr_max_temp";
static const char *FIELD_DISK_CURRMINTEMP = "curr_min_temp";
static const char *FIELD_DISK_CURRTEMP = "curr_temp";
static const char *FIELD_DISK_SPECMAXTEMP = "spec_max_temp";
static const char *FIELD_DISK_SPECMINTEMP = "spec_min_temp";
static const char *FIELD_DISK_TYPE = "type";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CSystemHealthTemperatureTopic::CSystemHealthTemperatureTopic() :
    CGrSystemHealthTopic( TOPIC )
{ }

CSystemHealthTemperatureTopic::~CSystemHealthTemperatureTopic()
{ }

const QByteArray CSystemHealthTemperatureTopic::prepareData() const
{
    QJsonObject jsonMainObj{ { FIELD_AMPLIFER, m_amplifierTemp } };
    QJsonObject obj( { { FIELD_CONGA_BOARDTEMP, m_congaBoardTemp },
                       { FIELD_CONGA_CPUTEMP, m_congaCpuTemp } } );
    jsonMainObj.insert( FIELD_CONGATEC, obj );
    obj = QJsonObject();
    for ( const auto & i : m_disksTemperature.keys() )
    {
        const SDisckTemperature diskTemp( m_disksTemperature.value( i ) );
        const QJsonObject obj2( { { FIELD_DISK_CURRMAXTEMP, diskTemp.m_currMaxTemp },
                                  { FIELD_DISK_CURRMINTEMP, diskTemp.m_currMinTemp },
                                  { FIELD_DISK_CURRTEMP, diskTemp.m_currTemp },
                                  { FIELD_DISK_SPECMAXTEMP, diskTemp.m_specMaxTemp },
                                  { FIELD_DISK_SPECMINTEMP, diskTemp.m_specMinTemp },
                                  { FIELD_DISK_TYPE, diskTemp.m_type } } );
        obj.insert( i, obj2 );
    }
    jsonMainObj.insert( FIELD_DISKS, obj );
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

bool CSystemHealthTemperatureTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    m_amplifierTemp = -1;
    m_congaBoardTemp = -1;
    m_congaCpuTemp = -1;
    m_disksTemperature.clear();
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_AMPLIFER ) )
        {
            m_amplifierTemp = jsonMainObj.value( FIELD_AMPLIFER ).toInt();
        }
        if ( jsonMainObj.contains( FIELD_CONGATEC ) )
        {
            const QJsonObject obj( jsonMainObj.value( FIELD_CONGATEC ).toObject() );
            if ( obj.contains( FIELD_CONGA_BOARDTEMP )
                 && obj.contains( FIELD_CONGA_CPUTEMP ) )
            {
//                PRINT_CONSOLE_MESSAGE_INFO( QString::number( obj.value( FIELD_CONGA_CPUTEMP ).type() ) );
                m_congaBoardTemp = obj.value( FIELD_CONGA_BOARDTEMP ).toInt();
                m_congaCpuTemp = obj.value( FIELD_CONGA_CPUTEMP ).toInt();
            }
            else
            {
                PRINT_CONSOLE_MESSAGE_WARN( "Внутренний JSON объект FIELD_CONGATEC содержит неизвестный список полей: "
                                            + obj.keys().join( "," ) );
            }
        }
        if ( jsonMainObj.contains( FIELD_DISKS ) )
        {
            const QJsonObject obj( jsonMainObj.value( FIELD_DISKS ).toObject() );
            for ( const auto & i : obj.keys() )
            {
                const QJsonObject obj2( obj.value( i ).toObject() );
                if ( obj2.contains( FIELD_DISK_CURRMAXTEMP )
                     && obj2.contains( FIELD_DISK_CURRMINTEMP )
                     && obj2.contains( FIELD_DISK_CURRTEMP )
                     && obj2.contains( FIELD_DISK_SPECMAXTEMP )
                     && obj2.contains( FIELD_DISK_SPECMINTEMP )
                     && obj2.contains( FIELD_DISK_TYPE ) )
                {
                    SDisckTemperature diskTemp{};
                    diskTemp.m_currMaxTemp = obj2.value( FIELD_DISK_CURRMAXTEMP ).toString();
                    diskTemp.m_currMinTemp = obj2.value( FIELD_DISK_CURRMINTEMP ).toString();
                    diskTemp.m_currTemp = obj2.value( FIELD_DISK_CURRTEMP ).toString();
                    diskTemp.m_specMaxTemp = obj2.value( FIELD_DISK_SPECMAXTEMP ).toString();
                    diskTemp.m_specMinTemp = obj2.value( FIELD_DISK_SPECMINTEMP ).toString();
                    diskTemp.m_type = obj2.value( FIELD_DISK_TYPE ).toString();
                    m_disksTemperature.insert( i, diskTemp );
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_WARN( "Внутренний JSON объект FIELD_DISKS содержит неизвестный список полей: "
                                                + obj.keys().join( "," ) );
                }
            }
        }
        res = true;
    }
    else
    {
        printEmptyJsonMessage();
    }
    return setValidState( res );


}
