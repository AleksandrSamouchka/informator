#include "CTopicStmRoMfi.h"

namespace
{
    const char *FIELD_MFI = "MFI";

    const char *FIELD_TYPE = "Type";
    const char *FIELD_VALUE = "Value";

    const QStringList FIELDS{
        FIELD_TYPE,
        FIELD_VALUE
    };

    const char *TYPE_ANALOG = "analog";
    const char *TYPE_DIGITAL = "digital";
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicStmRoMfi::CTopicStmRoMfi( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_StmRo_mfi )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicStmRoMfi::initFromData()
{
    m_sensors.clear();
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_MFI ) )
        {
            for ( const auto & i : jsonMainObj.value( FIELD_MFI ).toArray() )
            {
                if ( i.toObject().keys() == FIELDS )
                {
                    CSensorState sensor{};
                    sensor.m_value = i.toObject().value( FIELD_VALUE ).toDouble();
                    const QString type{ i.toObject().value( FIELD_TYPE ).toString() };
                    if ( type == TYPE_ANALOG )
                    {
                        sensor.m_type = CSensorState::ETypes::Type_analog;
                    }
                    else if ( type == TYPE_DIGITAL )
                    {
                        sensor.m_type = CSensorState::ETypes::Type_digital;
                    }
                    else
                    {
                        PRINT_CONSOLE_MESSAGE_WARN( "Неизвестный тип датчика [" + type + "]" );
                    }
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
