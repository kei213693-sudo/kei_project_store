/**
 * @file i2c.c
 * @brief
 * @date Wed 31 May 2017 07:15:09 PM CST
 * @author liqiang
 *
 * @addtogroup
 * @ingroup
 * @details
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"
#include "co.h"

/*********************************************************************
 * MACROS
 */
#define I2C_SLAVE_DEFAULT_ADDR  0x62

#define I2C_TX_FIFO_DEPTH       16
#define I2C_RX_FIFO_DEPTH       16

#define I2CD_NO_ERROR           0x00   /**< @brief No error.            */
#define I2CD_BUS_ERROR          0x01   /**< @brief Bus Error.           */
#define I2CD_ARBITRATION_LOST   0x02   /**< @brief Arbitration Lost.    */
#define I2CD_ACK_FAILURE        0x04   /**< @brief Acknowledge Failure. */
#define I2CD_OVERRUN            0x08   /**< @brief Overrun/Underrun.    */
#define I2CD_PEC_ERROR          0x10   /**< @brief PEC Error in reception. */
#define I2CD_TIMEOUT            0x20   /**< @brief Hardware timeout.    */
#define I2CD_SMB_ALERT          0x40   /**< @brief SMBus Alert.         */

#define I2C_INTR_DEFAULT_MASK   (I2C_INTR_RX_FULL | I2C_INTR_TX_EMPTY | I2C_INTR_TX_ABRT | I2C_INTR_STOP_DET)
#define I2C_INTR_SLAVE_MASK     (I2C_INTR_RD_REQ | I2C_INTR_TX_ABRT | I2C_INTR_RX_FULL | I2C_INTR_STOP_DET)

#define I2C_DEFAULT_TIMEOUT     330000 // 0.5s@CPU64MHz

#define I2C_DEAD_WORKAROUND

/*********************************************************************
 * TYPEDEFS
 */

typedef struct
{
    __IO uint32_t *cpm_cfg;
    cpm_clk_t      cpm_type;
    pinmux_t       pinmux_sck;
    pinmux_t       pinmux_sda;
} i2c_friend_t;

typedef struct
{
    uint8_t *tx_buf1;
    uint32_t tx_len1;
    uint8_t *tx_buf2;
    uint32_t tx_len2;

    uint8_t *rx_buf;
    uint32_t rx_len;
}i2c_op_t;

#ifdef I2C_DEAD_WORKAROUND
typedef struct
{
    uint32_t speed[I2C_MODULE_NUM];
}i2c_env_t;
#endif

/*********************************************************************
 * CONSTANTS
 */
static const i2c_friend_t i2c_friend_tbl[I2C_MODULE_NUM] =
{
    {&HS_PSO->I2C_CFG, CPM_I2C_CLK,   PINMUX_I2C_MST_SCK_CFG, PINMUX_I2C_MST_SDA_CFG},
#if I2C_MODULE_NUM > 1
    {&HS_PSO->I2C1_CFG, CPM_I2C1_CLK, PINMUX_I2C1_MST_SCK_CFG,PINMUX_I2C1_MST_SDA_CFG},
#endif
#if I2C_MODULE_NUM > 2
    {&HS_PSO->I2C2_CFG, CPM_I2C2_CLK, PINMUX_I2C2_MST_SCK_CFG,PINMUX_I2C2_MST_SDA_CFG},
#endif
};

/*********************************************************************
 * LOCAL VARIABLES
 */
#ifdef I2C_DEAD_WORKAROUND
static i2c_env_t i2c_env;
#endif

/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */

/**
 * @brief  i2c index
 *
 * @param[in] i2c  i2c
 *
 * @return index
 **/
static int i2c_index(HS_I2C_Type *i2c)
{
    return i2c==HS_I2C  ? 0 :
#if I2C_MODULE_NUM > 2
           i2c==HS_I2C2 ? 2 :
#endif
#if I2C_MODULE_NUM > 3
#error I2C_MODULE_NUM too larger!
#endif
                        1;
}

#ifdef I2C_DEAD_WORKAROUND
/**
 * @brief  i2c dead workaround
 **/
