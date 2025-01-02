/**
 ****************************************************************************************
 *
 * @file board_led_demo.c
 *
 * @brief BLE mesh board led demo base tmall genie.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

/*********************************************************************
 * INCLUDES
 */
#include "m_defines.h"
#include "m_tb.h"
#include "tmall_led_demo.h"

#if APP_MESH_TMALL

#if (MESH_BOARD_TYPE == MESH_BOARD_OM_DEMO)
/*********************************************************************
 * MACROS
 */
#if (MESH_BOARD_VERSIOON == HS6621_20200313)
/* LED PIN MAP */
#define PIN_LED_RED                 (18)
#define PIN_LED_GREEN               (19)
#define PIN_LED_BLUE                (17)

#define PIN_LED_WHITE               (20)
#define PIN_LED_YELLOW              (16)
#elif (MESH_BOARD_VERSIOON == HS6621C_20200605)
/* LED PIN MAP */
#define PIN_LED_RED                 (12)
#define PIN_LED_GREEN               (11)
#define PIN_LED_BLUE                (20)

#define PIN_LED_WHITE               (21)
#define PIN_LED_YELLOW              (10)
#elif (MESH_BOARD_VERSIOON == HS6621CA2_20200923)
/* LED PIN MAP */
#define PIN_LED_RED                 (23)
#define PIN_LED_GREEN               (22)
#define PIN_LED_BLUE                (21)

#define PIN_LED_WHITE               (20)
#define PIN_LED_YELLOW              (24)
#endif /* (MESH_BOARD_VERSIOON == HS6621_20200313) */

/// Invalid value of lamp attribute
#define LN_ATTR_INVAILD             (0xFFFF)
/*********************************************************************
 * TYPEDEFS
 */
/// RGB
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}COLOR_RGB;

/// WY
typedef struct {
    uint16_t white;
    uint16_t yellow;
}COLOR_WY;

/// Hardware attribute sturcture use for operate
typedef struct {
    COLOR_RGB   rgb;
    COLOR_WY    wy;
    uint16_t    level;
    int16_t     duv;
    int16_t     scene;
}hw_op_attr_t;

/// Hardware attribute sturcture save to NVDS
typedef struct {
    /// Lightness
    uint16_t lns;
    /// Color Temperature
    uint16_t cct;
    /// Onoff State
    uint8_t onoff;
    /// Reset Number for Delete Node to Unporved
    uint8_t rst_num;
    /// Lamp Color
    uint16_t hsl[3];
}hw_nvds_attr_t;

/*********************************************************************
 * LOCAL VARIABLES
 */
/// Operate Use
static hw_op_attr_t m_op_hw;
/// Timer for delete Node to Device
static co_timer_t m_timer_node_to_device;
/// Timer for indication proved or unproved
static co_timer_t m_timer_node_or_device;

/// Save to NVDS
static hw_nvds_attr_t m_nvds_hw;

/// PWM0 Config for RGB
static const tim_config_t pwm0_cfg = {
    .mode = TIM_PWM_MODE,
    .config.pwm = {
        .count_freq = 16000000,
        .period_count = PWM_RGB_MAX,
        .channel = {
            {true,  {TIM_PWM_POL_LOW2HIGH,0 /* duty */}},
            {true,  {TIM_PWM_POL_LOW2HIGH,0 /* duty */}},
            {true,  {TIM_PWM_POL_LOW2HIGH,0 /* duty */}},
            {false, {TIM_PWM_POL_LOW2HIGH,PWM_RGB_MAX /* duty */}},
        },
        .callback = NULL,
    },
};

/// PWM1 Config for WY
static const tim_config_t pwm1_cfg = {
    .mode = TIM_PWM_MODE,
    .config.pwm = {
        .count_freq = 16000000,
        .period_count = PWM_WY_MAX,
        .channel = {
            {true,  {TIM_PWM_POL_LOW2HIGH,0 /* duty */}},
            {true,  {TIM_PWM_POL_LOW2HIGH,0 /* duty */}},
            {false, {TIM_PWM_POL_LOW2HIGH,0 /* duty */}},
            {false, {TIM_PWM_POL_LOW2HIGH,PWM_WY_MAX /* duty */}},
        },
        .callback = NULL,
    },
};

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
/// When power on more than 3 seconds, reset quick start removes the node counter
static void timer_node2device_handler(co_timer_t *timer, void *param)
{
    /// Reset counter
    m_nvds_hw.rst_num = false;
    /// Check Node state
    if(M_TB_STATE_PROV_STATE_PROV == m_tb_state_get_prov_state()) {
        /// Update m_nvds_hw
        NVDS_IND_HW_CTX_SAVE();
    }
    /// Delete the timer
    co_timer_del(&m_timer_node_to_device);
}

