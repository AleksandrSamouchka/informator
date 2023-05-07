#include "CTopicInformatorSelectedBusCmd.h"

namespace
{
    const char *FIELD_BUSNUM = "Bus_num";
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicInformatorSelectedBusCmd::CTopicInformatorSelectedBusCmd( int busNum ) :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Informator_selectedBus ),
    m_busNum{ busNum }
{ }

/** ***********************************************************************
 * @fn prepareData
 * @brief метод формирования значения топика из полей структуры
 * @param none
 * @return QByteArray
 ** ***********************************************************************/
QByteArray CTopicInformatorSelectedBusCmd::prepareData()
{
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ FIELD_BUSNUM, QJsonValue( m_busNum ) } };
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
