#ifndef CTOPICSTMDISCRETVALUE_H
#define CTOPICSTMDISCRETVALUE_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicStmDiscretValue
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicStmDiscretValue : public CTopicValueEntry
{
public:
    int m_value{ 0 };

public:
    CTopicStmDiscretValue( const int value, const QByteArray & json = QByteArray{} );
    CTopicStmDiscretValue() = default;

    bool initFromData( const CTopicValueEntry & value );
    virtual QByteArray prepareData() const;
    virtual ~CTopicStmDiscretValue() = default;
};

#endif // CTOPICSTMDISCRETVALUE_H
