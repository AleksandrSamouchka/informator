#ifndef __CSTMIFCCANTXTOPIC_H__
#define __CSTMIFCCANTXTOPIC_H__

#include <Mqtt/Topics/Stm/CStmIfcTopic.h>

class CStmIfcCanTxTopic : public CStmIfcTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CStmIfcCanTxTopic() : CStmIfcTopic( QString( "CAN%1TX" ).arg( 10 )) {}
#endif
    public:
        CStmIfcCanTxTopic(int canId);
        CStmIfcCanTxTopic(int canId, const QByteArray &data);
        virtual ~CStmIfcCanTxTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

    private:
        int m_canId;
};

#endif
