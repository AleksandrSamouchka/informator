#include "COnvifFoundCamerasTopic.h"

static const char * TOPIC = "FOUND_CAMERAS";

const QStringList COnvifFoundCamerasTopic::CCameraParams::STATUS_STRINGS =
        QStringList{
            "НЕИЗВЕСТНО",
            "НЕИЗВЕСТНОЕ УСТРОЙСТВО",
            "ОШИБКИ",
            "ОШИБКА АВТОРИЗАЦИИ",
            "ОТКЛЮЧЕНО",
            "В РАБОТЕ",
        };

static const char *FIELD_FOUNDCAMERAS = "found_cameras";
static const char *FIELD_IP = "ip";
static const char *FIELD_MAC = "mac";
static const char *FIELD_FIRSTSTREAM = "first_stream";
static const char *FIELD_SECONDSTREAM = "second_stream";
static const char *FIELD_STATUS = "status";

static const QStringList STATUSES{ "UNKNOWN",
                                   "UNKNOWN_DEVICE",
                                   "ERROR",
                                   "INVALID_AUTH",
                                   "DISCONNECTED",
                                   "CONNECTED" };

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
COnvifFoundCamerasTopic::COnvifFoundCamerasTopic() :
    CGrOnvifTopic( TOPIC )
{ }

COnvifFoundCamerasTopic::~COnvifFoundCamerasTopic()
{ }

void COnvifFoundCamerasTopic::clearCameraList()
{
    m_cameras.clear();
}

void COnvifFoundCamerasTopic::appendCameraParams(const COnvifFoundCamerasTopic::CCameraParams & value)
{
    m_cameras.append( value );
}

void COnvifFoundCamerasTopic::setCameraList(const QList<COnvifFoundCamerasTopic::CCameraParams> & value)
{
    m_cameras = value;
}

/** ***********************************************************************
 * @brief prepareData
 ** ***********************************************************************/
const QByteArray COnvifFoundCamerasTopic::prepareData() const
{
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
    QJsonObject jsonObj{ { FIELD_FOUNDCAMERAS, jsonArr } };
    return QJsonDocument( jsonObj ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @brief parseData
 ** ***********************************************************************/
bool COnvifFoundCamerasTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    m_cameras.clear();
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
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
                    CCameraParams params{};
                    params.m_ip = obj.value( FIELD_IP ).toString();
                    params.m_mac = obj.value( FIELD_MAC ).toString();
                    params.m_firstStream = obj.value( FIELD_FIRSTSTREAM ).toString();
                    params.m_secondStream = obj.value( FIELD_SECONDSTREAM ).toString();
                    const int index{ STATUSES.indexOf( obj.value( FIELD_STATUS ).toString() ) };
                    if ( index == -1 )
                    {
                        params.m_status = CCameraParams::EStatus::unknown;
                    }
                    else
                    {
                        params.m_status = static_cast< CCameraParams::EStatus >( index );
                    }

                    m_cameras.append( params );
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_WARN( "Внутренний JSON объект содержит неизвестный список полей: "
                                                + obj.keys().join( "," ) );
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
    return setValidState( res );
}
