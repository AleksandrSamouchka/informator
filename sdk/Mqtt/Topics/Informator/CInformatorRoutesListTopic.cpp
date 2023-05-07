#include "CInformatorRoutesListTopic.h"

static const char *SUBTOPICNAME = "ROUTES_LIST";

static const char * FIELD_ROUTES = "Routes";
static const char * FIELD_SUBROUTES = "Subroutes";
static const char * FIELD_BUSNUM = "Num";
static const char * FIELD_BUSNAME = "Name";

CInformatorRoutesListTopic::CInformatorRoutesListTopic() :
    CGrInformatorTopic( SUBTOPICNAME )
{

}

CInformatorRoutesListTopic::CInformatorRoutesListTopic( const QList<CInformatorRoutesListTopic::SRoute> & routesList ) :
    CGrInformatorTopic( SUBTOPICNAME ),
    m_routesList( routesList )
{
    prepareData();
}

CInformatorRoutesListTopic::CInformatorRoutesListTopic( const QByteArray & data ) :
    CGrInformatorTopic( SUBTOPICNAME, data )
{

}

#ifdef MNT_TOPICS_TEST
QByteArray CInformatorRoutesListTopic::getEtalonJson()
{
    QJsonObject jsonMainObj{};
    QJsonArray jsonArr{};
    QStringList stringList{};

    stringList.append( "there" );
    stringList.append( "back" );
    QJsonObject jsonObj( { { FIELD_BUSNUM, "9999" },
                           { FIELD_BUSNAME, "testName" },
                           { FIELD_SUBROUTES, QJsonArray::fromStringList( stringList ) } } );
    jsonArr.append( jsonObj );
    jsonMainObj.insert( FIELD_ROUTES, jsonArr );

    jsonObj = { { FIELD_BUSNUM, "8888" },
              { FIELD_BUSNAME, "testName2" },
              { FIELD_SUBROUTES, QJsonArray::fromStringList( stringList ) } };
    jsonArr.append( jsonObj );
    jsonMainObj.insert( FIELD_ROUTES, jsonArr );

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

void CInformatorRoutesListTopic::setRoutesList(const QList < CInformatorRoutesListTopic::SRoute > & routesList)
{
    m_routesList = routesList;
}

const QByteArray CInformatorRoutesListTopic::prepareData() const 
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
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

bool CInformatorRoutesListTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    m_routesList.clear();
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
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
    return setValidState( res );
}
