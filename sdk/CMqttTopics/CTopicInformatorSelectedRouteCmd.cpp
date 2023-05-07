#include "CTopicInformatorSelectedRouteCmd.h"

namespace
{
    const QString FIELD_ROUTENUM = "Route_num";
    const QString FIELD_SUBROUTENUM = "Subroute_num";
    const QString FIELD_SENDER = "Sender";
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicInformatorSelectedRouteCmd::CTopicInformatorSelectedRouteCmd() :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Informator_selectedRoute )
{ }

CTopicInformatorSelectedRouteCmd::CTopicInformatorSelectedRouteCmd(const QString &route , const int subroute , int sender) :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Informator_selectedRoute ),
    m_routeNum{ route },
    m_subrouteNum{ subroute },
    m_sender(sender)
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param const CTopicValueEntry
 * @return bool
 ** ***********************************************************************/
bool CTopicInformatorSelectedRouteCmd::initFromData(const CTopicValueEntry & value)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_ROUTENUM ) &&
             jsonMainObj.contains( FIELD_SUBROUTENUM ) )
        {
            m_routeNum = jsonMainObj.value( FIELD_ROUTENUM ).toString();
            m_subrouteNum = jsonMainObj.value( FIELD_SUBROUTENUM ).toInt();
            m_sender = jsonMainObj.value( FIELD_SENDER ).toInt();
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
    setValidState( res );
    return res;
}

/** ***********************************************************************
 * @fn prepareData
 * @brief метод формирования значения топика из полей структуры
 * @param none
 * @return QByteArray
 ** ***********************************************************************/
QByteArray CTopicInformatorSelectedRouteCmd::prepareData() const
{
    const QJsonObject jsonMainObj( { { FIELD_ROUTENUM, QJsonValue( m_routeNum ) },
                                     { FIELD_SUBROUTENUM, m_subrouteNum },
                                     { FIELD_SENDER, m_sender }} );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}


