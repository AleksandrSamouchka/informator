#ifndef __MNT_DEFS_CAN_H__
#define __MNT_DEFS_CAN_H__

#include <stdint.h>

#define CAN_COUNT               3
#define CAN_ID_FILTER_COUNT     14


#define DEFAULT_CAN_BAUDRATE   250

#pragma pack(1)

#ifdef __ICCARM__
typedef enum
#else 
typedef enum : uint8_t
#endif
{
  EMPTY_CAN_ID = 0,
  STD_CAN_ID, 
  EXT_CAN_ID,
}EIDType;

typedef struct
{
  EIDType      IdType;    
  uint32_t     Id;        
  uint8_t      UseMask;   
  uint32_t     Mask;
}TCANIdFilter;

typedef struct
{
  uint32_t baudrate;
  TCANIdFilter IdFilter[CAN_ID_FILTER_COUNT];
}TCANSettings;


typedef struct
{
  uint32_t   time;
  uint32_t   id;
  uint8_t    id_type;
  uint8_t    dlen;
  uint8_t    data[8];
}TCANFrame;
#pragma pack()


#endif
