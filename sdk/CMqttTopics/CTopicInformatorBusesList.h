#ifndef CTOPICINFORMATORBUSESLIST_H
#define CTOPICINFORMATORBUSESLIST_H

#include <CMqttTopics/CTopicValueEntry.h>


struct SRoute
{
    QString m_name;
    QString m_num;
    QStringList m_subroutes;

    SRoute() = default;
    SRoute( const QString & routeName,
            const QString & routeNumber,
            const QStringList & subroutes ) :
        m_name( routeName ),
        m_num( routeNumber ),
        m_subroutes( subroutes )
    {}
};

/** ***********************************************************************
 * @class CTopicInformatorBusesList
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicInformatorBusesList : public CTopicValueEntry
{
public:
    QList < SRoute > m_routesList;

public:
    CTopicInformatorBusesList( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual QByteArray prepareData() const override;
    virtual ~CTopicInformatorBusesList() override = default;
};

#endif // CTOPICINFORMATORBUSESLIST_H
