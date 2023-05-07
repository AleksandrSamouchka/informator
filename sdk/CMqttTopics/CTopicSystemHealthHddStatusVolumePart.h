#ifndef CTOPICSYSTEMHEALTHHDDSTATUSVOLUMEPART_H
#define CTOPICSYSTEMHEALTHHDDSTATUSVOLUMEPART_H

/** ***********************************************************************
 * @name CTopicSystemHealthHddStatusVolumePart
 * @brief Содержимое топика SYSTEM/HEALTH/HDD_STATUS
 * @author Мальнев А
 * @date 08-05-2019
 ** ***********************************************************************/

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicStmRoMfi
 ** ***********************************************************************/
class CTopicSystemHealthHddStatusVolumePart : public CTopicValueEntry
{
public:
    CTopicSystemHealthHddStatusVolumePart( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicSystemHealthHddStatusVolumePart() override = default;

public:
    QString m_partition{};
    int m_isMounted{ 0 };
    QString m_mountPoint{};
    QString m_totalVolume{};
    QString m_freeVolume{};
};


#endif // CTOPICSYSTEMHEALTHHDDSTATUSVOLUMEPART_H
