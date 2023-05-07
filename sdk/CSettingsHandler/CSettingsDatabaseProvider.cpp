#include "CSettingsDatabaseProvider.h"
#include <CConsoleLogger/CConsoleLogger.h>
#include <QtCore/QTimer>
#include <QtCore/QSemaphore>
#include <QtCore/QThread>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QVariant>

static const char *DB_SETTINGS_TABLE_NAME = "mntconti_settings";
static const char *DbTableSettings[] = 
{
    "parameter",
    "human_name",
    "value",
    "type",
    "module",
    "description",
    "is_private",
    "subgroup_name",
    "subgroup_human_name"
};
static const char *SETTINGS_DB_BASE_CONNECTION_NAME = "settingsDb";

/** ***********************************************************************
 * @brief Класс по работе с настрйоками приложения, хранящимися в БД
 ** ***********************************************************************/
CSettingsDatabaseProvider::CSettingsDatabaseProvider(QObject * parent) :
    CAbstractSettingsProvider( parent )
{ }

/** ***********************************************************************
 * @fn getSettingsTotalListSlot
 * @brief приватный слот для получения из БД полного списка настроек
 * @param none
 * @return none
 ** ***********************************************************************/
QList < CSettingsEntry > 
CSettingsDatabaseProvider::getSettingsTotalListSlot(const QString &module, bool showPrivate )
{
    QList < CSettingsEntry > settingsList{};
    if ( m_database.isNull() )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Объект БД не проинициализирован или был удален извне!" );
    }
    else
    {
        if ( ! m_database.data()->isDbActive() )
        {
            PRINT_CONSOLE_MESSAGE_WARN( "БД не подключена, запрос проигнорирован" );
        }
        else
        {
            QSqlQuery query( m_database.data()->getQuery() );
            const QString queryStr( QString( "SELECT %2, %3, %4, %5, %6, %7, %8, %9, %10 FROM %1 WHERE %6='%11'%12;" )
                                    .arg( DB_SETTINGS_TABLE_NAME )
                                    .arg( DbTableSettings[TableSettingsColumn_parameter] )     /*! 2 */
                                    .arg( DbTableSettings[TableSettingsColumn_humanName] )
                                    .arg( DbTableSettings[TableSettingsColumn_value])
                                    .arg( DbTableSettings[TableSettingsColumn_type])          /*! 5 */
                                    .arg( DbTableSettings[TableSettingsColumn_module])
                                    .arg( DbTableSettings[TableSettingsColumn_description])
                                    .arg( DbTableSettings[TableSettingsColumn_private])       /*! 8 */
                                    .arg( DbTableSettings[TableSettingsColumn_subgroupName])
                                    .arg( DbTableSettings[TableSettingsColumn_subgroupHumanName])
                                    .arg( module )
                                    .arg( showPrivate ? "" : QString( " AND %1=0" ).arg( DbTableSettings[TableSettingsColumn_private] ) ) );
            if ( ! query.exec( queryStr ) )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( QString( "Невозможно выполнить SQL запрос: запрос[%1], ошибка[%2]" )
                                   .arg( queryStr )
                                   .arg( query.lastError().text() ) );
                emit m_database.data()->invalidDb( );
            }
            else
            {
                while ( query.next() )
                {
                    settingsList.append( CSettingsEntry( query.value( TableSettingsColumn_parameter ).toString(),
                                                         query.value( TableSettingsColumn_humanName ).toString(),
                                                         query.value( TableSettingsColumn_value ).toString(),
                                                         query.value( TableSettingsColumn_type ).toString(),
                                                         query.value( TableSettingsColumn_description ).toString(),
                                                         query.value( TableSettingsColumn_module ).toString(),
                                                         query.value( TableSettingsColumn_private ).toInt() > 0,
                                                         query.value( TableSettingsColumn_subgroupName ).toString(),
                                                         query.value( TableSettingsColumn_subgroupHumanName ).toString()
                                                         ) );
                }
            }
        }
    }
    emit getSettingsTotalListResult( settingsList/*, QPrivateSignal{} */);
    return settingsList;
}

