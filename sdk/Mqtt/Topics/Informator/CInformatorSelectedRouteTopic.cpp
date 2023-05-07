//Local
#include "CInformatorSelectedRouteTopic.h"

//Qt
#include <QMap>

namespace
{
    const QString SUBTOPICNAME      = "SELECTED_ROUTE";

    const QString FIELD_ROUTENUM    = "Route_num";
    const QString FIELD_SUBROUTENUM = "Subroute_num";
    const QString FIELD_SENDER      = "Sender";

    const QMap<Sender, QString> SENDER_NAME
    {
        {Informator, "Informator"},
        {GUI, "Gui"},
        {Undefine, "Undefine"}
    };
}



/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CInformatorSelectedRouteTopic::CInformatorSelectedRouteTopic(const QString &route , int subroute,
                                                             Sender sender)
    :   CGrInformatorTopic( SUBTOPICNAME ),
        m_routeNum(route),
        m_subrouteNum(subroute),
        m_sender(sender)
{
    prepareData();
}

CInformatorSelectedRouteTopic::CInformatorSelectedRouteTopic( const QByteArray & data ) :
    CGrInformatorTopic( SUBTOPICNAME, data )
{ }

CInformatorSelectedRouteTopic::CInformatorSelectedRouteTopic( )
    : CGrInformatorTopic( SUBTOPICNAME ),
      m_subrouteNum(0),
      m_sender(Undefine)
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CInformatorSelectedRouteTopic::getEtalonJson()
{
    QJsonObject jsonMainObj( { { FIELD_ROUTENUM, QJsonValue( "00" ) },
                                     { FIELD_SUBROUTENUM, 1 } } );
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

void CInformatorSelectedRouteTopic::setRouteNum(const QString & value)
{
    m_routeNum = value;
}

void CInformatorSelectedRouteTopic::setSubrouteNum(int value)
{
    m_subrouteNum = value;
}

void CInformatorSelectedRouteTopic::setSender(Sender sender)
{
    m_sender = sender;
}

QString CInformatorSelectedRouteTopic::senderToString()
{
    return SENDER_NAME.value(m_sender);
}

/** ***********************************************************************
 * @fn prepareData
 * @brief метод формирования значения топика из полей структуры
 * @param none
 * @return QByteArray
 ** ***********************************************************************/
const QByteArray CInformatorSelectedRouteTopic::prepareData() const
{
    QJsonObject jsonMainObj( { { FIELD_ROUTENUM, QJsonValue( m_routeNum ) },
                               { FIELD_SUBROUTENUM, m_subrouteNum },
                               { FIELD_SENDER, m_sender}} );
//    QByteArray jsonData{ 
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
//};
//    return setData( /*jsonData*/jsonMainObj );
}

/** ***********************************************************************
 * @fn parseData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param const QByteArray & data
 * @return bool
 ** ***********************************************************************/
bool CInformatorSelectedRouteTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_ROUTENUM ) &&
             jsonMainObj.contains( FIELD_SUBROUTENUM ) )
        {
            m_routeNum = jsonMainObj.value( FIELD_ROUTENUM ).toString();
            m_subrouteNum = jsonMainObj.value( FIELD_SUBROUTENUM ).toInt();
            m_sender = static_cast<Sender>(jsonMainObj.value( FIELD_SENDER ).toInt());
            res = true;
        }
        else
        {
            printUnknownFieldsListMessage(jsonMainObj.keys());
        }
    }
    else
    {
        printEmptyJsonMessage();
    }
    return setValidState( res );
}


