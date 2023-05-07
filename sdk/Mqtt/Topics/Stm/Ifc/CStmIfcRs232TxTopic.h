#ifndef __CSTMIFCRS232TXTOPIC_H__
#define __CSTMIFCRS232TXTOPIC_H__

#include <Mqtt/Topics/Stm/CStmIfcTopic.h>

class CStmIfcRs232TxTopic : public CStmIfcTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CStmIfcRs232TxTopic() : CStmIfcTopic( QString( "RS232%1TX" ).arg( 10 )) {}
#endif
    public:
        CStmIfcRs232TxTopic(int rsId);
        CStmIfcRs232TxTopic(int rsId, const QByteArray &data);
        virtual ~CStmIfcRs232TxTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
    
    private:
        int m_rsId;
};

#endif
