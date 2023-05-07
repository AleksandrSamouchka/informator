#include "CTopicInformatorChangeModeCmd.h"

namespace
{
    const char *FIELD_MODE = "Mode";

    const QStringList MODES_LIST{
        "Auto",
        "Manual"
    };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicInformatorChangeModeCmd::CTopicInformatorChangeModeCmd( const EModes mode ) :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Informator_cmdToChangeMode ),
    m_mode{ mode }
{ }

/** ***********************************************************************
 * @fn prepareData
 * @brief метод формирования значения топика из полей структуры
 * @param none
 * @return QByteArray
 ** ***********************************************************************/
QByteArray CTopicInformatorChangeModeCmd::prepareData()
{
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{
            FIELD_MODE, QJsonValue( MODES_LIST.value( static_cast< int >( m_mode ) ) ) } };
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
