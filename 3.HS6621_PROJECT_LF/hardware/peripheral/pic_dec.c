/**
 * @file example_pic_dec.c
 * @brief example for picture decoder
 * @date 2017/07/12 15:54:27
 * @author tangtanglin
 *
 * @defgroup EXAMPLE_PIC_DEC Picture Decoder Example
 * @ingroup EXAMPLE
 * @brief example for picture decoder
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"

/*********************************************************************
 * MACROS
 */


/*********************************************************************
 * PUBLIC FUNCTIONS
 */
 
/**
 * @brief uncompress pic data from 8bits to 16bits
 *
 * @param[in] table          compress table
 * @param[in] src            compressed data
 * @param[in] dst            origin data
 * @param[in] len_2bytes     len with 16bits
 *
 * @return None
 **/
void pic_uncompress(uint8_t *table, const uint8_t *src, uint8_t *dst, uint32_t len_2bytes )
{
    co_assert(((uint32_t)table & 3) == 0);
    co_assert(((uint32_t)src & 1) == 0);
    co_assert(((uint32_t)dst & 1) == 0);

    /* open pic_dec clock */
    HS_PSO->PIC_DEC_CFG = 0;

    if(table)
    {
        /* set table_address */
        HS_PIC_DEC->PIC_DEC_TABLE_ADDR = (uint32_t)table;
    }

    /* set enc_data_address */
    HS_PIC_DEC->PIC_DEC_ENC_DATA_ADDR = (uint32_t)src;

    /* set dec_data_address */
    HS_PIC_DEC->PIC_DEC_DEC_DATA_ADDR = (uint32_t)dst;

    /* set dec_data_len*/
    HS_PIC_DEC->PIC_DEC_DATA_LEN = len_2bytes;

    /* set the reg to start move table and dec data ----save table and data to ram */
    HS_PIC_DEC->PIC_DEC_FETCH_START = 1;

    /* start decompress the compressed data  */
    HS_PIC_DEC->PIC_DEC_START = 1;

    /* wait the operation done--- bit:1 means done */
    while(!(HS_PIC_DEC->PIC_DEC_INT & PIC_DEC_INT_STATUS_MASK));
    /* clear */
    HS_PIC_DEC->PIC_DEC_INT = PIC_DEC_INT_STATUS_MASK;
}

/** @} */

