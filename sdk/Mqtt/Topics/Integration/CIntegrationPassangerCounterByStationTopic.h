#ifndef CINTEGRATIONPASSANGERCOUNTERBYSTATIONTOPIC_H
#define CINTEGRATIONPASSANGERCOUNTERBYSTATIONTOPIC_H

#include <Mqtt/Topics/CGrIntegrationTopic.h>
#include <Mqtt/Topics/Validators/CValidatorsPassangerCounterByStationTopic.h>

/** ***********************************************************************
 * @class CIntegrationPassangerCounterByStationTopic
 * @brief класс, пассажиропоток и валидаторы (по остановкам) - для интеграции
 ** ***********************************************************************/
class CIntegrationPassangerCounterByStationTopic : public CGrIntegrationTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CIntegrationPassangerCounterByStationTopic();
    explicit CIntegrationPassangerCounterByStationTopic( const CValidatorsPassangerCounterByStationTopic & other );

    inline const QString & getRouteName() const
    {
        return m_routeName;
    }
    inline const QString & getStationName() const
    {
        return m_stationName;
    }
    inline int getStationNum() const
    {
        return m_stationNum;
    }
    inline const QList < CValidatorsPassangerCounterByStationTopic::SPassangerCounterInfo >
    & getPassangerCounterList() const
    {
        return m_counters;
    }
    inline int getTransactionCounter() const
    {
        return m_transactionCounter;
    }
    inline int getTransactionCounterPrevStage() const
    {
        return m_transactionCounterPrevStage;
    }
    inline CValidatorsPassangerCounterByStationTopic::EStates getState() const
    {
        return m_state;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QString m_routeName{};
    int m_stationNum{ -1 };
    QString m_stationName{};
    QList < CValidatorsPassangerCounterByStationTopic::SPassangerCounterInfo > m_counters{};
    int m_transactionCounter{ -1 };
    CValidatorsPassangerCounterByStationTopic::EStates m_state{
        CValidatorsPassangerCounterByStationTopic::EStates::uncknownRoute };
    int m_transactionCounterPrevStage{ -1 };
};

#endif // CINTEGRATIONPASSANGERCOUNTERBYSTATIONTOPIC_H
