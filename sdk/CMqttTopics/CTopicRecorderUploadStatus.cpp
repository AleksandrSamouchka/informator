#include "CTopicRecorderUploadStatus.h"

namespace
{
    const char *MAIN_FIELD = "Active_tasks";

    const char *FIELD_TASKID = "Task_id";
    const char *FIELD_PROGRESS = "Progress";
    const char *FIELD_STATUS = "Status";
    const char *FIELD_ERRSTRING = "ErrString";
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicRecorderUploadStatus::CTopicRecorderUploadStatus( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Recorder_uploadStatus )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicRecorderUploadStatus::initFromData(const CTopicValueEntry &value)
{
    bool res{ false };
    if ( value.isValid() )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
        if ( ! jsonMainObj.isEmpty() )
        {
            if ( jsonMainObj.contains( MAIN_FIELD ) )
            {
                for ( const auto val : jsonMainObj.value( MAIN_FIELD ).toArray() )
                {
                    const QJsonObject obj( val.toObject() );
                    if ( obj.contains( FIELD_TASKID ) &&
                         obj.contains( FIELD_PROGRESS ) &&
                         obj.contains( FIELD_STATUS ) )
                    {
                        CRecorderUploadStatus state( obj.value( FIELD_TASKID ).toString(),
                                                     obj.value( FIELD_PROGRESS ).toInt(),
                                                     obj.value( FIELD_STATUS ).toString() );
                        if ( obj.contains( FIELD_ERRSTRING ) )
                        {
                            state.m_errString = obj.value( FIELD_ERRSTRING ).toString();
                        }
                        m_uploadStatusList.append( state );
                    }
                    else
                    {
                        PRINT_CONSOLE_MESSAGE_WARN( "Внутренний JSON объект содержит неизвестный список полей: " +
                                                    obj.keys().join( "," ) );
                    }
                }
                res = true;
            }
            else
            {
//                PRINT_CONSOLE_MESSAGE_WARN( "JSON объект содержит неизвестный список полей: " +
//                                            jsonMainObj.keys().join( "," ) );
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


