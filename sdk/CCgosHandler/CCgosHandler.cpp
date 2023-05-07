//#include <QtCore/QMutex>

#include <CConsoleLogger/CConsoleLogger.h>
#include <CUtils/CUtils.h>

#include <CCgosHandler/CCgosHandler.h>

//const std::uint8_t CCgosHandler::i2cAddr_tas5414cRead{ 0xD9 };
//const std::uint8_t CCgosHandler::i2cAddr_tas5414cWrite{ 0xD8 };
//const std::uint8_t CCgosHandler::i2cAddr_tas5414c{ 0xD8 };

const char *cgosIsNotInitedErrStr = "CGOS is not inited";

CCgosHandler::CCgosHandler():
    m_hcgos(0),
    m_outputPinsState(0) // Workaround for unmute amplifer
{
}

bool CCgosHandler::init()
{
    if ( CgosLibInitialize() == 0 ) 
    {
        PRINT_CONSOLE_MESSAGE_ERROR("Can't initialize CGOS library" );
        if (!CgosLibInstall(1)) 
        {
            PRINT_CONSOLE_MESSAGE_ERROR("The driver could not be installed. Check your rights!");
            return false;
        }
        PRINT_CONSOLE_MESSAGE_INFO("The driver has been installed.");
        if (!CgosLibInitialize()) 
        {
            PRINT_CONSOLE_MESSAGE_ERROR("Still could not open driver, a reboot might be required!");
            return false;
        }
    } 

    PRINT_CONSOLE_MESSAGE_INFO( QString( "Using CGOS: library version: %1; Driver version: %2 " )
                                            .arg(CgosLibGetVersion(), 8, 16, QChar('0'))
                                            .arg(CgosLibGetDrvVersion(), 8, 16, QChar('0')));

    if ( CgosLibSetLastErrorAddress( & m_error ) == 0 )
    {
        PRINT_CONSOLE_MESSAGE_WARN("Can't install cgos last error address");
    }

    if ( CgosBoardOpen( 0, 0, 0, & m_hcgos ) == 0 )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Can't open CGOS board. Error:  %1" ).arg(m_error) );
        return false;
    }

    ulong ioUnits = CgosIOCount( m_hcgos );
    PRINT_CONSOLE_MESSAGE_INFO(QString("Number of Digital IO Units : %1").arg(ioUnits));

    for(ulong i=0; i<ioUnits; i++)
    {
        unsigned int inputPins = 0;
        unsigned int outputPins = 0;
        unsigned int pinsConfig = 0;
        unsigned int pinsData = 0;
        if ( CgosIOGetDirection( m_hcgos, i, &pinsConfig ))
        {
            PRINT_CONSOLE_MESSAGE_INFO(QString("[%1] Pins direction: %2(%3)")
                                        .arg(i)
                                        .arg(QString::number(pinsConfig, 2))
                                        .arg(pinsConfig , 0, 16));
        }
        if ( CgosIOGetDirectionCaps( m_hcgos, i, &inputPins, &outputPins ))
        {
            PRINT_CONSOLE_MESSAGE_INFO(QString("[%1] Input pins: %2(%3)")
                                        .arg(i)
                                        .arg(QString::number(inputPins, 2))
                                        .arg(inputPins , 0, 16));
            PRINT_CONSOLE_MESSAGE_INFO(QString("[%1] Output pins: %2(%3)")
                                        .arg(i)
                                        .arg(QString::number(outputPins, 2))
                                        .arg(outputPins , 0, 16));
        }

        if(CgosIORead(m_hcgos, i, &pinsData))
        {
            PRINT_CONSOLE_MESSAGE_INFO(QString("[%1] Input Pins value: %2(%3)")
                                        .arg(i)
                                        .arg(QString::number(pinsData, 2))
                                        .arg(pinsData , 0, 16));
        }
    }




