#ifndef CSYSTEMHEALTHHDDSTATUSVOLUMETOPIC_H
#define CSYSTEMHEALTHHDDSTATUSVOLUMETOPIC_H

#include <Mqtt/Topics/System/Health/CGrSystemHealthHddStatusTopic.h>

/** ***********************************************************************
 * @class CSystemHealthHddStatusVolumeTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CSystemHealthHddStatusVolumeTopic : public CGrSystemHealthHddStatusTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    CSystemHealthHddStatusVolumeTopic( const QString & volume );
    virtual ~CSystemHealthHddStatusVolumeTopic() override;

    inline const QString & getPartitions() const
    {
        return m_partition;
    }
    inline int getSystem() const
    {
        return m_system;
    }
    inline const QString & getManufacturer() const
    {
        return m_manufacturer;
    }
    inline const QString & getModel() const
    {
        return m_model;
    }
    inline const QString & getSn() const
    {
        return m_sn;
    }
    inline const QString &  getPowerOnHours() const
    {
        return m_powerOnHours;
    }


    void setPartitions( const QString & value );
    void setSystem( const int value );
    void setManufacturer( const QString & value );
    void setModel( const QString & value );
    void setSn( const QString & value );
    void setPowerOnHours( const QString & value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QString m_partition{};
    int m_system{ 0 };
    QString m_manufacturer{};
    QString m_model{};
    QString m_sn{};
    QString m_powerOnHours{};
};

#endif // CSYSTEMHEALTHHDDSTATUSVOLUMETOPIC_H
