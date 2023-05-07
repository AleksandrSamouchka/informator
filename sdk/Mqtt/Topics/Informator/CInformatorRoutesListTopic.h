#ifndef CINFORMATORROUTESLISTTOPIC_H
#define CINFORMATORROUTESLISTTOPIC_H

#include <Mqtt/Topics/CGrInformatorTopic.h>

/** ***********************************************************************
 * @class CInformatorRoutesListTopic
 ** ***********************************************************************/
class CInformatorRoutesListTopic : public CGrInformatorTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
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

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    CInformatorRoutesListTopic();
    CInformatorRoutesListTopic( const QList < SRoute > & routesList );
    CInformatorRoutesListTopic( const QByteArray &data );

    inline const QList < SRoute > & routesList() const
    {
        return m_routesList;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setRoutesList( const QList < SRoute > & routesList );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QList < SRoute > m_routesList{};
};

#endif // CINFORMATORROUTESLISTTOPIC_H
