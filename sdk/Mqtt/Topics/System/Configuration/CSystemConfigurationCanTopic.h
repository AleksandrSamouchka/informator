#ifndef SYSTEM_CONFIGURATION_CAN_TOPIC_H
#define SYSTEM_CONFIGURATION_CAN_TOPIC_H

#include <QVector>

#include <Mqtt/Topics/System/CSystemConfigurationTopic.h>

#include <mnt_defs/can.h>

class CSystemConfigurationCanTopic : public CSystemConfigurationTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CSystemConfigurationCanTopic();
        virtual ~CSystemConfigurationCanTopic();

#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

        void setFilter(int8_t canId, int8_t filterId, const TCANIdFilter *filter);
        void setBaudrate(int8_t canId, uint32_t baudrate );

    protected:
        virtual const QByteArray prepareData() const;
                bool parseData( const QByteArray & data );

    private:
        void initData();

        QVector<TCANSettings>   m_canList;
};

#endif 
