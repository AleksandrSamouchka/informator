#ifndef CGRSTMROTOPIC_H
#define CGRSTMROTOPIC_H

#include <Mqtt/Topics/CGrStmTopic.h>

class CGrStmRoTopic : public CGrStmTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrStmRoTopic() : CGrStmTopic( "RO/" ) {}
#endif

public:
    CGrStmRoTopic( const QString &topicName );
    CGrStmRoTopic( const QString &topicName, const QByteArray &data );

    virtual ~CGrStmRoTopic();
};

#endif // CGRSTMROTOPIC_H