/*    if(CgosIOIsAvailable(m_hcgos, 0)) 
    {
        PRINT_CONSOLE_MESSAGE_INFO("Port 0 available");
    }
    if(CgosIOIsAvailable(m_hcgos, 1)) 
    {
        PRINT_CONSOLE_MESSAGE_INFO("Port 1 available");
    }
    if(CgosIOIsAvailable(m_hcgos, 2)) 
    {
        PRINT_CONSOLE_MESSAGE_INFO("Port 2 available");
    }
    if(CgosIOIsAvailable(m_hcgos, 3)) 
    {
        PRINT_CONSOLE_MESSAGE_INFO("Port 3 available");
    }
    if(CgosIOIsAvailable(m_hcgos, 4)) 
    {
        PRINT_CONSOLE_MESSAGE_INFO("Port 4 available");
    }
    if(CgosIOIsAvailable(m_hcgos, 100)) 
    {
        PRINT_CONSOLE_MESSAGE_INFO("Port 100 available");
    }

    char p[100] = {0};
    CgosIOGetName(m_hcgos, 0, p, 100);
    PRINT_CONSOLE_MESSAGE_INFO(QString(" p = %1").arg(p));

    unsigned int dwInputs = 0, dwOutputs = 0;
    CgosIOGetDirectionCaps(m_hcgos, 0, &dwInputs, &dwOutputs);
    PRINT_CONSOLE_MESSAGE_INFO(QString("Port 0 have %1 inputs and %2 outputs").arg(dwInputs).arg(dwOutputs));
    CgosIOGetDirectionCaps(m_hcgos, 1, &dwInputs, &dwOutputs);
    PRINT_CONSOLE_MESSAGE_INFO(QString("Port 1 have %1 inputs and %2 outputs").arg(dwInputs).arg(dwOutputs));
    CgosIOGetDirectionCaps(m_hcgos, 2, &dwInputs, &dwOutputs);
    PRINT_CONSOLE_MESSAGE_INFO(QString("Port 2 have %1 inputs and %2 outputs").arg(dwInputs).arg(dwOutputs));
    CgosIOGetDirectionCaps(m_hcgos, 3, &dwInputs, &dwOutputs);
    PRINT_CONSOLE_MESSAGE_INFO(QString("Port 3 have %1 inputs and %2 outputs").arg(dwInputs).arg(dwOutputs));
*/

    PRINT_CONSOLE_MESSAGE_INFO(QString("Number of I2C Buses: %1").arg(CgosI2CIsAvailable( m_hcgos, 0 )));
//    handlerInited = true;

    return true;
}

CCgosHandler::~CCgosHandler() 
{
    if ( m_hcgos )
    {
        CgosBoardClose(m_hcgos);
        PRINT_CONSOLE_MESSAGE_INFO("CGOS board closed");
        m_hcgos = 0;
    }

    CgosLibUninitialize();
    PRINT_CONSOLE_MESSAGE_INFO("CGOS library unloaded");
}

/*
CCgosHandler * CCgosHandler::getInstance()
{
    if ( handler == nullptr )
    {
        QMutexLocker locker{ & instanceMutex };
        Q_UNUSED( locker );
        if ( handler == nullptr )
        {
            handler = new CCgosHandler{};
        }
    }
    Q_ASSERT( handler != nullptr );
    return handler;
}

void CCgosHandler::deinitCgosHandler()
{
    QMutexLocker locker{ & initHandlerMutex };
    Q_UNUSED( locker );
    if ( handler )
    {
        delete handler;
        handler = nullptr;
    }
}
*/
bool CCgosHandler::readI2CRegister( uint8_t address,
                                    uint16_t regnum,
                                    uint8_t & byte )
{
    if(m_hcgos == 0)
    {
        PRINT_CONSOLE_MESSAGE_INFO(cgosIsNotInitedErrStr);
        return false;
    }
/*    PRINT_CONSOLE_MESSAGE_INFO( QString( "Reading from I2C address %1 and register %2" )
                                .arg(QString::number( address, 16 ))
                                .arg(QString::number( regnum, 16 )) );
*/
    return CgosI2CReadRegister( m_hcgos, 0, address | 1, regnum, & byte ) == 1;
}

bool CCgosHandler::writeI2CRegister( std::uint8_t address,
                                     std::uint16_t regnum,
                                     std::uint8_t byte )
{
    if(m_hcgos == 0)
    {
        PRINT_CONSOLE_MESSAGE_INFO(cgosIsNotInitedErrStr);
        return false;
    }
    PRINT_CONSOLE_MESSAGE_INFO( QString( "Writting %1 to I2C address %2 and register %3" )
                                .arg(QString::number( byte, 16 ))
                                .arg(QString::number( address, 16 ))
                                .arg(QString::number( regnum, 16 )) );
    return CgosI2CWriteRegister( m_hcgos, 0, address & ~1, regnum, byte ) == 1;
}

