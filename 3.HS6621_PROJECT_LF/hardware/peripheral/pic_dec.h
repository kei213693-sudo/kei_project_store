/**
 * @file pic_dec.h
 * @brief picture decoder driver
 * @date Fri, Jul 12, 2019  3:16:15 PM
 * @author tangtanglin
 *
 * @defgroup pic_dec
 * @ingroup PERIPHERAL
 * @brief picture decoder driver
 * @details picture decoder driver
 *
 * @{
 *
 * @example example_pic_dec.c
 * This is an example of how to use the picture decoder
 *
 */

#ifndef __PIC_DEC_H__
#define __PIC_DEC_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"

/*********************************************************************
 * MACROS
 */


/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * EXTERN VARIABLES
 */


/*********************************************************************
 * EXTERN FUNCTIONS
 */

/**
 * @brief uncompress pic data from 8bits to 16bits
 *
 * @param[in] table        compress table
 * @param[in] src          compressed data
 * @param[in] dst          origin data
 * @param[in] len_2bytes   len with 16bits
 *
 * @return None
 **/
void pic_uncompress(uint8_t *table, const uint8_t *src, uint8_t *dst, uint32_t len_2bytes );

#ifdef __cplusplus
}
#endif

#endif

/** @} */

