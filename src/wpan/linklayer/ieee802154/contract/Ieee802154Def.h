#ifndef IEEE_802154_DEF_H
#define IEEE_802154_DEF_H

#include <stdint.h>
#include "MACAddress.h"
typedef uint8_t       UINT_8;
typedef uint16_t      UINT_16;
typedef uint32_t      UINT_32;
typedef MACAddress    IE3ADDR;    //actually this should be a 64-bit address, but we use 16-bit address here for convenience (in this way, we don't need to distinguish IEEE address from short logical address or worry about platform compatibility problems caused by 64-bit integer or byte order)

#endif


