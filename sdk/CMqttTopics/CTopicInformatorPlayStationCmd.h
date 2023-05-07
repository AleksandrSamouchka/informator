#ifndef CTOPICINFORMATORPLAYSTATIONCMD_H
#define CTOPICINFORMATORPLAYSTATIONCMD_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicInformatorPlayStationCmd
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicInformatorPlayStationCmd : public CTopicValueEntry
{
public:
    int m_stationNum{};

public:
    CTopicInformatorPlayStationCmd( int stationNum );
    QByteArray prepareData();
    virtual ~CTopicInformatorPlayStationCmd() = default;
};

#endif // CTOPICINFORMATORPLAYSTATIONCMD_H
