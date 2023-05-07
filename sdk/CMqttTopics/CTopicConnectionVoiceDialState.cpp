#include "CTopicConnectionVoiceDialState.h"

namespace
{
    const char *FIELD_DIALSTATE = "Dial_state";
    const char *FIELD_NUMBER = "Number";
    const char *FIELD_NAME = "Name";

    const QStringList DIALSTATES{
        "dialing",
        "alerting",
        "active",
        "disconnected",
        "held",
        "incoming",
        "waiting",
    };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicConnectionVoiceDialState::CTopicConnectionVoiceDialState(const QByteArray &json) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Connection_voiceDialState )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicConnectionVoiceDialState::initFromData( const CTopicValueEntry &value )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_DIALSTATE ) )
        {
            QString indexStr( jsonMainObj.value( FIELD_DIALSTATE ).toString() );
            int index{ DIALSTATES.indexOf( indexStr ) };
            if ( index >= static_cast< int >( EDialState::DialState_dialingNumber ) &&
                 index <= static_cast< int >( EDialState::DialState_waiting ) )
            {
                m_dialState = static_cast< EDialState >( index );
            }
            if ( jsonMainObj.contains( FIELD_NAME ) )
            {
                m_name = jsonMainObj.value( FIELD_NAME ).toString();
            }
            if ( jsonMainObj.contains( FIELD_NUMBER ) )
            {
                m_number = jsonMainObj.value( FIELD_NUMBER ).toString();
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

