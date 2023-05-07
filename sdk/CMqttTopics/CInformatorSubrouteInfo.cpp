#include "CInformatorSubrouteInfo.h"

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CInformatorSubrouteInfo::CInformatorSubrouteInfo( int number,
                                                  const QString &name,
                                                  const QList<CInformatorStationInfo> &stations ) :
    m_number{ number },
    m_name( name ),
    m_stations( stations )
{ }
