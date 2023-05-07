#ifndef CCONNECTIONCONNECTIONSTATETOPIC_H
#define CCONNECTIONCONNECTIONSTATETOPIC_H

#include <Mqtt/Topics/CGrConnectionTopic.h>

/** ***********************************************************************
 * @class CConnectionConnectionStateTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CConnectionConnectionStateTopic : public CGrConnectionTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    enum class EUsedNetType
    {
        NetType_cellular = 0,
        NetType_wiFi,
        NetType_ethernet,

        NetType_unknown,
    };
    enum class ECellurarNetType
    {
        CellurarType_4G = 0,
        CellurarType_3G,
        CellurarType_2G,
        CellurarType_noCoverage,

        CellurarType_unknown,
    };

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CConnectionConnectionStateTopic();
    explicit CConnectionConnectionStateTopic( const QByteArray & data );

    inline EUsedNetType usedNetType() const
    {
        return m_usedNetType;
    }
    inline ECellurarNetType cellurarNetType() const
    {
        return m_cellurarNetType;
    }
    inline int cellurarQuality() const
    {
        return m_cellurarQuality;
    }
    inline int wiFiQuality() const
    {
        return m_wiFiQuality;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setUsedNetType( const EUsedNetType value );
    void setCellurarNetType( const ECellurarNetType value );
    void setCellurarQuality( const int value );
    void setWiFiQuality( const int value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    EUsedNetType m_usedNetType{ EUsedNetType::NetType_unknown };
    ECellurarNetType m_cellurarNetType{ ECellurarNetType::CellurarType_unknown };
    int m_cellurarQuality{ 0 };
    int m_wiFiQuality{ 0 };
};

#endif // CCONNECTIONCONNECTIONSTATETOPIC_H
