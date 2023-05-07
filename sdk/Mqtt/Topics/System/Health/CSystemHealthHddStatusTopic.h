#ifndef CSYSTEMHEALTHHDDSTATUSTOPIC_H
#define CSYSTEMHEALTHHDDSTATUSTOPIC_H

#include <Mqtt/Topics/System/CGrSystemHealthTopic.h>

/** ***********************************************************************
 * @class CSystemHealthHddStatusTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CSystemHealthHddStatusTopic : public CGrSystemHealthTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    CSystemHealthHddStatusTopic();
    virtual ~CSystemHealthHddStatusTopic() override;
    inline const QStringList & getVolumes() const
    {
        return m_volumes;
    }

    void clearVolumes();
    void setVolumes( const QStringList & value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QStringList m_volumes{};
};

#endif // CSYSTEMHEALTHHDDSTATUSTOPIC_H