bool CCgosHandler::readIO(CgosGpio ioPin, int8_t &value)
{
    unsigned int pinsData = 0;
    unsigned int inputPins = 0;
    unsigned int outputPins = 0;

    if(m_hcgos == 0)
    {
        PRINT_CONSOLE_MESSAGE_INFO(cgosIsNotInitedErrStr);
        return false;
    }

//    unsigned int pinMask = CGOS_GPIO_MASK(ioPin);
//std::cout << "GPI num = " << num << "; mask = " << std::bitset<8>(mask) << std::endl;
//                sscanf(argv[i], "GPI%d", &num);
    if ( CgosIOGetDirectionCaps( m_hcgos, 0, &inputPins, &outputPins ))
    {
//                  std::cout << "Input pins: " << std::bitset<8>(inputPins) << "(" << std::hex << inputPins << ")" << std::endl;
//                  std::cout << "Output pins: " << std::bitset<8>(outputPins) << "(" << std::hex << outputPins << ")" << std::endl;
        if(CgosIORead(m_hcgos, 0, &pinsData))
        {
            pinsData &= inputPins;
            value = CGOS_GET_GPIO_VALUE(ioPin, pinsData);
//                        std::cout << "Input Pins value: " << std::bitset<8>(pinsData) << "(" << std::hex << pinsData << ")" << std::endl;
//                        std::cout << "Pin value: " << std::bitset<8>(pinsData & mask) << "(" << ((pinsData & mask) >> num) << ")" << std::endl;
            return true;
        }
    }

    return false;
}

bool CCgosHandler::writeIO(CgosGpio ioPin, int8_t value)
{
    unsigned int pinsData = 0;
    int8_t regData = 0;
//    unsigned int inputPins = 0;
//    unsigned int outputPins = 0;
//    unsigned int pinMask = CGOS_GPIO_MASK(ioPin);

//    PRINT_CONSOLE_MESSAGE_INFO(QString("writeIO: %1(%2)")
//                                       .arg(ioPin)
//                                       .arg(QString::number(value))                                     
//    );


    if(m_hcgos == 0)
    {
        PRINT_CONSOLE_MESSAGE_INFO(cgosIsNotInitedErrStr);
        return false;
    }

    if(value > 1 || value < 0)
    {
        return false;
    }

    regData = m_outputPinsState;

    PRINT_CONSOLE_MESSAGE_INFO(QString("writeIO(1): %1(%2, %3)")
                                       .arg(ioPin)
                                       .arg(QString::number(value))
                                       .arg(QString::number( regData, 2 ))
    );


    pinsData = CGOS_SET_GPIO_VALUE(ioPin, 1);

    if(value == 1)
    {
        pinsData |= regData;
    } 
    else
    {
        pinsData ^= regData;
    }
    PRINT_CONSOLE_MESSAGE_INFO(QString("writeIO(2): %1(%2,%3,%4)")
                                       .arg(ioPin)
                                       .arg(QString::number(value))
                                       .arg(QString::number( regData, 2 ))
                                       .arg(QString::number( pinsData, 2 ))
    );


//    if ( CgosIOGetDirectionCaps( hcgos, 0, &inputPins, &outputPins ))
//    {
//        pinsData = CGOS_SET_GPIO_VALUE(ioPin, value); //(value << (num+4));
    PRINT_CONSOLE_MESSAGE_INFO(QString("writeIO(3): %1(%2,%3,%4)")
                                       .arg(ioPin)
                                       .arg(QString::number(value))
                                       .arg(QString::number( regData, 2 ))
                                       .arg(QString::number( pinsData, 2 ))
    );
        if(CgosIOWrite(m_hcgos, 0, pinsData))
        {
/*                        pinsData = 0;
                        if(CgosIORead(hcgos, 0, &pinsData))
                        {
                            pinsData &= inputPins;
                            std::cout << "Input Pins value: " << std::bitset<8>(pinsData) << "(" << std::hex << pinsData << ")" << std::endl;
                            std::cout << "Pin value: " << std::bitset<8>(pinsData & mask) << "(" << ((pinsData & mask) >> num) << ")" << std::endl;
                        }*/
            m_outputPinsState = pinsData;
    PRINT_CONSOLE_MESSAGE_INFO(QString("writeIO(4): %1(%2,%3,%4)")
                                       .arg(ioPin)
                                       .arg(QString::number(value))
                                       .arg(QString::number( regData, 2 ))
                                       .arg(QString::number( m_outputPinsState, 2 ))
    );
            return true;
        }
//    }

    return false;
}

