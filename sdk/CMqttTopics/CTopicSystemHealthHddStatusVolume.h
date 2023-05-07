#ifndef CTOPICSYSTEMHEALTHHDDSTATUSVOLUME_H
#define CTOPICSYSTEMHEALTHHDDSTATUSVOLUME_H

/** ***********************************************************************
 * @name CTopicSystemHealthHddStatusVolume
 * @brief Список с состоянием подключенного жесткого диска
 * @author Мальнев А
 * @date 08-05-2019
 ** ***********************************************************************/

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicSystemHealthHddStatusVolume
 ** ***********************************************************************/
class CTopicSystemHealthHddStatusVolume : public CTopicValueEntry
{
public:
    static const int DEFAULT_TEMP{ -250 };

public:
    CTopicSystemHealthHddStatusVolume( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicSystemHealthHddStatusVolume() override = default;

public:
    QString m_partition{};
    int m_currTemp{ DEFAULT_TEMP };
    int m_currMinTemp{ DEFAULT_TEMP };
    int m_currMaxTemp{ DEFAULT_TEMP };
    int m_specMinTemp{ DEFAULT_TEMP };
    int m_specMaxTemp{ DEFAULT_TEMP };
    int m_system{ 0 };
};

#endif // CTOPICSYSTEMHEALTHHDDSTATUSVOLUME_H
