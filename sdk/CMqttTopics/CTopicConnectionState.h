#ifndef CTOPICCONNECTIONSTATE_H
#define CTOPICCONNECTIONSTATE_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicConnectionState
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicConnectionState : public CTopicValueEntry
{
public:
    enum class EUsedNetType
    {
        NetType_cellular = 0,
        NetType_wiFi,
        NetType_ethernet,

        NetType_unknown,
    };
    enum class ECellurarNetType
    {
        CellurarType_4G = 0,
        CellurarType_3G,
        CellurarType_2G,
        CellurarType_noCoverage,

        CellurarType_unknown,
    };

public:
    EUsedNetType m_usedNetType{ EUsedNetType::NetType_unknown };
    ECellurarNetType m_cellurarNetType{ ECellurarNetType::CellurarType_unknown };
    int m_cellurarQuality{ 0 };
    int m_wiFiQuality{ 0 };

public:
    CTopicConnectionState( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicConnectionState() = default;
};

#endif // CTOPICCONNECTIONSTATE_H
