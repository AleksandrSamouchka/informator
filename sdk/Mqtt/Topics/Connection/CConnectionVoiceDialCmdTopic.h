#ifndef CCONNECTIONVOICEDIALCMDTOPIC_H
#define CCONNECTIONVOICEDIALCMDTOPIC_H

#include <Mqtt/Topics/CGrConnectionTopic.h>

/** ***********************************************************************
 * @class CConnectionVoiceDialCmdTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CConnectionVoiceDialCmdTopic : public CGrConnectionTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    enum class ECmdType
    {
        Cmd_startCall = 0,
        Cmd_hungUp,
        Cmd_sendSms,

        Cmd_unknown,
    };

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CConnectionVoiceDialCmdTopic();
    explicit CConnectionVoiceDialCmdTopic( const ECmdType cmdType, const QString & phone, const QString & text );
    explicit CConnectionVoiceDialCmdTopic( const QByteArray & data );

    inline ECmdType cmdType() const
    {
        return m_cmdType;
    }
    inline const QString & phone() const
    {
        return m_phone;
    }
    inline const QString & text() const
    {
        return m_text;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setCmdType( const ECmdType value );
    void setPhone( const QString & value );
    void setText( const QString & value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    ECmdType m_cmdType{ ECmdType::Cmd_unknown };
    QString m_phone{};
    QString m_text{};
};

#endif // CCONNECTIONVOICEDIALCMDTOPIC_H
