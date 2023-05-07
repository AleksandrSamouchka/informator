// Qt
#include <QDir>
#include <QtCore/QCoreApplication>
#include <QMqttTopicFilter>

// SDK
#include <CMainHandler/CMainHandler.h>

// Local
#include "informatorhandler.h"

Q_DECLARE_METATYPE(RouteFullInfo_t);

int main(int argc, char *argv[])
{
    int retCode = 0;
    QCoreApplication app(argc, argv);
    CMainHandler::setApplicationVersion( APP_VERSION, APP_REVISION );
    retCode = CMainHandler::parseArgs();

    qRegisterMetaType<QVector<RouteFullInfo_t>>();
    qRegisterMetaType<QMqttTopicFilter>();
    //qRegisterMetaType<QMqttMessage>();

    QDir::setCurrent("/home/mntconti/informator");

    if(retCode != 0)
    {
        return retCode;
    }
    return CMainHandler::runController( new InformatorHandler(& app) );
}
