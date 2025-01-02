/**
 * @file    ir_dma.c
 * @brief   Using PWM DMA to send IR
 * @date    2020-04-27
 * @author  Cheng Yongfei
 *
 * @addtogroup APP_EXAMPLES_RC_IR_DMA ir_dma.c
 * @ingroup BLE_APP_RC
 * @brief ir functions
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"
#include "ir_dma.h"


/*********************************************************************
 * MACROS
 */
 /// the ccr block size
#define CCR_BLOCK_SIZE        512//1024
/// the ccr dma buffer size
#define CCR_DMA_BUFFER_SIZE (1*512)//  (4*1024)
/// the ccr block num
#define CCR_BLOCK_NUM         (CCR_DMA_BUFFER_SIZE/CCR_BLOCK_SIZE)


/*********************************************************************
 * TYPEDEFS
 */
 /// the ir's config of timer
typedef struct
{
    bool           use_fifo;    /**< fifo buffer */
    void          *buffer;      /**< DMA fifo buffer */
    uint32_t       buffer_len;  /**< DMA buffer len */
    dma_llip_t    *block_llip;  /**< block llip */
    uint32_t       block_num;   /**< block num */
    __IO uint32_t *tim_addr;    /**< Absolute address of the TIM register to access */
    uint32_t       num;         /**< number of registers under updating //DMA burst length */
    uint32_t       req;         /**< TIM DMA request source: TIM_DIER_UDE, TIM_DIER_CC1DE, ..., TIM_DIER_TDE */
    dma_callback_t callback;    /**<  DMA event callback */
} ir_tim_dma_config_t;


/*********************************************************************
 * CONSTANTS
 */
/// the pwm config
const tim_config_t pwm_cfg =
{
    .mode = TIM_PWM_MODE,
    .config.pwm =
    {
        .count_freq = 64000000,
        .period_count = 1684,
        .channel =
        {
            {true,  {TIM_PWM_POL_LOW2HIGH, 1684}},
            {false, {TIM_PWM_POL_HIGH2LOW, 5000}},
            {false, {TIM_PWM_POL_HIGH2LOW, 5000}},
            {false, {TIM_PWM_POL_HIGH2LOW, 5000}},
        },
        .callback = NULL,
    },
};


/*********************************************************************
 * LOCAL VARIABLES
 */
 /// the ccr dma buffer
CO_ALIGN(4) uint16_t ccr_dma_buffer[CCR_DMA_BUFFER_SIZE] = {0}; //DMA data buffer
/// the block llip
DMA_DECLARE_LLIP(ccr_dma_block_llip, CCR_BLOCK_NUM);
/// the timer dma
static HS_DMA_CH_Type *tim_dma = NULL;
/// the dma fifo
static dma_fifo_t ccr_dma_fifo;
/// the pwm dma callback
static void pwm_dma_cb(dma_status_t status, uint32_t cur_src_addr, uint32_t cur_dst_addr, uint32_t xfer_size);
/// the dma config
static ir_tim_dma_config_t tim_dma_cfg = {
    .use_fifo   = false,
    .buffer     = ccr_dma_buffer,
    .buffer_len = sizeof(ccr_dma_buffer),
    .block_llip = ccr_dma_block_llip,
    .block_num  = CCR_BLOCK_NUM,
    .tim_addr   = &HS_TIM0->CCR[0],
    .num        = 1,
    .req        = (1 << 8),
    .callback   = pwm_dma_cb,
};
/// arr value
static uint32_t arr_val;
/// the data index
static uint16_t data_idx;

static bool       ir_send_flag; /**< send flag */
static bool       ir_stop_dealy_flag;   /**< stop dealy flag */
static co_timer_t ir_repeat_timer;  /**< repeat timer */
static uint16_t   ir_repeat_count;  /**< repeat count */


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void pwm_dma_cb(dma_status_t status, uint32_t cur_src_addr, uint32_t cur_dst_addr, uint32_t xfer_size)
{
    tim_stop(HS_TIM0);
    dma_stop(tim_dma);
    dma_release(tim_dma);
    tim_dma = NULL;
    tim_pwm_channel_force_output(HS_TIM0, TIM_PWM_CHANNEL_0, TIM_PWM_FORCE_LOW);
//    log_debug("rxdma(%d): %08X->%08X [%d]\n", status, cur_src_addr, cur_dst_addr, xfer_size);
}

static HS_DMA_CH_Type* ir_tim_dma_config(HS_TIM_Type *tim, HS_DMA_CH_Type *dma, const ir_tim_dma_config_t*config)
{
    dma_id_t dma_id;

    if(dma == NULL)
        dma = dma_allocate();

    dma_id = tim==HS_TIM0 ? TIMER0_DMA_ID : (tim==HS_TIM1 ? TIMER1_DMA_ID : TIMER2_DMA_ID);

    if(dma)
    {
        bool res;
        dma_config_t dconfig;

        dconfig.slave_id       = dma_id;
        dconfig.direction      = DMA_MEM_TO_DEV;
        dconfig.src_addr_width = DMA_SLAVE_BUSWIDTH_16BITS;
        dconfig.dst_addr_width = DMA_SLAVE_BUSWIDTH_16BITS;
        dconfig.src_burst      = DMA_BURST_LEN_1UNITS;
        dconfig.dst_burst      = DMA_BURST_LEN_1UNITS;
        dconfig.dev_flow_ctrl  = false;
        dconfig.priority       = 0;
        dconfig.callback       = config->callback;

        dconfig.lli.enable     = false;
        dconfig.lli.use_fifo   = config->use_fifo;
        dconfig.lli.src_addr   = (uint32_t)config->buffer;
        dconfig.lli.dst_addr   = (uint32_t)&tim->DMAR;
        dconfig.lli.block_num  = config->block_num;
        dconfig.lli.block_len  = config->buffer_len/config->block_num;
        dconfig.lli.llip       = config->block_llip;

        res = dma_config(dma, &dconfig);
        if(!res)
        {
            dma_release(dma);
            return NULL;
        }

        tim->DCR = ((config->num - 1)<<8) | ((uint32_t)config->tim_addr-(uint32_t)tim)/4;
        tim->DIER |= config->req;
    }

    return dma;
}

