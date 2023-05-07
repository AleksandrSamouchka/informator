#ifndef CTOPICSYSTEMHEALTHHDDSTATUS_H
#define CTOPICSYSTEMHEALTHHDDSTATUS_H

/** ***********************************************************************
 * @name CTopicSystemHealthHddStatus
 * @brief Содержимое топика SYSTEM/HEALTH/HDD_STATUS
 * @author Мальнев А
 * @date 08-05-2019
 ** ***********************************************************************/

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicSystemHealthHddStatus
 ** ***********************************************************************/
class CTopicSystemHealthHddStatus : public CTopicValueEntry
{
public:
    CTopicSystemHealthHddStatus( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicSystemHealthHddStatus() override = default;

public:
    QStringList m_volumes{};
};

#endif // CTOPICSYSTEMHEALTHHDDSTATUS_H
