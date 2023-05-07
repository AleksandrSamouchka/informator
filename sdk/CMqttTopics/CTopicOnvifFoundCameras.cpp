#include "CTopicOnvifFoundCameras.h"

namespace
{
    const char *FIELD_FOUNDCAMERAS = "found_cameras";
    const char *FIELD_IP = "ip";
    const char *FIELD_MAC = "mac";
    const char *FIELD_FIRSTSTREAM = "first_stream";
    const char *FIELD_SECONDSTREAM = "second_stream";
    const char *FIELD_STATUS = "status";

    const QStringList STATUSES{ "UNKNOWN",
                                "UNKNOWN_DEVICE",
                                "ERROR",
                                "INVALID_AUTH",
                                "DISCONNECTED",
                                "CONNECTED" };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicOnvifFoundCameras::CTopicOnvifFoundCameras( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Onvif_foundCameras )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicOnvifFoundCameras::initFromData(const CTopicValueEntry &value)
{
    if ( value.isValid() )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
        return initFromData(jsonMainObj);
//        setValidState( res );
    }
    return false;
}

bool CTopicOnvifFoundCameras::initFromData( const QJsonObject &jsonMainObj )
{
    bool res{ false };
    m_cameras.clear();
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_FOUNDCAMERAS ) )
        {
            for ( const auto val : jsonMainObj.value( FIELD_FOUNDCAMERAS ).toArray() )
            {
                const QJsonObject obj( val.toObject() );
                if ( obj.contains( FIELD_IP ) &&
                     obj.contains( FIELD_MAC ) &&
                     obj.contains( FIELD_FIRSTSTREAM ) &&
                     obj.contains( FIELD_SECONDSTREAM ) &&
                     obj.contains( FIELD_STATUS ) )
                {
                    COnvifCameraParams params{};
                    params.m_ip = obj.value( FIELD_IP ).toString();
                    params.m_mac = obj.value( FIELD_MAC ).toString();
                    params.m_firstStream = obj.value( FIELD_FIRSTSTREAM ).toString();
                    params.m_secondStream = obj.value( FIELD_SECONDSTREAM ).toString();
                    const int index{ STATUSES.indexOf( obj.value( FIELD_STATUS ).toString() ) };
                    if ( index == -1 )
                    {
                        params.m_status = COnvifCameraParams::EStatus::unknown;
                    }
                    else
                    {
                        params.m_status = static_cast< COnvifCameraParams::EStatus >( index );
                    }

                    m_cameras.append( params );
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

/** ***********************************************************************
* @fn prepareData
* @brief метод формирования значения топика из полей структуры
** ***********************************************************************/
QByteArray CTopicOnvifFoundCameras::prepareData() const
{
    QJsonObject jsonObj{};
    QJsonArray jsonArr{};
    for ( const auto & i : m_cameras )
    {
        QJsonObject obj{};
        obj.insert( FIELD_IP, i.m_ip );
        obj.insert( FIELD_MAC, i.m_mac );
        obj.insert( FIELD_FIRSTSTREAM, i.m_firstStream );
        obj.insert( FIELD_SECONDSTREAM, i.m_secondStream );
        obj.insert( FIELD_STATUS, STATUSES.value( static_cast< int >( i.m_status ), "UNKNOWN" ) );
        jsonArr.append( obj );
    }
    jsonObj.insert( FIELD_FOUNDCAMERAS, jsonArr );
    const QByteArray jsonData{ QJsonDocument( jsonObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
