#pragma once

#include <Mqtt/Topics/CGrValidatorsTopic.h>

/*!
 * \class The CValidatorsPassangerCounterOnStationTopic
 * \class Passanger flow and dorr state on station for MGT
 */

class CValidatorsPassangerCounterOnStationTopic : public CGrValidatorsTopic
{
public:
    explicit CValidatorsPassangerCounterOnStationTopic();
    explicit CValidatorsPassangerCounterOnStationTopic(const QByteArray& data);

    inline const uint8_t & getIrmaPresentDoor() const
    {
        return m_irma_present_door;
    }
    inline const QList<uint8_t> & getInPassangerCounters() const
    {
        return m_inPassangerCounters;
    }
    inline const QList<uint8_t> & getOutPassangerCounters() const
    {
        return m_outPassangerCounters;
    }
public:
    void setIrmaPresentDoor(uint8_t value);
    void setInPassangerCounters(const QList<uint8_t> values);
    void setOutPassangerCounters(const QList<uint8_t> values);
protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;
private:
    /*!
     * \brief m_irma_present_door
     * bit0...3 - door exist (0 - doesn't exist, 1 - exist)
     * bit4...7 - door state (0 - door wasn't open, 1 - door was open)
     */
    uint8_t m_irma_present_door;
    QList<uint8_t> m_inPassangerCounters;
    QList<uint8_t> m_outPassangerCounters;
};


