#ifndef CLIAZPASSANGERREQUESTTOPIC_H
#define CLIAZPASSANGERREQUESTTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazPassangerRequestTopic : public CSimpleTopic< int >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CLiazPassangerRequestTopic()
        : CSimpleTopic<int>( QString("LIAZ/PASSENGERREQ%1").arg(10) ),
          m_doorNum{ 10 } {}
#endif

public:
    CLiazPassangerRequestTopic(int num);
    virtual ~CLiazPassangerRequestTopic();

    inline int getNum() const
    {
        return m_doorNum;
    }

private:
    int m_doorNum;
};

#endif // CLIAZPASSANGERREQUESTTOPIC_H
