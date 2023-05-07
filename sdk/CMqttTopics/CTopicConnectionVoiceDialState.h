#ifndef CTOPICCONNECTIONVOICEDIALSTATE_H
#define CTOPICCONNECTIONVOICEDIALSTATE_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicConnectionVoiceDialState
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicConnectionVoiceDialState : public CTopicValueEntry
{
public:
    enum class EDialState
    {
        DialState_dialingNumber = 0, // TODO: change to DialState_dialing (catch "dialing")
        DialState_ring, // TODO: change to DialState_alerting (catch "alerting")
        DialState_talking, // TODO: change to DialState_active (catch "active")
        DialState_disconnected,
        DialState_held,
        DialState_incoming,
        DialState_waiting,

        DialState_unknown,
    };

public:
    EDialState m_dialState{ EDialState::DialState_unknown };
    QString m_number{};
    QString m_name{};

public:
    CTopicConnectionVoiceDialState( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicConnectionVoiceDialState() override = default;
};

#endif // CTOPICCONNECTIONVOICEDIALSTATE_H
