#ifndef CTOPICSTRINGVALUE_H
#define CTOPICSTRINGVALUE_H


#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicStringValue
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicStringValue : public CTopicValueEntry
{
public:
    QString m_value{};

public:
    CTopicStringValue( const QString & value, const QByteArray json = QByteArray{} );
    CTopicStringValue();

    virtual bool initFromData( const CTopicValueEntry & value ) override;
    virtual QByteArray prepareData() const override;
    virtual ~CTopicStringValue() override = default;
};


#endif // CTOPICSTRINGVALUE_H
