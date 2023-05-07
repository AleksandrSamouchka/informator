#include "CTopicLiazKneeling.h"

namespace
{
    const char *FIELD_VALUE = "Value";

    const QStringList STATES{
        "Not_active",
        "Lowering_active",
        "Kneeling_level_reached",
        "Lifting_active",
        "Kneeling_aborted",
        "Error",
        "Unknown"
    };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicLiazKneeling::CTopicLiazKneeling(const QByteArray &json) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Liaz_kneeling )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicLiazKneeling::initFromData( const CTopicValueEntry &value )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_VALUE ) )
        {
            QString indexStr( jsonMainObj.value( FIELD_VALUE ).toString() );
            int index{ STATES.indexOf( indexStr ) };
            if ( index >= static_cast< int >( EKneelingStates::Kneeling_notActive ) &&
                 index <= static_cast< int >( EKneelingStates::Kneeling_error ) )
            {
                m_state = static_cast< EKneelingStates >( index );
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
