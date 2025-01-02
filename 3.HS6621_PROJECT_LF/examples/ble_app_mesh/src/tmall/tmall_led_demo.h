/**
 ****************************************************************************************
 *
 * @file board.c
 *
 * @brief BLE mesh board header file.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

#ifndef __MESH_BOARD_H__
#define __MESH_BOARD_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

#include "rwip_config.h"
#include "nvds.h"
#include "rwip.h"
#include "app.h"
#include "app_mesh.h"
#include "peripheral.h"
#include "tmall_timer.h"
#include "mesh_log.h"

/*********************************************************************
 * MACROS
 */

/* Board kind */
#define MESH_BOARD_OM_DEMO          (0)
#define MESH_BOARD_OM_ZK            (1)
#define MESH_BOARD_TYPE             MESH_BOARD_OM_DEMO
/* Board version */
#define HS6621_20200313             (0)
#define HS6621C_20200605            (1)
#define HS6621CA2_20200923          (2)
#define MESH_BOARD_VERSIOON         HS6621CA2_20200923

/* PWM set */
#define HW_TIM_PWM0                 HS_TIM0
#define HW_TIM_PWM1                 HS_TIM1
#define HW_TIM_TIMER2               HS_TIM2

#define PWM_R_SET(x)                tim_pwm_channel_change_pulse_count(HW_TIM_PWM0, TIM_PWM_CHANNEL_0, (uint16_t)x)
#define PWM_G_SET(x)                tim_pwm_channel_change_pulse_count(HW_TIM_PWM0, TIM_PWM_CHANNEL_1, (uint16_t)x)
#define PWM_B_SET(x)                tim_pwm_channel_change_pulse_count(HW_TIM_PWM0, TIM_PWM_CHANNEL_2, (uint16_t)x)

#define PWM_W_SET(x)                tim_pwm_channel_change_pulse_count(HW_TIM_PWM1, TIM_PWM_CHANNEL_0, (uint16_t)x)
#define PWM_Y_SET(x)                tim_pwm_channel_change_pulse_count(HW_TIM_PWM1, TIM_PWM_CHANNEL_1, (uint16_t)x)

#define PWM_RGB_MAX                 (255)
#define PWM_RGB_OFF                 (0)
#define PWM_WY_MAX                  (2048)
#define PWM_WY_OFF                  (0)

/// CCT limit
#define LAMP_CCT_MAX                (20000)
#define LAMP_CCT_MIN                (800)

/// PWM incremental regulation
#define PWM_INCREASE_RATIO          (1)
/// Default value
#define DEFAULT_VALUE_CCT           (60*65535/100)
#define DEFAULT_VALUE_HUE           (0x8000)
#define DEFAULT_VALUE_SAT           (0xffff)
#define DEFAULT_VALUE_LIT           (0x8000)

/* Delete Node times when quick reset */
#define DEFAULT_DEL_NODE_RST_NUM    (5)
/* Clear Delete Node times after reset chip */
#define DEFAULT_RST_DEL_NODE_DLY    (3000) /* ms */
/* Save hw context indicate */
#define NVDS_IND_HW_CTX_SAVE()      tmall_nvds_store_handler()

/* Indicate Tmall when Attr Changed */
#if CONFIG_TMALL_IND
#define TMALL_IND_ATTR(attr_type,tid,p_attr_value) mm_vnd_tmall_indication(attr_type,tid,p_attr_value)
#else
#define TMALL_IND_ATTR(attr_type,tid,p_attr_value)
#endif
/* Length hw attribute context */
#define LEN_HW_ATTR_CTX             (12)

#define READING_MODE                (3) 	// 阅读模式：3
#define CINEMA_MODE                 (4) 	// 影院模式：4
#define WARM_MODE                   (5) 	// 温暖模式：5
#define NIGHT_LIGHT_MODE            (6) 	// 夜灯模式：6
#define GETTING_UP_MODE             (8)		// 起床模式：8
#define SLEEP_MODE                  (14) 	// 睡眠模式：14
#define EYE_PROTECTION_MODE         (57) 	// 护眼模式：57
#define NORMAL_MODE                 (353) 	// 正常模式：353
#define LEISURE_MODE                (378) 	// 休闲模式：378
#define TIMED_REST_MODE             (409) 	// 定时休息模式：409

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void tmall_app_init(void);
void tmall_ind_prov_state_handler(bool en);
int  board_get_attr(uint16_t attr_type, void *p_attr_value);
int  board_set_attr(uint16_t attr_type, void *p_attr_value);
int  board_cfm_attr(void *p_attr_value);
void board_ind_attr(uint16_t attr_type, uint8_t tid, uint8_t *p_attr_value,uint8_t attr_len);

extern uint16_t m_bearer_adv_stop(void);
extern void m_prov_bearer_gatt_stop(void);
extern uint16_t m_bearer_adv_start(void);
extern void m_prov_bearer_gatt_start(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef __MESH_BOARD_H__ */

/**
 * @}
 */

