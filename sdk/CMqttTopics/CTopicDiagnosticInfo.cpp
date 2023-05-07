#include "CTopicDiagnosticInfo.h"

namespace
{
    const char *DIAGNOSTIC_FIELD = "Diagnostic";
}

/** ***********************************************************************
 * @fn Конструктор
 ** ***********************************************************************/
CTopicDiagnosticInfo::CTopicDiagnosticInfo(const QStringList &subsystemNames, 
                                            const QStringList &humanReadableNames)
{
    if ( subsystemNames.size() != humanReadableNames.size() )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Несовпадение размеров списка подсистем и их наименований" );
    }
    for ( int i{ 0 }; i < subsystemNames.size(); i++ )
    {
        const QString subsystemName{ subsystemNames.at( i ) };
        if ( ! mSubsystems.contains( subsystemName ) )
        {
            mSubsystems.insert( subsystemName,
                                CDiagnosticEntryInfo( subsystemName, humanReadableNames.at( i ) ) );
        }
        else
        {
            PRINT_CONSOLE_MESSAGE_WARN( "Повторное наименование подсистемы: " + subsystemName );
        }
    }
}

/** ***********************************************************************
 * @fn prepareData
 ** ***********************************************************************/
QByteArray CTopicDiagnosticInfo::prepareData()
{
    QJsonObject jsonMainObj{};
    QJsonArray jsonArr{};
    for ( const auto & i : mSubsystems )
    {
        jsonArr.append( i.prepareJson() );
    }
    jsonMainObj.insert( DIAGNOSTIC_FIELD, jsonArr );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    mIsChanged = false;
    return jsonData;
}

/** ***********************************************************************
 * @fn initFromData
 ** ***********************************************************************/
bool CTopicDiagnosticInfo::initFromData(const CTopicValueEntry &value)
{
    Q_UNUSED( value )
    return false;
}

/** ***********************************************************************
 * @fn changeState
 ** ***********************************************************************/
bool CTopicDiagnosticInfo::changeState( const QString &subsystem, const CDiagnosticEntryInfo::EState state )
{
    if ( mSubsystems.contains( subsystem ) )
    {
        CDiagnosticEntryInfo info{ mSubsystems.value( subsystem ) };
        if ( info.mState != state )
        {
            info.mState = state;
            mIsChanged = true;
            mSubsystems.insert( subsystem, info );
        }
    }
    else
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Неизвестное наименование подсистемы: " + subsystem );
    }
    return mIsChanged;
}

/** ***********************************************************************
 * @fn changeStatus
 ** ***********************************************************************/
bool CTopicDiagnosticInfo::changeStatus( const QString &subsystem, const CDiagnosticEntryInfo::EStatus status )
{
    if ( mSubsystems.contains( subsystem ) )
    {
        CDiagnosticEntryInfo info{ mSubsystems.value( subsystem ) };
        if ( info.mStatus != status )
        {
            info.mStatus = status;
            mIsChanged = true;
            mSubsystems.insert( subsystem, info );
        }
    }
    else
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Неизвестное наименование подсистемы: " + subsystem );
    }
    return mIsChanged;
}

/** ***********************************************************************
 * @fn isChanges
 ** ***********************************************************************/
bool CTopicDiagnosticInfo::isChanges() const
{
    return mIsChanged;
}
