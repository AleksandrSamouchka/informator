#ifndef CTOPICCONNECTIONVOICEDIALCMD_H
#define CTOPICCONNECTIONVOICEDIALCMD_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicConnectionVoiceDialCmd
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicConnectionVoiceDialCmd : public CTopicValueEntry
{
public:
    enum class ECmdType
    {
        Cmd_startCall = 0,
        Cmd_answerCall,
        Cmd_hungUp,
        Cmd_sendSms,

        Cmd_unknown,
    };

public:
    ECmdType m_cmdType{ ECmdType::Cmd_unknown };
    QString m_phone{};
    QString m_text{};

public:
    CTopicConnectionVoiceDialCmd( const ECmdType type, 
                                    const QString &phone = QString{}, 
                                    const QString &text = QString{} );
    QByteArray prepareData() const;
    virtual ~CTopicConnectionVoiceDialCmd() = default;
};

#endif // CTOPICCONNECTIONVOICEDIALCMD_H
