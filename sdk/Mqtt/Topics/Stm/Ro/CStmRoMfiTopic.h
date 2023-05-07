#ifndef CSTMROMFITOPIC_H
#define CSTMROMFITOPIC_H

#include <Mqtt/Topics/Stm/CGrStmRoTopic.h>

/** ***********************************************************************
 * @class CStmRoMfiTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CStmRoMfiTopic : public CGrStmRoTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    struct SInputState
    {
    public:
        enum class ETypes
        {
            Type_analog = 0,
            Type_digital,
            Type_unknown,
            Type_tamper,
        };

    public:
        ETypes m_type = ETypes::Type_unknown;
        double m_value{ 0.0 };

    public:
        SInputState() = default;
        SInputState( const ETypes type,
                     const double value ) :
            m_type{ type },
            m_value{ value }
        {}
    };

public:
    CStmRoMfiTopic();
    virtual ~CStmRoMfiTopic() override;
    inline const QList< SInputState > & getInputStates() const
    {
        return m_sensors;
    }

public:
    void appendInputState( const SInputState & value );
    void setInputStateList( const QList < SInputState > & value );
    void clearInputStateList();

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QList< SInputState > m_sensors{};
};






#endif // CSTMROMFITOPIC_H
