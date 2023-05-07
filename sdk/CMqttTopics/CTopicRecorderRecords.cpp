#include "CTopicRecorderRecords.h"

namespace
{
    const char *FIELD_EVENTTYPE = "Event_type";
    const char *FIELD_CAMERANUMBER = "Camera_number";
    const char *FIELD_PATH = "Path";
    const char *FIELD_BEGIN = "Begin";
    const char *FIELD_END = "End";
    const char *FIELD_RECORD_DB_ID = "Record_db_id";

    const QStringList FIELDS
    {
        FIELD_BEGIN,
        FIELD_CAMERANUMBER,
        FIELD_END,
        FIELD_EVENTTYPE,
        FIELD_PATH,
        FIELD_RECORD_DB_ID
    };

    const QStringList EVENT_TYPES{
        "ADD",
        "DELETE"
    };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicRecorderRecords::CTopicRecorderRecords( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Recorder_cameras )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicRecorderRecords::initFromData(const CTopicValueEntry &value)
{
    bool res{ false };
    if ( value.isValid() )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
        if ( ! jsonMainObj.isEmpty() )
        {
            if ( jsonMainObj.keys() == FIELDS )
            {
                int index{ EVENT_TYPES.indexOf( jsonMainObj.value( FIELD_EVENTTYPE ).toString() ) };
                if ( index >= EventType_add && index <= EventType_delete  )
                {
                    m_eventType = static_cast< ERecordEventType >( index );
                    m_cameraNumber = jsonMainObj.value( FIELD_CAMERANUMBER ).toInt();
                    m_path = jsonMainObj.value( FIELD_PATH ).toString();
                    m_beginTs = jsonMainObj.value( FIELD_BEGIN ).toVariant().toLongLong();
                    m_endTs = jsonMainObj.value( FIELD_END ).toVariant().toLongLong();
                    m_dbId = jsonMainObj.value( FIELD_RECORD_DB_ID ).toVariant().toLongLong();
                    res = true;
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_WARN( "Ошибка определения типа события [" +
                                                jsonMainObj.value( FIELD_EVENTTYPE ).toString() + "]" );
                }
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