/** ***********************************************************************
 * @fn getSettingSlot
 * @brief приватный слот для получения из БД настройки с указанным наименованием
 * @param QString
 * @return none
 ** ***********************************************************************/
CSettingsEntry 
CSettingsDatabaseProvider::getSettingSlot(const QString &name, 
                                          const QString &module, 
                                          bool showPrivate)
{
    CSettingsEntry setting{};
    if ( m_database.isNull() )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Объект БД не проинициализирован или был удален извне!" );
    }
    else
    {
        if ( ! m_database.data()->isDbActive() )
        {
            PRINT_CONSOLE_MESSAGE_WARN( "БД не подключена, запрос проигнорирован" );
        }
        else
        {
            QSqlQuery query( m_database.data()->getQuery() );
            const QString queryStr( QString( "SELECT %2, %3, %4, %5, %6, %7, %8, %9, %10 FROM %1 WHERE %6='%11' AND %12='%13'%14;" )
                                    .arg( DB_SETTINGS_TABLE_NAME )
                                    .arg( DbTableSettings[TableSettingsColumn_parameter] )     /*! 2 */
                                    .arg( DbTableSettings[TableSettingsColumn_humanName] )
                                    .arg( DbTableSettings[TableSettingsColumn_value] )
                                    .arg( DbTableSettings[TableSettingsColumn_type] )          /*! 5 */
                                    .arg( DbTableSettings[TableSettingsColumn_module] )
                                    .arg( DbTableSettings[TableSettingsColumn_description] )
                                    .arg( DbTableSettings[TableSettingsColumn_private] )       /*! 8 */
                                    .arg( DbTableSettings[TableSettingsColumn_subgroupName] )
                                    .arg( DbTableSettings[TableSettingsColumn_subgroupHumanName] )
                                    .arg( module )
                                    .arg( DbTableSettings[ TableSettingsColumn_parameter ] )     /*! 12 */
                                    .arg( name )
                                    .arg( showPrivate ? "" : QString( " AND %1=0" ).arg( DbTableSettings[ TableSettingsColumn_private ] ) ) );
            if ( ! query.exec( queryStr ) )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( QString( "Невозможно выполнить SQL запрос: запрос[%1], ошибка[%2]" )
                                   .arg( queryStr )
                                   .arg( query.lastError().text() ) );
                emit m_database.data()->invalidDb( );
            }
            else
            {
                if ( query.next() )
                {
                    setting.setName( query.value( TableSettingsColumn_parameter ).toString() );
                    setting.setHumanName( query.value( TableSettingsColumn_humanName ).toString() );
                    setting.setValue( query.value( TableSettingsColumn_value ).toString() );
                    setting.setType( query.value( TableSettingsColumn_type ).toString() );
                    setting.setDescription( query.value( TableSettingsColumn_description ).toString() );
                    setting.setModule( query.value( TableSettingsColumn_module ).toString() );
                    setting.setPrivate( query.value( TableSettingsColumn_private ).toInt() > 0 );
                    setting.setSubgroupName( query.value( TableSettingsColumn_subgroupName ).toString() );
                    setting.setSubgroupHumanName( query.value( TableSettingsColumn_subgroupHumanName ).toString() );
                }
            }
        }
    }
    emit getSettingResult( setting/*, QPrivateSignal{} */);
    return setting;
}

/** ***********************************************************************
 * @fn setSettingSlot
 * @brief приватный слот для записи настрйоки в БД
 * @param CSettingsEntry
 * @return none
 ** ***********************************************************************/
