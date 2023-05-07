#include "CTopicRecorderUploadCmd.h"

namespace
{
    const char *FIELD_TASKID = "Task_id";
    const char *FIELD_CAMERANUMBER = "Camera_number";
    const char *FIELD_STARTTIME = "Start_time";
    const char *FIELD_STOPTIME = "Stop_time";
    const char *FIELD_FORMAT = "Format";
    const char *FIELD_DESTINATION = "Destination";
}

/** ***********************************************************************
* @brief Конструктор
** ***********************************************************************/
CTopicRecorderUploadCmd::CTopicRecorderUploadCmd() :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Recorder_upload )
{ }

/** ***********************************************************************
* @fn prepareData
* @brief метод формирования значения топика из полей структуры
* @param none
* @return QByteArray
** ***********************************************************************/
QByteArray CTopicRecorderUploadCmd::prepareData()
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_TASKID, m_taskId );
    jsonMainObj.insert( FIELD_CAMERANUMBER, m_cameraNumber );
    jsonMainObj.insert( FIELD_STARTTIME, QJsonValue::fromVariant( QVariant( m_startTime ) ) );
    jsonMainObj.insert( FIELD_STOPTIME, QJsonValue::fromVariant( QVariant( m_stopTime ) ) );
    jsonMainObj.insert( FIELD_FORMAT, m_format );
    jsonMainObj.insert( FIELD_DESTINATION, m_destination );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
