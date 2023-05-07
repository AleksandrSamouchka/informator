#include "CConnectionVoiceDialState.h"

static const char * TOPIC = "VOICE_DIAL_STATE";

static const char *FIELD_DIALSTATE = "Dial_state";
static const char *FIELD_NUMBER = "Number";
static const char *FIELD_NAME = "Name";

static const QStringList DIALSTATES{
    "dialing",
    "alerting",
    "active",
    "disconnected",
    "held",
    "incoming",
    "waiting",
};

CConnectionVoiceDialState::CConnectionVoiceDialState() :
    CGrConnectionTopic( TOPIC )
{ }

CConnectionVoiceDialState::CConnectionVoiceDialState( const CConnectionVoiceDialState::EDialState dialState,
                                                      const QString & number,
                                                      const QString & name ) :
    CGrConnectionTopic( TOPIC ),
    m_dialState{ dialState },
    m_number{ number },
    m_name{ name }
{
    prepareData();
}

CConnectionVoiceDialState::CConnectionVoiceDialState(const QByteArray & data) :
    CGrConnectionTopic( TOPIC, data )
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CConnectionVoiceDialState::getEtalonJson()
{
    QJsonObject jsonMainObj( { { FIELD_DIALSTATE, DIALSTATES.value( static_cast< int >( EDialState::DialState_ring ) ) },
                                     { FIELD_NUMBER, "00" },
                                     { FIELD_NAME, "testName" } } );
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

void CConnectionVoiceDialState::setDialState(const CConnectionVoiceDialState::EDialState value)
{
    m_dialState = value;
}

void CConnectionVoiceDialState::setNumber(const QString & value)
{
    m_number = value;
}

void CConnectionVoiceDialState::setName(const QString & value)
{
    m_name = value;
}

const QByteArray CConnectionVoiceDialState::prepareData() const
{
    QJsonObject jsonMainObj( { { FIELD_DIALSTATE, DIALSTATES.value( static_cast< int >( m_dialState ) ) },
                                     { FIELD_NUMBER, m_number },
                                     { FIELD_NAME, m_name } } );
//    QByteArray jsonData{ 
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
//};
}

bool CConnectionVoiceDialState::parseData(const QByteArray & data)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_DIALSTATE ) )
        {
            QString indexStr( jsonMainObj.value( FIELD_DIALSTATE ).toString() );
            int index{ DIALSTATES.indexOf( indexStr ) };
            if ( index >= static_cast< int >( EDialState::DialState_dialingNumber ) &&
                 index <= static_cast< int >( EDialState::DialState_disconnected ) )
            {
                m_dialState = static_cast< EDialState >( index );
            }
            if ( jsonMainObj.contains( FIELD_NAME ) )
            {
                m_name = jsonMainObj.value( FIELD_NAME ).toString();
            }
            if ( jsonMainObj.contains( FIELD_NUMBER ) )
            {
                m_number = jsonMainObj.value( FIELD_NUMBER ).toString();
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
