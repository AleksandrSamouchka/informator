#ifndef CTOPICSTMROMFI_H
#define CTOPICSTMROMFI_H

#include <CMqttTopics/CTopicValueEntry.h>
#include <CMqttTopics/CSensorState.h>

/** ***********************************************************************
 * @class CTopicStmRoMfi
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicStmRoMfi : public CTopicValueEntry
{
public:
    QList< CSensorState > m_sensors{};

public:
    CTopicStmRoMfi( const QByteArray &json );
    bool initFromData();
    virtual ~CTopicStmRoMfi() = default;
};

#endif // CTOPICSTMROMFI_H
