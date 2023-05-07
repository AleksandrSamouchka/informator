#ifndef __CSTMIFCTOPIC_H__
#define __CSTMIFCTOPIC_H__

#include <Mqtt/Topics/CGrStmTopic.h>

class CStmIfcTopic : public CGrStmTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CStmIfcTopic() : CGrStmTopic() {}
#endif
    public:
        CStmIfcTopic(const QString &subtopicName);
        CStmIfcTopic(const QString &subtopicName, const QByteArray &data);
        virtual ~CStmIfcTopic();

};

#endif
