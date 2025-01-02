/**
 * @file ir_dma.h
 * @brief
 * @date Wed, Sep  5, 2018  5:44:25 PM
 * @author liqiang
 *
 * @addtogroup APP_EXAMPLES_RC_IR_DMA_H ir_dma.h
 * @ingroup BLE_APP_RC
 * @brief the rc of interface
 * @details
 *
 * @{
 */

#ifndef __IR_DMA_H__
#define __IR_DMA_H__

/**
 ****************************************************************************************
 * @brief Initialize the IR
 * @return void.
 ****************************************************************************************
 */
void ir_dma_init(void);
/**
 ****************************************************************************************
 * @brief send ir data
 * @return void.
 ****************************************************************************************
 */
void ir_dma_send(uint8_t ir_code);
/**
 ****************************************************************************************
 * @brief stop ir send
 * @return void.
 ****************************************************************************************
 */
void ir_dma_stop(void);

#endif
/** @} */