static void i2c_dead_workaround(HS_I2C_Type *i2c)
{
    int unlock_timeout = 200;
    int index = i2c_index(i2c);
    pinmux_t pinmux_sck = i2c_friend_tbl[index].pinmux_sck;
    pinmux_t pinmux_sda = i2c_friend_tbl[index].pinmux_sda;

    // get pin number
    int scl_pin = pinmux_search(pinmux_sck);
    int sda_pin = pinmux_search(pinmux_sda);
    co_assert (scl_pin>=0 && sda_pin>=0);

    // pinmux to  gpio
    pinmux_config(scl_pin, PINMUX_GPIO_MODE_CFG);
    pinmux_config(sda_pin, PINMUX_GPIO_MODE_CFG);

    // gpio mode
    gpio_set_direction(BITMASK(scl_pin), GPIO_OUTPUT);
    gpio_set_direction(BITMASK(sda_pin), GPIO_INPUT);

    // unlock
    while((gpio_read(BITMASK(sda_pin)) == 0) && (--unlock_timeout > 0))
    {
        gpio_toggle(BITMASK(scl_pin));
        co_delay_10us(10);
    }

    // restore
    pinmux_config(scl_pin, pinmux_sck);
    pinmux_config(sda_pin, pinmux_sda);

    // reset
    i2c_open_x(i2c, I2C_MODE_MASTER, i2c_env.speed[index]);
}
#endif

/**
 * @brief i2c_set_mode()
 *
 * @param[in] mode
 *
 * @return
 **/
static void i2c_set_mode(HS_I2C_Type *i2c, i2c_mode_t mode)
{
    switch(mode)
    {
        case I2C_MODE_MASTER:
            i2c->CON = I2C_CON_MASTER | I2C_CON_SLAVE_DISABLE | I2C_CON_RESTART_EN;
            break;
        case I2C_MODE_SLAVE:
            i2c->CON = I2C_CON_RESTART_EN;
            break;
    }
}

/**
 * @brief i2c_scl_hcnt()
 *
 * @param[in] ic_clk
 * @param[in] tsymbol
 * @param[in] tf
 * @param[in] cond
 * @param[in] offset
 *
 * @return
 **/
static uint32_t i2c_scl_hcnt(uint32_t ic_clk, uint32_t tsymbol, uint32_t tf, int cond, int offset)
{
    /*
     * DesignWare I2C core doesn't seem to have solid strategy to meet
     * the tHD;STA timing spec.  Configuring _HCNT based on tHIGH spec
     * will result in violation of the tHD;STA spec.
     */
    if (cond)
        /*
         * Conditional expression:
         *
         *   IC_[FS]S_SCL_HCNT + (1+4+3) >= IC_CLK * tHIGH
         *
         * This is based on the manuals, and represents an ideal
         * configuration.  The resulting I2C bus speed will be
         * faster than any of the others.
         *
         * If your hardware is free from tHD;STA issue, try this one.
         */
        return (ic_clk * tsymbol + 5000) / 10000 - 8 + offset;
    else
        /*
         * Conditional expression:
         *
         *   IC_[FS]S_SCL_HCNT + 3 >= IC_CLK * (tHD;STA + tf)
         *
         * This is just experimental rule; the tHD;STA period turned
         * out to be proportinal to (_HCNT + 3).  With this setting,
         * we could meet both tHIGH and tHD;STA timing specs.
         *
         * If unsure, you'd better to take this alternative.
         *
         * The reason why we need to take into account "tf" here,
         * is the same as described in i2c_lld_scl_lcnt().
         */
        return (ic_clk * (tsymbol + tf) + 5000) / 10000 - 3 + offset;
}

/**
 * @brief i2c_scl_lcnt()
 *
 * @param[in] ic_clk
 * @param[in] tLOW
 * @param[in] tf
 * @param[in] offset
 *
 * @return
 **/
static uint32_t i2c_scl_lcnt(uint32_t ic_clk, uint32_t tlow, uint32_t tf, int offset)
{
    /*
     * Conditional expression:
     *
     *   IC_[FS]S_SCL_LCNT + 1 >= IC_CLK * (tLOW + tf)
     *
     * DW I2C core starts counting the SCL CNTs for the LOW period
     * of the SCL clock (tLOW) as soon as it pulls the SCL line.
     * In order to meet the tLOW timing spec, we need to take into
     * account the fall time of SCL signal (tf).  Default tf value
     * should be 0.3 us, for safety.
     */
    return ((ic_clk * (tlow + tf) + 5000) / 10000) - 1 + offset;
}

/**
 * @brief i2c_set_speed()
 *
 * @param[in] i2cp
 *
 * @return
 **/
