#ifndef __AMPLIFER_MANAGER_HPP__
#define __AMPLIFER_MANAGER_HPP__

#include <QObject>

#include <CCgosHandler/CCgosHandler.h>

//#include    "defs.h"

typedef enum
{
    UNKNOWN_CHANNEL = 0,
    AMP_CHANNEL1,
    AMP_CHANNEL2,
    AMP_CHANNEL3,
    AMP_CHANNEL4
} AmpChannel_t;

typedef enum
{
    GAIN_12DB = 0x00,
    GAIN_20DB = 0x01,
    GAIN_26DB = 0x02,
    GAIN_32DB = 0x03
} ChannelGain_t;


typedef enum
{
    AMP_TEMP_UNKNOWN = -1,
    AMP_TEMP_OK = 0,
    AMP_TEMP_WARNING = 1,
    AMP_TEMP_LEVEL_1 = 3,
    AMP_TEMP_LEVEL_2 = 5,
    AMP_TEMP_LEVEL_3 = 7
} AmpTempLevel_t;

class AmpliferManager : public QObject
{
    Q_OBJECT

    public:
        AmpliferManager( QObject *parent, CCgosHandler *cgos);
        virtual ~AmpliferManager();

        bool init();
//        bool mute();
//        bool unmute();

        bool setGain(ChannelGain_t gain);  
        bool setChannelGain(AmpChannel_t channel, ChannelGain_t gain);
        bool muteChannel(AmpChannel_t, bool mute);

        AmpTempLevel_t getTemperature();

    private:
        CCgosHandler *m_cgos;
};


#endif
