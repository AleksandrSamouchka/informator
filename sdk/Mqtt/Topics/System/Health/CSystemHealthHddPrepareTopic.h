#ifndef CSYSTEMHEALTHHDDPREPARETOPIC_H
#define CSYSTEMHEALTHHDDPREPARETOPIC_H

#include <Mqtt/Topics/System/CGrSystemHealthTopic.h>

/** ***********************************************************************
 * @class CSystemHealthHddPrepareTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CSystemHealthHddPrepareTopic : public CGrSystemHealthTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    CSystemHealthHddPrepareTopic();
    virtual ~CSystemHealthHddPrepareTopic() override;
    inline const QString & getVolume() const
    {
        return m_volume;
    }
    inline int getPrepared() const
    {
        return m_prepared;
    }

    void setVolume( const QString & value );
    void setPrepared( const int value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QString m_volume{};
    int m_prepared{ 0 };
};

#endif // CSYSTEMHEALTHHDDPREPARETOPIC_H
