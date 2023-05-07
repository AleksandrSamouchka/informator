#ifndef __CSTMIFCCANRXTOPIC_H__
#define __CSTMIFCCANRXTOPIC_H__

#include <Mqtt/Topics/Stm/CStmIfcTopic.h>

class CStmIfcCanRxTopic : public CStmIfcTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CStmIfcCanRxTopic() : CStmIfcTopic( QString( "CAN%1RX" ).arg( 10 )) {}
#endif
    public:
        CStmIfcCanRxTopic(int canId);
        virtual ~CStmIfcCanRxTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

    private:
        int m_canId;
};

#endif
