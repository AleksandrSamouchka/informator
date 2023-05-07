#ifndef CVALIDATORSSTATETOPIC_H
#define CVALIDATORSSTATETOPIC_H

#include <Mqtt/Topics/CGrValidatorsTopic.h>

/** ***********************************************************************
 * @class CValidatorsStateTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CValidatorsStateTopic : public CGrValidatorsTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CValidatorsStateTopic() : CGrValidatorsTopic( "STATE" ) {}
#endif

public:
    explicit CValidatorsStateTopic( const int enabled,
                                    const int inWork );
    explicit CValidatorsStateTopic();

    inline int enabled() const
    {
        return m_enabled;
    }
    inline int inWork() const
    {
        return m_inWork;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setEnabled( const int value );
    void setInWork( const int value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    int m_enabled{ 0 };
    int m_inWork{ 0 };
};

#endif // CVALIDATORSSTATETOPIC_H
