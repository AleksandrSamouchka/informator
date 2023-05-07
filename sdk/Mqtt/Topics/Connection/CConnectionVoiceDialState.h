#ifndef CCONNECTIONVOICEDIALSTATE_H
#define CCONNECTIONVOICEDIALSTATE_H

#include <Mqtt/Topics/CGrConnectionTopic.h>

/** ***********************************************************************
 * @class CConnectionVoiceDialState
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CConnectionVoiceDialState : public CGrConnectionTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    enum class EDialState
    {
        DialState_dialingNumber = 0,    // TODO: change to DialState_dialing (catch "dialing")
        DialState_ring,                 // TODO: change to DialState_alerting (catch "alerting")
        DialState_talking,              // TODO: change to DialState_active (catch "active")
        DialState_disconnected,
        DialState_held,
        DialState_incoming,
        DialState_waiting,

        DialState_unknown
    };

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CConnectionVoiceDialState();
    explicit CConnectionVoiceDialState( const EDialState dialState, const QString & number, const QString & name );
    explicit CConnectionVoiceDialState( const QByteArray & data );

    inline EDialState dialState() const
    {
        return m_dialState;
    }
    inline const QString & number() const
    {
        return m_number;
    }
    inline const QString & name() const
    {
        return m_name;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setDialState( const EDialState value );
    void setNumber( const QString & value );
    void setName( const QString & value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    EDialState m_dialState{ EDialState::DialState_unknown };
    QString m_number{};
    QString m_name{};
};

#endif // CCONNECTIONVOICEDIALSTATE_H
