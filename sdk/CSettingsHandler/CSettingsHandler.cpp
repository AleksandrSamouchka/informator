#include "CSettingsHandler.h"
#include <CConsoleLogger/CConsoleLogger.h>
#include <QtCore/QThread>
#include <QtCore/QSemaphore>

QPointer<CSettingsHandler> CSettingsHandler::handler;
QScopedPointer<QThread> CSettingsHandler::handlerThread;
QMutex CSettingsHandler::initHandlerMutex;
QMutex CSettingsHandler::instanceMutex;

const QString CSettingsHandler::SETT_GLOBAL_MODULE = "global";
const QString CSettingsHandler::SETT_TEMP_GROUP = "TemperatureSensors";
const QString CSettingsHandler::SETT_TEMP_SOURCE_SALON = "TempSourceSalon";
const QString CSettingsHandler::SETT_TEMP_SOURCE_OUTSIDE = "TempSourceOutside";
const QString CSettingsHandler::SETT_TEMP_1WIRE_SALON_NAME = "Temp1WireSalonName";
const QString CSettingsHandler::SETT_TEMP_1WIRE_OUTSIDE_NAME = "Temp1WireOutsideName";
const QString CSettingsHandler::SETT_TEMP_NOTUSED_SOURCE = "TempSource";

const QString CSettingsHandler::SETT_INFO_GROUP = "BkInfo";
const QString CSettingsHandler::SETT_INFO_MANUFDATE = "ManufactureDate";
const QString CSettingsHandler::SETT_INFO_SN = "SerialNumber";

const QString CSettingsHandler::SETT_TRANSPORT_GROUP = "TransportType";
const QString CSettingsHandler::SETT_TRANSPORT_DOOR_NUMBER = "busDoorNumber";
const QString CSettingsHandler::SETT_TRANSPORT_BUS_TYPE = "BusType";

const QString CSettingsHandler::SETT_BKSETTINGS_GROUP = "BkSettings";
const QString CSettingsHandler::SETT_BKSETTINGS_TIMEZONE = "TimeZone";

static const int NORM_WAITING_TIMEOUT{ 5000 };

/*
const QHash < int, QString > CSettingsHandler::BUS_TYPE_NAMES{
    { static_cast< int >( CSettingsHandler::EBusType::nefaz5299 ), "НЕФАЗ 5299" },
    { static_cast< int >( CSettingsHandler::EBusType::nefaz5299_intern ), "НЕФАЗ 5299 (Стажер)" },
    { static_cast< int >( CSettingsHandler::EBusType::nefaz6282 ), "НЕФАЗ 6282" },
    { static_cast< int >( CSettingsHandler::EBusType::nefaz6282_intern ), "НЕФАЗ 6282 (Стажер)" },
    { static_cast< int >( CSettingsHandler::EBusType::nefaz5299_new ), "НЕФАЗ 5299 (Новый)" },
    { static_cast< int >( CSettingsHandler::EBusType::nefaz6282_new ), "НЕФАЗ 6282 (Новый)" },
    { static_cast< int >( CSettingsHandler::EBusType::liaz621365_79 ), "ЛИАЗ 621365-79" },
    { static_cast< int >( CSettingsHandler::EBusType::liaz621365_80_intern ), "ЛИАЗ 621365-80 (Стажер)" },
    { static_cast< int >( CSettingsHandler::EBusType::liaz6274 ), "ЛИАЗ 6274" },
    { static_cast< int >( CSettingsHandler::EBusType::liaz6274_intern ), "ЛИАЗ 6274 (Стажер)" },
    { static_cast< int >( CSettingsHandler::EBusType::manLionsCoachR07 ), "МАN LION'S COACH R07" },
};
*/


/** ***********************************************************************
 * @fn deinitDatabaseHandler
 * @brief функция для деинициализации singleton объекта обработчика запросов в БД
 * @param none
 * @return none
 ** ***********************************************************************/
void CSettingsHandler::deinitSettingsHandler( )
{
    bool needDel = false;
    {   // Создали область видимость для блокировки мьютекса
        QMutexLocker locker( & initHandlerMutex );
        if ( ! handler.isNull() )
        {
            if ( ! QMetaObject::invokeMethod( handler,
                                              "threadStopedSlot",
                                              Qt::BlockingQueuedConnection ) )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
            }
            handlerThread->quit();
            handlerThread->wait();
            // Здесь handler уже удален, и указатель автоматом обнулился после
            needDel = true;
            // его удаления, в этом весь смысл QPointer
            //handlerThread = nullptr;
            //delete handler;           // Удалится автоматическим вызовом слота
            // deleteLater
            //handler = nullptr;
            //handlerInited = false;
        }
    } // Область видимости закончилась, тут мьютекс анлочится.
    if(needDel) handlerThread.reset(); // Поток удаляем, когда мьютекс уже разлочен,
                            // в противном случае получим сообщение в
                            // консоль: QMutex: destroing locked mutex
                            // Поток удаляем если он не был ранее удален.
                            // Метод reset() сам проверяет существование
                            // объекта
}

