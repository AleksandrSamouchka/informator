#ifndef CTOPICVALIDATORSSTATUS_H
#define CTOPICVALIDATORSSTATUS_H

/** ***********************************************************************
 * @name CTopicValidatorsStatus
 * @brief Содержимое топика VALIDATORS/STATE
 * @author Мальнев А
 * @date 17-05-2019
 ** ***********************************************************************/

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicValidatorsStatus
 ** ***********************************************************************/
class CTopicValidatorsStatus : public CTopicValueEntry
{
public:
    CTopicValidatorsStatus( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicValidatorsStatus() override = default;
    virtual QByteArray prepareData() const override;

public:
    int m_enabled{ 0 };
    int m_inWork{ 0 };
};

#endif // CTOPICVALIDATORSSTATUS_H
