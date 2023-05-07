#ifndef CTOPICINFORMATORSELECTEDBUSCMD_H
#define CTOPICINFORMATORSELECTEDBUSCMD_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicInformatorSelectedBusCmd
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicInformatorSelectedBusCmd : public CTopicValueEntry
{
public:
    int m_busNum = 0;

public:
    explicit CTopicInformatorSelectedBusCmd( int busNum );
    QByteArray prepareData();
};

#endif // CTOPICINFORMATORSELECTEDBUSCMD_H
