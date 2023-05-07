#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>


namespace DB {
//#error "Name of DB table with application settings"
    static const QString SETTINGS_DB_NAME( QString("mntconti_application_settings") );
//#error "Flag, allow handler to create scheme (or database)"
    static const bool CREATE_DB_ENABLE{ false };


}


#endif // GLOBAL_H
