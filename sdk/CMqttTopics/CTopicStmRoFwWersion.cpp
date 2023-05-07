#include "CTopicStmRoFwWersion.h"

namespace
{
    const char *FIELD_MAIN = "FW_VERSION";

    const char *FIELD_TYPE = "Type";
    const char *FIELD_VALUE = "Value";

    const char *TYPE_BOOTLOADER = "Bootloader";
    const char *TYPE_APPLICATION = "Application";
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicStmRoFwWersion::CTopicStmRoFwWersion( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_StmRo_fwWersion )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicStmRoFwWersion::initFromData( const CTopicValueEntry &value )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_MAIN ) )
        {
            QJsonArray arr( jsonMainObj.value( FIELD_MAIN ).toArray() );
            for ( const auto i : arr )
            {
                const QJsonObject obj( i.toObject() );
                if ( obj.contains( FIELD_TYPE ) &&
                     obj.contains( FIELD_VALUE ) )
                {
                    const QString type( obj.value( FIELD_TYPE ).toString() );
                    if ( type == TYPE_BOOTLOADER )
                    {
                        mBootloaderVersion = obj.value( FIELD_VALUE ).toString();
                    }
                    else if ( type == TYPE_APPLICATION )
                    {
                        mApplicationVersion = obj.value( FIELD_VALUE ).toString();
                    }
                    else
                    {
                        PRINT_CONSOLE_MESSAGE_WARN( "Неизвестный тип: " + obj.value( FIELD_VALUE ).toString() );
                    }
                }
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
