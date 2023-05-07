#ifndef __CFILE_UTILS_H__
#define __CFILE_UTILS_H__

#include <QDir>


class CFileUtils 
{
    public:
        static void removeFilesInDir( const QDir & dir,
                                  const QSet<QString> & excludes );

        static bool touch(const char *filePath, const QDateTime &time);
};

#endif