bool CCgosHandler::checkVersion()
{
    unsigned int dwLibVersion, dwDrvVersion;

    dwLibVersion = CgosLibGetVersion();
    dwDrvVersion = CgosLibGetDrvVersion();

    if ( dwLibVersion < 0x0102000A || dwDrvVersion < 0x0102000A )
    {
/*        sprintf(buf,
          TEXT("Error: outdated CGOS Library/Driver version.\n")
            TEXT("detected library version: 0x%08x\n")
            TEXT("required library version: 0x0102000A\n")
            TEXT("detected driver  version: 0x%08x\n")
            TEXT("required driver  version: 0x0102000A\n"),
            dwLibVersion,
            dwDrvVersion
            );
        printf(buf);
      CgosBoardClose(hCgos);*/
        return false;
    }

    return true;
}

bool CCgosHandler::readTemperatureSensors(CongatecTemperatureSensorsList &sensors)
{
    int i;
    static CGOSTEMPERATUREINFO temperatureInfo = {};
    unsigned int dwUnit, setting, status, monCount = 0;
    struct sensor {
        char name[32];
        unsigned int value;
    };
    struct sensor temp_values[8] =
    {
        {"CPU temperature" , CGOS_TEMP_CPU },
        {"Box temperature" , CGOS_TEMP_BOX },
        {"Environment temperature" , CGOS_TEMP_ENV },
        {"Board temperature" , CGOS_TEMP_BOARD },
        {"Backplane temperature" , CGOS_TEMP_BACKPLANE },
        {"Chipset temperature" , CGOS_TEMP_CHIPSETS },
        {"Unknown temperature" , CGOS_TEMP_OTHER },
        {"", 0 }
    };

    if(m_hcgos == 0)
    {
        PRINT_CONSOLE_MESSAGE_INFO(cgosIsNotInitedErrStr);
        return false;
    }

    temperatureInfo.dwSize = sizeof (temperatureInfo);
    monCount = CgosTemperatureCount(m_hcgos);

    if(monCount != 0)
    {
        CongatecTemperatureSensor_t sensor;
        for(dwUnit = 0; dwUnit < monCount; dwUnit++)
        {
            if(CgosTemperatureGetInfo(m_hcgos, dwUnit, &temperatureInfo))
            {
                if(CgosTemperatureGetCurrent(m_hcgos, dwUnit, &setting, &status))
                {
                    //scan type
                    i=0;
                    while (temp_values[i].value !=0)
                    {
                        if (temperatureInfo.dwType == temp_values[i].value)
                            break;
                        i++;
                    }

                    temperatureInfo.dwMin = (int) temperatureInfo.dwMin / 1000;
                    temperatureInfo.dwMax /= 1000L;
                    setting = (int) setting / 1000;

                    sensor.name = temp_values[i].name;
                    sensor.value = static_cast<int>(setting);

                    sensors << sensor;

/*                    sprintf(buf,TEXT("Type of sensor:      %s\n"),temp_values[i].name); printf(buf);
                    sprintf(buf,TEXT("Unit number:         %d\n"), (int) dwUnit); printf(buf);
                    sprintf(buf,TEXT("Resolution:          %d\n"), (int) temperatureInfo.dwRes); printf(buf);
                    sprintf(buf,TEXT("Minimum temperature: %d degrees centigrade\n"), (int) temperatureInfo.dwMin); printf(buf);
                    sprintf(buf,TEXT("Maximum temperature: %d degrees centigrade\n"), (int) temperatureInfo.dwMax); printf(buf);
                    sprintf(buf,TEXT("Current temperature: %d degrees centigrade\n"), (int) setting); printf(buf);
                    */
                }
            }
        }
    }


    return true;
}
