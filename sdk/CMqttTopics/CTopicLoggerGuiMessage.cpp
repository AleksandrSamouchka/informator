#include "CTopicLoggerGuiMessage.h"

namespace
{
    const char *FIELD_MODULENAME = "Module_name";
    const char *FIELD_MESSAGENAME = "Message_name";
    const char *FIELD_MESSAGE = "Message";
    const char *FIELD_CRITERIA = "Criteria";

    const QStringList FIELDS{
        FIELD_CRITERIA,
        FIELD_MESSAGE,
        FIELD_MESSAGENAME,
        FIELD_MODULENAME
    };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicLoggerGuiMessage::CTopicLoggerGuiMessage( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Logger_guiMessage )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicLoggerGuiMessage::initFromData()
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.keys() == FIELDS )
        {
//            int index{ CLoggerMessage::LOG_CRITERIA_LIST.indexOf( jsonMainObj.value( FIELD_CRITERIA ).toString() ) };
            int index = CLoggerMessage::getIndexOfCriteria(jsonMainObj.value( FIELD_CRITERIA ).toString()); ////{ CLoggerMessage::LOG_CRITERIA_LIST.indexOf( jsonMainObj.value( FIELD_CRITERIA ).toString() ) };
            if ( index >= static_cast< int >( CLoggerMessage::ELogCriteria::LogCriteria_error ) &&
                 index <= static_cast< int >( CLoggerMessage::ELogCriteria::LogCriteria_info ) )
            {
                m_criteria = static_cast< CLoggerMessage::ELogCriteria >( index );
            }
            else
            {
                PRINT_CONSOLE_MESSAGE_WARN( "Некорректное значение поля [Criteria]: " +
                                            jsonMainObj.value( FIELD_CRITERIA ).toString() );
            }
            m_moduleName = jsonMainObj.value( FIELD_MODULENAME ).toString();
            m_messageName = jsonMainObj.value( FIELD_MESSAGENAME ).toString();
            m_message = jsonMainObj.value( FIELD_MESSAGE ).toString();
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
