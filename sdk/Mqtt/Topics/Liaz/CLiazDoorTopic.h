#ifndef CLIAZDOORTOPIC_H
#define CLIAZDOORTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazDoorTopic : public CSimpleTopic< int >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CLiazDoorTopic()
        : CSimpleTopic<int>( QString("%1%2").arg("LIAZ/DOOR").arg(10) ),
          m_doorNum{ 10 } {}
#endif

public:
    CLiazDoorTopic(int num);
    virtual ~CLiazDoorTopic();

    inline int getNum() const
    {
        return m_doorNum;
    }

private:
    int m_doorNum;
};

#endif // CLIAZDOORTOPIC_H
