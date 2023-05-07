#ifndef __MNT_DEFS_MFI_H__
#define __MNT_DEFS_MFI_H__

#define DIGITAL_OUTPUTS_COUNT   2
#define MFI_COUNT               9

#pragma pack(1)

#ifdef __ICCARM__
typedef enum
#else 
typedef enum : uint8_t
#endif
{
  DIG_IN = 0,
  FREQ_IN = 1,
  PULSE_IN = 2,
  ANALOG_IN = 3,
}EMFIType;

#ifdef __ICCARM__
typedef enum
#else 
typedef enum : uint8_t
#endif
{
  ACTIVE_0 = 0,
  ACTIVE_1 = 1,
}EMFIActivePolarity;

typedef struct
{
  EMFIType MFIType;
  EMFIActivePolarity MFIActivePolarity;
}TMFISettings;

typedef struct
{
  EMFIType MfiType;
  uint32_t MfiValue;
}TMFIValues;

#pragma pack()

#endif