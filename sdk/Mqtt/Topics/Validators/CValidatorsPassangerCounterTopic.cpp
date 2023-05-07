#include "CValidatorsPassangerCounterTopic.h"

static const char * TOPIC = "PASSANGER_COUNTER";

static const char *FIELD_TOTAL = "total_counter";
static const char *FIELD_CURRENT = "current_in_bus";
static const char *FIELD_NOTPAYED = "not_payed_counter";

/** ***********************************************************************
 * @fn Конструкторы
 ** ***********************************************************************/
CValidatorsPassangerCounterTopic::CValidatorsPassangerCounterTopic() :
    CGrValidatorsTopic( TOPIC )
{ }
CValidatorsPassangerCounterTopic::CValidatorsPassangerCounterTopic( const int total,
                                                                    const int currentInBus,
                                                                    const int notPayed ) :
    CGrValidatorsTopic( TOPIC ),
    m_total{ total },
    m_currentInBus{ currentInBus },
    m_notPayed{ notPayed }
{ }
CValidatorsPassangerCounterTopic::CValidatorsPassangerCounterTopic( const QByteArray &data ) :
    CGrValidatorsTopic( TOPIC, data )
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CValidatorsPassangerCounterTopic::getEtalonJson()
{
    QJsonObject jsonMainObj( { { FIELD_TOTAL, 1 },
                               { FIELD_CURRENT, 2 },
                               { FIELD_NOTPAYED, 3 } } );
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

/** ***********************************************************************
 * @fn сеттеры
 ** ***********************************************************************/
void CValidatorsPassangerCounterTopic::setTotal(const int value)
{
    m_total = value;
}

void CValidatorsPassangerCounterTopic::setCurrentInBus(const int value)
{
    m_currentInBus = value;
}

void CValidatorsPassangerCounterTopic::setNotPayed(const int value)
{
    m_notPayed = value;
}

/** ***********************************************************************
 * @fn prepareData
 ** ***********************************************************************/
const QByteArray CValidatorsPassangerCounterTopic::prepareData() const
{
    QJsonObject jsonMainObj( { { FIELD_TOTAL, m_total },
                               { FIELD_CURRENT, m_currentInBus },
                               { FIELD_NOTPAYED, m_notPayed } } );
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @fn parseData
 ** ***********************************************************************/
bool CValidatorsPassangerCounterTopic::parseData( const QByteArray & data )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_TOTAL ) &&
             jsonMainObj.contains( FIELD_CURRENT ) &&
             jsonMainObj.contains( FIELD_NOTPAYED ) )
        {
            m_total = jsonMainObj.value( FIELD_TOTAL ).toInt();
            m_currentInBus = jsonMainObj.value( FIELD_CURRENT ).toInt();
            m_notPayed = jsonMainObj.value( FIELD_NOTPAYED ).toInt();
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

