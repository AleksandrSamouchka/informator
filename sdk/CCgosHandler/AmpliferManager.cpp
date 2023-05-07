#include <QThread>
#include <QDebug>

#include <CUtils/CUtils.h>
#include <CLoggerHandler/CLoggerHandler.h>

#include "AmpliferManager.h"

#define TAS5414C_ADDR   0xD8

//#define MUTE_IO         CGOS_GPO3

//const std::uint8_t CCgosHandler::i2cAddr_tas5414cRead{ 0xD9 };
//const std::uint8_t CCgosHandler::i2cAddr_tas5414cWrite{ 0xD8 };
//const std::uint8_t CCgosHandler::i2cAddr_tas5414c{ 0xD8 };

#define CHANNEL1_GAIN(x)    x    
#define CHANNEL2_GAIN(x)    (x<<2)
#define CHANNEL3_GAIN(x)    (x<<4)
#define CHANNEL4_GAIN(x)    (x<<6)

/*
#define MUTE_CHANNEL1 MUTE_CHANNEL(1, 1)
#define MUTE_CHANNEL2 MUTE_CHANNEL(2, 1)
#define MUTE_CHANNEL3 MUTE_CHANNEL(3, 1)
#define MUTE_CHANNEL4 MUTE_CHANNEL(4, 1)

#define UNMUTE_CHANNEL1_VALUE       MUTE_CHANNEL(1, 1)
#define UNMUTE_CHANNEL2_VALUE       MUTE_CHANNEL(2, 1)
#define UNMUTE_CHANNEL3_VALUE       MUTE_CHANNEL(3, 1)
#define UNMUTE_CHANNEL4_VALUE       MUTE_CHANNEL(4, 1)
*/

#define IS_MUTED_CHANNEL(channel, byte) ((byte >> (channel-1) & 1) == 1)
#define MUTE_CHANNEL(channel, byte)     (byte | (1 << (channel-1)))
#define UNMUTE_CHANNEL(channel, byte)     (byte ^ (1 << (channel-1)))


#define CHANNEL_GAIN(channel, gain)    (gain << (channel-1)*2)   

#define ALL_CHANNELS_GAIN(x) (CHANNEL1_GAIN(x) | CHANNEL2_GAIN(x) | CHANNEL3_GAIN(x) | CHANNEL4_GAIN(x))   

#define GAIN_DEFAULT                GAIN_26DB
#define ALL_CHANNELS_GAIN_DEFAULT   ALL_CHANNELS_GAIN(GAIN_DEFAULT)



AmpliferManager::AmpliferManager(QObject *parent, CCgosHandler *cgos) :
    QObject(parent),
    m_cgos(cgos)
{
}

AmpliferManager::~AmpliferManager()
{
}

bool AmpliferManager::init()
{
//#ifdef HAS_CCGOSHANDLER
    if ( m_cgos->writeI2CRegister( TAS5414C_ADDR, 0x0C, 0x9F ) )
    {
        auto counter{ 0 };
        QThread::msleep( CUtils::waitTimeout() / 8 );

        for ( uint8_t byte0{ 1 }, byte1{ 1 }; ( byte0 != 0 || byte1 != 0 ) && ( counter < CUtils::waitTimeout() ); ++counter )
        {
            if ( ! m_cgos->readI2CRegister( TAS5414C_ADDR, 0x00, byte0 ) )
            {
                byte0 = 1;
            }
            if ( ! m_cgos->readI2CRegister( TAS5414C_ADDR, 0x01, byte1 ) )
            {
                byte1 = 1;
            }
            QThread::currentThread()->msleep( 1 );
        }

        if ( counter >= CUtils::waitTimeout() ||
             /*! m_cgos->writeI2CRegister( TAS5414C_ADDR, 0x08, 0xFF ) || */
             ! m_cgos->writeI2CRegister( TAS5414C_ADDR, 0x0C, 0x00 ) )
        {
            PRINT_CONSOLE_MESSAGE_WARN( "Can't initialize TAS5414S" );
            return false;
        }
    }
//#endif

    return true;
}
  
/*  
bool AmpliferManager::mute()
{
    return m_cgos->writeIO(MUTE_IO, 1);
}
    
bool AmpliferManager::unmute()
{
    return m_cgos->writeIO(MUTE_IO, 0);
}
*/

bool AmpliferManager::setGain(ChannelGain_t gain)
{
    if(!setChannelGain(AMP_CHANNEL1, gain) ||
        !setChannelGain(AMP_CHANNEL2, gain) ||
        !setChannelGain(AMP_CHANNEL3, gain) ||
        !setChannelGain(AMP_CHANNEL4, gain) )
    {
        return false;
    }
    return true;
}

bool AmpliferManager::setChannelGain(AmpChannel_t channel, ChannelGain_t gain)
{
qDebug() << "===> setChannelGain(" << channel << "," << gain << ") = " << QString("%1").arg(CHANNEL_GAIN(channel, gain) , 0, 16);

    if(!m_cgos->writeI2CRegister( TAS5414C_ADDR, 0x08, CHANNEL_GAIN(channel, gain) )) 
    {
        PRINT_CONSOLE_MESSAGE_WARN( QString("Can't set channel %1 gain (%2)").arg(channel).arg(gain) );
        return false;
    }

    return true;
}

bool AmpliferManager::muteChannel(AmpChannel_t channel, bool mute)
{
    uint8_t byte = 0;

    if ( ! m_cgos->readI2CRegister( TAS5414C_ADDR, 0x0C, byte ) )
    {
        PRINT_CONSOLE_MESSAGE_WARN( "Can't read control register" );
        return false;
    }

//qDebug() << "=====> Control: " <<  bin << byte;

    if(mute)
    {
//qDebug() << "=====> Mute channel " << channel << "," << mute << ")";
        if(!IS_MUTED_CHANNEL(channel, byte))
        {
            byte = MUTE_CHANNEL(channel, byte);
        }
    }
    else 
    {
//qDebug() << "=====> UnMute channel " << channel << "," << mute << ")";
        if(IS_MUTED_CHANNEL(channel, byte))
        {
            byte = UNMUTE_CHANNEL(channel, byte);
        }
    }

//qDebug() << "=====> Control2: " <<  bin << byte;

    if(!m_cgos->writeI2CRegister( TAS5414C_ADDR, 0x0C, byte )) 
    {
        PRINT_CONSOLE_MESSAGE_WARN( "Unable to update control register" );
        return false;
    }

    
    return true;
}

AmpTempLevel_t AmpliferManager::getTemperature()
{
    uint8_t byte;

    if ( ! m_cgos->readI2CRegister( TAS5414C_ADDR, 0x04, byte ) )
    {
        PRINT_CONSOLE_MESSAGE_WARN( "Can't read external status register" );
        return AMP_TEMP_UNKNOWN;
    }
if(byte != 0)
{
    qDebug() << "getTemperature(1) = " << hex << byte;
    byte >>= 5;
    qDebug() << "getTemperature(2) = " << hex << byte;
    byte &= 7;
    qDebug() << "getTemperature(3) = " << hex << byte;
}
   
    return static_cast<AmpTempLevel_t>(byte);
}