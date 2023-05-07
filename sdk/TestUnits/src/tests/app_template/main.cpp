#include <QtCore/QCoreApplication>

#include <CMainHandler.h>

#include "AppController.hpp"


int main( int argc,
          char * argv[] )
{
    int retCode = 0;
    QCoreApplication app{ argc, argv };
    QCoreApplication::setApplicationVersion( APP_VERSION );
    retCode = CMainHandler::parseArgs();
    if(retCode != 0)
    {
        return retCode;
    }
    return CMainHandler::runController( new AppController{ & app } );

/*
    QCoreApplication app{ argc, argv };
    app.setApplicationVersion( QStringLiteral( APP_VERSION ) );
    prctl( PR_SET_PDEATHSIG, SIGTERM );
    return CMainHandler::runController( new AppController{ & app } );
*/
}