bool CSettingsDatabaseProvider::setSettingSlot(const CSettingsEntry &setting , bool override)
{
    bool res{ false };
    if ( m_database.isNull() )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Объект БД не проинициализирован или был удален извне!" );
    }
    else
    {
        if ( ! m_database.data()->isDbActive() )
        {
            PRINT_CONSOLE_MESSAGE_WARN( "БД не подключена, запрос проигнорирован" );
        }
        else
        {
            QSqlQuery query( m_database.data()->getQuery() );
            QString queryStr( QString( "SELECT %2 FROM %1 WHERE %3='%4' AND %2='%5';" )
                              .arg( DB_SETTINGS_TABLE_NAME )
                              .arg( DbTableSettings[ TableSettingsColumn_parameter] )
                              .arg( DbTableSettings[ TableSettingsColumn_module] )
                              .arg( setting.getModule() )
                              .arg( setting.getName() ) );
            if ( ! query.exec( queryStr ) )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( QString( "Невозможно выполнить SQL запрос: запрос[%1], ошибка[%2]" )
                                   .arg( queryStr )
                                   .arg( query.lastError().text() ) );
                emit m_database.data()->invalidDb( );
            }
            else
            {
                if ( query.next() && ! query.value( 0 ).toString().isEmpty() )
                {
                    if ( override )
                    {
                    /*! Обновляем настройку */
                        queryStr = QString( "UPDATE %1 SET %2=?, %3=?, %4=?, %5=?, %6=?, %7=?, %8=? WHERE %9='%11' AND %10='%12';" )
                                .arg( DB_SETTINGS_TABLE_NAME )
                                .arg( DbTableSettings[ TableSettingsColumn_humanName ] )
                                .arg( DbTableSettings[ TableSettingsColumn_value ] )
                                .arg( DbTableSettings[ TableSettingsColumn_description ] )
                                .arg( DbTableSettings[ TableSettingsColumn_private ] )
                                .arg( DbTableSettings[ TableSettingsColumn_subgroupName ] )
                                .arg( DbTableSettings[ TableSettingsColumn_subgroupHumanName ] ) /*!< 7 */
                                .arg( DbTableSettings[ TableSettingsColumn_type ] )
                                .arg( DbTableSettings[ TableSettingsColumn_parameter ] )
                                .arg( DbTableSettings[ TableSettingsColumn_module ] )
                                .arg( setting.getName() )
                                .arg( setting.getModule() );
                        if ( query.prepare( queryStr ) )
                        {
                            query.addBindValue( setting.getHumanName() );
                            query.addBindValue( setting.getValue() );
                            query.addBindValue( setting.getDescription() );
                            query.addBindValue( setting.isPrivate() ? 1 : 0 );
                            query.addBindValue( setting.getSubgroupName() );
                            query.addBindValue( setting.getSubgroupHumanName() );
                            query.addBindValue( setting.getType() );
                            if ( ! query.exec() )
                            {
                                PRINT_CONSOLE_MESSAGE_ERROR( QString( "Невозможно выполнить SQL запрос: запрос[%1], ошибка[%2]" )
                                                   .arg( queryStr )
                                                   .arg( query.lastError().text() ) );
                                emit m_database.data()->invalidDb( );
                            }
                            else
                            {
                                res = true;
                            }
                        }
                        else
                        {
                            PRINT_CONSOLE_MESSAGE_ERROR( QString( "Некорректная строка SQL запрос: запрос[%1], ошибка[%2]" )
                                               .arg( queryStr )
                                               .arg( query.lastError().text() ) );
                        }
                    }
                    else
                    {
                    /*! Обновляем все, кроме значения */
                        queryStr = QString( "UPDATE %1 SET %2=?, %3=?, %4=?, %5=?, %6=?, %7=? WHERE %8='%10' AND %9='%11';" )
                                .arg( DB_SETTINGS_TABLE_NAME )
                                .arg( DbTableSettings[ TableSettingsColumn_humanName ] )
                                .arg( DbTableSettings[ TableSettingsColumn_description ] )
                                .arg( DbTableSettings[ TableSettingsColumn_private ] )
                                .arg( DbTableSettings[ TableSettingsColumn_subgroupName ] )
                                .arg( DbTableSettings[ TableSettingsColumn_subgroupHumanName ] ) /*!< 6 */
                                .arg( DbTableSettings[ TableSettingsColumn_type ] )
                                .arg( DbTableSettings[ TableSettingsColumn_parameter ] )
                                .arg( DbTableSettings[ TableSettingsColumn_module ] )
                                .arg( setting.getName() )
                                .arg( setting.getModule() );
                        if ( query.prepare( queryStr ) )
                        {
                            query.addBindValue( setting.getHumanName() );
                            query.addBindValue( setting.getDescription() );
                            query.addBindValue( setting.isPrivate() ? 1 : 0 );
                            query.addBindValue( setting.getSubgroupName() );
                            query.addBindValue( setting.getSubgroupHumanName() );
                            query.addBindValue( setting.getType() );
                            if ( ! query.exec() )
                            {
                                PRINT_CONSOLE_MESSAGE_ERROR( QString( "Невозможно выполнить SQL запрос: запрос[%1], ошибка[%2]" )
                                                   .arg( queryStr )
                                                   .arg( query.lastError().text() ) );
                                emit m_database.data()->invalidDb( );
                            }
                            else
                            {
                                res = true;
                            }
                        }
                        else
                        {
                            PRINT_CONSOLE_MESSAGE_ERROR( QString( "Некорректная строка SQL запрос: запрос[%1], ошибка[%2]" )
                                               .arg( queryStr )
                                               .arg( query.lastError().text() ) );
                        }
                    }
                }
                else
                {
                /*! Создаем запись настройки */
                    queryStr = QString( "INSERT INTO %1 ( %2, %3, %4, %5, %6, %7, %8, %9, %10 ) "
                                        "VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ? );" )
                            .arg( DB_SETTINGS_TABLE_NAME )
                            .arg( DbTableSettings[ TableSettingsColumn_parameter ] )
                            .arg( DbTableSettings[ TableSettingsColumn_humanName ] )
                            .arg( DbTableSettings[ TableSettingsColumn_value ] )
                            .arg( DbTableSettings[ TableSettingsColumn_module ] )
                            .arg( DbTableSettings[ TableSettingsColumn_description ] )
                            .arg( DbTableSettings[ TableSettingsColumn_private ] )
                            .arg( DbTableSettings[ TableSettingsColumn_subgroupName ] )
                            .arg( DbTableSettings[ TableSettingsColumn_subgroupHumanName ] ) /*!< 9 */
                            .arg( DbTableSettings[ TableSettingsColumn_type ] );
                    if ( query.prepare( queryStr ) )
                    {
                        query.addBindValue( setting.getName() );
                        query.addBindValue( setting.getHumanName() );
                        query.addBindValue( setting.getValue() );
                        query.addBindValue( setting.getModule() );
                        query.addBindValue( setting.getDescription() );
                        query.addBindValue( setting.isPrivate() ? 1 : 0 );
                        query.addBindValue( setting.getSubgroupName() );
                        query.addBindValue( setting.getSubgroupHumanName() );
                        query.addBindValue( setting.getType() );
                        if ( ! query.exec() )
                        {
                            PRINT_CONSOLE_MESSAGE_ERROR( QString( "Невозможно выполнить SQL запрос: запрос[%1], ошибка[%2]" )
                                               .arg( queryStr )
                                               .arg( query.lastError().text() ) );
                            emit m_database.data()->invalidDb( );
                        }
                        else
                        {
                            res = true;
                        }
                    }
                    else
                    {
                        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Некорректная строка SQL запрос: запрос[%1], ошибка[%2]" )
                                           .arg( queryStr )
                                           .arg( query.lastError().text() ) );
                    }
                }
            }
        }
    }
    emit setSettingResult( res/*, QPrivateSignal{} */);
    return res;
}

