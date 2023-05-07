#ifndef __SYSTEM_FACTORY_TESTING_H__
#define __SYSTEM_FACTORY_TESTING_H__

#include <Mqtt/Topics/CSimpleTopic.h>


class CSystemFactoryTesting : public CSimpleTopic<int>
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CSystemFactoryTesting();
        virtual ~CSystemFactoryTesting();

//    protected:
//        virtual bool parseData( const QByteArray & data );
};

#endif
