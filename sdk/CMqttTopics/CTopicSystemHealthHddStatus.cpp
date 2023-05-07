#include "CTopicSystemHealthHddStatus.h"

#include <QtCore/QStringList>

static const char *FIELD_LIST = "List";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicSystemHealthHddStatus::CTopicSystemHealthHddStatus( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_System_HealthHddStatus )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicSystemHealthHddStatus::initFromData(const CTopicValueEntry &value)
{
    bool res{ false };
    m_volumes.clear();
    if ( value.isValid() )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
        if ( ! jsonMainObj.isEmpty() )
        {
            if ( jsonMainObj.contains( FIELD_LIST ) )
            {
                for ( const auto val : jsonMainObj.value( FIELD_LIST ).toArray() )
                {
                    m_volumes.append( val.toString() );
                }
                res = true;
            }
            else
            {
//                PRINT_CONSOLE_MESSAGE_WARN( "JSON объект содержит неизвестный список полей: " + jsonMainObj.keys().join( "," ) );
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