/** ***********************************************************************
 * @fn setSettingsListSlot
 * @brief приватный слот для записи списка настроек в БД
 * @param QList < CSettingsEntry >
 * @return none
 ** ***********************************************************************/
bool CSettingsDatabaseProvider::setSettingsListSlot(const QList<CSettingsEntry> &settingsList, 
                                                    bool override)
{
    bool res{ true };
    if ( m_database.isNull() )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Объект БД не проинициализирован или был удален извне!" );
        res = false;
    }
    else
    {
        if ( ! m_database.data()->isDbActive() )
        {
            PRINT_CONSOLE_MESSAGE_WARN( "БД не подключена, запрос проигнорирован" );
            res = false;
        }
        else
        {
            QSqlQuery query( m_database.data()->getQuery() );
            QString queryStr( QString( "SELECT %2, %3 FROM %1 WHERE " )
                              .arg( DB_SETTINGS_TABLE_NAME )
                              .arg( DbTableSettings[ TableSettingsColumn_module ] )      /*!< 2, val-0 */
                              .arg( DbTableSettings[ TableSettingsColumn_parameter ] ) );/*!< 3, val-1 */
            for ( const auto & i : settingsList )
            {
                if ( ! i.isValid() )
                {
                    continue;
                }
                queryStr += QString( "( %1='%2' AND %3='%4' ) OR " )
                        .arg( DbTableSettings[ TableSettingsColumn_module ] )
                        .arg( i.getModule() )
                        .arg( DbTableSettings[ TableSettingsColumn_parameter] )
                        .arg( i.getName() );
            }
            queryStr.remove( queryStr.size() - 4, 4 );
            queryStr += " ;";
            if ( ! query.exec( queryStr ) )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( QString( "Невозможно выполнить SQL запрос: запрос[%1], ошибка[%2]" )
                                   .arg( queryStr )
                                   .arg( query.lastError().text() ) );
                emit m_database.data()->invalidDb( );
                res = false;
            }
            else
            {
                QMap < QString, QStringList > existParamMap{};
                QList < CSettingsEntry > insertSettingsList{};
                while ( query.next() )
                {
                    const QString module{ query.value( 0 ).toString() };
                    const QString parameter{ query.value( 1 ).toString() };
                    QStringList parameters{ existParamMap.value( module, QStringList{} ) };
                    if ( parameters.contains( parameter ) )
                    {
                        PRINT_CONSOLE_MESSAGE_WARN( tr( "Дублирование параметра %1 для модуля %2" )
                                                    .arg( parameter )
                                                    .arg( module ) );
                        continue;
                    }
                    parameters.append( parameter );
                    existParamMap.insert( module, parameters );
                }
                for ( const auto & i : settingsList )
                {
                    if ( ! i.isValid() )
                    {
                        continue;
                    }
                    const QStringList parameters{ existParamMap.value( i.getModule() ) };
                    if ( parameters.contains( i.getName() ) )
                    {
                        if ( override )
                        {
                        /*! Обновляем настройку */
                            queryStr = QString( "UPDATE %1 SET %2=?, %3=?, %4=?, %5=?, %6=?, %7=?, %8=? WHERE %9='%11' AND %10='%12';" )
                                    .arg( DB_SETTINGS_TABLE_NAME )
                                    .arg( DbTableSettings[ TableSettingsColumn_humanName ] )
                                    .arg( DbTableSettings[ TableSettingsColumn_value ] )
                                    .arg( DbTableSettings[ TableSettingsColumn_description ] )
                                    .arg( DbTableSettings[ TableSettingsColumn_private ] )
                                    .arg( DbTableSettings[ TableSettingsColumn_subgroupName ] )
                                    .arg( DbTableSettings[ TableSettingsColumn_subgroupHumanName ] ) /*!< 7 */
                                    .arg( DbTableSettings[ TableSettingsColumn_type ] )
                                    .arg( DbTableSettings[ TableSettingsColumn_parameter ] )
                                    .arg( DbTableSettings[ TableSettingsColumn_module ] )
                                    .arg( i.getName() )
                                    .arg( i.getModule() );
                            if ( query.prepare( queryStr ) )
                            {
                                query.addBindValue( i.getHumanName() );
                                query.addBindValue( i.getValue() );
                                query.addBindValue( i.getDescription() );
                                query.addBindValue( i.isPrivate() ? 1 : 0 );
                                query.addBindValue( i.getSubgroupName() );
                                query.addBindValue( i.getSubgroupHumanName() );
                                query.addBindValue( i.getType() );
                                if ( ! query.exec() )
                                {
                                    PRINT_CONSOLE_MESSAGE_ERROR( QString( "Невозможно выполнить SQL запрос: запрос[%1], ошибка[%2]" )
                                                       .arg( queryStr )
                                                       .arg( query.lastError().text() ) );
                                    emit m_database.data()->invalidDb( );
                                }
                                else
                                {
                                    res = true;
                                }
                            }
                            else
                            {
                                PRINT_CONSOLE_MESSAGE_ERROR( QString( "Некорректная строка SQL запрос: запрос[%1], ошибка[%2]" )
                                                   .arg( queryStr )
                                                   .arg( query.lastError().text() ) );
                            }
                        }
                        else
                        {
                        /*! Обновляем все, кроме значения */
                            queryStr = QString( "UPDATE %1 SET %2=?, %3=?, %4=?, %5=?, %6=?, %7=? WHERE %8='%10' AND %9='%11';" )
                                    .arg( DB_SETTINGS_TABLE_NAME )
                                    .arg( DbTableSettings[ TableSettingsColumn_humanName ] )
                                    .arg( DbTableSettings[ TableSettingsColumn_description ] )
                                    .arg( DbTableSettings[ TableSettingsColumn_private ] )
                                    .arg( DbTableSettings[ TableSettingsColumn_subgroupName ] )
                                    .arg( DbTableSettings[ TableSettingsColumn_subgroupHumanName ] ) /*!< 6 */
                                    .arg( DbTableSettings[ TableSettingsColumn_type ] )
                                    .arg( DbTableSettings[ TableSettingsColumn_parameter ] )
                                    .arg( DbTableSettings[ TableSettingsColumn_module ] )
                                    .arg( i.getName() )
                                    .arg( i.getModule() );
                            if ( query.prepare( queryStr ) )
                            {
                                query.addBindValue( i.getHumanName() );
                                query.addBindValue( i.getDescription() );
                                query.addBindValue( i.isPrivate() ? 1 : 0 );
                                query.addBindValue( i.getSubgroupName() );
                                query.addBindValue( i.getSubgroupHumanName() );
                                query.addBindValue( i.getType() );
                                if ( ! query.exec() )
                                {
                                    PRINT_CONSOLE_MESSAGE_ERROR( QString( "Невозможно выполнить SQL запрос: запрос[%1], ошибка[%2]" )
                                                       .arg( queryStr )
                                                       .arg( query.lastError().text() ) );
                                    emit m_database.data()->invalidDb( );
                                }
                                else
                                {
                                    res = true;
                                }
                            }
                            else
                            {
                                PRINT_CONSOLE_MESSAGE_ERROR( QString( "Некорректная строка SQL запрос: запрос[%1], ошибка[%2]" )
                                                   .arg( queryStr )
                                                   .arg( query.lastError().text() ) );
                            }
                        }
                    }
                    else
                    {
                        queryStr = QString( "INSERT INTO %1 ( %2, %3, %4, %5, %6, %7, %8, %9, %10 ) "
                                            "VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ? );" )
                                .arg( DB_SETTINGS_TABLE_NAME )
                                .arg( DbTableSettings[ TableSettingsColumn_parameter ] )
                                .arg( DbTableSettings[ TableSettingsColumn_humanName ] )
                                .arg( DbTableSettings[ TableSettingsColumn_value ] )
                                .arg( DbTableSettings[ TableSettingsColumn_module ] )
                                .arg( DbTableSettings[ TableSettingsColumn_description ] )
                                .arg( DbTableSettings[ TableSettingsColumn_private ] )
                                .arg( DbTableSettings[ TableSettingsColumn_subgroupName ] )
                                .arg( DbTableSettings[ TableSettingsColumn_subgroupHumanName ] ) /*!< 9 */
                                .arg( DbTableSettings[ TableSettingsColumn_type ] );
                        if ( query.prepare( queryStr ) )
                        {
                            query.addBindValue( i.getName() );
                            query.addBindValue( i.getHumanName() );
                            query.addBindValue( i.getValue() );
                            query.addBindValue( i.getModule() );
                            query.addBindValue( i.getDescription() );
                            query.addBindValue( i.isPrivate() ? 1 : 0 );
                            query.addBindValue( i.getSubgroupName() );
                            query.addBindValue( i.getSubgroupHumanName() );
                            query.addBindValue( i.getType() );
                            if ( ! query.exec() )
                            {
                                PRINT_CONSOLE_MESSAGE_ERROR( QString( "Невозможно выполнить SQL запрос: запрос[%1], ошибка[%2]" )
                                                   .arg( queryStr )
                                                   .arg( query.lastError().text() ) );
                                emit m_database.data()->invalidDb( );
                            }
                            else
                            {
                                res = true;
                            }
                        }
                        else
                        {
                            PRINT_CONSOLE_MESSAGE_ERROR( QString( "Некорректная строка SQL запрос: запрос[%1], ошибка[%2]" )
                                               .arg( queryStr )
                                               .arg( query.lastError().text() ) );
                        }
                    }
                }
            }
        }
    }
    emit setSettingsListResult( false/*, QPrivateSignal{} */);
    return res;
}

