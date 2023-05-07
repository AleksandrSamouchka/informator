#include "CValidatorsPassangerCounterByStationTopic.h"

static const char * TOPIC = "PASSANGER_COUNTER_BY_STATION";

static const char *FIELD_PASSANGERS = "passangers";
static const char *FIELD_DOOR_NAME = "door_name";
static const char *FIELD_ROUTE_NAME = "route_name";
static const char *FIELD_STATION_NAME = "station_name";
static const char *FIELD_STATION_NUM = "station_num";
static const char *FIELD_STATION_STOPID = "station_stopid";
static const char *FIELD_COME_IN_COUNTER = "come_in_counter";
static const char *FIELD_COME_OUT_COUNTER = "come_out_counter";
static const char *FIELD_TRANSACTION_COUNTER = "transaction_counter";
static const char *FIELD_TRANSACTION_COUNTER_PREV_STAGE = "transaction_counter_prev_stage";
static const char *FIELD_STATE = "state";

const QStringList CValidatorsPassangerCounterByStationTopic::STATES = QStringList {
        "DOOR_OPENED",
        "DOOR_CLOSED",
        "IN_MOVEMENT",
        "UNKNOWN_ROUTE",
        "STAGE_CHANGED" };

/** ***********************************************************************
 * @fn Конструкторы
 ** ***********************************************************************/
CValidatorsPassangerCounterByStationTopic::CValidatorsPassangerCounterByStationTopic() :
    CGrValidatorsTopic( TOPIC )
{ }

CValidatorsPassangerCounterByStationTopic::CValidatorsPassangerCounterByStationTopic(const QByteArray & data)  :
    CGrValidatorsTopic( TOPIC, data )
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CValidatorsPassangerCounterByStationTopic::getEtalonJson()
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
                               { FIELD_STATION_NAME, "testStationName" },
                               { FIELD_STATION_NUM, 1 },
                               { FIELD_PASSANGERS, jsonArr },
                               { FIELD_TRANSACTION_COUNTER, 100 },
                               { FIELD_STATE, STATES.value( static_cast< int >( EStates::stageChanged ), "UNCKNOWN" ) } } );
    jsonMainObj.insert( FIELD_TRANSACTION_COUNTER_PREV_STAGE, 300 );

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

void CValidatorsPassangerCounterByStationTopic::setRouteName(const QString & value)
{
    m_routeName = value;
}

void CValidatorsPassangerCounterByStationTopic::setStationNum(const int value)
{
    m_stationNum = value;
}

void CValidatorsPassangerCounterByStationTopic::setStationName(const QString & value)
{
    m_stationName = value;
}

void CValidatorsPassangerCounterByStationTopic::clearPassangerCounterList()
{
    m_counters.clear();
}

void CValidatorsPassangerCounterByStationTopic::setStationStopID(int value)
{
    m_stationStopID = value;
}

void CValidatorsPassangerCounterByStationTopic::appendPassangerCounter(
        const CValidatorsPassangerCounterByStationTopic::SPassangerCounterInfo &value)
{
    m_counters.append( value );
}

void CValidatorsPassangerCounterByStationTopic::setTransactionCounter(const int value)
{
    m_transactionCounter = value;
}

void CValidatorsPassangerCounterByStationTopic::setTransactionCounterPrevStage(const int value)
{
    m_transactionCounterPrevStage = value;
}

void CValidatorsPassangerCounterByStationTopic::setState(const CValidatorsPassangerCounterByStationTopic::EStates value)
{
    m_state = value;
}

/** ***********************************************************************
 * @fn prepareData
 ** ***********************************************************************/
const QByteArray CValidatorsPassangerCounterByStationTopic::prepareData() const
{
    QJsonArray jsonArr{};
    for ( const auto & i : m_counters )
    {
        QJsonObject jsonObj( { { FIELD_COME_IN_COUNTER, i.m_comeInCounter },
                               { FIELD_COME_OUT_COUNTER, i.m_comeOutCounter },
                               { FIELD_DOOR_NAME, i.m_channelName } } );
        jsonArr.append( jsonObj );
    }
    QJsonObject jsonMainObj( { { FIELD_ROUTE_NAME, m_routeName },
                               { FIELD_STATION_NUM, m_stationNum },
                               { FIELD_STATION_NAME, m_stationName },
                               { FIELD_STATION_STOPID, m_stationStopID },
                               { FIELD_PASSANGERS, jsonArr },
                               { FIELD_TRANSACTION_COUNTER, m_transactionCounter },
                               { FIELD_STATE, STATES.value( static_cast< int >( m_state ), "UNCKNOWN" ) } } );
    if ( m_state == EStates::doorOpened )
    {
        jsonMainObj.insert( FIELD_TRANSACTION_COUNTER_PREV_STAGE, m_transactionCounterPrevStage );
    }
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @fn parseData
 ** ***********************************************************************/
bool CValidatorsPassangerCounterByStationTopic::parseData(const QByteArray & data)
{
    m_counters.clear();
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_ROUTE_NAME ) &&
             jsonMainObj.contains( FIELD_STATION_NAME ) &&
             jsonMainObj.contains( FIELD_STATION_NUM ) &&
             jsonMainObj.contains( FIELD_STATION_STOPID ) &&
             jsonMainObj.contains( FIELD_PASSANGERS ) &&
             jsonMainObj.contains( FIELD_TRANSACTION_COUNTER ) &&
             jsonMainObj.contains( FIELD_STATE ) )
        {
            m_routeName = jsonMainObj.value( FIELD_ROUTE_NAME ).toString();
            m_stationName = jsonMainObj.value( FIELD_STATION_NAME ).toString();
            m_stationNum = jsonMainObj.value( FIELD_STATION_NUM ).toInt();
            m_stationStopID = jsonMainObj.value( FIELD_STATION_STOPID ).toInt();
            for ( const auto i : jsonMainObj.value( FIELD_PASSANGERS ).toArray() )
            {
                QJsonObject jsonObj = i.toObject();
                if ( jsonObj.contains( FIELD_COME_IN_COUNTER ) &&
                     jsonObj.contains( FIELD_COME_OUT_COUNTER ) &&
                     jsonObj.contains( FIELD_DOOR_NAME ) )
                {
                    m_counters.append( SPassangerCounterInfo(
                                           jsonObj.value( FIELD_DOOR_NAME ).toString(),
                                           jsonObj.value( FIELD_COME_IN_COUNTER ).toInt(),
                                           jsonObj.value( FIELD_COME_OUT_COUNTER ).toInt() ) );
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_WARN( "Топик[" + topicName()
                                                + "]: внутренний JSON объект содержит неизвестный список полей: "
                                                + jsonObj.keys().join( "," ) );
                }
            }
            m_transactionCounter = jsonMainObj.value( FIELD_TRANSACTION_COUNTER ).toInt();
            int index = STATES.indexOf( jsonMainObj.value( FIELD_STATE ).toString() );
            if ( index >= static_cast< int >( EStates::doorOpened ) &&
                 index <= static_cast< int >( EStates::stageChanged ) )
            {
                m_state = static_cast< EStates >( index );
            }
            else
            {
                m_state = EStates::uncknownRoute;
            }

            if ( jsonMainObj.contains( FIELD_TRANSACTION_COUNTER_PREV_STAGE ) )
            {
                m_transactionCounterPrevStage = jsonMainObj.value( FIELD_TRANSACTION_COUNTER_PREV_STAGE ).toInt();
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

