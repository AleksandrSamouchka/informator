#pragma once

#include <QtCore/QStringList>
#include <memory>

#include <CUtils/CFileUtils.h>

class QDir;
class QStringList;

class CUtils
{
private:
    static QString getNotExistFileName(const QString& filename);
public:
    static const int dbPollTimeout;
//    static const QString localhost;
//    static const QString actionRemoveFile;
//    static const QString actionReboot;
//    static const QString actionUpgrade;
//    static const QString mSdkVersion;

    template<typename T, typename D = void(*)(T*)>
    static std::unique_ptr<T, D> makeUniquePtr( T * p,
                                                const D d )
    {
        return std::move( std::unique_ptr<T, D>{ p, d } );
    }

    static int waitTimeout( const bool inSecs = false );
    static bool runAsRoot( const char *action);
    static bool runAsRoot( const QString &action);
    static bool runAsRoot( const char *action, QString *error);
    static bool runAsRoot( const char *action, const QStringList &args);
    static bool runAsRoot( const char *action, const QStringList &args, QString *error);
/*    static bool runAsRoot( const char *action,
                           const QStringList args = QStringList{},
                           QString &error);*/
    static QStringList getLocalIpAddresses();
    static const char *sdkVersion();

};
