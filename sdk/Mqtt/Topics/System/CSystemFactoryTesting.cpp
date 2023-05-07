#include "CSystemFactoryTesting.h"

CSystemFactoryTesting::CSystemFactoryTesting() :
    CSimpleTopic<int>("SYSTEM/FACTORY_TESTING")
{
}

CSystemFactoryTesting::~CSystemFactoryTesting()
{
}
/*
bool CSystemFactoryTesting::parseData( const QByteArray & data )
{
    int value{ -1 };
    bool result{ false };
    QJsonParseError jsonErr{};
    QJsonObject jsonObj( QJsonDocument::fromJson( data, & jsonErr ).object() );
    if ( jsonErr.error != QJsonParseError::NoError )
    {
        printJsonParseErrorMessage( data, jsonErr );
    }
    else
    {
        if ( ! jsonObj.contains( "Value" ) )
        {
            printUnknownFieldsListMessage( jsonObj.keys() );
        }
        else
        {
            value = jsonObj.value( FIELD_VALUE ).toInt( -1 );
            result = true;
        }
    }

    setValidState( result );
    if ( res != nullptr )
    {
        *res = result;
    }
    return value;

}
*/