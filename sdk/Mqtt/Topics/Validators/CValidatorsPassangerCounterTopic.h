#ifndef CVALIDATORSPASSANGERCOUNTERTOPIC_H
#define CVALIDATORSPASSANGERCOUNTERTOPIC_H

#include <Mqtt/Topics/CGrValidatorsTopic.h>

/** ***********************************************************************
 * @class CValidatorsPassangerCounterTopic
 * @brief класс, пассажиропоток и валидаторы
 ** ***********************************************************************/
class CValidatorsPassangerCounterTopic : public CGrValidatorsTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CValidatorsPassangerCounterTopic();
    explicit CValidatorsPassangerCounterTopic( const int total,
                                               const int currentInBus,
                                               const int notPayed );
    explicit CValidatorsPassangerCounterTopic( const QByteArray &data );

    inline int total() const
    {
        return m_total;
    }
    inline int currentInBus() const
    {
        return m_currentInBus;
    }
    inline int notPayed() const
    {
        return m_notPayed;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setTotal( const int value );
    void setCurrentInBus( const int value );
    void setNotPayed( const int value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    int m_total{ 0 };
    int m_currentInBus{ 0 };
    int m_notPayed{ 0 };
};

#endif // CVALIDATORSPASSANGERCOUNTERTOPIC_H
