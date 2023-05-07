#ifndef CINFORMATORSUBROUTEINFO_H
#define CINFORMATORSUBROUTEINFO_H

#include <CMqttTopics/CInformatorStationInfo.h>
#include <QtCore/QList>

/** ***********************************************************************
 * @class CInformatorStationInfo
 * @brief класс, содержащий информацию об остановочном пункте
 ** ***********************************************************************/
class CInformatorSubrouteInfo
{
public:
    int m_number{ 0 };
    QString m_name{};
    QList < CInformatorStationInfo > m_stations{};

public:
    explicit CInformatorSubrouteInfo( int number,
                                      const QString &name,
                                      const QList < CInformatorStationInfo > &stations = QList < CInformatorStationInfo >{} );
    CInformatorSubrouteInfo() = default;
    virtual ~CInformatorSubrouteInfo() = default;
};

#endif // CINFORMATORSUBROUTEINFO_H