/// Indication unproved state
static void timer_ind_unprov_handler(co_timer_t *timer, void *param)
{
    /// Blink
    m_op_hw.wy.white = PWM_WY_MAX - m_op_hw.wy.white;
    PWM_W_SET(m_op_hw.wy.white);
}

/// Update reset counter handler
static void update_reset_counter_handler(void)
{
    if(M_TB_STATE_PROV_STATE_PROV == m_tb_state_get_prov_state())
    {
        if(++m_nvds_hw.rst_num >= DEFAULT_DEL_NODE_RST_NUM) {
            /// Clear counter
            m_nvds_hw.rst_num = false;
            NVDS_IND_HW_CTX_SAVE();
            /// Remove bond info
            app_mesh_remove_bond();
        }
        else {
            __LOG(LOG_SRC_API, LOG_LEVEL_DBG2, "Quick reset num %x,Delete node num %x\n",m_nvds_hw.rst_num,DEFAULT_DEL_NODE_RST_NUM);
            NVDS_IND_HW_CTX_SAVE();
        }
    }
}

/// HUE to RGB
static float hue2rgb(float p,float q,float t)
{
    if (t < 0.0) t += 1.0;
    if (t > 1.0) t -= 1.0;
    if (t < 1.0/6) return p + (q - p) * 6.0 * t;
    if (t < 1.0/2) return q;
    if (t < 2.0/3) return p + (q - p) * (2.0/3 - t) * 6.0;
    return p;
}

/// HSL to RGB
static void hsl2rgb(uint16_t hfix, uint16_t sfix, uint16_t lfix, COLOR_RGB *rgb)
{
    float r, g, b, h, s, l;

    h = hfix/65535.0;
    s = sfix/65535.0;
    l = lfix/65535.0;
    if (s == 0) {
        r = g = b = l; // achromatic
    }
    else
    {
        float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
        float p = 2.0 * l - q;
        r = hue2rgb(p, q, h + 1.0/3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1.0/3);
    }
    rgb->red = r * 255;
    rgb->green = g * 255;
    rgb->blue = b * 255;
}

/// Lammp set lightness
static void board_set_lamp_lns(uint16_t lns)
{
    /// Clear cct value prevent conflict
    m_nvds_hw.cct       = false;
    /// Update lns
    m_nvds_hw.lns       = lns;
    m_op_hw.wy.white    = lns / (65536/PWM_WY_MAX);
    m_op_hw.wy.yellow   = lns / (65536/PWM_WY_MAX);
    PWM_W_SET(m_op_hw.wy.white);
    PWM_Y_SET(m_op_hw.wy.yellow);
}

/// Lamp set cct
static void board_set_lamp_cct(int16_t cct)
{
    if((cct > LAMP_CCT_MAX) || (cct < LAMP_CCT_MIN))
    {
        cct = LAMP_CCT_MIN;
    }
    /// Update cct
    m_nvds_hw.cct = cct;
    m_op_hw.wy.white  = (cct - LAMP_CCT_MIN) / 10;
    m_op_hw.wy.yellow = (LAMP_CCT_MAX - cct) / 10;

    PWM_W_SET(PWM_WY_MAX - m_op_hw.wy.white);
    PWM_Y_SET(PWM_WY_MAX - m_op_hw.wy.yellow);
}

/// Lamp set rgb
static void board_set_lamp_rgb(COLOR_RGB *rgb)
{
    PWM_R_SET(rgb->red);
    PWM_G_SET(rgb->green);
    PWM_B_SET(rgb->blue);
}

