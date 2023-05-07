#ifndef CTOPICLOGGERGUIMESSAGE_H
#define CTOPICLOGGERGUIMESSAGE_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicLoggerGuiMessage
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicLoggerGuiMessage : public CTopicValueEntry
{

public:
    QString m_moduleName = QString{};
    QString m_messageName = QString{};
    QString m_message = QString{};
    CLoggerMessage::ELogCriteria m_criteria = CLoggerMessage::ELogCriteria::LogCriteria_error;

public:
    CTopicLoggerGuiMessage( const QByteArray &json = QByteArray{} );
    virtual ~CTopicLoggerGuiMessage() = default;
    bool initFromData();
};

#endif // CTOPICLOGGERGUIMESSAGE_H
