#include "CTopicConnectionVoiceDialCmd.h"

namespace
{
    const char *FIELD_CMDTYPE = "Cmd_type";
    const char *FIELD_PHONE = "Phone";
    const char *FIELD_TEXT = "Text";

    const QStringList CMD_LIST{
        "Start_call",
        "Answer_call",
        "Hung_up",
        "Send_SMS"
    };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicConnectionVoiceDialCmd::CTopicConnectionVoiceDialCmd( const ECmdType type, 
                                                            const QString &phone, 
                                                            const QString &text ) :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Connection_voiceDialCmd ),
    m_cmdType{ type },
    m_phone( phone ),
    m_text( text )
{ }

/** ***********************************************************************
 * @fn prepareData
 * @brief метод формирования значения топика из полей структуры
 * @param none
 * @return QByteArray
 ** ***********************************************************************/
QByteArray CTopicConnectionVoiceDialCmd::prepareData() const
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
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
