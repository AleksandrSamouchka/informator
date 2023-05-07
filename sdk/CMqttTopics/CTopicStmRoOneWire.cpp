#include "CTopicStmRoOneWire.h"

namespace
{
    const char *FIELD_MAIN = "OneWire";

    const char *FIELD_ID = "Id";
    const char *FIELD_TEMP = "Temperature";
    const char *FIELD_NAME = "Name";

    const QStringList FIELDS{
        FIELD_ID,
        FIELD_NAME,
        FIELD_TEMP
    };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicStmRoOneWire::CTopicStmRoOneWire( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_StmRo_oneWire )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicStmRoOneWire::initFromData( const CTopicValueEntry &value )
{
    m_sensors.clear();
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_MAIN ) )
        {
            for ( const auto & i : jsonMainObj.value( FIELD_MAIN ).toArray() )
            {
                if ( i.toObject().keys() == FIELDS )
                {
                    CTermoState sensor( i.toObject().value( FIELD_NAME ).toString(),
                                        i.toObject().value( FIELD_ID ).toString(),
                                        i.toObject().value( FIELD_TEMP ).toDouble() );
                    m_sensors.append( sensor );
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_WARN( "JSON объект списка содержит неизвестный список полей: " + i.toObject().keys().join( "," ) );
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