static void i2c_set_speed(HS_I2C_Type *i2c, uint32_t speed)
{
    uint32_t clk = cpm_get_clock(CPM_I2C_CLK);

    if (speed <= 100000) //100kbps
    {
        /* set standard and fast speed deviders for high/low periods */
        /* Standard-mode @100k period=10us */
        i2c->SS_SCL_HCNT = i2c_scl_hcnt(clk/1000,
                40,/* tHD;STA = tHIGH = 4.0 us */
                3, /* tf = 0.3 us */
                0, /* 0: default, 1: Ideal */
                0);/* No offset */
        i2c->SS_SCL_LCNT = i2c_scl_lcnt(clk/1000,
                47,/* tLOW = 4.7 us */
                3, /* tf = 0.3 us */
                0);/* No offset */

        /* Standard mode clock_div calculate: Tlow/Thigh = 1/1.*/
        /* Sets the Maximum Rise Time for standard mode.*/
        i2c->CON = (i2c->CON & ~I2C_CON_SPEED_MASK) | I2C_CON_SPEED_STD;
    }
    else// if (speed <= 400000) //400kbps
    {
        /* Fast-mode @400k period=2.5us */
        i2c->FS_SCL_HCNT = i2c_scl_hcnt(clk/1000,
                6, /* tHD;STA = tHIGH = 0.6 us */
                3, /* tf = 0.3 us */
                0, /* 0: default, 1: Ideal */
                0);/* No offset */
        i2c->FS_SCL_LCNT = i2c_scl_lcnt(clk/1000,
                13,/* tLOW = 1.3 us */
                3, /* tf = 0.3 us */
                0);/* No offset */

        /* Sets the Maximum Rise Time for fast mode.*/
        i2c->CON = (i2c->CON & ~I2C_CON_SPEED_MASK) | I2C_CON_SPEED_FAST;
    }
}

/**
 * @brief i2c_read_clear_intrbits()
 *
 * @param[in] err  
 *
 * @return 
 **/
static uint32_t i2c_read_clear_intrbits(HS_I2C_Type *i2c, uint32_t *err)
{
    uint32_t dummy, stat = i2c->INTR_STAT;
    uint32_t errors = 0;

    /* Do not use the IC_CLR_INTR register to clear interrupts. */
    if (stat & I2C_INTR_RX_UNDER)
    {
        errors |= I2CD_OVERRUN;
        dummy = i2c->CLR_RX_UNDER;
    }

    if (stat & I2C_INTR_RX_OVER)
    {
        errors |= I2CD_OVERRUN;
        dummy = i2c->CLR_RX_OVER;
    }

    if (stat & I2C_INTR_TX_OVER)
    {
        errors |= I2CD_OVERRUN;
        dummy = i2c->CLR_TX_OVER;
    }

    if (stat & I2C_INTR_RD_REQ)
        dummy = i2c->CLR_RD_REQ;

    if (stat & I2C_INTR_TX_ABRT)
    {
        /*
         * The IC_TX_ABRT_SOURCE register is cleared whenever
         * the IC_CLR_TX_ABRT is read.  Preserve it beforehand.
         */
        dummy = i2c->TX_ABRT_SOURCE;
        if (dummy & I2C_TX_ARB_LOST)
            errors |= I2CD_ARBITRATION_LOST;
        if (dummy & 0x1f/*I2C_TX_ABRT_xxx_NOACK*/)
            errors |= I2CD_ACK_FAILURE;
        if (dummy & 0xfe0)
            errors |= I2CD_BUS_ERROR; /* it is trigged by wrong sw behaviours */
        dummy = i2c->CLR_TX_ABRT;
    }

    if (stat & I2C_INTR_RX_DONE)
        dummy = i2c->CLR_RX_DONE;

    if (stat & I2C_INTR_ACTIVITY)
        dummy = i2c->CLR_ACTIVITY;

    if (stat & I2C_INTR_STOP_DET)
        dummy = i2c->CLR_STOP_DET;

    if (stat & I2C_INTR_START_DET)
        dummy = i2c->CLR_START_DET;

    if (stat & I2C_INTR_GEN_CALL)
        dummy = i2c->CLR_GEN_CALL;

    if(err)
        *err |= errors;

    (void)dummy;
    return stat;
}

/**
 * @brief i2c_read_pio()
 *
 * @param[in] op  
 *
 * @return 
 **/
