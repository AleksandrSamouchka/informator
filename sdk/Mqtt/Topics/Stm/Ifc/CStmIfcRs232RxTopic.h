#ifndef __CSTMIFCRS232RXTOPIC_H__
#define __CSTMIFCRS232RXTOPIC_H__

#include <Mqtt/Topics/Stm/CStmIfcTopic.h>

class CStmIfcRs232RxTopic : public CStmIfcTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CStmIfcRs232RxTopic() : CStmIfcTopic( QString( "RS232%1RX" ).arg( 10 )) {}
#endif
    public:
        CStmIfcRs232RxTopic(int rsId);
        virtual ~CStmIfcRs232RxTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
    
    private:
        int m_rsId;
};      

#endif
