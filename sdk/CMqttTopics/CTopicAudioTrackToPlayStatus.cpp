#include "CTopicAudioTrackToPlayStatus.h"

static const char *FIELD_STATUS = "Status";
static const char *FIELD_UUID = "Uuid";

static const QStringList STATUSES{ "Queued", "Playing", "Finished" };

/** ***********************************************************************
* @brief Конструктор
** ***********************************************************************/
CTopicAudioTrackToPlayStatus::CTopicAudioTrackToPlayStatus( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Audio_trackToSalonStatus )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicAudioTrackToPlayStatus::initFromData( const CTopicValueEntry & value )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_UUID ) &&
             jsonMainObj.contains( FIELD_STATUS ) )
        {
            int index{ STATUSES.indexOf( jsonMainObj.value( FIELD_STATUS ).toString() ) };
            if ( index < 0 )
            {
                m_status = EStatuses::finished;
            }
            else
            {
                m_status = static_cast< CTopicAudioTrackToPlayStatus::EStatuses >( index );
            }
            m_uuid = jsonMainObj.value( FIELD_UUID ).toString();
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
* @param none
* @return QByteArray
** ***********************************************************************/
QByteArray CTopicAudioTrackToPlayStatus::prepareData() const
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_STATUS, STATUSES.value( static_cast< int >( m_status ), "Unknown" ) );
    jsonMainObj.insert( FIELD_UUID, m_uuid );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
