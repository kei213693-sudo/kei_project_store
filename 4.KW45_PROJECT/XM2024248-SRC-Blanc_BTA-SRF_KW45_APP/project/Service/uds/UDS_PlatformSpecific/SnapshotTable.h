/* This file was automatically generated */

#ifndef SNAPSHOTTABLE_H_
#define SNAPSHOTTABLE_H_
#include "typedefs.h"

    typedef  struct
    {
        const uint16_t  *group;
        uint16_t        count;
        uint32_t        length;
    }Snapshot_Info;

    #define SNAPSHOT_TOTAL_GROUP_NUM 10u
    extern const Snapshot_Info snapshot_group_map[];

#endif
