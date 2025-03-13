/*
 * Copyright 2019-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#if defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c >= 1)
#include "fsl_component_serial_manager.h"
#endif
#if defined(gAppLedCnt_c) && (gAppLedCnt_c > 0)
#include "fsl_component_led.h"
#endif
#if defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)
#include "fsl_component_button.h"
#endif
#include "board.h"
#include "hal_sys.h"
#include "ComBusoff.h"
#include "app_dtc.h"
#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"
#include "hal_test.h"
#include "fsl_debug_console.h"
#include "crc.h"
#include "fsl_os_abstraction.h"
#include "NCK2910.h"
#include "canbus_app.h"
#include "time.h"
#include "onvo_nt3_lite_bta_srf_v_1_10_0.h"
#include "hal_rte.h"
#include "canbus_app.h"
#include "LowPW.h"
#include "bleTP.h"
#include "bleTP_api.h"
//#include "timers.h"
#include "uwbTaskApp.h"
#include "uciMsg.h"
#include "D6Update.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*${macro:start}*/
/* UART instance and clock */
#define DEMO_UART          LPUART0
#define DEMO_UART_CLKSRC   LPUART0_CLK_SRC
#define DEMO_UART_CLK_FREQ (12000000)

#if (defined(gBoardLedCnt_c) && defined(gAppLedCnt_c) && (gBoardLedCnt_c < gAppLedCnt_c))
#undef gAppLedCnt_c
#define gAppLedCnt_c gBoardLedCnt_c
#endif

#if (defined(gBoardButtonCnt_c) && defined(gAppButtonCnt_c) && (gBoardButtonCnt_c < gAppButtonCnt_c))
#undef gAppButtonCnt_c
#define gAppButtonCnt_c gBoardButtonCnt_c
#endif
#define LED_FLASH_CONFIG_ARRAY_DEFINE(name, count) led_flash_config_t name[count]

#if (defined(gAppLedCnt_c) && (gAppLedCnt_c > 0))
#define LedOn(ledNo)  (void)LED_TurnOnOff((led_handle_t)g_ledHandle[ledNo], 1U)
#define LedOff(ledNo) (void)LED_TurnOnOff((led_handle_t)g_ledHandle[ledNo], 0U)
#define LedFlashing(ledNo) \
    (void)LED_Flash((led_handle_t)g_ledHandle[ledNo], (led_flash_config_t *)&g_ledFlashConfig[ledNo])
#define LedSetColor(ledNo, ledColor) (void)LED_SetColor((led_handle_t)g_ledHandle[ledNo], (uint32_t)ledColor)
#define Led1On()                     LedOn(0U)
#define Led1Off()                    LedOff(0U)
#define Led1Flashing()               LedFlashing(0U)

#if gAppLedCnt_c > 1
#define Led2On()       LedOn(1U)
#define Led2Off()      LedOff(1U)
#define Led2Flashing() LedFlashing(1U)
#else
#define Led2On()
#define Led2Off()
#define Led2Flashing()
#endif /* gAppLedCnt_c > 1 */
#else
#define LedOn(ledNo)
#define LedOff(ledNo)
#define LedSetColor(ledNo, ledColor)
#define Led1On()
#define Led1Off()
#define Led1Flashing()
#define Led2On()
#define Led2Off()
#define Led2Flashing()
#define LedFlashing(ledNo)
#define LedStopFlashingAllLeds()
#define LedStartFlashingAllLeds()
#endif /* (defined(gAppLedCnt_c) && (gAppLedCnt_c > 0)) */

/*${macro:end}*/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
#if defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c > 0)
/*serial manager handle*/
extern SERIAL_MANAGER_HANDLE_DEFINE(gSerMgrIf);

#if (gAppUseSerialManager_c > 1)
/*serial manager handle*/
extern SERIAL_MANAGER_HANDLE_DEFINE(gSerMgrIf2);
#endif

/*Define fsci serial manager handle*/
#if defined(gFsciIncluded_c) && (gFsciIncluded_c > 0)
extern serial_handle_t g_fsciHandleList[gFsciIncluded_c];
#endif /*gFsciIncluded_c > 0*/
#endif

#if (defined(gAppLedCnt_c) && (gAppLedCnt_c > 0))
/*Define led handle*/
extern LED_HANDLE_ARRAY_DEFINE(g_ledHandle, gAppLedCnt_c);
#endif /*gAppLedCnt_c > 0*/

#if (defined(gAppLedCnt_c) && (gAppLedCnt_c > 0))
/*Define led flash configuration*/
extern LED_FLASH_CONFIG_ARRAY_DEFINE(g_ledFlashConfig, gAppLedCnt_c);
#endif /*gAppLedCnt_c > 0*/

#if (defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0))
/*Define button handle*/
extern BUTTON_HANDLE_ARRAY_DEFINE(g_buttonHandle, gAppButtonCnt_c);
#endif /*gAppButtonCnt_c > 0*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
/*!
 * \brief Initialize all services required by application (only), timer manager, serial
 *      manager, led, button, low power. this is optional if application does not use any of these
 *
 */
void APP_InitServices(void);

#define USE_OWNER_BLETP		1

#define ECUM_State_Init      0
#define ECUM_State_Normal    1
#define ECUM_State_Wakeup    2
#define ECUM_State_PreSleep  3
#define ECUM_State_Sleep     4
#define ECUM_State_Error     5

/*!
 * \brief Initialize all services required for BLE connectivity : clocks, link layer, HCI transport, etc..
 *      This is mandatory for BLE connectivity support.
 *      Shall be called before host stack initialization
 *
 * \return int 0 if success, negative value if error.
 */
int APP_InitBle(void);
void TaskCreate(void);
void task_EcuM(void *argument);
void swTimerCreate(void);
void swTimerDestroy(void);
void task_Com(void *argument);
void task_Diag(void *argument);
void task_BleStack(void *argument);
void task_BleService(void *argument);
void task_UWB(void *argument);
void task_RF(void *argument);
void task_NFC(void *argument);
void log_task(void *argument);
//void LedStartFlashingAllLeds(void);
//void LedStopFlashingAllLeds(void);
uint8_t EcuM_GetCurrentState();
void EcuM_SetModuleState(uint8_t statusint);
uint8_t diag_GetSleepFlag();
void diag_SetSleepFlag(uint8_t statusint);
uint8_t rf_GetSleepFlag();
void rf_SetSleepFlag(uint8_t statusint);
uint8_t test_GetSleepFlag();
void test_SetSleepFlag(uint8_t statusint);
uint8_t ble_GetSleepFlag(void);
void ble_SetSleepFlag(uint8_t statusint);
void OsStart(void);
void SwTimer1msCallback(TimerHandle_t xTimer);
void SwTimer10msCallback(TimerHandle_t xTimer);
uint8_t EcuM_GetUwbSleepFlg();
void EcuM_SetUwbSleepFlg(uint8_t stateflg);
uint8_t EcuM_GetUwb_timeout_SleepFlg();
void EcuM_SetUwb_timeout_SleepFlg(uint8_t stateflg);
/*${prototype:end}*/

#endif /* _APP_H_ */
