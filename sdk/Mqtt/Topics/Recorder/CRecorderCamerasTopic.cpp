#include "CRecorderCamerasTopic.h"

static const QStringList ACTIVE_STATE_STR =
        QStringList{ "OFFLINE", "ONLINE", "RECORDING" };

SCameraState::EActiveState SCameraState::getActiveState(const QString & str)
{
    int index{ ACTIVE_STATE_STR.indexOf( str ) };
    if ( index < 0 )
    {
        return EActiveState::offline;
    }
    else
    {
        return static_cast< EActiveState >( index );
    }
}

static const char * TOPIC = "CAMERAS";

static const char * FIELD_CAMERAS = "Cameras";

static const char * FIELD_ACTIVE_STATE = "Active_state";
static const char * FIELD_CAMERA_NUMBER = "Camera_number";


/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CRecorderCamerasTopic::CRecorderCamerasTopic() :
    CGrRecorderTopic( TOPIC )
{ }

CRecorderCamerasTopic::~CRecorderCamerasTopic()
{ }

void CRecorderCamerasTopic::setCamerasList(const QList<SCameraState> & value)
{
    m_camerasList = value;
}

/** ***********************************************************************
 * @fn prepareData
 ** ***********************************************************************/
const QByteArray CRecorderCamerasTopic::prepareData() const
{
    QJsonArray arr{};
    for ( const auto & i : m_camerasList )
    {
        arr.append( QJsonObject{ { FIELD_ACTIVE_STATE, ACTIVE_STATE_STR.value( static_cast< int >( i.m_activeState ) ) },
                                 { FIELD_CAMERA_NUMBER, i.m_cameraNumber } } );
    }
    QJsonObject jsonMainObj{ { FIELD_CAMERAS, arr } };
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @fn parseData
 ** ***********************************************************************/
bool CRecorderCamerasTopic::parseData( const QByteArray & data )
{
    bool res{ false };
    m_camerasList.clear();
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_CAMERAS ) )
        {
            for ( const auto val : jsonMainObj.value( FIELD_CAMERAS ).toArray() )
            {
                const QJsonObject obj( val.toObject() );
                if ( obj.contains( FIELD_ACTIVE_STATE )
                     && obj.contains( FIELD_CAMERA_NUMBER ) )
                {
                    SCameraState camState{};
                    camState.m_activeState = SCameraState::getActiveState( obj.value( FIELD_ACTIVE_STATE ).toString() );
                    camState.m_cameraNumber = obj.value( FIELD_CAMERA_NUMBER ).toInt();
                    m_camerasList.append( camState );
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
    return setValidState( res );
}
