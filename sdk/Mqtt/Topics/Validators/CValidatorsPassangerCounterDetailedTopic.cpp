#include "CValidatorsPassangerCounterDetailedTopic.h"

static const char * TOPIC = "PASSANGER_COUNTER_DETAILED";

static const char *FIELD_ROUTE_NAME = "route_name";
static const char *FIELD_ROUTE_ID = "route_id";
static const char *FIELD_STATE = "state";
static const char *FIELD_CURRENT_DATA = "current_data";
static const char *FIELD_PREVIOUS_DATA = "previous_data";
static const char *FIELD_ABSOLUTE_COUNTERS = "absolute_counters";
static const char* FIELD_ABSOLUT_TRANSACTION_COUNTER = "absolute_transaction_counter";

static const char *FIELD_STATION_NUM = "station_num";
static const char *FIELD_STATION_NAME = "station_name";
static const char *FIELD_STATION_STOPID = "station_stopid";
static const char *FIELD_PASSANGERS = "passanger_counters";
static const char *FIELD_TRANSACTION_COUNTER = "transaction_counter";

static const char *FIELD_DOOR_NAME = "door_name";
static const char *FIELD_COME_IN_COUNTER = "come_in_counter";
static const char *FIELD_COME_OUT_COUNTER = "come_out_counter";

const QStringList CValidatorsPassangerCounterDetailedTopic::STATES = QStringList {
        "DOOR_OPENED",
        "DOOR_CLOSED",
        "IN_MOVEMENT",
        "UNKNOWN_ROUTE",
        "STAGE_CHANGED",
        "TRIP_START",
        "TRIP_END"};

/** ***********************************************************************
 * @fn Конструкторы
 ** ***********************************************************************/
CValidatorsPassangerCounterDetailedTopic::CValidatorsPassangerCounterDetailedTopic() :
    CGrValidatorsTopic( TOPIC ),
    m_absoluteTransactionCounter(-1)
{ }

