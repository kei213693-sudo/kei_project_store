/*

Copyright 2020 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/
#ifndef _APP_PREINCLUDE_COMMON_H_
#define _APP_PREINCLUDE_COMMON_H_

/* Radio drivers support both Gen 3.5 and Gen 4.0. */
#ifndef RF_OSC_26MHZ
#define RF_OSC_26MHZ        0
#endif /* RF_OSC_26MHZ */

/* Enable Memory manager light heap extention for All BLE Applications
 *  Overide default configuration in fsl_component_mem_manager.c file.
 *  The memory Allocator will use all the available space in RAM for the Heap section.
 *  It requires specific symbol and placement in linker script
 *  For each Application, you can now set MinimalHeapSize_c to ensure minimal heap size is available
 *  at link time */
#ifndef gMemManagerLightExtendHeapAreaUsage
#define gMemManagerLightExtendHeapAreaUsage   1
#endif

#endif /* _APP_PREINCLUDE_COMMON_H_ */