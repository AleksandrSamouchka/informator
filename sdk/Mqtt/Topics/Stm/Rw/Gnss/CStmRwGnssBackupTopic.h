#ifndef __CSTMRWGNSSBACKUPTOPIC_H__
#define __CSTMRWGNSSBACKUPTOPIC_H__

#include <Mqtt/Topics/Stm/Rw/CStmRwGnssTopic.h>

class CStmRwGnssBackupTopic : public CStmRwGnssTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CStmRwGnssBackupTopic();
        virtual ~CStmRwGnssBackupTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};

#endif
