#ifndef CTOPICSTMDOUBLEVALUE_H
#define CTOPICSTMDOUBLEVALUE_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicStmDoubleValue
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicStmDoubleValue : public CTopicValueEntry
{
public:
    double m_value{ 0 };

public:
    CTopicStmDoubleValue( const double value, const QByteArray &json = QByteArray{} );
    CTopicStmDoubleValue();

    bool initFromData( const CTopicValueEntry &value ) override;
    virtual QByteArray prepareData() const override;
    virtual ~CTopicStmDoubleValue() override = default;
};

#endif // CTOPICSTMDOUBLEVALUE_H
