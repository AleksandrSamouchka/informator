#include "CStmRoOneWireTopic.h"

static const char * TOPIC = "ONEWIRE";

static const char *FIELD_MAIN = "OneWire";

static const char *FIELD_ID = "Id";
static const char *FIELD_TEMP = "Temperature";
static const char *FIELD_NAME = "Name";

const QString CStmRoOneWireTopic::EMPTY_ID = "0000000000000000";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CStmRoOneWireTopic::CStmRoOneWireTopic() :
    CGrStmRoTopic( TOPIC )
{ }

CStmRoOneWireTopic::~CStmRoOneWireTopic()
{ }

/** ***********************************************************************
 * @fn appendTermoSensor
 ** ***********************************************************************/
void CStmRoOneWireTopic::appendTermoSensor(const CStmRoOneWireTopic::STermoState & value)
{
    m_sensors.append( value );
}

/** ***********************************************************************
 * @fn setTermoSensorList
 ** ***********************************************************************/
void CStmRoOneWireTopic::setTermoSensorList(const QList<CStmRoOneWireTopic::STermoState> & value)
{
    m_sensors = value;
}

/** ***********************************************************************
 * @fn clearTermoSensorList
 ** ***********************************************************************/
void CStmRoOneWireTopic::clearTermoSensorList()
{
    m_sensors.clear();
}

/** ***********************************************************************
 * @fn prepareData
 ** ***********************************************************************/
const QByteArray CStmRoOneWireTopic::prepareData() const
{
    QJsonArray jsonArr{};
    for ( const auto & i : m_sensors )
    {
        QJsonObject jsonObj{ { FIELD_NAME, i.m_name },
                             { FIELD_ID, i.m_id },
                             { FIELD_TEMP, i.m_value } };
        jsonArr.append( jsonObj );
    }
    QJsonObject jsonMainObj{ { FIELD_MAIN, jsonArr } };
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @fn parseData
 ** ***********************************************************************/
bool CStmRoOneWireTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    m_sensors.clear();
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_MAIN ) )
        {
            for ( const auto i : jsonMainObj.value( FIELD_MAIN ).toArray() )
            {
                QJsonObject jsonObj = i.toObject();
                if ( jsonObj.contains( FIELD_NAME )
                     && jsonObj.contains( FIELD_ID )
                     && jsonObj.contains( FIELD_TEMP ) )
                {
                    m_sensors.append( STermoState( jsonObj.value( FIELD_NAME ).toString(),
                                                   jsonObj.value( FIELD_ID ).toString(),
                                                   jsonObj.value( FIELD_TEMP ).toDouble() ) );
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_WARN( "JSON объект списка содержит неизвестный список полей: " + i.toObject().keys().join( "," ) );
                }
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



