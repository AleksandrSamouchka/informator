#include "CTopicIntegrationValidators.h"

namespace
{
    const char *FIELD_DATE = "date";
    const char *FIELD_VALIDATORID = "validator_id";
//    const char *FIELD_ROUTEID = "route_id";
//    const char *FIELD_EXITID = "exit_id";
//    const char *FIELD_PARKID = "park_id";
//    const char *FIELD_DRIVERID = "driver_id";
//    const char *FIELD_CARDID = "card_id";
    const char *FIELD_TICKETID = "ticket_id";
    const char *FIELD_APPLICATIONTYPEID = "application_type_id";
    const char *FIELD_TICKETTYPEID = "ticket_type_id";
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicIntegrationValidators::CTopicIntegrationValidators( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Integration_Validators )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicIntegrationValidators::initFromData( const CTopicValueEntry &value )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_DATE ) &&
             jsonMainObj.contains( FIELD_VALIDATORID ) &&
             jsonMainObj.contains( FIELD_TICKETID ) &&
             jsonMainObj.contains( FIELD_APPLICATIONTYPEID ) &&
             jsonMainObj.contains( FIELD_TICKETTYPEID ) )
        {
            m_date = jsonMainObj.value( FIELD_DATE ).toString();
            m_validatorId = jsonMainObj.value( FIELD_VALIDATORID ).toString();
            m_ticketId = jsonMainObj.value( FIELD_TICKETID ).toString();
            m_applicationTypeId = jsonMainObj.value( FIELD_APPLICATIONTYPEID ).toString();
            m_ticketTypeId = jsonMainObj.value( FIELD_TICKETTYPEID ).toString();
            res = true;
        }
        else
        {
//            PRINT_CONSOLE_MESSAGE_WARN( "JSON объект содержит неизвестный список полей: "
//                                        + jsonMainObj.keys().join( "," ) );
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
