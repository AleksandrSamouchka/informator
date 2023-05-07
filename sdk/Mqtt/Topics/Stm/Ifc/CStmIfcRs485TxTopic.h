#ifndef __CSTMIFCRS485TXTOPIC_H__
#define __CSTMIFCRS485TXTOPIC_H__

#include <Mqtt/Topics/Stm/CStmIfcTopic.h>

class CStmIfcRs485TxTopic : public CStmIfcTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CStmIfcRs485TxTopic() : CStmIfcTopic( QString( "RS485%1TX" ).arg( 10 )) {}
#endif
    public:
        CStmIfcRs485TxTopic(int rsIs);
        CStmIfcRs485TxTopic(int rsIs, const QByteArray &data);
        virtual ~CStmIfcRs485TxTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

    private:
        int m_rsId;
};

#endif
