#ifndef CINCARNETINSPECTORDEVICESTATETOPIC_H
#define CINCARNETINSPECTORDEVICESTATETOPIC_H

#include <Mqtt/Topics/CGrIncarnetInspectorTopic.h>

/** ***********************************************************************
 * @class CIncarnetInspectorDeviceStateTopic
 ** ***********************************************************************/
class CIncarnetInspectorDeviceStateTopic : public CGrIncarnetInspectorTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    struct SDeviceInfo
    {
        QString m_mac{};
        QStringList m_channels{};
        bool m_isActive{ false };

        SDeviceInfo() = default;
        SDeviceInfo( const QString & mac,
                     const QStringList & channels,
                     const bool isActive ) :
            m_mac{ mac },
            m_channels{ channels },
            m_isActive{ isActive }
        {}
    };

public:
    explicit CIncarnetInspectorDeviceStateTopic();
    explicit CIncarnetInspectorDeviceStateTopic( const QByteArray &data );

    inline const QList < SDeviceInfo > & getDeviceStates() const
    {
        return m_devices;
    }
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void clearDeviceStates();
    void appendDeviceState( const SDeviceInfo & info );
    void setDeviceStates( const QList < SDeviceInfo > & states );

protected:
    virtual const QByteArray prepareData() const override;
    virtual bool parseData( const QByteArray & data ) override;

private:
    QList < SDeviceInfo > m_devices{};
};

#endif // CINCARNETINSPECTORDEVICESTATETOPIC_H
