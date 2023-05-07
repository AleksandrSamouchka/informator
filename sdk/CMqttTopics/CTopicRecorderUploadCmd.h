#ifndef CTOPICRECORDERUPLOADCMD_H
#define CTOPICRECORDERUPLOADCMD_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicRecorderUploadCmd
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicRecorderUploadCmd : public CTopicValueEntry
{
public:
    QString m_taskId{};
    int m_cameraNumber{ 0 };
    qint64 m_startTime{ 0 };
    qint64 m_stopTime{ 0 };
    QString m_format{};         /*!< (RAW, ZIP) */
    QString m_destination{};    /*!< (URL, i.e. file:// or sftp://) */

public:
    CTopicRecorderUploadCmd();
    QByteArray prepareData();
    virtual ~CTopicRecorderUploadCmd() = default;
};

#endif // CTOPICRECORDERUPLOADCMD_H