/** ***********************************************************************
 * @brief Класс по работе с настройками приложения
 ** ***********************************************************************/
CSettingsHandler::CSettingsHandler( QObject * parent ) :
    CAbstractSettingsProvider( parent )
{ }

/** ***********************************************************************
 * @fn getInstance [static]
 * @brief метод для создания singleton объекта класса
 * @param none
 * @return CSettingsHandler *
 ** ***********************************************************************/
CSettingsHandler * CSettingsHandler::getInstance( )
{
    if ( handler.isNull() )
    {
        QMutexLocker locker( & instanceMutex );
        if ( handler.isNull() )
        {
            QMutexLocker locker( & initHandlerMutex );
            handler = new CSettingsHandler( );
            handlerThread.reset( new QThread{});
            handler.data()->moveToThread( handlerThread.data() );
            connect( handlerThread.data(), & QThread::started,
                     handler.data(), & CSettingsHandler::threadStartedSlot );
            connect( handlerThread.data(), & QThread::finished, handler.data(), & CSettingsHandler::deleteLater );
            QSemaphore sema(0);
            auto conn = connect( handler.data(), & CSettingsHandler::ready, [ &sema ]()
            {
                sema.release(1);
            });
            handlerThread->start();

            if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
            }
            disconnect( conn );
        }
    }
    Q_ASSERT( handler != nullptr );
    return handler;
}

/** ***********************************************************************
 * @fn threadStartedSlot
 * @brief слот, вызывается при запуске потока
 * @param none
 * @return none
 ** ***************************************************************************/
void CSettingsHandler::threadStartedSlot()
{
    m_settingsDatabase = new CSettingsDatabaseProvider();
    QObject::connect( handlerThread.data(), & QThread::finished,
            m_settingsDatabase.data(), & CSettingsDatabaseProvider::deleteLater/*,Qt::DirectConnection*/);
    m_settingsDatabase.data()->threadStartedSlot();
    emit ready( /*QPrivateSignal{}*/ );
}

void CSettingsHandler::threadStopedSlot()
{
    m_settingsDatabase.data()->threadStopedSlot();
    //delete m_settingsDatabase.data(); // будет удален автоматически
                                    // слотом deleteLater(), вызваным
                                // по сигналу handlerThread.data()->finished
}

/** ****************************************************************************
 * @fn setSetting     [thread safe]
 * @brief метод для сохранения одной настройки
 * @param CSettingsEntry
 * @return bool
 ** ***************************************************************************/
bool CSettingsHandler::setSetting( const CSettingsEntry &setting,
        bool override )
{
    bool res{ false };
    if ( m_settingsDatabase.isNull() )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Объект БД не проинициализирован или "
                "был удален извне!" );
    }
    else
    {
        res = m_settingsDatabase.data()->setSetting( setting, override );
    }
    return res;
}

/** ****************************************************************************
 * @fn setSettingsList     [thread safe]
 * @brief метод для сохранения списка настроек
 * @param QList < CSettingsEntry >
 * @return bool
 ** ***************************************************************************/
bool CSettingsHandler::setSettingsList( const QList<CSettingsEntry> &settingsList, 
                                        bool override )
{
    bool res{ false };
    if ( m_settingsDatabase.isNull() )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Объект БД не проинициализирован или был удален извне!" );
    }
    else
    {
        res = m_settingsDatabase.data()->setSettingsList( settingsList, override );
    }
    return res;
}

/** ***********************************************************************
 * @fn getSettingsTotalList     [thread safe]
 * @brief метод для получения общего списка настроек для текущего приложения
 * @param none
 * @return QList < CSettingsEntry >
 ** ***********************************************************************/
QList < CSettingsEntry > CSettingsHandler::getAllSettings( bool showPrivate, 
                                                            const QString &module )
{
    QList < CSettingsEntry > settingsList{};
    if ( m_settingsDatabase.isNull() )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Объект БД не проинициализирован или был удален извне!" );
    }
    else
    {
        settingsList = m_settingsDatabase.data()->getAllSettings( showPrivate, module );
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
CSettingsEntry CSettingsHandler::getSetting( const QString &name, 
                                            bool showPrivate, 
                                            const QString module )
{
    CSettingsEntry setting{};
    if ( m_settingsDatabase.isNull() )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Объект БД не проинициализирован или был удален извне!" );
    }
    else
    {
        setting = m_settingsDatabase.data()->getSetting( name, showPrivate, module );
    }
    return setting;
}

CSettingsEntry settings_entry( const QString &name, bool showPrivate )
{
    return CSettingsHandler::getInstance()->getSetting( name, showPrivate );
}
