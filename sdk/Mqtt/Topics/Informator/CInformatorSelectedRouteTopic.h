#ifndef CINFORMATORSELECTEDROUTETOPIC_H
#define CINFORMATORSELECTEDROUTETOPIC_H

#include <Mqtt/Topics/CGrInformatorTopic.h>

/** ***********************************************************************
 * @class CInformatorSelectedRouteTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/

enum Sender
{
    Informator = 0,
    GUI,
    Undefine = -1
};

class CInformatorSelectedRouteTopic : public CGrInformatorTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CInformatorSelectedRouteTopic( );
    explicit CInformatorSelectedRouteTopic( const QString &route, int subroute, Sender sender );
    explicit CInformatorSelectedRouteTopic( const QByteArray &data );

    inline const QString & routeNum() const
    {
        return m_routeNum;
    }
    inline int subrouteNum() const
    {
        return m_subrouteNum;
    }
    inline Sender getSender() const
    {
        return m_sender;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setRouteNum( const QString & value );
    void setSubrouteNum( int value );
    void setSender(Sender sender);
    QString senderToString();

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QString m_routeNum;
    int     m_subrouteNum;
    Sender  m_sender;
};

#endif // CINFORMATORSELECTEDROUTETOPIC_H
