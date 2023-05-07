#ifndef CTOPICSTMROFWWERSION_H
#define CTOPICSTMROFWWERSION_H

/** ***********************************************************************
 * @name CTopicStmRoFwWersion
 * @brief Содержимое топика с версией ПО МК
 * @author Мальнев А
 * @date 01-02-2019
 ** ***********************************************************************/

#include <CMqttTopics/CTopicValueEntry.h>
#include <CMqttTopics/CTermoState.h>

/** ***********************************************************************
 * @class CTopicStmRoFwWersion
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicStmRoFwWersion : public CTopicValueEntry
{
public:
    QString mBootloaderVersion{};
    QString mApplicationVersion{};

public:
    CTopicStmRoFwWersion( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value );
    virtual ~CTopicStmRoFwWersion() = default;
};

#endif // CTOPICSTMROFWWERSION_H
