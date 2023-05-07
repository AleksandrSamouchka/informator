#include "CTopicValidatorsPassangerCounter.h"

static const char *FIELD_TOTAL = "total_counter";
static const char *FIELD_CURRENT = "current_in_bus";
static const char *FIELD_NOTPAYED = "not_payed_counter";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicValidatorsPassangerCounter::CTopicValidatorsPassangerCounter( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Validators_passangerCounter )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicValidatorsPassangerCounter::initFromData(const CTopicValueEntry &value)
{
    bool res{ false };
    if ( value.isValid() )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
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
//                PRINT_CONSOLE_MESSAGE_WARN( "JSON объект содержит неизвестный список полей: "
//                                            + jsonMainObj.keys().join( "," ) );
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

/** ***********************************************************************
* @fn prepareData
* @brief метод формирования значения топика из полей структуры
* @param none
* @return QByteArray
** ***********************************************************************/
QByteArray CTopicValidatorsPassangerCounter::prepareData() const
{
    QJsonObject jsonMainObj( { { FIELD_TOTAL, m_total },
                               { FIELD_CURRENT, m_currentInBus },
                               { FIELD_NOTPAYED, m_notPayed } } );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
