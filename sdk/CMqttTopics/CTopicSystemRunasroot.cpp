#include "CTopicSystemRunasroot.h"

namespace
{
    const char *FIELD_ID = "id";
    const char *FIELD_ACTION = "action";
    const char *FIELD_STATUS = "status";
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicSystemRunasroot::CTopicSystemRunasroot(const QByteArray &json) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_System_runasroot )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicSystemRunasroot::initFromData( const CTopicValueEntry &value )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_ID ) &&
             jsonMainObj.contains( FIELD_ACTION ) )
        {
            mId = jsonMainObj.value( FIELD_ID ).toString();
            mAction = jsonMainObj.value( FIELD_ACTION ).toString();
            if ( jsonMainObj.contains( FIELD_STATUS ) )
            {
                mStatus = jsonMainObj.value( FIELD_STATUS ).toString();
            }
            res = true;
        }
        else
        {
//            PRINT_CONSOLE_MESSAGE_WARN( "JSON объект содержит неизвестный список полей: " + jsonMainObj.keys().join( "," ) );
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
