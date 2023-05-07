#ifndef CLIAZDOOROPENINGTOPIC_H
#define CLIAZDOOROPENINGTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazDoorOpeningTopic : public CSimpleTopic< int >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CLiazDoorOpeningTopic()
        : CSimpleTopic<int>( QString("LIAZ/DOOR%1OPENING").arg(10) ),
          m_doorNum{ 10 } {}
#endif

public:
    CLiazDoorOpeningTopic(int num);
    virtual ~CLiazDoorOpeningTopic();

    inline int getNum() const
    {
        return m_doorNum;
    }

private:
    int m_doorNum;
};

#endif // CLIAZDOOROPENING_H
