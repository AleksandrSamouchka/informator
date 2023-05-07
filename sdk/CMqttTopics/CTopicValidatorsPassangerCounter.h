#ifndef CTOPICVALIDATORSPASSANGERCOUNTER_H
#define CTOPICVALIDATORSPASSANGERCOUNTER_H

/** ***********************************************************************
 * @name CTopicValidatorsPassangerCounter
 * @brief Содержимое топика VALIDATORS/PASSANGER_COUNTER
 * @author Мальнев А
 * @date 17-05-2019
 ** ***********************************************************************/

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicValidatorsPassangerCounter
 ** ***********************************************************************/
class CTopicValidatorsPassangerCounter : public CTopicValueEntry
{
public:
    CTopicValidatorsPassangerCounter( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicValidatorsPassangerCounter() override = default;
    virtual QByteArray prepareData() const override;

public:
    int m_total{ 0 };
    int m_currentInBus{ 0 };
    int m_notPayed{ 0 };
};

#endif // CTOPICVALIDATORSPASSANGERCOUNTER_H
