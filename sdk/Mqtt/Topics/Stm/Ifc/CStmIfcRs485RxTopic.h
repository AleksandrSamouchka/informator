#ifndef __CSTMIFCRS485RXTOPIC_H__
#define __CSTMIFCRS485RXTOPIC_H__

#include <Mqtt/Topics/Stm/CStmIfcTopic.h>

class CStmIfcRs485RxTopic : public CStmIfcTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CStmIfcRs485RxTopic() : CStmIfcTopic( QString( "RS485%1RX" ).arg( 10 )) {}
#endif
    public:
        CStmIfcRs485RxTopic(int rsId);
        virtual ~CStmIfcRs485RxTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
    
    private:
        int m_rsId;
};

#endif
