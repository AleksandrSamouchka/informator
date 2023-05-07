#ifndef CGRLEDTOPIC_H
#define CGRLEDTOPIC_H

#include <Mqtt/CMqttTopic.h>

class CGrLedTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrLedTopic() : CMqttTopic() {}
#endif

public:
    enum ETargets
    {
        frontPanel = 0,
        sidePanel,
        salonPanel,
        rearPanel,
        stopPanel,

        targetsCount,

        unknownPanel = 255,
    };

    static QString getTargetName( ETargets value );
    static ETargets getTargetCode( const QString & value );

    inline ETargets getTarget() const
    {
        return m_target;
    }

    inline void setTarget( CGrLedTopic::ETargets value )
    {
        m_target = value;
    }


public:
    CGrLedTopic( const QString &topicName );
    CGrLedTopic( const QString &topicName, const QByteArray &data );

    virtual ~CGrLedTopic();

private:
    ETargets m_target{};    

};

#endif // CGRLEDTOPIC_H
