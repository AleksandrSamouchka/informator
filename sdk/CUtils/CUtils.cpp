#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <QtNetwork/QNetworkInterface>

#include "CUtils.h"
#include "CConsoleLogger/CConsoleLogger.h"
#include "CLoggerHandler/CLoggerHandler.h"

const int CUtils::dbPollTimeout{ 1234 };
//static int dbPollTimeout
//const QString CUtils::localhost{ QStringLiteral( "localhost" ) };
//const QString CUtils::actionRemoveFile{ QStringLiteral( "remove_file" ) };
static const char *actionRemoveFile = "remove_file";
static const char *actionUpdatePPPDSecrets = "update_pppd_secrets";
//const QString CUtils::actionReboot{ QStringLiteral( "reboot" ) };
//const QString CUtils::actionUpgrade{ QStringLiteral( "upgrade" ) };
//const char CUtils::mSdkVersion{ SDK_VERSION };

QString CUtils::getNotExistFileName(const QString& filename)
{
    static const QString fileTemplate { "%1_%2" };
    quint16 counter { 0U };
    QString ret { fileTemplate.arg(filename).arg(counter) };

    while ( QFile::exists(ret) )
        ret = fileTemplate.arg(filename).arg(++counter);

    return ret;
}

int CUtils::waitTimeout( const bool inSecs )
{
    auto result{ 4444 };

    if ( inSecs )
    {
        result /= 1000;
    }

    return result;
}

bool CUtils::runAsRoot( const char *action)
{
    return runAsRoot(action, QStringList{}, nullptr);
}

bool CUtils::runAsRoot( const QString &action)
{
    return runAsRoot(action.toLatin1().data(), QStringList{}, nullptr);
}

bool CUtils::runAsRoot( const char *action, QString *error)
{
    return runAsRoot(action, QStringList{}, error);
}

bool CUtils::runAsRoot( const char *action, const QStringList &args)
{
    return runAsRoot(action, args, nullptr);
}

bool CUtils::runAsRoot( const char *action,
                        const QStringList &args,
                        QString *error )
{
//    static const QChar prefix{ QLatin1Char{ '.' } };
    const char prefix = '.';        
//    static const QString jobFileName{ QStringLiteral( "user2root.job_%1" ) };
//    const char *jobFileName = "user2root.job_%1";    
    static const QHash<QString, QStringList> actionArgNames{
            { actionRemoveFile, QStringList{ QStringLiteral( "path=" ) } },
            { actionUpdatePPPDSecrets, QStringList { QStringLiteral( "line=" ) } },
            { "run_script", QStringList { QStringLiteral( "script=" ), QStringLiteral( "script_args=" ) } }
        };
    auto result{ false };
    QString fileName(QString("user2root.job_%1").arg( QDateTime::currentMSecsSinceEpoch() ));

    QFile file{ QDir::temp().absoluteFilePath( prefix + fileName ) };
    QTextStream text{ & file };

    if ( file.open( QIODevice::WriteOnly ) )
    {
        text << "USER2ROOT_ACTION=" << action << '\n';

        if ( actionArgNames.contains( action ) )
        {
            auto it{ actionArgNames[ action ].begin() };
            const auto end{ actionArgNames[ action ].end() };

            for ( const auto & arg : args )
            {
                text << "USER2ROOT_ARG_";
                if ( it != end )
                {
                    text << * it;
                    ++it;
                }
                text << arg << '\n';
            }
        }

        text.flush();
        file.flush();
        file.close();
        fileName = QDir::temp().absoluteFilePath(fileName);
        if ( CConsoleLogger::isVerbose() )
            PRINT_LOG_MESSAGE_INFO( QString("Переименование %1 в %2")
                                    .arg(file.fileName())
                                    .arg(fileName));
        if ( QFile::exists(fileName) )
        {
            PRINT_LOG_MESSAGE_WARN( QString("Файл %1 уже существует! Переименуем его...").arg(fileName));
            fileName = CUtils::getNotExistFileName(fileName);
            PRINT_LOG_MESSAGE_WARN( QString("Новое имя: %1").arg(fileName));
        }

        result = file.rename(fileName);
    }

    if ( ! result &&
         error != nullptr )
    {
        * error = file.errorString();
    }

    return result;
}

QStringList CUtils::getLocalIpAddresses()
{
    QStringList result{};
    for ( const auto & iface : QNetworkInterface::allInterfaces() )
    {
        if ( ! iface.flags().testFlag( QNetworkInterface::IsLoopBack ) )
        {
            for ( const auto & address : iface.addressEntries() )
            {
                result.append( address.ip().toString() );
            }
        }
    }
    return result;
}

const char *CUtils::sdkVersion()
{
    return SDK_VERSION/*mSdkVersion*/;
}
