#include <QtGlobal>

#include <sys/types.h>

#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
#include <utime.h>
#else
#include <sys/utime.h>
#endif

#include <QSet>
#include <QDirIterator>

#include <CConsoleLogger/CConsoleLogger.h>

#include "CFileUtils.h"


void CFileUtils::removeFilesInDir( const QDir & dir,
                               const QSet<QString> & excludes )
{
    QDirIterator dit{ dir, QDirIterator::Subdirectories };

    while ( dit.hasNext() )
    {
        auto name{ dit.next() };

        if ( name != dit.fileInfo().absoluteFilePath() )
        {
            name = dit.fileInfo().absoluteFilePath();
        }

        if ( CConsoleLogger::isVerbose() )
        {
            PRINT_CONSOLE_MESSAGE_INFO( QString( "Looking local file %1 - %2: %3" )
                                        .arg(name)
                                        .arg(dit.filePath())
                                        .arg(excludes.contains( name )) );
        }

        if ( ! excludes.contains( name ) &&
             ! ( dit.fileInfo().isDir() ?
                 QDir{ name }.removeRecursively() :
                 QFile::remove( name ) ) )
        {

            PRINT_CONSOLE_MESSAGE_WARN( QString( "Can't remove local %1: %2" )
                                        .arg( dit.fileInfo().isDir() ? "dir" : "file" )
                                        .arg(name) );
        }
    }
}

bool CFileUtils::touch(const char *filePath, const QDateTime &time) 
{
    struct utimbuf timebuffer;
    timebuffer.modtime=time.toTime_t();
    
    PRINT_CONSOLE_MESSAGE_INFO(QString("touch %1 to %2")
                                .arg(filePath)
                                .arg(time.toString("dd.MM.yyyy hh:mm:ss")));

    if((utime(filePath,&timebuffer))<0)
    {
        return false;
    }

    return true;
}
