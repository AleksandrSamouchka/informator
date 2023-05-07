#ifndef CSTMROONEWIRETOPIC_H
#define CSTMROONEWIRETOPIC_H

#include <Mqtt/Topics/Stm/CGrStmRoTopic.h>

/** ***********************************************************************
 * @class CStmRoOneWireTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CStmRoOneWireTopic : public CGrStmRoTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    struct STermoState
    {
        QString m_name{};
        QString m_id{};
        double m_value{ 0.0 };

        STermoState() = default;
        explicit STermoState( const QString &name,
                              const QString &id,
                              double value ) :
            m_name{ name },
            m_id{ id },
            m_value{ value }
        { }
    };
    static const QString EMPTY_ID;

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    CStmRoOneWireTopic();
    virtual ~CStmRoOneWireTopic() override;
    inline const QList < STermoState > & getSensors() const
    {
        return m_sensors;
    }

public:
    void appendTermoSensor( const STermoState & value );
    void setTermoSensorList( const QList < STermoState > & value );
    void clearTermoSensorList();

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QList< STermoState > m_sensors{};
};

#endif // CSTMROONEWIRETOPIC_H
