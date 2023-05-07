#include "CIntegrationPassangerCounterByStationTopic.h"

static const char * TOPIC = "PASSANGER_COUNTER_BY_STATION";

static const char *FIELD_PASSANGERS = "passangers";
static const char *FIELD_DOOR_NAME = "door_name";
static const char *FIELD_ROUTE_NAME = "route_name";
static const char *FIELD_STATION_NAME = "station_name";
static const char *FIELD_STATION_NUM = "station_num";
static const char *FIELD_COME_IN_COUNTER = "come_in_counter";
static const char *FIELD_COME_OUT_COUNTER = "come_out_counter";
static const char *FIELD_TRANSACTION_COUNTER = "transaction_counter";
static const char *FIELD_TRANSACTION_COUNTER_PREV_STAGE = "transaction_counter_prev_stage";
static const char *FIELD_STATE = "state";


/** ***********************************************************************
 * @fn Конструкторы
 ** ***********************************************************************/
CIntegrationPassangerCounterByStationTopic::CIntegrationPassangerCounterByStationTopic() :
    CGrIntegrationTopic( TOPIC )
{ }

CIntegrationPassangerCounterByStationTopic::CIntegrationPassangerCounterByStationTopic(
        const CValidatorsPassangerCounterByStationTopic & other ) :
    CGrIntegrationTopic( TOPIC ),
    m_routeName{ other.getRouteName() },
         m_stationNum{ other.getStationNum() },
         m_stationName{ other.getStationName() },
         m_counters{ other.getPassangerCounterList() },
         m_transactionCounter{ other.getTransactionCounter() },
         m_state{ other.getState() },
         m_transactionCounterPrevStage{ other.getTransactionCounterPrevStage() }
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CIntegrationPassangerCounterByStationTopic::getEtalonJson()
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

/** ***********************************************************************
 * @fn prepareData
 ** ***********************************************************************/
const QByteArray CIntegrationPassangerCounterByStationTopic::prepareData() const
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
                               { FIELD_PASSANGERS, jsonArr },
                               { FIELD_TRANSACTION_COUNTER, m_transactionCounter },
                               { FIELD_STATE,
                                 CValidatorsPassangerCounterByStationTopic::STATES.value( static_cast< int >( m_state ), "UNCKNOWN" ) } } );
    if ( m_state == CValidatorsPassangerCounterByStationTopic::EStates::doorOpened )
    {
        jsonMainObj.insert( FIELD_TRANSACTION_COUNTER_PREV_STAGE, m_transactionCounterPrevStage );
    }
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @fn parseData
 ** ***********************************************************************/
bool CIntegrationPassangerCounterByStationTopic::parseData(const QByteArray & data)
{
    m_counters.clear();
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_ROUTE_NAME ) &&
             jsonMainObj.contains( FIELD_STATION_NAME ) &&
             jsonMainObj.contains( FIELD_STATION_NUM ) &&
             jsonMainObj.contains( FIELD_PASSANGERS ) &&
             jsonMainObj.contains( FIELD_TRANSACTION_COUNTER ) &&
             jsonMainObj.contains( FIELD_STATE ) )
        {
            m_routeName = jsonMainObj.value( FIELD_ROUTE_NAME ).toString();
            m_stationName = jsonMainObj.value( FIELD_STATION_NAME ).toString();
            m_stationNum = jsonMainObj.value( FIELD_STATION_NUM ).toInt();
            for ( const auto i : jsonMainObj.value( FIELD_PASSANGERS ).toArray() )
            {
                QJsonObject jsonObj = i.toObject();
                if ( jsonObj.contains( FIELD_COME_IN_COUNTER ) &&
                     jsonObj.contains( FIELD_COME_OUT_COUNTER ) &&
                     jsonObj.contains( FIELD_DOOR_NAME ) )
                {
                    m_counters.append( CValidatorsPassangerCounterByStationTopic::SPassangerCounterInfo(
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
            int index = CValidatorsPassangerCounterByStationTopic::STATES.indexOf( jsonMainObj.value( FIELD_STATE ).toString() );
            if ( index >= static_cast< int >( CValidatorsPassangerCounterByStationTopic::EStates::doorOpened ) &&
                 index <= static_cast< int >( CValidatorsPassangerCounterByStationTopic::EStates::stageChanged ) )
            {
                m_state = static_cast< CValidatorsPassangerCounterByStationTopic::EStates >( index );
            }
            else
            {
                m_state = CValidatorsPassangerCounterByStationTopic::EStates::uncknownRoute;
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
