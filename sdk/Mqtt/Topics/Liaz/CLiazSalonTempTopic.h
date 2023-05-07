#ifndef CLIAZSALONTEMPTOPIC_H
#define CLIAZSALONTEMPTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazSalonTempTopic : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CLiazSalonTempTopic()
        : CSimpleTopic< double >( "LIAZ/SALOONTEMP10" ),
          m_sensorNum{ 10 } {}
#endif

public:
    CLiazSalonTempTopic(int num);
    virtual ~CLiazSalonTempTopic();

    inline int getNum() const
    {
        return m_sensorNum;
    }

private:
    int m_sensorNum;
};

#endif // CLIAZSALONTEMPTOPIC_H