/// Lamp set scene
static void board_set_lamp_scene(int scene)
{
    switch (scene) {
    case READING_MODE:
        PWM_R_SET(PWM_RGB_OFF);
        PWM_G_SET(PWM_RGB_OFF);
        PWM_B_SET(PWM_RGB_OFF);
        PWM_W_SET(PWM_WY_MAX);
        PWM_Y_SET(PWM_WY_OFF);
        break;
    case CINEMA_MODE:
        PWM_R_SET(PWM_RGB_MAX / 10);
        PWM_G_SET(PWM_RGB_MAX / 10);
        PWM_B_SET(PWM_RGB_MAX / 10);
        PWM_W_SET(PWM_WY_OFF);
        PWM_Y_SET(PWM_WY_OFF);
        break;
    case WARM_MODE:
        PWM_R_SET(PWM_RGB_OFF);
        PWM_G_SET(PWM_RGB_OFF);
        PWM_B_SET(PWM_RGB_OFF);
        PWM_W_SET(PWM_WY_OFF);
        PWM_Y_SET(PWM_WY_MAX);
        break;
    case NIGHT_LIGHT_MODE:
        PWM_R_SET(PWM_RGB_OFF);
        PWM_G_SET(PWM_RGB_OFF);
        PWM_B_SET(PWM_RGB_OFF);
        PWM_W_SET(PWM_WY_MAX / 10);
        PWM_Y_SET(PWM_WY_MAX / 10);
        break;
    case GETTING_UP_MODE:
        break;
    case SLEEP_MODE:
        break;
    case EYE_PROTECTION_MODE:
        break;
    case NORMAL_MODE:
        break;
    case LEISURE_MODE:
        break;
    case TIMED_REST_MODE:
        break;
    }
}

/// Lamp main onoff
static void board_set_mainlamp_onoff(bool en)
{
    if(en)
    {
        m_nvds_hw.onoff = true;

        if(m_nvds_hw.lns == 0) {
            m_nvds_hw.lns = DEFAULT_VALUE_LIT;
        }
        if(m_nvds_hw.cct == LN_ATTR_INVAILD) {
            m_nvds_hw.cct = DEFAULT_VALUE_CCT;
        }
        /// set WY
        if(m_nvds_hw.cct != 0) {
            board_set_lamp_cct(m_nvds_hw.cct);
        }
        else {
            board_set_lamp_lns(m_nvds_hw.lns);
        }
    }
    else {
        PWM_W_SET(PWM_WY_OFF);
        PWM_Y_SET(PWM_WY_OFF);
    }
}

/// Lamp background onoff
static void board_set_backlamp_onoff(bool en)
{
    if(en)
    {
        m_nvds_hw.onoff = true;

        if((m_nvds_hw.hsl[0] == LN_ATTR_INVAILD) && (m_nvds_hw.hsl[1] == LN_ATTR_INVAILD) && (m_nvds_hw.hsl[2] == LN_ATTR_INVAILD)) {
            m_nvds_hw.hsl[0] = DEFAULT_VALUE_HUE;
            m_nvds_hw.hsl[1] = DEFAULT_VALUE_SAT;
            m_nvds_hw.hsl[2] = DEFAULT_VALUE_LIT;
        }
        /// set RGB
        hsl2rgb(m_nvds_hw.hsl[0],m_nvds_hw.hsl[1],m_nvds_hw.hsl[2],&m_op_hw.rgb);
        board_set_lamp_rgb(&m_op_hw.rgb);
    }
    else {
        PWM_R_SET(PWM_RGB_OFF);
        PWM_G_SET(PWM_RGB_OFF);
        PWM_B_SET(PWM_RGB_OFF);
    }
}

