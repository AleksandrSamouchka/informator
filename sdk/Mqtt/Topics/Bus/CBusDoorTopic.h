#ifndef BUS_DOOR_TOPIC_H
#define BUS_DOOR_TOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CBusDoorTopic : public CSimpleTopic<int>
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CBusDoorTopic()
        : CSimpleTopic<int>(QString("%1%2").arg("LIAZ/DOOR").arg(10)),
          m_doorNum{ 10 } {}
#endif
    public:
        CBusDoorTopic(int num);
        virtual ~CBusDoorTopic();

        inline int getNum() const
        {
            return m_doorNum;
        }

    private:
        int m_doorNum;
};


#endif