/** ***********************************************************************
 * @fn threadStartedSlot
 * @brief слот, вызывается при запуске потока
 * @param none
 * @return none
 ** ***********************************************************************/
void CSettingsDatabaseProvider::threadStartedSlot()
{
    QString name(SETTINGS_DB_BASE_CONNECTION_NAME);
    name += DB_SETTINGS_TABLE_NAME;

    m_database = new CDatabaseHandler( name/*SETTINGS_DB_BASE_CONNECTION_NAME+DB_SETTINGS_TABLE_NAME*/, this );
    m_database.data()->threadStartedSlot();
}

void CSettingsDatabaseProvider::threadStopedSlot()
{
    m_database.data()->threadStopedSlot();
}

/** ***********************************************************************
 * @fn getSettingsTotalList     [thread safe]
 * @brief метод для получения общего списка настроек для текущего приложения
 * @param none
 * @return QList < CSettingsEntry >
 ** ***********************************************************************/
QList < CSettingsEntry > 
CSettingsDatabaseProvider::getAllSettings( bool showPrivate, const QString &module )
{
    QList < CSettingsEntry > settingsList{};
    if ( QThread::currentThread() == thread() )
    {
        settingsList = getSettingsTotalListSlot( module, showPrivate );
    }
    else
    {
        QSemaphore sema(0);
        QMetaObject::Connection conn = connect( this, &CSettingsDatabaseProvider::getSettingsTotalListResult, [ &sema, &settingsList ]( const QList < CSettingsEntry > list )
        {
            settingsList = list;
            sema.release(1);
        });
        if ( ! QMetaObject::invokeMethod( this,
                                          "getSettingsTotalListSlot",
                                          Qt::QueuedConnection,
                                          Q_ARG( QString, module ),
                                          Q_ARG( bool, showPrivate ) ) )
        {
            PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
        }
        else
        {
            sema.tryAcquire( 1, 2000 );
        }
        disconnect( conn );
    }
    return settingsList;
}

