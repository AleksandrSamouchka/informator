#include <QDebug>
#include <CMqttTopics/CTopicAudioTrackToPlay.h>

    static const char *FIELD_SENDER = "Sender";
    static const char *FIELD_ACTION = "Action";
    static const char *FIELD_TRACK = "Track";
    static const char *FIELD_UUID = "Uuid";

    static const QStringList ACTIONS{ "Start", "Stop" };

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/

CTopicAudioTrackToPlay::CTopicAudioTrackToPlay( ) :
    CTopicValueEntry( QByteArray(), ETopicsNamesIndex::Topic_unknown )
{
}

CTopicAudioTrackToPlay::CTopicAudioTrackToPlay(const ETopicsNamesIndex topicIndex ) :
    CTopicValueEntry( QByteArray(), topicIndex )
{
}

CTopicAudioTrackToPlay::CTopicAudioTrackToPlay( const QByteArray &json/*, const ETopicsNamesIndex topicIndex*/ ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_unknown/*topicIndex*//*ETopicsNamesIndex::Topic_Audio_trackToSalon*/ )
{ 
}

/** ***********************************************************************
* @fn prepareData
** ***********************************************************************/
bool CTopicAudioTrackToPlay::initFromData( const CTopicValueEntry &value )
{
    bool res = false;

    QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    QStringList keys = {
        FIELD_ACTION,
        FIELD_SENDER,
        FIELD_TRACK,
        FIELD_UUID
    };

    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.keys() == keys)
        {
            m_sender = jsonMainObj.value( FIELD_SENDER ).toString();
            QString actionString = jsonMainObj.value( FIELD_ACTION ).toString();
            if(actionString == "Start")
            {
                m_action = CAudioTrackToTopic::EAction::start;
            }
            else if(actionString == "Stop")
            {
                m_action = CAudioTrackToTopic::EAction::stop;
            } 
            else
            {
                PRINT_CONSOLE_MESSAGE_WARN( "Unknown track action: " + 
                                            actionString + ". Expected: " +
                                            ACTIONS.join(","));
            }
//            m_action = static_cast< EAction >(jsonMainObj.value( FIELD_ACTION ).toInt());
            m_track = jsonMainObj.value( FIELD_TRACK ).toString();
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
QByteArray CTopicAudioTrackToPlay::prepareData() const
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_SENDER, m_sender );
    if( m_action == CAudioTrackToTopic::EAction::start)
    {
        jsonMainObj.insert( FIELD_ACTION, "Start");
    }
    else
    {
        jsonMainObj.insert( FIELD_ACTION, "Stop");
    }
//    jsonMainObj.insert( FIELD_ACTION, ACTIONS.value( static_cast< int >( m_action ), "Unknown" ) );
    jsonMainObj.insert( FIELD_TRACK, m_track );
    jsonMainObj.insert( FIELD_UUID, m_uuid );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
