#include <CConsoleLogger/CConsoleLogger.h>
#include "CTopicRecorderCameras.h"

namespace
{
    const char *MAIN_FIELD_CAMERAS = "Cameras";
    const char *MAIN_FIELD_IPS = "IPs";

    const QStringList FIELDS
    {
        "Active_state",
        "Camera_name",
        "Camera_number",
        "Camera_title",
        "Primary_stream",
        "Secondary_stream"
    };

    enum class EFieldsIndexes
    {
        Field_activeState = 0,
        Field_cameraName,
        Field_cameraNumber,
        Field_cameraTitle,
        Field_primaryStream,
        Field_secondaryStream
    };

    QString getFieldName( const EFieldsIndexes index )
    {
        return FIELDS.value( static_cast< int >( index ), "" );
    }
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicRecorderCameras::CTopicRecorderCameras( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Recorder_cameras )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicRecorderCameras::initFromData(const CTopicValueEntry &value)
{
    bool res{ false };
    m_cameraIps.clear();
    m_camerasList.clear();
    if ( value.isValid() )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
        if ( ! jsonMainObj.isEmpty() )
        {
            if ( jsonMainObj.contains( MAIN_FIELD_CAMERAS ) )
            {
                for ( const auto val : jsonMainObj.value( MAIN_FIELD_CAMERAS ).toArray() )
                {
                    const QJsonObject obj( val.toObject() );
                    if ( obj.keys() == FIELDS )
                    {
                        const CCameraState state(
                                    CCameraState::getActiveState( obj.value( getFieldName( EFieldsIndexes::Field_activeState ) ).toString() ),
                                    obj.value( getFieldName( EFieldsIndexes::Field_cameraNumber ) ).toInt(),
                                    obj.value( getFieldName( EFieldsIndexes::Field_cameraName ) ).toString(),
                                    obj.value( getFieldName( EFieldsIndexes::Field_primaryStream ) ).toString(),
                                    obj.value( getFieldName( EFieldsIndexes::Field_secondaryStream ) ).toString() );
                        m_camerasList.append( state );
                    }
                    else
                    {
                        PRINT_CONSOLE_MESSAGE_WARN( "Внутренний JSON объект содержит неизвестный список полей: " + obj.keys().join( "," ) );
                    }
                }
                res = true;
            }
            else
            {
//                PRINT_CONSOLE_MESSAGE_WARN( "JSON объект содержит неизвестный список полей: " + jsonMainObj.keys().join( "," ) );
                printUnknownFieldsListMessage(jsonMainObj.keys());
            }
            if ( jsonMainObj.contains( MAIN_FIELD_IPS ) )
            {
                for ( const auto val : jsonMainObj.value( MAIN_FIELD_IPS ).toArray() )
                {
                    m_cameraIps.append( val.toString() );
                }
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


