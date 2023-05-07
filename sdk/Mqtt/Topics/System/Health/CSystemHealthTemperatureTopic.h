#ifndef CSYSTEMHEALTHTEMPERATURETOPIC_H
#define CSYSTEMHEALTHTEMPERATURETOPIC_H

#include <Mqtt/Topics/System/CGrSystemHealthTopic.h>

#include <QtCore/QHash>

/** ***********************************************************************
 * @class CSystemHealthTemperatureTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CSystemHealthTemperatureTopic : public CGrSystemHealthTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    struct SDisckTemperature
    {
        QString m_currMaxTemp{};
        QString m_currMinTemp{};
        QString m_currTemp{};
        QString m_specMaxTemp{};
        QString m_specMinTemp{};
        QString m_type{};

        SDisckTemperature() = default;
        SDisckTemperature( const SDisckTemperature & other ) = default;
    };

public:
    CSystemHealthTemperatureTopic();
    virtual ~CSystemHealthTemperatureTopic() override;

    inline int getAmplifierTemp() const
    {
        return m_amplifierTemp;
    }
    inline int getCongaBoardTemp() const
    {
        return m_congaBoardTemp;
    }
    inline int getCongaCpuTemp() const
    {
        return m_congaCpuTemp;
    }
    inline const QHash < QString, SDisckTemperature > & getDiscksTemperature () const
    {
        return m_disksTemperature;
    }

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    int m_amplifierTemp{ -1 };
    int m_congaBoardTemp{ -1 };
    int m_congaCpuTemp{ -1 };

    QHash < QString, SDisckTemperature > m_disksTemperature{};
};

#endif // CSYSTEMHEALTHTEMPERATURETOPIC_H