/** ***********************************************************************
 * @fn getSettings     [thread safe]
 * @brief метод для получения параметров указанной настройки.
 * Если настройка не найдена - вернется пустая настройка
 * @param QString                   - параметр (наименование настройки) поддерживается только латиница!
 * @return CSettingsEntry
 ** ***********************************************************************/
CSettingsEntry 
CSettingsDatabaseProvider::getSetting( const QString &name, bool showPrivate, const QString module )
{
    CSettingsEntry setting{};
    if ( QThread::currentThread() == thread() )
    {
        setting = getSettingSlot( name, module, showPrivate );
    }
    else
    {
        QSemaphore sema(0);
        QMetaObject::Connection conn = connect( this, &CSettingsDatabaseProvider::getSettingResult, [ &sema, &setting ]( const CSettingsEntry readedSetting )
        {
            setting = readedSetting;
            sema.release(1);
        });
        if ( ! QMetaObject::invokeMethod( this,
                                          "getSettingSlot",
                                          Qt::QueuedConnection,
                                          Q_ARG( QString, name ),
                                          Q_ARG( QString, module ),
                                          Q_ARG( bool, showPrivate ) ) )
        {
            PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
        }
        else
        {
            sema.tryAcquire( 1, 2000 );
        }
        disconnect( conn );
    }
    return setting;
}