/// Lamp onoff
static void board_set_lamp_onoff(bool en)
{
    if(en)
    {
        m_nvds_hw.onoff = true;

        if(m_nvds_hw.lns == 0) {
            m_nvds_hw.lns = DEFAULT_VALUE_LIT;
        }
        if(m_nvds_hw.cct == LN_ATTR_INVAILD) {
            m_nvds_hw.cct = DEFAULT_VALUE_CCT;
        }
        if((m_nvds_hw.hsl[0] == LN_ATTR_INVAILD) && (m_nvds_hw.hsl[1] == LN_ATTR_INVAILD) && (m_nvds_hw.hsl[2] == LN_ATTR_INVAILD)) {
            m_nvds_hw.hsl[0] = DEFAULT_VALUE_HUE;
            m_nvds_hw.hsl[1] = DEFAULT_VALUE_SAT;
            m_nvds_hw.hsl[2] = DEFAULT_VALUE_LIT;
        }

        /// set WY
        if(m_nvds_hw.cct != 0) {
            board_set_lamp_cct(m_nvds_hw.cct);
        }
        else {
            board_set_lamp_lns(m_nvds_hw.lns);
        }
        /// set RGB
        hsl2rgb(m_nvds_hw.hsl[0],m_nvds_hw.hsl[1],m_nvds_hw.hsl[2],&m_op_hw.rgb);
        board_set_lamp_rgb(&m_op_hw.rgb);
    }
    else
    {
        m_nvds_hw.onoff = false;
        PWM_R_SET(PWM_RGB_OFF);
        PWM_G_SET(PWM_RGB_OFF);
        PWM_B_SET(PWM_RGB_OFF);
        PWM_W_SET(PWM_WY_OFF);
        PWM_Y_SET(PWM_WY_OFF);
    }
}

