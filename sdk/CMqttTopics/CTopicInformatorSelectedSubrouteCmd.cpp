#include "CTopicInformatorSelectedSubrouteCmd.h"

namespace
{
    const char *FIELD_SUBROUTENUM = "Current_subroute";
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/

CTopicInformatorSelectedSubrouteCmd::CTopicInformatorSelectedSubrouteCmd() :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Informator_selectedSubroute),
    m_subrouteNum(-1)
{ }

CTopicInformatorSelectedSubrouteCmd::CTopicInformatorSelectedSubrouteCmd( int subroute ) :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Informator_selectedSubroute),
    m_subrouteNum{ subroute }
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param const CTopicValueEntry
 * @return bool
 ** ***********************************************************************/
bool CTopicInformatorSelectedSubrouteCmd::initFromData(const CTopicValueEntry & value)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_SUBROUTENUM ) )
        {
            m_subrouteNum = jsonMainObj.value( FIELD_SUBROUTENUM ).toInt();
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
QByteArray CTopicInformatorSelectedSubrouteCmd::prepareData() const
{
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ FIELD_SUBROUTENUM, QJsonValue( m_subrouteNum ) } };
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
