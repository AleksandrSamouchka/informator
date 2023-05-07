#include "CInformatorStationInfo.h"

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CInformatorStationInfo::CInformatorStationInfo( int number,
                                                const QString &name,
                                                qint64 arriveTime,
                                                qint64 duration,
                                                int type,
                                                const QString &additinalInfo) :
    m_number{ number },
    m_name( name ),
    m_arriveTime{ arriveTime },
    m_duration{ duration },
    m_type {type},
    m_additionalInfo{additinalInfo}
{}