/** ***********************************************************************
 * @fn setSetting     [thread safe]
 * @brief метод для сохранения одной настройки
 * @param CSettingsEntry
 * @return bool
 ** ***********************************************************************/
bool CSettingsDatabaseProvider::setSetting( const CSettingsEntry &setting, bool override )
{
    bool res{ false };
    if ( QThread::currentThread() == thread() )
    {
        res = setSettingSlot( setting, override );
    }
    else
    {
        QSemaphore sema(0);
        if ( setting.isValid() )
        {
            QMetaObject::Connection conn = connect( this, &CSettingsDatabaseProvider::setSettingResult, [ &sema, &res ]( const bool state )
            {
                res = state;
                sema.release(1);
            });
            if ( ! QMetaObject::invokeMethod( this,
                                              "setSettingSlot",
                                              Qt::QueuedConnection,
                                              Q_ARG( CSettingsEntry, setting ),
                                              Q_ARG(  bool, override ) ) )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
            }
            else
            {
                sema.tryAcquire( 1, 2000 );
            }
            disconnect( conn );
        }
    }
    return res;
}

/** ***********************************************************************
 * @fn setSettingsList     [thread safe]
 * @brief метод для сохранения списка настроек
 * @param QList < CSettingsEntry >
 * @return bool
 ** ***********************************************************************/
