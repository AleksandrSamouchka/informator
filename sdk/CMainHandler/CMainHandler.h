#ifndef __CMAINHANDLER_H__
#define __CMAINHANDLER_H__

#include <QObject>
#include <CMainHandler/CCoreApplication.h>

class CAbstractController;

class CMainHandler
{
public:
    static void setApplicationVersion(const char *version, const char *revision = nullptr);
    static int runController( CAbstractController * controller );
    static int runController( CCoreApplication * app );
    static int parseArgs(bool enableOutput = true);
};

#endif
