#include "CDiagnosticEntryInfo.h"

namespace
{
    const QString STATE_FIELD{ QString::fromLatin1( "State" ) };
    const QString STATUS_FIELD{ QString::fromLatin1( "Status" ) };
    const QString NAME_FIELD{ QString::fromLatin1( "Subsystem_name" ) };
    const QString MESSAGE_FIELD{ QString::fromLatin1( "Subsystem_message" ) };
    const QString HUMAN_NAME_FIELD{ QString::fromLatin1( "Subsystem_human_name:" ) };

    const QStringList STATE_LIST{
        QString::fromLatin1( "OK" ),
        QString::fromLatin1( "ERROR" ),
        QString::fromLatin1( "WARNING" ),
        QString::fromLatin1( "UNKNOWN" ),
    };

    const QStringList STATUS_LIST{
        QString::fromLatin1( "READY" ),
        QString::fromLatin1( "WAITING" ),
        QString::fromLatin1( "NEED_ACTION" ),
    };

    QString getStateString( const CDiagnosticEntryInfo::EState state )
    {
        return STATE_LIST.value( static_cast< int >( state ), "ERROR" );
    }

    QString getStatusString( const CDiagnosticEntryInfo::EStatus status )
    {
        return STATUS_LIST.value( static_cast< int >( status ), "WAITING" );
    }
}

/** ***********************************************************************
 * @fn Конструктор
 ** ***********************************************************************/
CDiagnosticEntryInfo::CDiagnosticEntryInfo(const QString name , const QString humanName) :
    mName{ name },
    mHumanName{ humanName }
{ }

/** ***********************************************************************
 * @fn Конструктор
 ** ***********************************************************************/
CDiagnosticEntryInfo::CDiagnosticEntryInfo( const QJsonObject json )
{
    Q_UNUSED( json )
}

/** ***********************************************************************
 * @fn prepareJson
 ** ***********************************************************************/
QJsonObject CDiagnosticEntryInfo::prepareJson() const
{
    QJsonObject jsonObj{};
    jsonObj.insert( STATE_FIELD, getStateString( mState ) );
    jsonObj.insert( STATUS_FIELD, getStatusString( mStatus ) );
    jsonObj.insert( NAME_FIELD, mName );
    jsonObj.insert( MESSAGE_FIELD, mMessage );
    jsonObj.insert( HUMAN_NAME_FIELD, mHumanName );
    return jsonObj;
}


