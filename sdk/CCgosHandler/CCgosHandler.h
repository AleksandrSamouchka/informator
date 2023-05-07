#ifndef __CCGOSHANDLER_H__
#define __CCGOSHANDLER_H__

#include <cinttypes>

#include <QList>

#include <Cgos.h>

typedef struct
{
    QString name;
    int value;
} CongatecTemperatureSensor_t;

typedef QList<CongatecTemperatureSensor_t> CongatecTemperatureSensorsList;

typedef enum
{
    CGOS_GPI0 = 0,
    CGOS_GPI1,
    CGOS_GPI2,
    CGOS_GPI3,
    CGOS_GPO0,
    CGOS_GPO1,
    CGOS_GPO2,
    CGOS_GPO3
} CgosGpio;

#define CGOS_GPIO_MASK(n)           (1 << n)
#define CGOS_GET_GPIO_VALUE(n, data)    ((data & CGOS_GPIO_MASK(n)) >> n)
#define CGOS_SET_GPIO_VALUE(n, value)   (unsigned int)(value << (n))
//(((pinsData & mask) >> num) << n)
#define CGOS_GPI0_MASK      CGOS_GPIO_MASK(GPI0)
#define CGOS_GPI1_MASK      CGOS_GPIO_MASK(GPI1)
#define CGOS_GPI2_MASK      CGOS_GPIO_MASK(GPI2)
#define CGOS_GPI3_MASK      CGOS_GPIO_MASK(GPI3)
#define CGOS_GPO0_MASK      CGOS_GPIO_MASK(GPO0)
#define CGOS_GPO1_MASK      CGOS_GPIO_MASK(GPO1)
#define CGOS_GPO2_MASK      CGOS_GPIO_MASK(GPO2)
#define CGOS_GPO3_MASK      CGOS_GPIO_MASK(GPO3)


class CCgosHandler
{
public:
//    static const std::uint8_t i2cAddr_tas5414c;

    CCgosHandler();
    virtual ~CCgosHandler();

    bool init();

    bool checkVersion();

    inline bool isAvailable()
    {
        return m_hcgos != 0;
    }

//    static CCgosHandler * getInstance();
//    static void deinitCgosHandler();

    bool readI2CRegister( uint8_t address,
                          uint16_t regnum,
                          uint8_t & byte );
    bool writeI2CRegister( uint8_t address,
                           uint16_t regnum,
                           uint8_t byte );

    bool readIO(CgosGpio ioPin, int8_t &value);
    bool writeIO(CgosGpio ioPin, int8_t value);

    bool readTemperatureSensors(CongatecTemperatureSensorsList &sensors);

//private:

private:
    HCGOS m_hcgos;
    unsigned int m_error;
    uint8_t m_outputPinsState;
};

#endif