bool CSettingsDatabaseProvider::setSettingsList( const QList<CSettingsEntry> &settingsList, 
                                                    bool override )
{
    bool res{ false };
    if ( QThread::currentThread() == thread() )
    {
        res = setSettingsListSlot( settingsList, override );
    }
    else
    {
        QSemaphore sema(0);
        if ( ! settingsList.isEmpty() )
        {
            QMetaObject::Connection conn = connect( this, &CSettingsDatabaseProvider::setSettingsListResult,
                                                    [ &sema, &res ]( const bool state )
            {
                res = state;
                sema.release(1);
            });
            if ( ! QMetaObject::invokeMethod( this,
                                              "setSettingsListSlot",
                                              Qt::QueuedConnection,
                                              Q_ARG( QList < CSettingsEntry >, settingsList ),
                                              Q_ARG(  bool, override ) ) )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
            }
            else
            {
                sema.tryAcquire( 1, 2000 );
            }
            disconnect( conn );
        }
    }
    return res;
}

/** ***********************************************************************
 * @fn getDatabaseTransport
 * @brief метод, возвращает указатель на обработчик подключения к БД
 * @param none
 * @return CDatabaseHandler *
 ** ***********************************************************************/
const CDatabaseHandler * CSettingsDatabaseProvider::getDatabaseTransport()
{
    return m_database.data();
}


const char *CSettingsDatabaseProvider::getTableName()
{
    return DB_SETTINGS_TABLE_NAME;
}

const char *CSettingsDatabaseProvider::getTableColumn(CSettingsDatabaseProvider::EDbTableSettingsColumn index)
{
    return DbTableSettings[index];
}
