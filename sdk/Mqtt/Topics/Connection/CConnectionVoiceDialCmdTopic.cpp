#include "CConnectionVoiceDialCmdTopic.h"

static const char * TOPIC = "VOICE_DIAL_CMD";

static const char *FIELD_CMDTYPE = "Cmd_type";
static const char *FIELD_PHONE = "Phone";
static const char *FIELD_TEXT = "Text";

static const QStringList CMD_LIST{
    "Start_call",
    "Hung_up",
    "Send_SMS"
};

CConnectionVoiceDialCmdTopic::CConnectionVoiceDialCmdTopic() :
    CGrConnectionTopic( TOPIC )
{ }

CConnectionVoiceDialCmdTopic::CConnectionVoiceDialCmdTopic( const CConnectionVoiceDialCmdTopic::ECmdType cmdType,
                                                            const QString & phone,
                                                            const QString & text ) :
    CGrConnectionTopic( TOPIC ),
    m_cmdType{ cmdType },
    m_phone{ phone },
    m_text{ text }
{
    prepareData();
}

CConnectionVoiceDialCmdTopic::CConnectionVoiceDialCmdTopic( const QByteArray & data ) :
    CGrConnectionTopic( TOPIC, data )
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CConnectionVoiceDialCmdTopic::getEtalonJson()
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_CMDTYPE, CMD_LIST.value( static_cast< int >( ECmdType::Cmd_sendSms), "UNKNOWN" ) );
    jsonMainObj.insert( FIELD_PHONE, "8888" );
    jsonMainObj.insert( FIELD_TEXT, "testText" );

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

void CConnectionVoiceDialCmdTopic::setCmdType(const CConnectionVoiceDialCmdTopic::ECmdType value)
{
    m_cmdType = value;
}

void CConnectionVoiceDialCmdTopic::setPhone(const QString & value)
{
    m_phone = value;
}

void CConnectionVoiceDialCmdTopic::setText(const QString & value)
{
    m_text = value;
}

const QByteArray CConnectionVoiceDialCmdTopic::prepareData() const 
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_CMDTYPE, CMD_LIST.value( static_cast< int >( m_cmdType ), "UNKNOWN" ) );
    if ( m_cmdType == ECmdType::Cmd_startCall )
    {
        jsonMainObj.insert( FIELD_PHONE, m_phone );
    }
    else if ( m_cmdType == ECmdType::Cmd_sendSms )
    {
        jsonMainObj.insert( FIELD_PHONE, m_phone );
        jsonMainObj.insert( FIELD_TEXT, m_text );
    }
//    const QByteArray jsonData{ 
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
//};
}

bool CConnectionVoiceDialCmdTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_CMDTYPE ) &&
             jsonMainObj.contains( FIELD_PHONE ) &&
             jsonMainObj.contains( FIELD_TEXT ) )
        {
            QString indexStr( jsonMainObj.value( FIELD_CMDTYPE ).toString() );
            int index{ CMD_LIST.indexOf( indexStr ) };
            if ( index >= static_cast< int >( ECmdType::Cmd_startCall ) &&
                 index <= static_cast< int >( ECmdType::Cmd_sendSms ) )
            {
                m_cmdType = static_cast< ECmdType >( index );
            }
            m_phone = jsonMainObj.value( FIELD_PHONE ).toString();
            m_text = jsonMainObj.value( FIELD_TEXT ).toString();
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