static void ir_data_set_header(void)
{
    uint16_t idx = 0;

    for(; idx<342; idx++)
    {
        ccr_dma_buffer[data_idx++] = arr_val>>1;
    }
    for(; idx<513; idx++)
    {
        ccr_dma_buffer[data_idx++] = arr_val;
    }
}

static void ir_data_set_bit_0(void)
{
    uint8_t idx = 0;

    for(; idx<21; idx++)
    {
        ccr_dma_buffer[data_idx++] = arr_val>>1;
    }
    for(; idx<42; idx++)
    {
        ccr_dma_buffer[data_idx++] = arr_val;
    }
}

static void ir_data_set_bit_1(void)
{
    uint8_t idx = 0;

    for(; idx<21; idx++)
    {
        ccr_dma_buffer[data_idx++] = arr_val>>1;
    }
    for(; idx<82; idx++)
    {
        ccr_dma_buffer[data_idx++] = arr_val;
    }
}

static void ir_data_set_tail(void)
{
    uint8_t idx = 0;

    for(; idx<21; idx++)
    {
        ccr_dma_buffer[data_idx++] = arr_val>>1;
    }
}

static void ir_data_set(uint8_t ir_code)
{
    uint32_t ir_data = ((~ ir_code)<<24 | ir_code<<16 | IR_USER_CODE);
    
    data_idx = 0;
    ir_data_set_header();
//    log_debug("data_idx = %d\n", data_idx);

    for(int i=0; i<32; i++)
    {
        if(ir_data & (1<<i))
            ir_data_set_bit_1();
        else
            ir_data_set_bit_0();
//        log_debug("data_idx = %d\n", data_idx);
    }

    ir_data_set_tail();

    ccr_dma_buffer[data_idx] = arr_val;//dummy one
//    log_debug("data_idx = %d\n", data_idx);

//    for(int i=0; i<data_idx; i++)
//    {
//        if(i%16 == 0)
//            log_debug("\n");
//        log_debug("%04d ", ccr_dma_buffer[i]);
//    }
}

static void ir_repeat_data_set(void)
{
    data_idx = 0;

    uint16_t idx = 0;

    for(; idx<342; idx++)
    {
        ccr_dma_buffer[data_idx++] = arr_val>>1;
    }
    for(; idx<427; idx++)
    {
        ccr_dma_buffer[data_idx++] = arr_val;
    }
    for(; idx<448; idx++)
    {
        ccr_dma_buffer[data_idx++] = arr_val>>1;
    }
    ccr_dma_buffer[data_idx] = arr_val;//dummy one
}

static void ir_repeat_timer_cb(co_timer_t *timer, void *param)
{
    ir_repeat_count++;
    if(ir_stop_dealy_flag && ir_repeat_count>1)
    {
        ir_dma_stop();
        return;
    }

//    log_debug("%s @ %d\n", __FUNCTION__, __LINE__);

    ir_dma_init();

    ir_repeat_data_set();

    dma_start(tim_dma, (uint32_t)tim_dma_cfg.buffer, (uint32_t)&HS_TIM0->DMAR, data_idx);
    tim_start(HS_TIM0);
}


/*********************************************************************
 * PUBLIC FUNCTIONS
 */
void ir_dma_init(void)
{
    pinmux_config(PIN_IR_CONTROL, PINMUX_TIMER0_IO_0_CFG);

    dma_fifo_init(&ccr_dma_fifo, ccr_dma_buffer, sizeof(ccr_dma_buffer));
    dma_init();
    tim_config(HS_TIM0, &pwm_cfg);
    tim_dma = ir_tim_dma_config(HS_TIM0, tim_dma, &tim_dma_cfg);
    
    arr_val = HS_TIM0->ARR+1;
//    log_debug("arr_val = %d\n", arr_val);
}

void ir_dma_send(uint8_t ir_code)
{
    if(ir_send_flag)
        return;//ir is sending now

//    log_debug("%s @ %d\n", __FUNCTION__, __LINE__);

    ir_repeat_count = 0;
    co_timer_set(&ir_repeat_timer, 108, TIMER_REPEAT, ir_repeat_timer_cb, NULL);

    ir_dma_init();

    ir_data_set(ir_code);

    dma_start(tim_dma, (uint32_t)tim_dma_cfg.buffer, (uint32_t)&HS_TIM0->DMAR, data_idx);
    tim_start(HS_TIM0);
    ir_send_flag = true;
    ir_stop_dealy_flag = false;
}

void ir_dma_stop(void)
{
    if(!ir_send_flag)
        return;//ir is already stopped

    if(ir_repeat_count < 2)
    {
        ir_stop_dealy_flag = true;
        return;//must send one ir repeat frame at least
    }

//    log_debug("%s @ %d\n", __FUNCTION__, __LINE__);

    tim_stop(HS_TIM0);
    dma_stop(tim_dma);
    dma_release(tim_dma);
    tim_dma = NULL;

    co_timer_del(&ir_repeat_timer);
    ir_send_flag = false;
}

/** @} */