static void i2c_read_pio(HS_I2C_Type *i2c, i2c_op_t *op)
{
    uint32_t rx_valid;

    if(op->tx_len1>0 || op->tx_len2>0)
        return;

    if(op->rx_len==0)
        return;

    rx_valid = i2c->RXFLR;

    for (; op->rx_len > 0 && rx_valid > 0; op->rx_len--, rx_valid--)
        *op->rx_buf++ = i2c->DATA_CMD;
}

/**
 * @brief i2c_xfer_pio()
 *
 * @param[in] op  
 *
 * @return 
 **/
static void i2c_xfer_pio(HS_I2C_Type *i2c, i2c_op_t *op)
{
    uint32_t tx_limit;

    if(op->tx_len1==0 && op->tx_len2==0)
        return;

    tx_limit = I2C_TX_FIFO_DEPTH - i2c->TXFLR;

    for(; op->tx_len1 > 0 && tx_limit > 0; tx_limit--,op->tx_len1--)
        i2c->DATA_CMD = *op->tx_buf1++;

    if(op->tx_len1 == 0)
    {
        for(; op->tx_len2 > 0 && tx_limit > 0; tx_limit--,op->tx_len2--)
            i2c->DATA_CMD = *op->tx_buf2++;
    }

    if(op->tx_len1==0 && op->tx_len2==0)
    {
        if(op->rx_len > 0)
        {
            // BUG: tx to rx must be delay
            co_delay_us(9);
            i2c->CON1 = op->rx_len | I2C_CON1_RX_ENABLE | I2C_CON1_READBYTES_UPDATE;
        }
    }
}

/**
 * @brief i2c_serve_interrupt()
 *
 * @param[in] op  
 * @param[in] errors  
 *
 * @return 
 **/
static bool i2c_serve_interrupt(HS_I2C_Type *i2c, i2c_op_t *op, uint32_t *errors)
{
    uint32_t stat;

    stat = i2c_read_clear_intrbits(i2c, errors);

    if (stat & I2C_INTR_RX_FULL)
        i2c_read_pio(i2c, op);

    if (stat & (I2C_INTR_TX_EMPTY | I2C_INTR_RD_REQ))
        i2c_xfer_pio(i2c, op);

    if (stat & I2C_INTR_STOP_DET)
    {
        i2c->ENABLE = 0;
        return true;
    }

    return false;
}

/**
 * @brief i2c_master_transmit_ex()
 *
 * @param[in] addr  
 * @param[in] op  
 * @param[in] timeout  
 *
 * @return 
 **/
