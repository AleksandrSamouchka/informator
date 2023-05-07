#include <CConsoleLogger/CConsoleLogger.h>
#include "CTopicInformatorSelectedMode.h"

namespace
{
    const char *FIELD_MODE = "Mode";

    const QStringList MODES_LIST{
        "Auto",
        "Manual"
    };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicInformatorSelectedMode::CTopicInformatorSelectedMode( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Informator_selectedMode )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicInformatorSelectedMode::initFromData()
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_MODE ) )
        {
            const int index{ MODES_LIST.indexOf( jsonMainObj.value( FIELD_MODE ).toString() ) };
            if ( index >= static_cast< int >( EModes::Mode_auto ) &&
                 index <= static_cast< int >( EModes::Mode_manual ) )
            {
                m_mode = static_cast< EModes >( index );
                res = true;
            }
            else
            {
                PRINT_CONSOLE_MESSAGE_WARN( "Некорректное значение поля [Mode]: " + jsonMainObj.value( FIELD_MODE ).toString() );
            }
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
