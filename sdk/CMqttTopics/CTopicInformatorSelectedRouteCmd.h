#ifndef CTOPICINFORMATORSELECTEDROUTECMD_H
#define CTOPICINFORMATORSELECTEDROUTECMD_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicInformatorSelectedRouteCmd
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/



class CTopicInformatorSelectedRouteCmd : public CTopicValueEntry
{
public:
    QString m_routeNum{};
    int m_subrouteNum{ 0 };
    int m_sender{-1};
public:

    CTopicInformatorSelectedRouteCmd();
    CTopicInformatorSelectedRouteCmd( const QString &route, const int subroute, int sender );

    bool initFromData( const CTopicValueEntry & value ) override;
    QByteArray prepareData() const override;
    virtual ~CTopicInformatorSelectedRouteCmd() override = default;

};

#endif // CTOPICINFORMATORSELECTEDROUTECMD_H