static bool i2c_master_transmit_ex(HS_I2C_Type *i2c, uint16_t addr, i2c_op_t *op, uint32_t timeout)
{
    uint32_t errors = 0;
    uint32_t count = 0;
    __IO uint32_t dummy;
    bool res;

    i2c->ENABLE = 0;
    i2c->TAR = addr;
    i2c->CON1 = op->tx_len1 ? I2C_CON1_TX_ENABLE : (op->rx_len | I2C_CON1_RX_ENABLE | I2C_CON1_READBYTES_UPDATE);
    i2c->ENABLE = 1;
    dummy = i2c->CLR_INTR; (void) dummy;
    i2c->INTR_MASK = I2C_INTR_DEFAULT_MASK;

    while(1)
    {
        res = i2c_serve_interrupt(i2c, op, &errors);
        if(res)
            break;

        if(timeout && count++>timeout)
        {
            errors |= I2CD_TIMEOUT;

#ifdef I2C_DEAD_WORKAROUND
            i2c_dead_workaround(i2c);
#endif

            break;
        }
    }

    return errors ? false : true;
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/**
 * @brief i2c initialize
 *
 * @param[in] mode  mode
 * @param[in] speed  speed
 *
 * @return None
 **/
void i2c_open_x(HS_I2C_Type *i2c, i2c_mode_t mode, uint32_t speed)
{
    int index = i2c_index(i2c);
    __IO uint32_t *cpm_cfg = i2c_friend_tbl[index].cpm_cfg;

    // Open clock
    register_set1(cpm_cfg, CPM_I2C_SOFT_RESET_MASK);
    register_set0(cpm_cfg, CPM_I2C_GATE_EN_MASK);
    HS_PSO_UPD_RDY();

    i2c->ENABLE = 0;

    i2c_set_mode(i2c, mode);
    i2c_set_speed(i2c, speed);

    i2c->TX_TL = 0;/* tx fifo has one byte or below then trigger the tx empty interrupt.*/
    i2c->RX_TL = 0;/* rx fifo has received one byte then trigger the rx full interrupt.*/

    if(I2C_MODE_SLAVE == mode)
    {
        /* setting I2C slave addr */
        i2c->SAR = I2C_SLAVE_DEFAULT_ADDR;
    }

//    tx_fifo_depth = ((i2c->COMP_PARAM_1 >> 16) & 0xff) + 1;
//    rx_fifo_depth = ((i2c->COMP_PARAM_1 >> 8) & 0xff) + 1;

    /* disable interrupts */
    i2c->INTR_MASK = 0;

#ifdef I2C_DEAD_WORKAROUND
    i2c_env.speed[index] = speed;
#endif
}

/**
 * @brief i2c close
 *
 * @return None
 **/
void i2c_close_x(HS_I2C_Type *i2c)
{
    // Open clock
    register_set1(&HS_PSO->I2C_CFG, CPM_I2C_GATE_EN_MASK);
    HS_PSO_UPD_RDY();
}

/**
 * @brief i2c_master_transmit()
 *
 * @param[in] addr  
 * @param[in] tx_buf  
 * @param[in] tx_len  
 * @param[in] rx_buf  
 * @param[in] rx_len  
 *
 * @return 
 **/
bool i2c_master_transmit_x(HS_I2C_Type *i2c, uint16_t addr, uint8_t *tx_buf, uint32_t tx_len, uint8_t *rx_buf, uint32_t rx_len)
{
    i2c_op_t op = {tx_buf, tx_len, NULL, 0, rx_buf, rx_len};
    return i2c_master_transmit_ex(i2c, addr, &op, I2C_DEFAULT_TIMEOUT);
}

/**
 * @brief i2c master write data
 *
 * @param[in] addr  slave address
 * @param[in] tx_buf  transmit data buffer
 * @param[in] tx_len  transmit data length
 *
 * @retval true success
 * @retval false fail
 **/
bool i2c_master_write_x(HS_I2C_Type *i2c, uint16_t addr, uint8_t *tx_buf, uint32_t tx_len)
{
    i2c_op_t op = {tx_buf, tx_len, NULL, 0, NULL, 0};
    return i2c_master_transmit_ex(i2c, addr, &op, I2C_DEFAULT_TIMEOUT);
}

/**
 * @brief i2c master read data
 *
 * @param[in] addr  slave address
 * @param[in] rx_buf  receive data buffer
 * @param[in] rx_len  receive buffer length
 *
 * @retval true success
 * @retval false fail
 **/
bool i2c_master_read_x(HS_I2C_Type *i2c, uint16_t addr, uint8_t *rx_buf, uint32_t rx_len)
{
    i2c_op_t op = {NULL, 0, NULL, 0, rx_buf, rx_len};
    return i2c_master_transmit_ex(i2c, addr, &op, I2C_DEFAULT_TIMEOUT);
}

/**
 * @brief i2c master read memery (EEPROM)
 *
 * @param[in] addr  I2C address
 * @param[in] offset  memery offset
 * @param[in] alen  memery offset bytes
 * @param[in] rx_buf  receive data buffer
 * @param[in] rx_len  receive data lenght
 *
 * @retval true success
 * @retval false fail
 **/
bool i2c_master_read_mem_x(HS_I2C_Type *i2c, uint16_t addr, uint32_t offset, uint32_t alen, uint8_t *rx_buf, uint32_t rx_len)
{
    i2c_op_t op = {(uint8_t *)&offset, alen, NULL, 0, rx_buf, rx_len};
    return i2c_master_transmit_ex(i2c, addr, &op, I2C_DEFAULT_TIMEOUT);
}

/**
 * @brief i2c master write memery (EEPROM)
 *
 * @param[in] addr  I2C address
 * @param[in] offset  memery offset
 * @param[in] alen  memery offset bytes
 * @param[in] tx_buf  transmit data buffer
 * @param[in] tx_len  transmit data length
 *
 * @retval true success
 * @retval false fail
 **/
bool i2c_master_write_mem_x(HS_I2C_Type *i2c, uint16_t addr, uint32_t offset, uint32_t alen, uint8_t *tx_buf, uint32_t tx_len)
{
    i2c_op_t op = {(uint8_t *)&offset, alen, tx_buf, tx_len, NULL, 0};
    return i2c_master_transmit_ex(i2c, addr, &op, I2C_DEFAULT_TIMEOUT);
}

/** @} */


