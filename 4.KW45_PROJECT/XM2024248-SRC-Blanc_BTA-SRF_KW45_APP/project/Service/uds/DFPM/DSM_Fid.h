#ifndef DSM_FID_H
#define DSM_FID_H
#include "typedefs.h"

typedef enum
{
    DSM_FID_UNUSE= 0u,
}Fid_Type;
extern const uint16_t FID_inhabitTable[1][32];
extern const uint16_t DSM_FID_NUM;
extern void DSM_fidInit(void);

#endif
