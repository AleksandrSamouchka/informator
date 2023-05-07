#ifndef CTOPICSYSTEMHEALTHHDDPREPARE_H
#define CTOPICSYSTEMHEALTHHDDPREPARE_H

/** ***********************************************************************
 * @name CTopicSystemHealthHddPrepare
 * @brief Содержимое топика SYSTEM/HEALTH/HDD_PREPARE
 * @author Мальнев А
 * @date 13-05-2019
 ** ***********************************************************************/

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicSystemHealthHddPrepare
 ** ***********************************************************************/
class CTopicSystemHealthHddPrepare : public CTopicValueEntry
{
public:
    CTopicSystemHealthHddPrepare( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicSystemHealthHddPrepare() override = default;
    virtual QByteArray prepareData() const override;

public:
    QString m_volume{};
    int m_prepared{ 0 };
};

#endif // CTOPICSYSTEMHEALTHHDDPREPARE_H
