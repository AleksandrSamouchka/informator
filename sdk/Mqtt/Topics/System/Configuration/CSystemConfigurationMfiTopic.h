#ifndef SYSTEM_CONFIGURATION_MFI_TOPIC_H
#define SYSTEM_CONFIGURATION_MFI_TOPIC_H

#include <Mqtt/Topics/System/CSystemConfigurationTopic.h>
#include <mnt_defs/mfi.h>

/** ***********************************************************************
 * @class CSystemConfigurationMfiTopic
 ** ***********************************************************************/
class CSystemConfigurationMfiTopic : public CSystemConfigurationTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    CSystemConfigurationMfiTopic();
    CSystemConfigurationMfiTopic( const QByteArray &data );

    virtual ~CSystemConfigurationMfiTopic() override;
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

    void setInputSetting( const int number, const EMFIType type, const EMFIActivePolarity polarity );

    inline const QMap < int, TMFISettings > & getInputSettings() const
    {
        return m_mfiList;
    }

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QMap < int, TMFISettings > m_mfiList{};
};

#endif
