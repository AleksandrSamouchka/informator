#ifndef CTOPICINFORMATORPLAYDOORSCMD_H
#define CTOPICINFORMATORPLAYDOORSCMD_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicInformatorPlayDoorsCmd
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicInformatorPlayDoorsCmd : public CTopicValueEntry
{
public:
    int m_stationNum{};

public:
    CTopicInformatorPlayDoorsCmd( const int stationNum );
    QByteArray prepareData();
    virtual ~CTopicInformatorPlayDoorsCmd() = default;
};

#endif // CTOPICINFORMATORPLAYDOORSCMD_H
