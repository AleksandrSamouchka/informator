#ifndef CINFORMATORSTATIONINFO_H
#define CINFORMATORSTATIONINFO_H

#include <QtCore/QObject>

/** ***********************************************************************
 * @class CInformatorStationInfo
 * @brief класс, содержащий информацию об остановочном пункте
 ** ***********************************************************************/
class CInformatorStationInfo
{
public:
    enum EStationType
    {
/*        Station_start = 0,
        Station_intermidiate,
        Station_end,
*/
        Station_start = 0,      // Начальная
        Station_intermidiate,      // Промежуточная
        Station_end,       // Конечная
        Station_message,      // точка звукового сообщения
        Station_base,
        Station_unknownType
    };

public:
    int m_number{ 0 };
    QString m_name{};
    qint64 m_arriveTime{ 0 };
    qint64 m_duration{ 0 };
    double m_latitude{ 0 };     /*!< NOT USED */
    double m_longitude{ 0 };    /*!< NOT USED */
    int m_type{ 0 };            /*!< NOT USED */
    QString m_additionalInfo{};

public:
    CInformatorStationInfo(){}
    explicit CInformatorStationInfo( int number,
                                     const QString &name,
                                     qint64 arriveTime,
                                     qint64 duration,
                                     int type,
                                     const QString &additionalInfo);
    virtual ~CInformatorStationInfo() = default;
};

#endif // CINFORMATORSTATIONINFO_H