CValidatorsPassangerCounterDetailedTopic::CValidatorsPassangerCounterDetailedTopic(const QByteArray & data) :
    CGrValidatorsTopic( TOPIC, data ),
    m_absoluteTransactionCounter(-1)
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CValidatorsPassangerCounterDetailedTopic::getEtalonJson()
{
    QJsonArray jsonArr{};
    for ( int i = 0; i < 2; i ++ )
    {
        QJsonObject jsonObj( { { FIELD_COME_IN_COUNTER, 100 },
                               { FIELD_COME_OUT_COUNTER, 200 },
                               { FIELD_DOOR_NAME, "testChannelName" } } );
        jsonArr.append( jsonObj );
    }
    QJsonObject jsonMainObj( { { FIELD_ROUTE_NAME, "testRouteName" },
                               { FIELD_STATION_NAME, "testStationName" } } );
    jsonMainObj.insert( FIELD_ABSOLUTE_COUNTERS, jsonArr );
    jsonMainObj.insert( FIELD_CURRENT_DATA,
                        QJsonObject( { { FIELD_STATION_NUM, 2 },
                                       { FIELD_STATION_NAME, "testStationName1" },
                                       { FIELD_PASSANGERS, jsonArr },
                                       { FIELD_TRANSACTION_COUNTER, 12 } } ) );
    jsonMainObj.insert( FIELD_PREVIOUS_DATA,
                        QJsonObject( { { FIELD_STATION_NUM, 3 },
                                       { FIELD_STATION_NAME, "testStationName2" },
                                       { FIELD_PASSANGERS, jsonArr },
                                       { FIELD_TRANSACTION_COUNTER, 13 } } ) );
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

void CValidatorsPassangerCounterDetailedTopic::setRouteName(const QString & value)
{
    m_routeName = value;
}

void CValidatorsPassangerCounterDetailedTopic::setRouteID(qint64 value)
{
    m_routeID = value;
}

void CValidatorsPassangerCounterDetailedTopic::setState( const CValidatorsPassangerCounterDetailedTopic::EStates value)
{
    m_state = value;
}

void CValidatorsPassangerCounterDetailedTopic::setCurrentData( const CValidatorsPassangerCounterDetailedTopic::SPassangerData & value)
{
    m_currentData = value;
}

void CValidatorsPassangerCounterDetailedTopic::setPreviousData( const CValidatorsPassangerCounterDetailedTopic::SPassangerData & value)
{
    m_previousData = value;
}

void CValidatorsPassangerCounterDetailedTopic::setAbsolutPassangerCounters( const QList<CValidatorsPassangerCounterDetailedTopic::SPassangerCounterInfo> & value)
{
    m_absolutCounters = value;
}

void CValidatorsPassangerCounterDetailedTopic::setAbsoluteTransactionCounter(int transactionCount)
{
    m_absoluteTransactionCounter = transactionCount;
}

/** ***********************************************************************
 * @fn prepareData
 ** ***********************************************************************/
const QByteArray CValidatorsPassangerCounterDetailedTopic::prepareData() const
{
    QJsonObject jsonMainObj( { { FIELD_ROUTE_NAME, m_routeName },
                               { FIELD_ROUTE_ID, m_routeID},
                               { FIELD_STATE, STATES.value( static_cast< int >( m_state ), "UNKNOWN" ) } } );

    QJsonArray jsonArr{};
    QJsonObject jsonObj{};

    /*! absolute_counters */
    for ( const auto & i : m_absolutCounters )
    {
        QJsonObject jsonArrObj( { { FIELD_COME_IN_COUNTER, i.m_comeInCounter },
                                  { FIELD_COME_OUT_COUNTER, i.m_comeOutCounter },
                                  { FIELD_DOOR_NAME, i.m_channelName } } );
        jsonArr.append( jsonArrObj );
    }
    jsonMainObj.insert( FIELD_ABSOLUTE_COUNTERS, jsonArr );

/*! current_data */
    if(m_state != EStates::tripEnd)
    {
        jsonArr = QJsonArray{};
        for ( const auto & i : m_currentData.m_counters )
        {
            QJsonObject jsonArrObj( { { FIELD_COME_IN_COUNTER, i.m_comeInCounter },
                                      { FIELD_COME_OUT_COUNTER, i.m_comeOutCounter },
                                      { FIELD_DOOR_NAME, i.m_channelName } } );
            jsonArr.append( jsonArrObj );
        }
        jsonObj = QJsonObject( { { FIELD_STATION_NUM, m_currentData.m_stationNum },
                                 { FIELD_STATION_NAME, m_currentData.m_stationName },
                                 { FIELD_STATION_STOPID, m_currentData.m_stationStopID},
                                 { FIELD_PASSANGERS, jsonArr },
                                 { FIELD_TRANSACTION_COUNTER, m_currentData.m_transactionCounter } } );
        jsonMainObj.insert( FIELD_CURRENT_DATA, jsonObj );
    }

/*! previous_data */
    if (m_state == EStates::doorOpened ||
        m_state == EStates::tripEnd)
    {
        jsonArr = QJsonArray{};
        for ( const auto & i : m_previousData.m_counters )
        {
            QJsonObject jsonArrObj( { { FIELD_COME_IN_COUNTER, i.m_comeInCounter },
                                   { FIELD_COME_OUT_COUNTER, i.m_comeOutCounter },
                                   { FIELD_DOOR_NAME, i.m_channelName } } );
            jsonArr.append( jsonArrObj );
        }
        jsonObj = QJsonObject( { { FIELD_STATION_NUM, m_previousData.m_stationNum },
                                 { FIELD_STATION_NAME, m_previousData.m_stationName },
                                 { FIELD_STATION_STOPID, m_previousData.m_stationStopID},
                                 { FIELD_PASSANGERS, jsonArr },
                                 { FIELD_TRANSACTION_COUNTER, m_previousData.m_transactionCounter } } );
        jsonMainObj.insert( FIELD_PREVIOUS_DATA, jsonObj );
    }

    jsonMainObj.insert(FIELD_ABSOLUT_TRANSACTION_COUNTER, m_absoluteTransactionCounter);

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @fn parseData
 ** ***********************************************************************/
bool CValidatorsPassangerCounterDetailedTopic::parseData(const QByteArray & data)
{
    m_absolutCounters.clear();
    m_currentData = SPassangerData{};
    m_previousData = SPassangerData{};
    m_absoluteTransactionCounter = 0;
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_ROUTE_NAME ) &&
             jsonMainObj.contains( FIELD_ROUTE_ID ) &&
             jsonMainObj.contains( FIELD_STATE ) &&
             jsonMainObj.contains( FIELD_ABSOLUTE_COUNTERS ) )
        {
            m_routeName = jsonMainObj.value( FIELD_ROUTE_NAME ).toString();
            m_routeID = jsonMainObj.value( FIELD_ROUTE_ID ).toInt();
            int index = STATES.indexOf( jsonMainObj.value( FIELD_STATE ).toString() );
            m_state = (index == -1) ? EStates::unknownRoute : static_cast<EStates>(index);

        /*! absolute_counters */
            for ( const auto i : jsonMainObj.value( FIELD_ABSOLUTE_COUNTERS ).toArray() )
            {
                QJsonObject jsonArrObj = i.toObject();
                if ( jsonArrObj.contains( FIELD_COME_IN_COUNTER ) &&
                     jsonArrObj.contains( FIELD_COME_OUT_COUNTER ) &&
                     jsonArrObj.contains( FIELD_DOOR_NAME ) )
                {
                    m_absolutCounters.append( SPassangerCounterInfo(
                                                  jsonArrObj.value( FIELD_DOOR_NAME ).toString(),
                                                  jsonArrObj.value( FIELD_COME_IN_COUNTER ).toInt(),
                                                  jsonArrObj.value( FIELD_COME_OUT_COUNTER ).toInt() ) );
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_WARN( "Топик[" + topicName()
                                                + "]: внутренний JSON объект содержит неизвестный список полей: "
                                                + jsonArrObj.keys().join( "," ) );
                }
            }

            /*! current_data */
            if(jsonMainObj.contains(FIELD_CURRENT_DATA))
            {
                QJsonObject jsonObj = jsonMainObj.value( FIELD_CURRENT_DATA ).toObject();
                if ( jsonObj.contains( FIELD_STATION_NUM ) &&
                     jsonObj.contains( FIELD_STATION_NAME ) &&
                     jsonObj.contains( FIELD_STATION_STOPID ) &&
                     jsonObj.contains( FIELD_PASSANGERS ) &&
                     jsonObj.contains( FIELD_TRANSACTION_COUNTER ) )
                {
                    m_currentData.m_stationNum = jsonObj.value( FIELD_STATION_NUM ).toInt();
                    m_currentData.m_stationName = jsonObj.value( FIELD_STATION_NAME ).toString();
                    m_currentData.m_stationStopID = jsonObj.value( FIELD_STATION_STOPID ).toInt();
                    m_currentData.m_transactionCounter = jsonObj.value( FIELD_TRANSACTION_COUNTER ).toInt();
                    for ( const auto i : jsonObj.value( FIELD_PASSANGERS ).toArray() )
                    {
                        QJsonObject jsonArrObj = i.toObject();
                        if ( jsonArrObj.contains( FIELD_COME_IN_COUNTER ) &&
                             jsonArrObj.contains( FIELD_COME_OUT_COUNTER ) &&
                             jsonArrObj.contains( FIELD_DOOR_NAME ) )
                        {
                            m_currentData.m_counters.append( SPassangerCounterInfo(
                                    jsonArrObj.value( FIELD_DOOR_NAME ).toString(),
                                    jsonArrObj.value( FIELD_COME_IN_COUNTER ).toInt(),
                                    jsonArrObj.value( FIELD_COME_OUT_COUNTER ).toInt() ) );
                        }
                        else
                        {
                            PRINT_CONSOLE_MESSAGE_WARN( "Топик[" + topicName()
                                                        + "]: внутренний JSON объект содержит неизвестный список полей: "
                                                        + jsonArrObj.keys().join( "," ) );
                        }
                    }
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_WARN( "Топик[" + topicName()
                                                + "]: внутренний JSON объект содержит неизвестный список полей: "
                                                + jsonObj.keys().join( "," ) );
                }
            }

            /*! previous_data */
            if ( jsonMainObj.contains( FIELD_PREVIOUS_DATA ) )
            {
                QJsonObject jsonObj = jsonMainObj.value( FIELD_PREVIOUS_DATA ).toObject();
                if ( jsonObj.contains( FIELD_STATION_NUM ) &&
                     jsonObj.contains( FIELD_STATION_NAME ) &&
                     jsonObj.contains( FIELD_STATION_STOPID ) &&
                     jsonObj.contains( FIELD_PASSANGERS ) &&
                     jsonObj.contains( FIELD_TRANSACTION_COUNTER ) )
                {
                    m_previousData.m_stationNum = jsonObj.value( FIELD_STATION_NUM ).toInt();
                    m_previousData.m_stationName = jsonObj.value( FIELD_STATION_NAME ).toString();
                    m_previousData.m_stationStopID = jsonObj.value( FIELD_STATION_STOPID ).toInt();
                    m_previousData.m_transactionCounter = jsonObj.value( FIELD_TRANSACTION_COUNTER ).toInt();
                    for ( const auto i : jsonObj.value( FIELD_PASSANGERS ).toArray() )
                    {
                        QJsonObject jsonArrObj = i.toObject();
                        if ( jsonArrObj.contains( FIELD_COME_IN_COUNTER ) &&
                             jsonArrObj.contains( FIELD_COME_OUT_COUNTER ) &&
                             jsonArrObj.contains( FIELD_DOOR_NAME ) )
                        {
                            m_previousData.m_counters.append( SPassangerCounterInfo(
                                                                 jsonArrObj.value( FIELD_DOOR_NAME ).toString(),
                                                                 jsonArrObj.value( FIELD_COME_IN_COUNTER ).toInt(),
                                                                 jsonArrObj.value( FIELD_COME_OUT_COUNTER ).toInt() ) );
                        }
                        else
                        {
                            PRINT_CONSOLE_MESSAGE_WARN( "Топик[" + topicName()
                                                        + "]: внутренний JSON объект содержит неизвестный список полей: "
                                                        + jsonArrObj.keys().join( "," ) );
                        }
                    }
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_WARN( "Топик[" + topicName()
                                                + "]: внутренний JSON объект содержит неизвестный список полей: "
                                                + jsonObj.keys().join( "," ) );
                }
            }

            m_absoluteTransactionCounter = jsonMainObj.value(FIELD_ABSOLUT_TRANSACTION_COUNTER).toInt();
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

