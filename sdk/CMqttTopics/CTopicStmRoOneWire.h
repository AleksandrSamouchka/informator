#ifndef CTOPICSTMROONEWIRE_H
#define CTOPICSTMROONEWIRE_H

#include <CMqttTopics/CTopicValueEntry.h>
#include <CMqttTopics/CTermoState.h>

/** ***********************************************************************
 * @class CTopicStmRoOneWire
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicStmRoOneWire : public CTopicValueEntry
{
public:
    QList< CTermoState > m_sensors{};

public:
    CTopicStmRoOneWire( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value );
    virtual ~CTopicStmRoOneWire() = default;
};

#endif // CTOPICSTMROONEWIRE_H
