#include "CTopicInformatorBusesList.h"

namespace
{
    const char *FIELD_ROUTES = "Routes";
    const char * FIELD_SUBROUTES = "Subroutes";
    const char * FIELD_BUSNUM = "Num";
    const char * FIELD_BUSNAME = "Name";
}


/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicInformatorBusesList::CTopicInformatorBusesList(const QByteArray &json) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Informator_routesList )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicInformatorBusesList::initFromData( const CTopicValueEntry &value )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_ROUTES ) )
        {
            const QJsonArray jsonArr( jsonMainObj.value( FIELD_ROUTES ).toArray() );
            for ( const auto & i : jsonArr )
            {
                const QJsonObject jsonObj( i.toObject() );
                if ( jsonObj.contains( FIELD_BUSNUM ) && 
                     jsonObj.contains( FIELD_BUSNAME ) &&
                     jsonObj.contains( FIELD_SUBROUTES )
                 )
                {
                    QStringList subroutes{};
                    for ( const auto y : jsonObj.value( FIELD_SUBROUTES ).toArray() )
                    {
                        subroutes.append( y.toString() );
                    }

                    m_routesList.append( SRoute( jsonObj.value( FIELD_BUSNAME ).toString(),
                                                 jsonObj.value( FIELD_BUSNUM ).toString(),
                                                 subroutes ) );
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_WARN( "Внутренний JSON объект содержит неизвестный список полей: " + jsonObj.keys().join( "," ) );
                }
            }
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
QByteArray CTopicInformatorBusesList::prepareData() const
{
    QJsonObject jsonMainObj{};
    QJsonArray jsonArr{};
    for ( const auto & i : m_routesList )
    {
        QJsonObject jsonObj( { { FIELD_BUSNUM, i.m_num },
                               { FIELD_BUSNAME, i.m_name },
                               { FIELD_SUBROUTES, QJsonArray::fromStringList( i.m_subroutes ) } } );
        jsonArr.append( jsonObj );
    }
    jsonMainObj.insert( FIELD_ROUTES, jsonArr );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}





