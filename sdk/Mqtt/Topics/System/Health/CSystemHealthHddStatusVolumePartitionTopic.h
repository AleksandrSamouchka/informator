#ifndef CSYSTEMHEALTHHDDSTATUSVOLUMEPARTITIONTOPIC_H
#define CSYSTEMHEALTHHDDSTATUSVOLUMEPARTITIONTOPIC_H

#include <Mqtt/Topics/System/Health/CGrSystemHealthHddStatusTopic.h>

/** ***********************************************************************
 * @class CSystemHealthHddStatusVolumeTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CSystemHealthHddStatusVolumePartitionTopic : public CGrSystemHealthHddStatusTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    CSystemHealthHddStatusVolumePartitionTopic( const QString & volume,
                                                const QString & partition );
    virtual ~CSystemHealthHddStatusVolumePartitionTopic() override;

    inline const QString & getPartition() const
    {
        return m_partition;
    }
    inline int getIsMounted() const
    {
        return m_isMounted;
    }
    inline const QString & getMountPoint() const
    {
        return m_mountPoint;
    }
    inline const QString & getTotalVolume() const
    {
        return m_totalVolume;
    }
    inline const QString & getFreeVolume() const
    {
        return m_freeVolume;
    }

    void setPartition( const QString & value );
    void setIsMounted( const int value );
    void setMountPoint( const QString & value );
    void setTotalVolume( const QString & value );
    void setFreeVolume( const QString & value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QString m_partition{};
    int m_isMounted{ 0 };
    QString m_mountPoint{};
    QString m_totalVolume{};
    QString m_freeVolume{};
};

#endif // CSYSTEMHEALTHHDDSTATUSVOLUMEPARTITIONTOPIC_H
