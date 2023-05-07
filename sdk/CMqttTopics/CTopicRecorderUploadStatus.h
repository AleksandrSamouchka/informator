#ifndef CTOPICRECORDERUPLOADSTATUS_H
#define CTOPICRECORDERUPLOADSTATUS_H

#include <CMqttTopics/CTopicValueEntry.h>
#include <CMqttTopics/CRecorderUploadStatus.h>

/** ***********************************************************************
 * @class CTopicRecorderUploadStatus
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicRecorderUploadStatus : public CTopicValueEntry
{
public:
    QList < CRecorderUploadStatus > m_uploadStatusList{};

public:
    CTopicRecorderUploadStatus( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicRecorderUploadStatus() = default;
};

#endif // CTOPICRECORDERUPLOADSTATUS_H