/// Init Relative Timers
static void board_timer_init(void)
{
    /// Init Tmall Timer
    tmall_init_timer();

    /// Reset Timer for delete Node to Device
    co_timer_set(&m_timer_node_to_device,DEFAULT_RST_DEL_NODE_DLY,TIMER_ONE_SHOT,timer_node2device_handler,NULL);
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
/// Init board pin mode
void tmall_app_init(void)
{
    // Set pin mux
    pinmux_config(PIN_LED_RED,   PINMUX_TIMER0_IO_0_CFG);
    pinmux_config(PIN_LED_GREEN, PINMUX_TIMER0_IO_1_CFG);
    pinmux_config(PIN_LED_BLUE,  PINMUX_TIMER0_IO_2_CFG);
    pinmux_config(PIN_LED_WHITE, PINMUX_TIMER1_IO_0_CFG);
    pinmux_config(PIN_LED_YELLOW,PINMUX_TIMER1_IO_1_CFG);
    // Set timer mode
    tim_init();
    tim_config(HW_TIM_PWM0,&pwm0_cfg);
    tim_config(HW_TIM_PWM1,&pwm1_cfg);
    // Start
    tim_start(HW_TIM_PWM0);
    tim_start(HW_TIM_PWM1);
    /// Init timer
    board_timer_init();
}

/// Init indication Prov or Unproved state Timer
void tmall_ind_prov_state_handler(bool en)
{
    if(en) {
        /// Unproved state
        co_timer_set(&m_timer_node_or_device,500,TIMER_REPEAT,timer_ind_unprov_handler,NULL);
    }
    else {
        /// Proved state
        co_timer_del(&m_timer_node_or_device);
        board_set_mainlamp_onoff(true);
    }
}

/// Save hardware state to NVDS
void tmall_nvds_store_handler(void)
{
#if NVDS_SUPPORT
    nvds_put(NVDS_TAG_MESH_HW_ATTR,NVDS_LEN_MESH_HW_ATTR,(uint8_t*)&m_nvds_hw);
#endif
}

/// Load hardware state form NVDS
void tmall_nvds_loaded_handler(void)
{
#if NVDS_SUPPORT
	nvds_tag_len_t length = NVDS_LEN_MESH_HW_ATTR;
    nvds_get(NVDS_TAG_MESH_HW_ATTR,&length,(uint8_t*)&m_nvds_hw);
    board_set_lamp_onoff(m_nvds_hw.onoff == true ? true : false);
#endif
    /// Update reset counter handler
    update_reset_counter_handler();
}

/// Board get attribute
int board_get_attr(uint16_t attr_type, void *attr_data)
{
    int ret = 0;
    uint8_t *p_attr_value = (uint8_t *)attr_data;

    switch (attr_type)
    {
        case ATTR_TYPE_ONOFF:
            *(p_attr_value + 0) = m_nvds_hw.onoff;
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board get onoff: %x\n",m_nvds_hw.onoff);
            break;
        case ATTR_TYPE_LEVEL:
            co_write16p(p_attr_value, m_op_hw.level);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board get level: %x\n",m_op_hw.level);
            break;
        case ATTR_TYPE_LIGHTNESS:
            co_write16p(p_attr_value, m_nvds_hw.lns);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board get lightness: %x\n",m_nvds_hw.lns);
            break;
        case ATTR_TYPE_CCT:
            co_write16p(p_attr_value, m_nvds_hw.cct);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board get cct: %x\n",m_nvds_hw.cct);
            break;
        case ATTR_TYPE_DUV:
            co_write16p(p_attr_value, m_op_hw.duv);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board get duv: %x\n",m_op_hw.duv);
            break;
        case ATTR_TYPE_HSL:
            co_write16p(p_attr_value + 0, m_nvds_hw.hsl[0]);
            co_write16p(p_attr_value + 2, m_nvds_hw.hsl[1]);
            co_write16p(p_attr_value + 4, m_nvds_hw.hsl[2]);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board get hsl: H(%x) S(%x) L(%x)\n",m_nvds_hw.hsl[0],m_nvds_hw.hsl[1],m_nvds_hw.hsl[2]);
            break;
        case ATTR_TYPE_SCENE_MODE:
            co_write32p(p_attr_value, m_op_hw.scene);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board get scene: %x\n",m_op_hw.scene);
            break;
        case ATTR_TYPE_FAN_DIRECTION:
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board get fan direction\n");
            break;
        default:
            ret = -1;
            break;
    }
    return ret;
}

/// Board set attribute
int board_set_attr(uint16_t attr_type, void *attr_data)
{
    int ret = 0;
    int value = 0;
    uint8_t *p_attr_value = (uint8_t *)attr_data;

    switch (attr_type)
    {
        case ATTR_TYPE_ONOFF:
            value = *p_attr_value;
            board_set_lamp_onoff(value == true ? true : false);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board set onoff: %x\n",value);
            break;
        case ATTR_TYPE_LEVEL:
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board set level\n");
            break;
        case ATTR_TYPE_LIGHTNESS:
            value = co_read16p(p_attr_value);
            board_set_lamp_lns((uint16_t)value);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board set lightness: %x\n",value);
            break;
        case ATTR_TYPE_CCT:
            value = co_read16p(p_attr_value);
            board_set_lamp_cct((int16_t)value);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board set cct: %x\n",value);
            break;
        case ATTR_TYPE_DUV:
            value = co_read16p(p_attr_value);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board set duv: %x\n",value);
            break;
        case ATTR_TYPE_HSL:
            m_nvds_hw.hsl[2] = co_read16p(p_attr_value + 0);
            m_nvds_hw.hsl[0] = co_read16p(p_attr_value + 2);
            m_nvds_hw.hsl[1] = co_read16p(p_attr_value + 4);
            hsl2rgb(m_nvds_hw.hsl[0], m_nvds_hw.hsl[1], m_nvds_hw.hsl[2], &m_op_hw.rgb);
            board_set_lamp_rgb(&m_op_hw.rgb);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board set rgb: R(%x) G(%x) B(%x)\n",m_op_hw.rgb.red,m_op_hw.rgb.green,m_op_hw.rgb.blue);
            break;
        case ATTR_TYPE_MAIN_LAMP_ONOFF:
            value = *p_attr_value;
            board_set_mainlamp_onoff(value == true ? true : false);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board set main lamp onoff: %x\n",value);
            break;
        case ATTR_TYPE_BG_LAMP_ONOFF:
            value = *p_attr_value;
            board_set_backlamp_onoff(value == true ? true : false);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board set bakground lamp onoff: %x\n",value);
            break;
        case ATTR_TYPE_SCENE_MODE:
            value = *p_attr_value;
            board_set_lamp_scene(value);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board set scene: %x\n",value);
            break;
        case ATTR_TYPE_FAN_DIRECTION:
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Board set fan direction\n");
            break;
        default:
            ret = -1;
            break;
    }
    return ret;
}

/// Board confirm attribute
int board_cfm_attr(void *p_attr_value)
{
    return false;
}

/// Board indication attribute
void board_ind_attr(uint16_t attr_type, uint8_t tid, uint8_t *p_attr_value,uint8_t attr_len)
{

}

#endif /* (MESH_BOARD_TYPE == MESH_BOARD_OM_DEMO) */

#endif /* APP_MESH_TMALL */