#ifndef CTOPICRECORDERRECORDS_H
#define CTOPICRECORDERRECORDS_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicRecorderRecords
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicRecorderRecords : public CTopicValueEntry
{
public:
    enum ERecordEventType {
        EventType_add = 0,
        EventType_delete,
        EventType_unknown,
    };

public:
    ERecordEventType m_eventType{ EventType_unknown };
    int m_cameraNumber{ 0 };
    QString m_path{};
    qlonglong m_beginTs{ 0 };
    qlonglong m_endTs{ 0 };
    qlonglong m_dbId{ 0 };

public:
    CTopicRecorderRecords( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicRecorderRecords() = default;
};

#endif // CTOPICRECORDERRECORDS_H
