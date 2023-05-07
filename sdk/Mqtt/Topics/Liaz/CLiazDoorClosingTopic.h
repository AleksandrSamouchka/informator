#ifndef CLIAZDOORCLOSINGTOPIC_H
#define CLIAZDOORCLOSINGTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazDoorClosingTopic : public CSimpleTopic< int >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CLiazDoorClosingTopic()
        : CSimpleTopic<int>( QString("LIAZ/DOOR%1CLOSING").arg(10) ),
          m_doorNum{ 10 } {}
#endif

public:
    CLiazDoorClosingTopic(int num);
    virtual ~CLiazDoorClosingTopic();

    inline int getNum() const
    {
        return m_doorNum;
    }

private:
    int m_doorNum;
};

#endif // CLIAZDOORCLOSING_H
