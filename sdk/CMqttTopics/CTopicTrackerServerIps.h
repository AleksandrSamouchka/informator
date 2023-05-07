#ifndef CTOPICTRACKERSERVERIPS_H
#define CTOPICTRACKERSERVERIPS_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicTrackerServerIps
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicTrackerServerIps : public CTopicValueEntry
{
public:
    QStringList m_serverIps{};

public:
    CTopicTrackerServerIps( const QStringList &servers = QStringList{} );
    QByteArray prepareData() const;
    virtual ~CTopicTrackerServerIps() = default;
};

#endif // CTOPICTRACKERSERVERIPS_H
