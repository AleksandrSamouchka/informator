#include <csignal>
//#include <iostream>

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QTextStream>
#include <QLockFile>
#include <QDebug>

#include <CConsoleLogger/CConsoleLogger.h>

#include "CMainHandler.h"
#include "CAbstractController.h"

#ifdef Q_OS_WIN32
#   define SIGQUIT SIGBREAK
#endif

static const char *verboseTitle = "--verbose";
static const char *helpTitle = "--help";
static const char *versionTitle = "--version";
static const char *logFormat = "--log-format";
static const char *quietTitle = "--quiet"; 


void printVersion()
{
    QTextStream out(stdout);
    out << QCoreApplication::applicationName() << " " << 
           QCoreApplication::applicationVersion() << ", " << 
           "sdk " << SDK_VERSION <<
           endl;
// http://tetran.ru/SiteContent/Details/15
    out << "Copyright (c) OOO \"Vega-Absolut\", 2018-2019" << endl;
}

int usage(const QString &appName, bool enableOutput, int retCode)
{
    Q_UNUSED(appName)
    if(enableOutput)
    {
        QTextStream out(stdout);
        out << "Usage: " << appName << " <cmd>" << endl;
        out << "    where cmd is: " << endl;
        out << "    " << verboseTitle << "             make application more verbose" << endl;
        out << "    " << quietTitle << "             no any output" << endl;
        out << "    " << versionTitle << "             get application version" << endl;
        out << "    " << logFormat    << "=<format>" << " set log format: 'short'(by default) or 'long'" << endl;
        out << "    " << helpTitle    << "                this message" << endl;
    }
    return retCode;
}


int CMainHandler::parseArgs(bool enableOutput /*= true*/)
{
    int result = 0;
    QString appVersion = QCoreApplication::applicationVersion();
    const QStringList argsList = QCoreApplication::arguments();
    QTextStream out(stdout);

    for(int i=1;i<argsList.size();i++)
    {
        QString arg = argsList.at(i);

        if(arg == helpTitle)
        {
            result = usage(argsList.at(0), enableOutput, -1);
        }
        else if(arg == versionTitle)
        {
            if(enableOutput)
            {
                printVersion();
            }
            result = -1;
        }
        else if(arg == quietTitle)
        {
            CConsoleLogger::setQuiet(true);
        }
        else if(arg == verboseTitle)
        {
            CConsoleLogger::setQuiet(false);
            CConsoleLogger::setVerbose(true);
        }
        else if(arg.startsWith(logFormat))
        {
            QStringList list = arg.split("=");
//qDebug() << list[1];
            if(list[1] == "long")
            {
                CConsoleLogger::setFormat(LF_LONG);
            }    
            else 
            {
                CConsoleLogger::setFormat(LF_SHORT);
            }
        }
        else if(arg == "test_arg")
        {
            result = 0;
        }
        else 
        {
            if(enableOutput)
            {
                out << "Error: Unknown option - " << arg << endl;
            }
            result = usage(argsList.at(0), enableOutput, -1);
        }
    }

    return result;
}

int CMainHandler::runController( CAbstractController * controller )
{
    int result = 0;
    QString tmpDir = QDir::tempPath();

    printVersion();
#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
    QLockFile lockFile(tmpDir + "/" + QCoreApplication::applicationName() + ".lock");
    lockFile.setStaleLockTime(0);

    if(!lockFile.tryLock(100)){
        QTextStream out(stdout);
        qint64 pid;
        QString hostname, appname;
        lockFile.getLockInfo(&pid, &hostname, &appname);
        out << "Application already running with pid " << pid << endl;
        return -1;
    }
#endif
    for ( const auto s : { SIGINT, SIGTERM, SIGABRT, SIGQUIT } )
    {
        std::signal( s, & QCoreApplication::exit );
    }

    controller->attach();

    try
    {
        result = QCoreApplication::exec();
    }
    catch ( ... )
    {
/*<<<<<<< HEAD
* /
        for ( const auto s : { SIGINT, SIGTERM, SIGABRT, SIGQUIT } )
        {
            std::signal( s, &QCoreApplication::exit );
        }
=======
*/
        result = -1;
    }
    if ( result == SIGTERM ||
         result == SIGQUIT )
    {
        result = 0;
    }

    controller->detach();

#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
    lockFile.unlock();
    lockFile.removeStaleLockFile();
#endif

    return result;
}

int CMainHandler::runController( CCoreApplication * app )
{
    int result = 0;
    QString tmpDir = QDir::tempPath();
    QTextStream out(stdout);
//>>>>>>> new_core_application

    printVersion();
#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
    QLockFile lockFile(tmpDir + "/" + QCoreApplication::applicationName() + ".lock");
    lockFile.setStaleLockTime(0);

    if(!lockFile.tryLock(100)){
        qint64 pid;
        QString hostname, appname;
        lockFile.getLockInfo(&pid, &hostname, &appname);
        out << "Application already running with pid " << pid << endl;
        return -1;
    }
#endif
    for ( const auto s : { SIGINT, SIGTERM, SIGABRT, SIGQUIT } )
    {
        std::signal( s, & QCoreApplication::exit );
    }

    if(app->attach() == 0)
    {
        try
        {
            result = QCoreApplication::exec();
        }
        catch ( ... )
        {
            result = -1;
        }
        if ( result == SIGTERM ||
            result == SIGQUIT )
        {
            result = 0;
        }
        app->detach();
    }
    else
    {
        out << "Unable to init application!" << endl;
        result = -1;
    }


#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
    lockFile.unlock();
    lockFile.removeStaleLockFile();
#endif

    return result;
}

void CMainHandler::setApplicationVersion(const char *version, const char *revision /*= nullptr*/)
{
    QString versionStr = version;
    
    if(revision && *revision)
    {
        versionStr.append("(").append(revision).append(")");
    }
    QCoreApplication::setApplicationVersion( versionStr );
}
