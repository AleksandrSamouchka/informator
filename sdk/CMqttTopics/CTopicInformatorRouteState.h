#ifndef CTOPICINFORMATORROUTESTATE_H
#define CTOPICINFORMATORROUTESTATE_H

#include <CMqttTopics/CTopicValueEntry.h>
#include <CMqttTopics/CInformatorSubrouteInfo.h>



/** ***********************************************************************
 * @class CTopicInformatorRouteState
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicInformatorRouteState : public CTopicValueEntry
{
public:
    QString m_routeNum;
    int m_currentRoute{ 0 };
    int m_currentSubroute{ 0 };
    int m_userCurrentStation{ 0 };
    int m_currentStation{ 0 };
    int m_nextStation{ 0 };
    int m_userNextStation{ 0 };
    int m_subroutePercentage{ 0 };
    bool m_isLocateOnStation{ false };

    QList < CInformatorSubrouteInfo > m_subroutes;

public:
    CTopicInformatorRouteState( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicInformatorRouteState() = default;
};

#endif // CTOPICINFORMATORROUTESTATE_H
