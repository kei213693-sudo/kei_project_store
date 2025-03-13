// Modification History
// created by ZhenJinHong, 2017/06/28 ,GuangZhou
// ��SPI��������ʱ��Ŀǰû��
#include "NCK2910.h"
#include "stdint.h"
#include "onvo_nt3_lite_bta_srf_v_1_10_0.h"
#include "time.h"
#include "autosar_nm.h"
#include "logger.h"
#include "hal_rte.h"
#include "ComSM.h"
#include "canbus_trans.h"
#include "app_dtc.h"
#include "E2Para.h"
#include "SysTimer.h"

/*********************************************************************************************************
  local function
*********************************************************************************************************/
static uint8_t _CalculateCRC8Byte(uint8_t u8InputByte, uint8_t u8InitialCrc);
static uint8_t _CalculateCRC8(uint8_t *pu8FrameBuf, uint8_t u8Len);
static bool_t _NCK2910_RciCmdSend(uint8_t u8Cmd, uint8_t u8SubCmd,
								uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_DC_SET(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_DC_GET(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_DC_COPY2RAM(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_DC_COPY2NV(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_DC_PATCHRAM(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_DC_HASH(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_DC_GET_STATUS(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_OS_SET_POLLING(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_OS_GET_POLLING(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_OS_STORE_POLLING(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_OS_SET_MODE(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_OS_STATUS(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_RB_READ_MSG(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_RB_READ_BYTES(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_RB_FLUSH_MSG(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_RB_STATUS(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_PE_VERSIONS(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_PE_CAPABILITY(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_DD_SET(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_DD_GET(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_DD_HASH(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_DD_SIG(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_DD_TIME(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_WI_CONFIG(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_WI_CLEAR(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_WI_STORE(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_WI_ECHO(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_WI_ECHO_NXP(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_OBM_CONF(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_OBM_READ(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_OBM_RSSI_OFMU(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_OBM_SET_FREQUENCY_OFFSET_OVERRIDE(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_OBM_GET_FREQUENCY_OFFSET(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_OBM_DO_LO_CORRECTION(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_MISC_WAIT_US(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_MISC_PIN_TOGGLE(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_EV_TIMER_HEARTBEAT(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_TX_TRIGGER(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_TX_WRITE_PAYLOAD(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static bool_t _RciCmd_TX_READ_PAYLOAD(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen);
static void _NCK2910_IoInit(void);
static bool_t _NCK2910_PFlashInit(void);
static bool_t _NCK2910_RfParaInit(void);
static bool_t NCK2910_InitParameFunc(void);
static bool_t NCK2910_InitFunc(void);
static void _NCK2910_Ready(void);
static void NCK2910_RfEventRead(void);

#define NCK2910_OS_STATUS_STANDBY	0x00
#define NCK2910_OS_STATUS_IDEL		0x01
#define NCK2910_OS_STATUS_SLEEP		0x02
#define NCK2910_OS_STATUS_CONTRX	0x03
#define NCK2910_OS_STATUS_POLLING	0x04
#define NCK2910_OS_STATUS_TRANSPORT	0x05
#define NCK2910_OS_STATUS_RESET		0x06

#define _2910_SPI_COMM_DELAY_1MS 0
#define OS_STATUS_VALUE_CONVERT_STRING(x,y) NCK2910_OS_STATUS_IDEL == (y=x)?"IDEL":NCK2910_OS_STATUS_CONTRX == y?"Continuous reception":NCK2910_OS_STATUS_POLLING == y?"Polling":"Unknown",y
uint32_t NCK2910_IntCnt, RfTaskWakeupNoProcCnt, RfEventProcCnt, RfEventSuccProcCnt, Rf_293Msg_SendCnt, Rf_297Msg_SendCnt;
uint8_t NCK2910_ProcessFlag;
uint32_t NCK2910_TmpCnt, received_frame_count;
uint8_t current_os_status;
uint32_t Total_Recv_FrameCnt, TPMS_Frame_RecvCnt, ChargingPile_Frame_RecvCnt, Error_Frame_RecvCnt, TpmsError_CRC8_Cnt, ChargeError_CRC8_Cnt;
uint8_t wakenmFlag = 0;
uint32_t NCK2910_ReCalibrationCnt, NCK2910_ContTimeoutCnt, NCK2910_ReCaliTimer;
uint8_t recv_data_cha;
uint16_t Last_NCK2910_ADCValue, NCK2910_ADCValue;
uint8_t NCK2910_ReCalibrationFlag, NCK2910_ReCaliTimerCntFlag;
uint8_t driving_stat_change;
uint32_t NCK2910_ReCfgPollingTimer;
uint32_t rf_task_timeout_timer;
uint8_t recv_cnt_zero_flag = 1;
uint32_t TmpsRecvDelayTimer, ChargeRecvDelayTimer;
uint32_t TmpsRecvCorrectDelayTimer;
uint32_t CMsgReadyReSendCnt, C293MsgReadyReSendCnt;
uint32_t TPMS_Frame_RecvNoMatch_ID;
uint32_t CorrectRecvNum = 0, Nck2910ShortTermIntNum = 0;
uint32_t TpmsCorrectRecvNum;
#if SUPPORT_RECV_RATE_TEST
uint8_t measurementTpmsPackageRCnt, measurementTpmsPackageTCnt;
uint32_t measurementTpmsPackageStartTime, measurementTpmsPackageEndTime, measurementTpmsPackageTime;
uint8_t measurementChargePackageRCnt, measurementChargePackageTCnt;
uint32_t measurementChargePackageStartTime, measurementChargePackageEndTime, measurementChargePackageTime;
#endif
#define RF_WAIT_TIMEOUT_CNT 	30000U
#define RF_WAIT_TIMEOUT_TRYCNT	100U
#if UNDER_VOLTAGE_IDLE_MODE_EN
static uint8_t NCK2910_ReInitFlag = 0;
static uint8_t NCK2910_DealLock = 0;
#endif
/*********************************************************************************************************
  local variable
*********************************************************************************************************/
static uint8_t	_u8RciCmdDCSendF;				// Device Configuration�������ͱ�־
static uint8_t	_u8RciCmdOSSendF;				// Operating State�������ͱ�־
static uint8_t	_u8RciCmdRBSendF;				// RX Buffer�������ͱ�־
static uint8_t	_u8RciCmdPESendF;				// Personality�������ͱ�־
static uint8_t	_u8RciCmdDDSendF;				// Debug�������ͱ�־
static uint8_t	_u8RciCmdWISendF;				// Wired Interface�������ͱ�־
static uint8_t	_u8RciCmdOBMSendF;				// On-Board Measurement�������ͱ�־
static uint8_t	_u8RciCmdMISCSendF;				// Miscellaneous�������ͱ�־
static uint8_t	_u8RciCmdEV_TIMERSendF;			// Event Handling�������ͱ�־
static uint8_t	_u8RciCmdTXSendF;				// Transmitter�������ͱ�־


/*********************************************************************************************************
  global variable
*********************************************************************************************************/
uint8_t	Gu8Nck2910RciTxBuf[256];				// NCK2910 RCI���ͻ�����
uint8_t Gu8Nck2910RciRxBuf[256];				// NCK2910 RCI���ջ�����
uint8_t	Gu8Nck2910RciRxLen;						// NCK2910 RC1���յ���֡����
uint8_t Tpms_Rxid[10];                          //存储收到的RF信息里的id
uint8_t	counter = 0;
RF_Data_Info RF_Dat_Info = {0,0,0,0};
osa_semaphore_handle_t semaphoreHandle = NULL;
uint8_t read_status = 0;
uint8_t end_time =0;
uint16_t dura_time = 0;
extern struct timeval tv;
extern uint32_t wol2min_counter;
extern BtaOutputRfData_msg BtaOutputRfData_t;
extern BtaOutputRfData1_msg BtaOutputRfData1_t;
uint32_t TPMS_IDs[4];
extern uint32_t tim1_cnt;
uint8_t receiveRFflag;
uint32_t send293 = 0;
uint8_t needwakenm = 0;
uint8_t RFReleaseflag = 0;
uint32_t Charge_IDE[3];
uint32_t ChrgrGunGateOpenReq_On_KEEP_TIME_cnt;
uint32_t privtVIN = 0;
uint8_t RF_PrivtVINReadFlag = 0;
extern bool_t rfInitSuccFlag;

#if RF_CLOSE_ALL_PRINTF
#define rf_dbg_Printf
#else
#define rf_dbg_Printf logPrintf
#endif

extern void RF_SendFSK_RecvInfo(void);
extern void RF_SendASK_RecvInfo(void);
extern void RF_SendError_Info(void);
extern void RF_SendRSSI_Info(uint8_t recv_ind, uint8_t rssi_hi, uint8_t rssi_lo);

/*********************************************************************************************************
  const variable
*********************************************************************************************************/
// ���µ��������飬Ҫ��RCI��λ�����ɵ�SPI�����¼�У�ȥ��LEN��CMD��SUBCMD��CRC�ֽڣ�Ҳ��ֻ����payload
#if !USE_OLD_CFG

#define	RX_CFG_NUM			2u
const uint8_t c_u8LoCfgBuf[RX_CFG_NUM][LO_CFG_BUF_LEN] =
											{
												{0x01,0x00,0x90,0x6e,0x3d,0x35,0x9f,0x00},
												{0x00,0x00,0x90,0x6e,0x3d,0x35,0x9f,0x00},
											};
const uint8_t c_u8DigFreqBuf[RX_CFG_NUM][DIG_FREQ_BUF_LEN] =
											{
												{0x00,0x01,0xae,0x1d,0x00},
												{0x00,0x01,0xae,0x1d,0x00},
											};
#if !USE_NEW_CFG_ZLG
const uint8_t c_u8FilterBuf[RX_CFG_NUM][FILTER_BUF_LEN] =
											{
												{0x01,0x02,0x25,0xae,0x1d,0x02,0x00,0xa9,0xaa,0x00,
												 0x00,0xaa,0xaa,0x00,0x00,0xef,0x15,0x40,0x15,0x20,
												 0x01,0x8a,0x00,0x20,0x58,0x44,0x08,0x02,0x20,0x78,
												 0x00,0x1d,0x50,0x07,0x62,0x00,0x11,0xcd,0x40,0x80,
												 0x21,0xb2,0x1d,0x21,0x23,0x74,0x00,0x00},
												{0x00,0x02,0x25,0xae,0x1d,0x03,0x00,0x03,0x00,0x00,
												 0x00,0x05,0x00,0x00,0x00,0x63,0x14,0x40,0x11,0x22,
												 0x01,0x8a,0x00,0x20,0x00,0x44,0x08,0x1e,0x20,0x87,
												 0x02,0xa1,0x70,0x34,0x70,0x8a,0x8a,0x6e,0x80,0x79,
												 0x6b,0xb1,0xe0,0x5f,0x23,0x78,0x00,0x00},
											};
const uint8_t c_u8BaseBandBuf[RX_CFG_NUM][BASEBAND_BUF_LEN] =
											{
												{0x01,0x03,0xcb,0x77,0x28,0xa0,0x10,0xa6,0x1a,0xa1,
												 0xb9,0x8c,0x5b,0x1a,0xb3,0x00,0xf6,0x48,0x5f,0x31,
												 0x2a,0x3d,0x3c,0x00,0x88,0x05,0x46,0x68,0x33,0x40,
												 0x37,0x88,0x59,0x20,0x40,0x06,0x80,0x00,0xf9,0x03,
												 0x00,0x00,0x66,0x08,0x00,0x00,0x00},
												{0x00,0x03,0xd0,0x6b,0x40,0xff,0x10,0xa6,0x9a,0x98,
												 0x9d,0x6c,0x5a,0x1e,0x0d,0x05,0xfc,0x16,0xb6,0x11,
												 0x78,0x13,0xaf,0x01,0xa5,0x05,0x46,0x68,0x33,0x40,
												 0x2f,0x88,0x2f,0xa0,0x13,0x06,0x80,0x00,0x00,0x00,
												 0x00,0x00,0x6c,0x08,0x00,0x00,0x00},
											};
const uint8_t c_u8FrameWupBuf[RX_CFG_NUM][FRAME_WUP_BUF_LEN] =
											{
												{0x01,0x04,0x40,0x25,0xa5,0x05,0x80,0x61,0xed,0xca,
												 0x96,0xe9,0x21,0x00,0x00},
												{0x00,0x04,0x80,0x59,0x79,0x0d,0x80,0x6a,0xbf,0xff,
												 0xcf,0xac,0xa9,0x00,0x00},
											};
#if !RSSI_EVENT_EN
const uint8_t c_u8SwBuf[RX_CFG_NUM][SW_BUF_LEN] =
											{
												{0x01,0x05,0x01,0x00,0x3a,0xa3,0x58,0x40,0x57,0x2a,
												 0x00,0xc0,0x00,0x10,0x01,0x00,0x00,0x00,0x00,0x00},
												{0x00,0x05,0x01,0x00,0xf5,0xa3,0x80,0x60,0x7f,0x2a,
												 0x00,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
											};
#else /* RSSI_EVENT_EN */
const uint8_t c_u8SwBuf[RX_CFG_NUM][SW_BUF_LEN] =
											{
												{0x01,0x05,0x01,0x00,0x3a,0xa3,0x58,0x40,0x57,0x2a,
												 0x00,0xc0,0x00,0x10,0x01, 0x0a,0x32, 0x00,0x00,0x00},
												{0x00,0x05,0x01,0x00,0xf5,0xa3,0x80,0x60,0x7f,0x2a,
												 0x00,0xc0,0x00,0x00,0x00, 0x0a,0x32, 0x00,0x00,0x00},
											};
#endif

#else /* USE_NEW_CFG_ZLG */

const uint8_t c_u8FilterBuf[RX_CFG_NUM][FILTER_BUF_LEN] =
											{
												{0x01,0x02,0x25,0xae,0x1d,0x01,0x00,0xa9,0xaa,0x00,
												 0x00,0xaa,0xaa,0x00,0x00,0xef,0x15,0x40,0x17,0x20,
												 0x09,0x8c,0x02,0xa0,0x5c,0x44,0x08,0x06,0x20,0xb4,
												 0x00,0x2c,0x50,0x0f,0x68,0xeb,0x50,0xb3,0x40,0x80,
												 0x3b,0xb6,0x11,0x42,0x23,0x74,0x00,0x00},
												{0x00,0x02,0x25,0xae,0x1d,0x02,0x00,0x03,0x00,0x00,
												 0x00,0x05,0x00,0x00,0x00,0x63,0x14,0x42,0x15,0x22,
												 0x01,0x8a,0x00,0x22,0x01,0x44,0x08,0x3e,0x20,0x5f,
												 0x03,0xd7,0x70,0x46,0x70,0x8a,0x8a,0x6e,0x80,0x5b,
												 0x7a,0xb7,0xe0,0x5f,0x23,0x78,0x00,0x00},
											};
const uint8_t c_u8BaseBandBuf[RX_CFG_NUM][BASEBAND_BUF_LEN] =
											{
												{0x01,0x03,0xd8,0x59,0x28,0xa0,0x10,0xa6,0x1a,0xf1,
												 0x93,0x6c,0x5b,0x1a,0x0d,0x01,0xcd,0x24,0x39,0x19,
												 0x03,0x1f,0x5a,0x00,0xa8,0x06,0x46,0x68,0x33,0x40,
												 0x27,0x88,0x3f,0x20,0x2b,0x06,0x80,0x00,0x00,0x00,
												 0x00,0x00,0x58,0x08,0x00,0x00,0x00},
												{0x00,0x03,0xe0,0x47,0x40,0xff,0x10,0xa6,0x1a,0xe1,
												 0x95,0x6c,0x5b,0x1e,0xbd,0x06,0x5e,0x17,0x94,0x13,
												 0xd7,0x14,0x3f,0x02,0xa5,0x06,0x46,0x68,0x33,0x40,
												 0x20,0x88,0x20,0xa0,0x0e,0x06,0x80,0x00,0xf9,0x03,
												 0x00,0x00,0x7c,0x08,0x00,0x00,0x00},
											};
const uint8_t c_u8FrameWupBuf[RX_CFG_NUM][FRAME_WUP_BUF_LEN] =
											{
												{0x01,0x04,0xc0,0x29,0xd4,0x02,0x80,0x7b,0xed,0xca,
												 0xb6,0x89,0x42,0x00,0x00},
												{0x00,0x04,0x80,0x59,0x01,0x00,0x80,0x6f,0xbf,0xff,
												 0x9f,0xfc,0xba,0x00,0x00},
											};
#if !RSSI_EVENT_EN
const uint8_t c_u8SwBuf[RX_CFG_NUM][SW_BUF_LEN] =
											{
												{0x01,0x05,0x01,0x00,0xe8,0x23,0x58,0x40,0x57,0x2a,
												 0x00,0xc0,0x00,0x10,0x01,0x00,0x00,0x00,0x00,0x00},
												{0x00,0x05,0x01,0x00,0xa2,0x24,0x80,0x60,0x7f,0x2a,
												 0x00,0xc0,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00},
											};
#else /* RSSI_EVENT_EN */
const uint8_t c_u8SwBuf[RX_CFG_NUM][SW_BUF_LEN] =
											{
												{0x01,0x05,0x01,0x00,0xe8,0x23,0x58,0x40,0x57,0x2a,
												 0x00,0xc0,0x00,0x10,0x01, 0x0a,0x32, 0x00,0x00,0x00},
												{0x00,0x05,0x01,0x00,0xa2,0x24,0x80,0x60,0x7f,0x2a,
												 0x00,0xc0,0x00,0x10,0x00, 0x0a,0x32, 0x00,0x00,0x00},
											};
#endif
#endif

const uint8_t c_u8MidMapBuf[RX_CFG_NUM][MID_MAP_BUF_LEN] =
											{
												{0x01,0x06,0xff,0xff,0xff,0xff,0x01,0x01,0x00,0x20,
												 0x00},
												{0x00,0x06,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x20,
												 0x00},
											};
const uint8_t c_u8MidBuf[RX_CFG_NUM][MID_BUF_LEN] =
											{
												{0x01,0x07,0x66,0x66,0x66,0x66,0x00},
												{0x00,0x07,0x55,0x55,0x55,0x55,0x00},
											};
const uint8_t c_u8SlotCfgBuf[RX_CFG_NUM][SLOT_CFG_BUF_LEN] =
											{
												{0x01,0x08,0x01,0x00,0x01,0x01,0x01,0x01,0x00},
												{0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
											};
const uint8_t c_u8ReceptionCfgBuf[RX_CFG_NUM][REC_CFG_BUF_LEN] =
											{
												{0x01,0x09,0x01,0x01,0x00},
												{0x00,0x09,0x00,0x00,0x00},
											};
const uint8_t c_u8PflashCfgBuf[PFLASH_CFG_BUF_LEN] =
											{
												0x01,0xb2,0x7e,0x4c
											};

#if ASK_FSK
#if USE_ZLG_POLLING_CFG
const uint8_t c_u8PollingCfgBuf[POLLING_CFG_BUF_LEN] =
											{   /* 1ms, 4.3ms, 42ms --->  0是ChargePile, 1是TPMS */
												0x01,0x00,0x0a,0x00, 0x00,0x00,0x2b,0x00, 0xff,0x00,0xa4,0x01
											};
const uint8_t c_u8PollingCfgBufOnlyTpms[POLLING_CFG_BUF_ONLY_TPMS_LEN] =
											{	/* 1ms, 46.3ms --->  0是ChargePile, 1是TPMS */
												0x01,0x00,0x0a,0x00, 0xff,0x00,0xcf,0x01
											};
#else /* !USE_ZLG_POLLING_CFG */
const uint8_t c_u8PollingCfgBuf[POLLING_CFG_BUF_LEN] =
											{   /* 5ms, 4ms, 36ms --->  0是ChargePile, 1是TPMS */
												0x01,0x00,0x32,0x00, 0x00,0x00,0x28,0x00, 0xff,0x00,0x68,0x01
											};
const uint8_t c_u8PollingCfgBufOnlyTpms[POLLING_CFG_BUF_ONLY_TPMS_LEN] =
											{	/* 5ms, 40ms --->  0是ChargePile, 1是TPMS */
												0x01,0x00,0x32,0x00, 0xff,0x00,0x90,0x01
											};
#endif
#elif FSK
#ifdef POLLING_CFG_BUF_LEN
#undef POLLING_CFG_BUF_LEN
#define POLLING_CFG_BUF_LEN 8
#endif
const uint8_t c_u8PollingCfgBuf[POLLING_CFG_BUF_LEN] =
											{   /* 5ms, 40ms --->  0是ChargePile, 1是TPMS */
												0x01,0x00,0x32,0x00, 0xff,0x00,0x90,0x01
											};
const uint8_t c_u8PollingCfgBufOnlyTpms[POLLING_CFG_BUF_ONLY_TPMS_LEN] =
											{	/* 5ms, 40ms --->  0是ChargePile, 1是TPMS */
												0x01,0x00,0x32,0x00, 0xff,0x00,0x90,0x01
											};
#elif ASK
#ifdef POLLING_CFG_BUF_LEN
#undef POLLING_CFG_BUF_LEN
#define POLLING_CFG_BUF_LEN 8
#endif
const uint8_t c_u8PollingCfgBuf[POLLING_CFG_BUF_LEN] =
											{   /* 4ms, 41ms --->  0是ChargePile, 1是TPMS */
												0x00,0x00,0x28,0x00, 0xff,0x00,0x9A,0x01
											};
const uint8_t c_u8PollingCfgBufOnlyTpms[POLLING_CFG_BUF_ONLY_TPMS_LEN] =
											{	/* 4ms, 41ms --->  0是ChargePile, 1是TPMS */
												0x00,0x00,0x28,0x00, 0xff,0x00,0x9A,0x01
											};
#endif

#define FALL_BACK_500MS	0
#define FALL_BACK_1S 	0
#define FALL_BACK_3S 	1
#define FALL_BACK_6S 	0
#if FALL_BACK_500MS
const uint8_t c_u8FallbackTimeoutCfgBuf[5] =
											{
												0x02,0x1e,0x7a,0xf4,0x01
											};
#elif FALL_BACK_1S
const uint8_t c_u8FallbackTimeoutCfgBuf[5] =
											{
												0x02,0x1e,0x7a,0xe8,0x03
											};
#elif FALL_BACK_3S
const uint8_t c_u8FallbackTimeoutCfgBuf[5] =
											{
												0x02,0x1e,0x7a,0xb8,0x0b
											};
#elif FALL_BACK_6S
const uint8_t c_u8FallbackTimeoutCfgBuf[5] =
											{
												0x02,0x1e,0x7a,0x70,0x17
											};
#endif



#else /* USE_OLD_CFG */


#define	RX_CFG_NUM			2u
const uint8_t c_u8LoCfgBuf[RX_CFG_NUM][LO_CFG_BUF_LEN] =
											{
												{0x00,0x00,0x90,0x6e,0x3d,0x35,0x9f,0x00},
												{0x01,0x00,0x90,0x6e,0x3d,0x35,0x9f,0x00},
											};
const uint8_t c_u8DigFreqBuf[RX_CFG_NUM][DIG_FREQ_BUF_LEN] =
											{
												{0x00,0x01,0xae,0x1d,0x00},
												{0x00,0x01,0xae,0x1d,0x00},
											};
const uint8_t c_u8FilterBuf[RX_CFG_NUM][FILTER_BUF_LEN] =
											{
												{0x00,0x02,0x25,0xae,0x1d,0x03,0x00,0x06,0x00,0x00,
												 0x00,0x55,0x00,0x00,0x00,0xe3,0x14,0x42,0x15,0x22,
												 0x01,0x8a,0x00,0x22,0x01,0x44,0x08,0x1e,0x20,0x87,
												 0x02,0xa1,0x70,0x34,0x70,0x8a,0x8a,0x6e,0x80,0x1b,
												 0x6b,0xb1,0xe0,0x5f,0x23,0x78,0x00,0x00},
												{0x01,0x02,0x25,0xae,0x1d,0x02,0x00,0xa9,0xaa,0x00,
												 0x00,0xaa,0xaa,0x00,0x00,0xef,0x15,0x42,0x15,0x22,
												 0x01,0x8a,0x00,0x22,0x59,0x44,0x08,0x02,0x20,0x78,
												 0x00,0x1d,0x50,0x07,0x62,0x00,0x11,0xcd,0x40,0x80,
												 0x3c,0xb3,0x18,0x31,0x23,0x74,0x00,0x00},
											};
const uint8_t c_u8BaseBandBuf[RX_CFG_NUM][BASEBAND_BUF_LEN] =
											{
												{0x00,0x03,0xd0,0x6b,0x40,0xff,0x10,0xa6,0x9a,0x98,
												 0x9d,0x6c,0x5a,0x1e,0x0d,0x05,0xfc,0x16,0xb6,0x11,
												 0x78,0x13,0xaf,0x01,0xa5,0x05,0x46,0x68,0x33,0x40,
												 0x2f,0x88,0x2f,0x20,0x13,0x06,0x80,0x00,0x00,0x00,
												 0x00,0x00,0x6c,0x08,0x00,0x00,0x00},
												{0x01,0x03,0xcb,0x77,0x28,0xa0,0x10,0xa6,0x1a,0x91,
												 0xb7,0x8c,0x5b,0x1e,0xb3,0x00,0xf6,0x48,0x5f,0x31,
												 0x2a,0x3d,0x3c,0x00,0x88,0x05,0x46,0x68,0x33,0x40,
												 0x3e,0x88,0x53,0x20,0x47,0x06,0x80,0x00,0xf9,0x03,
												 0x00,0x00,0x66,0x08,0x00,0x00,0x00},
											};
const uint8_t c_u8FrameWupBuf[RX_CFG_NUM][FRAME_WUP_BUF_LEN] =
											{
												{0x00,0x04,0x80,0x59,0x79,0x0d,0x80,0x6a,0xbf,0xff,
												 0xcf,0xac,0xa9,0x00,0x00},
												{0x01,0x04,0x40,0x25,0xa5,0x05,0x80,0x7c,0xed,0xca,
												 0x9e,0xc1,0x31,0x00,0x00},
											};
const uint8_t c_u8SwBuf[RX_CFG_NUM][SW_BUF_LEN] =
											{
												{0x00,0x05,0x01,0x00,0xaa,0xaa,0x80,0x60,0x01,0x2a,
												 0x00,0xc0,0x6a,0x0a,0x00,0x00,0x00,0x00,0x00,0x00},
												{0x01,0x05,0x01,0x00,0x20,0xa3,0x58,0x40,0x00,0x2a,
												 0x00,0xc0,0x00,0x10,0x01,0x00,0x00,0x00,0x00,0x00},
											};
const uint8_t c_u8MidMapBuf[RX_CFG_NUM][MID_MAP_BUF_LEN] =
											{
												{0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,
												 0x00},
												{0x01,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,
												 0x00},
											};
const uint8_t c_u8MidBuf[RX_CFG_NUM][MID_BUF_LEN] =
											{
												{0x00,0x07,0x00,0x00,0x00,0x00,0x00},
												{0x00,0x07,0x00,0x00,0x00,0x00,0x00},
											};
const uint8_t c_u8SlotCfgBuf[RX_CFG_NUM][SLOT_CFG_BUF_LEN] =
											{
												{0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
												{0x01,0x08,0x01,0x00,0x01,0x01,0x01,0x01,0x00},
											};
const uint8_t c_u8ReceptionCfgBuf[RX_CFG_NUM][REC_CFG_BUF_LEN] =
											{
												{0x00,0x09,0x00,0x00,0x00},
												{0x01,0x09,0x01,0x01,0x00},
											};
const uint8_t c_u8PollingCfgBuf[POLLING_CFG_BUF_LEN] =
											{
												0x00,0x00,0x64,0x00,0x01,0x00,0x64,0x00,0xff,0x00,0xe0,0x01
											};
const uint8_t c_u8PflashCfgBuf[PFLASH_CFG_BUF_LEN] =
											{
												0x01,0xb2,0x7e,0x4c
											};

const uint8_t c_u8PollingCfgBufOnlyTpms[PFLASH_CFG_BUF_LEN] =
											{
												0x01,0xb2,0x7e,0x4c
											};
const uint8_t c_u8FallbackTimeoutCfgBuf[5] =
											{
												0x02,0x1e,0x7a,0xb8,0x0b
											};
#endif

static uint8_t ChargeCalculateCRC8(uint8_t* data, size_t length)
{
	uint32_t sum = 0;
	uint8_t Charge_crc8;
       // 累加所有字节
	for (size_t i = 0; i < length; i++) {
	    sum += data[i];
	}
	// 返回累加和的低字节作为CRC值
	Charge_crc8 = (uint8_t)(sum & 0xFF);
	return Charge_crc8;
}

static uint8_t TmpsCalculateCRC8(uint8_t *ptr, uint8_t len, uint8_t poly, uint8_t init)
{
	uint8_t crc = init;
	uint8_t i,j;
	uint8_t genPoly = poly;

	for(i=0; i<len; i++)
	{
		crc ^= *(ptr+i);
		for (j=0; j<8; j++)
		{
			if(crc & 0x80)
				crc = (crc << 1) ^ genPoly;
			else
				crc <<= 1;
		}
	}

	return crc;
}

/*********************************************************************************************************
** Function name:       _CalculateCRC8Byte
** Descriptions:        NXP RCI_V1.1 CRC�㷨
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static uint8_t _CalculateCRC8Byte(uint8_t u8InputByte, uint8_t u8InitialCrc)
{
	uint8_t 	i			= 0;
	uint16_t	u16Crc 		= u8InitialCrc;
	uint8_t		inputShift	= 0;

	for (i=0; i<8; i++) {
		u16Crc		= u16Crc<<1U;
		inputShift	= 8 - i - 1;

		if ((u16Crc > 0xFF)^(((u8InputByte>>inputShift) & 0x01U) == 1U)) {
			u16Crc = u16Crc^0x07;
		}

		u16Crc = u16Crc&0xff;
	}

	return (uint8_t)u16Crc;
}

/*********************************************************************************************************
** Function name:       _CalculateCRC8
** Descriptions:        NXP RCI_V1.1 CRC�㷨
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static uint8_t _CalculateCRC8(uint8_t *pu8FrameBuf, uint8_t u8Len)
{
	uint8_t	u8CurrentCRC	= 0x00;
	uint8_t	idx 			= 0;

	for (idx = 0; idx < u8Len; idx++) {
		u8CurrentCRC = _CalculateCRC8Byte(pu8FrameBuf[idx], u8CurrentCRC);
	}

	return u8CurrentCRC;
}

/*********************************************************************************************************
** Function name:       _NCK2910_RciCmdSend
** Descriptions:        �������2910����ѭNXP RCIЭ��
** input parameters:    u8Cmd : ������
						u8SubCmd : ������
						pu8PayLoadBuf : ���ݻ���ָ��
						u8PayLoadLen : ���ݻ��峤��
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _NCK2910_RciCmdSend(uint8_t u8Cmd, uint8_t u8SubCmd,
								uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i = 1;	// ��һ���ֽ�Ϊ�����ֽڣ������ʼֵΪ1�������ֽ����Ÿ�ֵ
	uint8_t j = 0;
	uint8_t x,y;

	NCK2910_RfEventRead();

	// Step1 : �������֡
	memset(Gu8Nck2910RciTxBuf, 0, 256);
	Gu8Nck2910RciTxBuf[i++] = u8Cmd;								// Cmd
	// ���ָ�ʽ��ECHO�����û��SubCmd��Ҳ����������û��SubCmd
	if ((WI_ECHO_CMD != u8Cmd) && (WI_ECHO_NXP_CMD != u8Cmd)) {
		Gu8Nck2910RciTxBuf[i++] = u8SubCmd;							// SubCmd(��һ����)
	}
	for (j = 0; j < u8PayLoadLen; j++) {
		Gu8Nck2910RciTxBuf[i++] = pu8PayLoadBuf[j];					// PayLoad
	}
	Gu8Nck2910RciTxBuf[0]	= i;									// LEN
	Gu8Nck2910RciTxBuf[i++] = _CalculateCRC8(Gu8Nck2910RciTxBuf, Gu8Nck2910RciTxBuf[0]);// CRC

	NCK2910_SpiWrite(Gu8Nck2910RciTxBuf, i);

//	delay_us(2000);
	uint32_t wait_timeout = 0;
	while ((HAL_PIN_LEVEL_HIGH == hal_pin_if_read(NCK2910_INT_N_PIN)) && (wait_timeout < RF_WAIT_TIMEOUT_CNT)) wait_timeout ++;
//	rf_dbg_Printf("wait_timeout = %d\r\n", wait_timeout);

#if 0
	{
	rf_dbg_Printf("NCK2910 write data: ");
	rf_dbg_Printf("len=%d, data-->", Gu8Nck2910RciTxBuf[0]);
	for(uint8_t x = 0; x <= Gu8Nck2910RciTxBuf[0]; x ++)
	{
		rf_dbg_Printf("%02X ", Gu8Nck2910RciTxBuf[x]);
	}
	rf_dbg_Printf("\r\n");
	}
#endif

	if(0x08 == u8Cmd && 0x02 == u8SubCmd) return TRUE; //ADC_READ return

#if _2910_SPI_COMM_DELAY_1MS
	platform_time_delay(1);
#endif
	if(HAL_SPI_RESULT_SUCCESS != NCK2910_SpiRead(Gu8Nck2910RciRxBuf))	//ʵ�ʶ��ĳ��ȣ��ӵ�һ���ֽ������ж�
	{
		return FALSE;
	}

	// Step4 : ���CRC
//	Gu8Nck2910RciRxLen	= Gu8Nck2910RciRxBuf[0];
//	if (_CalculateCRC8(Gu8Nck2910RciRxBuf, Gu8Nck2910RciRxLen) !=
//		Gu8Nck2910RciRxBuf[Gu8Nck2910RciRxLen]) {
//		return FALSE;
//	}

	// Step5 : ���communication error & command error
	// Gu8Nck2910RciRxBuf[1]ΪCMD�ֽ�
	// Gu8Nck2910RciRxBuf[5]Ϊstatus�ֽ�
	if (0xFF == Gu8Nck2910RciRxBuf[1]) {
		return FALSE;
	} else if (Gu8Nck2910RciRxLen > 5) {
		if (Gu8Nck2910RciRxBuf[5]) {
			return FALSE;
		}
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_DC_SET
** Descriptions:        DC_SET����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_DC_SET(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(DC_SET_CMD, DC_SET_SUB_CMD, pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		rf_dbg_Printf("_RciCmd_DC_SET fail! u8PayLoadLen = %d\r\n", u8PayLoadLen);
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_DC_GET
** Descriptions:        DC_GET����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_DC_GET(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(DC_GET_CMD, DC_GET_SUB_CMD, pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_DC_COPY2RAM
** Descriptions:        DC_COPY2RAM����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_DC_COPY2RAM(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(DC_COPY2RAM_CMD, DC_COPY2RAM_SUB_CMD, pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_DC_COPY2NV
** Descriptions:        DC_COPY2NV����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_DC_COPY2NV(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(DC_COPY2NV_CMD, DC_COPY2NV_SUB_CMD, pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_DC_PATCHRAM
** Descriptions:        DC_PATCHRAM����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_DC_PATCHRAM(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(DC_PATCHRAM_CMD, DC_PATCHRAM_SUB_CMD, pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_DC_HASH
** Descriptions:        DC_HASH����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_DC_HASH(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(DC_HASH_CMD, DC_HASH_SUB_CMD, pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_DC_GET_STATUS
** Descriptions:        DC_GET_STATUS����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_DC_GET_STATUS(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(DC_GET_STATUS_CMD, DC_GET_STATUS_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_OS_SET_POLLING
** Descriptions:        OS_SET_POLLING����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_OS_SET_POLLING(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(OS_SET_POLLING_CMD, OS_SET_POLLING_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		rf_dbg_Printf("_RciCmd_OS_SET_POLLING fail! u8PayLoadLen = %d\r\n", u8PayLoadLen);
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_OS_GET_POLLING
** Descriptions:        OS_GET_POLLING����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_OS_GET_POLLING(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(OS_GET_POLLING_CMD, OS_GET_POLLING_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_OS_STORE_POLLING
** Descriptions:        OS_STORE_POLLING����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_OS_STORE_POLLING(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(OS_STORE_POLLING_CMD, OS_STORE_POLLING_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_OS_SET_MODE
** Descriptions:        OS_SET_MODE����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_OS_SET_MODE(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(OS_SET_MODE_CMD, OS_SET_MODE_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		rf_dbg_Printf("_RciCmd_OS_SET_MODE fail! u8PayLoadLen = %d, pu8PayLoadBuf[0] = 0x%02x\r\n", u8PayLoadLen, pu8PayLoadBuf[0]);
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_OS_STATUS
** Descriptions:        OS_STATUS����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_OS_STATUS(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(OS_STATUS_CMD, OS_STATUS_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		rf_dbg_Printf("_RciCmd_OS_STATUS fail! u8PayLoadLen = %d\r\n", u8PayLoadLen);
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_RB_READ_MSG
** Descriptions:        RB_READ_MSG����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_RB_READ_MSG(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(RB_READ_MSG_CMD, RB_READ_MSG_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_RB_READ_BYTES
** Descriptions:        RB_READ_BYTES����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_RB_READ_BYTES(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(RB_READ_BYTES_CMD, RB_READ_BYTES_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_RB_FLUSH_MSG
** Descriptions:        RB_FLUSH_MSG����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_RB_FLUSH_MSG(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(RB_FLUSH_MSG_CMD, RB_FLUSH_MSG_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_RB_STATUS
** Descriptions:        RB_STATUS����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_RB_STATUS(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(RB_STATUS_CMD, RB_STATUS_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_PE_VERSIONS
** Descriptions:        PE_VERSIONS����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_PE_VERSIONS(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(PE_VERSIONS_CMD, PE_VERSIONS_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_PE_CAPABILITY
** Descriptions:        PE_CAPABILITY����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_PE_CAPABILITY(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(PE_CAPABILITY_CMD, PE_CAPABILITY_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_DD_SET
** Descriptions:        DD_SET����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_DD_SET(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(DD_SET_CMD, DD_SET_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		rf_dbg_Printf("_RciCmd_DD_SET fail! payloadlen = %d\r\n", u8PayLoadLen);
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_DD_GET
** Descriptions:        DD_GET����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_DD_GET(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(DD_GET_CMD, DD_GET_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		rf_dbg_Printf("_RciCmd_DD_GET fail! u8PayLoadLen = %d\r\n", u8PayLoadLen);
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_DD_HASH
** Descriptions:        DD_HASH����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_DD_HASH(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(DD_HASH_CMD, DD_HASH_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_DD_SIG
** Descriptions:        DD_SIG����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_DD_SIG(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(DD_SIG_CMD, DD_SIG_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_DD_TIME
** Descriptions:        DD_TIME����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_DD_TIME(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(DD_TIME_CMD, DD_TIME_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_WI_CONFIG
** Descriptions:        WI_CONFIG����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_WI_CONFIG(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(WI_CONFIG_CMD, WI_CONFIG_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_WI_CLEAR
** Descriptions:        WI_CLEAR����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_WI_CLEAR(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(WI_CLEAR_CMD, WI_CLEAR_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_WI_STORE
** Descriptions:        WI_STORE����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_WI_STORE(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(WI_STORE_CMD, WI_STORE_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_WI_ECHO
** Descriptions:        WI_ECHO����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_WI_ECHO(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(WI_ECHO_CMD, WI_ECHO_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_WI_ECHO_NXP
** Descriptions:        WI_ECHO_NXP����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_WI_ECHO_NXP(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(WI_ECHO_NXP_CMD, WI_ECHO_NXP_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_OBM_CONF
** Descriptions:        OBM_CONF����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_OBM_CONF(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(OBM_CONF_CMD, OBM_CONF_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		rf_dbg_Printf("_RciCmd_OBM_CONF fail! u8PayLoadLen = %d, pu8PayLoadBuf[0] = 0x%02x\r\n", u8PayLoadLen, pu8PayLoadBuf[0]);
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_OBM_READ
** Descriptions:        OBM_READ����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_OBM_READ(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(OBM_READ_CMD, OBM_READ_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		rf_dbg_Printf("_RciCmd_OBM_READ fail! u8PayLoadLen = %d, pu8PayLoadBuf[0] = 0x%02x\r\n", u8PayLoadLen, pu8PayLoadBuf[0]);
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_OBM_RSSI_OFMU
** Descriptions:        OBM_RSSI_OFMU����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_OBM_RSSI_OFMU(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(OBM_RSSI_OFMU_CMD, OBM_RSSI_OFMU_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_OBM_SET_FREQUENCY_OFFSET_OVERRIDE
** Descriptions:        OBM_SET_FREQUENCY_OFFSET_OVERRIDE����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_OBM_SET_FREQUENCY_OFFSET_OVERRIDE(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(OBM_SET_FREQUENCY_OFFSET_OVERRIDE_CMD,
								OBM_SET_FREQUENCY_OFFSET_OVERRIDE_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_OBM_GET_FREQUENCY_OFFSET
** Descriptions:        OBM_GET_FREQUENCY_OFFSET����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_OBM_GET_FREQUENCY_OFFSET(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(OBM_GET_FREQUENCY_OFFSET_CMD, OBM_GET_FREQUENCY_OFFSET_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_OBM_DO_LO_CORRECTION
** Descriptions:        OBM_DO_LO_CORRECTION����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_OBM_DO_LO_CORRECTION(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(OBM_DO_LO_CORRECTION_CMD, OBM_DO_LO_CORRECTION_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_MISC_WAIT_US
** Descriptions:        MISC_WAIT_US����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_MISC_WAIT_US(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(MISC_WAIT_US_CMD, MISC_WAIT_US_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_MISC_PIN_TOGGLE
** Descriptions:        MISC_PIN_TOGGLE����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_MISC_PIN_TOGGLE(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(MISC_PIN_TOGGLE_CMD, MISC_PIN_TOGGLE_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_EV_TIMER_HEARTBEAT
** Descriptions:        EV_TIMER_HEARTBEAT����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_EV_TIMER_HEARTBEAT(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(EV_TIMER_HEARTBEAT_CMD, EV_TIMER_HEARTBEAT_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_TX_TRIGGER
** Descriptions:        TX_TRIGGER����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_TX_TRIGGER(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(TX_TRIGGER_CMD, TX_TRIGGER_SUB_CMD, pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_TX_WRITE_PAYLOAD
** Descriptions:        TX_WRITE_PAYLOAD����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_TX_WRITE_PAYLOAD(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(TX_WRITE_PAYLOAD_CMD, TX_WRITE_PAYLOAD_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _RciCmd_TX_READ_PAYLOAD
** Descriptions:        TX_READ_PAYLOAD����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _RciCmd_TX_READ_PAYLOAD(uint8_t *pu8PayLoadBuf, uint8_t u8PayLoadLen)
{
	uint8_t	i;

	for (i = 0; i < 3; i++) {
		if(_NCK2910_RciCmdSend(TX_READ_PAYLOAD_CMD, TX_READ_PAYLOAD_SUB_CMD,
								pu8PayLoadBuf, u8PayLoadLen)) {
			break;
		}
	}

	if (3 == i) {
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************************************************
** Function name:       _NCK2910_IoInit
** Descriptions:        ��ʼ��NCK2910���IO��
**						NCK2910_RDYN	: PTF2
						NCK2910_RSTN	: PTD2
						NCK2910_INTN	: PTF0
						NCK2910_CSN		: PTG7
						NCK2910_SCLK	: PTG4
						NCK2910_MISO	: PTG6
						NCK2910_MOSI	: PTG5
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static void _NCK2910_IoInit(void)
{
	//hal_pin_if_init();
	//hal_pin_if_write(NCK2910_INT_N_PIN, HAL_PIN_LEVEL_HIGH);
	hal_pin_if_write(K2910_CS_INDEX,HAL_PIN_LEVEL_HIGH);
	//hal_uart_if_init(1);
	hal_spi_if_init(NCK2910_SPI_CHANNEL);
	NVIC_SetPriority(GPIOA_INT0_IRQn, 3);
}

/*********************************************************************************************************
** Function name:       _NCK2910_PFlashInit
** Descriptions:        ��ʼ��NCK2910��PFlash
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _NCK2910_PFlashInit(void)
{
	bool_t ret;
	uint8_t	u8TmpBuf[3]	= {0};

	u8TmpBuf[0]	= 0x01;
	u8TmpBuf[1]	= 0xB2;
	u8TmpBuf[2]	= 0x7E;

	ret = _RciCmd_DD_GET(u8TmpBuf, 3);
	if (ret)
	{
		if (Gu8Nck2910RciRxBuf[6] != c_u8PflashCfgBuf[3])
		{
			rf_dbg_Printf("NO.1-read back PFlashInit Par is not same, Gu8Nck2910RciRxBuf[6] = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6]);
			ret = _RciCmd_DD_GET(u8TmpBuf, 3);
			if (ret)
			{
				if (Gu8Nck2910RciRxBuf[6] != c_u8PflashCfgBuf[3])
				{
					rf_dbg_Printf("NO.2-read back PFlashInit Par is not same, Gu8Nck2910RciRxBuf[6] = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6]);
					ret = _RciCmd_DD_GET(u8TmpBuf, 3);
					if (ret)
					{
						if (Gu8Nck2910RciRxBuf[6] != c_u8PflashCfgBuf[3])
						{
							rf_dbg_Printf("NO.3-read back PFlashInit Par is not same, Gu8Nck2910RciRxBuf[6] = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6]);
							rf_dbg_Printf("_NCK2910_PFlashInit()\r\n");
							ret = _RciCmd_DD_SET((uint8_t *)c_u8PflashCfgBuf, PFLASH_CFG_BUF_LEN);	// ʹ��auto flush
						}
						else
						{
							rf_dbg_Printf("NO.3-read back PFlashInit Par is same, Gu8Nck2910RciRxBuf[6] = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6]);
						}
					}
				}
				else
				{
					rf_dbg_Printf("NO.2-read back PFlashInit Par is same, Gu8Nck2910RciRxBuf[6] = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6]);
				}
			}
		}
		else
		{
			rf_dbg_Printf("NO.1-read back PFlashInit Par is same, Gu8Nck2910RciRxBuf[6] = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6]);
		}
	}
	return ret;
}




static bool_t _NCK2910_FallbackTimeoutInit(void)
{
	bool_t ret;
	uint8_t	u8TmpBuf[3]	= {0};

	u8TmpBuf[0]	= 0x02;
	u8TmpBuf[1]	= 0x1e;
	u8TmpBuf[2]	= 0x7a;

	ret = _RciCmd_DD_GET(u8TmpBuf, 3);
	if (ret)
	{
		if (Gu8Nck2910RciRxBuf[6] != c_u8FallbackTimeoutCfgBuf[3] || Gu8Nck2910RciRxBuf[7] != c_u8FallbackTimeoutCfgBuf[4])
		{
			rf_dbg_Printf("NO.1-read back FallbackTimeoutInit Par is not same, Gu8Nck2910RciRxBuf[6] = 0x%02X, Gu8Nck2910RciRxBuf[7] = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6], Gu8Nck2910RciRxBuf[7]);
			ret = _RciCmd_DD_GET(u8TmpBuf, 3);
			if (ret)
			{
				if (Gu8Nck2910RciRxBuf[6] != c_u8FallbackTimeoutCfgBuf[3] || Gu8Nck2910RciRxBuf[7] != c_u8FallbackTimeoutCfgBuf[4])
				{
					rf_dbg_Printf("NO.2-read back FallbackTimeoutInit Par is not same, Gu8Nck2910RciRxBuf[6] = 0x%02X, Gu8Nck2910RciRxBuf[7] = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6], Gu8Nck2910RciRxBuf[7]);
					ret = _RciCmd_DD_GET(u8TmpBuf, 3);
					if (ret)
					{
						if (Gu8Nck2910RciRxBuf[6] != c_u8FallbackTimeoutCfgBuf[3] || Gu8Nck2910RciRxBuf[7] != c_u8FallbackTimeoutCfgBuf[4])
						{
							rf_dbg_Printf("NO.3-read back FallbackTimeoutInit Par is not same, Gu8Nck2910RciRxBuf[6] = 0x%02X, Gu8Nck2910RciRxBuf[7] = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6], Gu8Nck2910RciRxBuf[7]);
							rf_dbg_Printf("_NCK2910_FallbackTimeoutInit()\r\n");
							ret = _RciCmd_DD_SET((uint8_t *)c_u8FallbackTimeoutCfgBuf, sizeof(c_u8FallbackTimeoutCfgBuf));
						}
						else
						{
							rf_dbg_Printf("NO.3-read back FallbackTimeoutInit Par is same, Gu8Nck2910RciRxBuf[6] = 0x%02X, Gu8Nck2910RciRxBuf[7] = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6], Gu8Nck2910RciRxBuf[7]);
						}
					}
				}
				else
				{
					rf_dbg_Printf("NO.2-read back FallbackTimeoutInit Par is same, Gu8Nck2910RciRxBuf[6] = 0x%02X, Gu8Nck2910RciRxBuf[7] = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6], Gu8Nck2910RciRxBuf[7]);
				}
			}
		}
		else
		{
			rf_dbg_Printf("NO.1-read back FallbackTimeoutInit Par is same, Gu8Nck2910RciRxBuf[6] = 0x%02X, Gu8Nck2910RciRxBuf[7] = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6], Gu8Nck2910RciRxBuf[7]);
		}
	}
	return ret;
}

/*********************************************************************************************************
** Function name:       _NCK2910_RfParaInit
** Descriptions:        ��ʼ��NCK2910��ص�RF����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
static bool_t _NCK2910_RfParaInit(void)
{
	bool_t ret;
	uint8_t	i;

	// step1 : OS_SET_MODE����idleģʽ
	ret = NCK2910_Idle();

//	NCK2910_GetPrintOSStatus();

	// step2 : DC_SET����RF����
	for(i = 0; i < RX_CFG_NUM; i++) {
		if(ret)
		ret = _RciCmd_DC_SET((uint8_t *)&c_u8LoCfgBuf[i][0], LO_CFG_BUF_LEN);			// Local oscillator
		if(ret)
		ret = _RciCmd_DC_SET((uint8_t *)&c_u8DigFreqBuf[i][0], DIG_FREQ_BUF_LEN);		// Digital frequency configuration
		if(ret)
		ret = _RciCmd_DC_SET((uint8_t *)&c_u8FilterBuf[i][0], FILTER_BUF_LEN);		// Channel filter configuration
		if(ret)
		ret = _RciCmd_DC_SET((uint8_t *)&c_u8BaseBandBuf[i][0], BASEBAND_BUF_LEN);	// Baseband configuration
		if(ret)
		ret = _RciCmd_DC_SET((uint8_t *)&c_u8FrameWupBuf[i][0], FRAME_WUP_BUF_LEN);	// Frame/WUP configuration

		if(ret)
		ret = _RciCmd_DC_SET((uint8_t *)&c_u8SwBuf[i][0], SW_BUF_LEN);				// SW configuration
		if(ret)
		ret = _RciCmd_DC_SET((uint8_t *)&c_u8MidMapBuf[i][0], MID_MAP_BUF_LEN);		// MIDMAP
		if(ret)
		ret = _RciCmd_DC_SET((uint8_t *)&c_u8MidBuf[i][0], MID_BUF_LEN);				// MID
		if(ret)
		ret = _RciCmd_DC_SET((uint8_t *)&c_u8SlotCfgBuf[i][0], SLOT_CFG_BUF_LEN);		// Slot configuration
		if(ret)
		ret = _RciCmd_DC_SET((uint8_t *)&c_u8ReceptionCfgBuf[i][0], REC_CFG_BUF_LEN);	// Reception configuration
	}

//	// step3 : OS_SET_POLLING����polling����
//	_RciCmd_OS_SET_POLLING((uint8_t *)c_u8PollingCfgBuf, POLLING_CFG_BUF_LEN);
//
//	// step4 : OS_SET_MODE����pollingģʽ����ʼpolling
//	NCK2910_Polling();

	if(ret)
	{
		ret = NCK2910_CfgAndSetPolling();
	}

	NCK2910_GetPrintOSStatus();

	// 配置为常接收
//	NCK2910_ConRx(0);
	return ret;
}

static bool_t NCK2910_InitParameFunc(void)
{
	bool_t ret;
	ret = _NCK2910_PFlashInit();
	if(ret)
	{
		ret = _NCK2910_FallbackTimeoutInit();
	}
	if(ret)
	{
		ret = _NCK2910_RfParaInit();
	}
	return ret;
}

static bool_t NCK2910_InitFunc(void)
{
	bool_t ret;
	ret = NCK2910_InitParameFunc();
	if(!ret)
	{
		dbg_Printf("RF Initialization1 failed..\r\n");
		hal_wdt_if_feed(0);
		ret = NCK2910_InitParameFunc();
		if(!ret)
		{
			dbg_Printf("RF Initialization2 failed..\r\n");
			hal_wdt_if_feed(0);
			ret = NCK2910_InitParameFunc();
			if(!ret)
			{
				/* Record Error Log */
				dbg_Printf("RF Initialization3 failed..\r\n");
				/* Record DTC */
				RFInitDTC_recordSet();
			}else RFInitDTC_recoverySet();
		}else RFInitDTC_recoverySet();
	}else RFInitDTC_recoverySet();
	return ret;
}


/*********************************************************************************************************
** Function name:       NCK2910_Init
** Descriptions:        ��ʼ��NCK2910
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
uint8_t NCK2910_Init(void)
{
	bool_t  ret;
	RF_TpmsID_Init();

	_NCK2910_IoInit();
	RFData_if_QueueInit(&RF_DataQueue);

	ret = NCK2910_InitFunc();

	if(ret)
	{
		hal_pin_if_irq(NCK2910_INT_N_PIN, 5, _NCK2910_Ready);
	}
	return ret;
}

/*********************************************************************************************************
** Function name:       NCK2910_Reset
** Descriptions:        ��λNCK2910
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
bool_t NCK2910_Reset(void)
{
#if HW_RST == 0x01
	NCK2910_RSTN_LOW();
	DelayRstHoldTime();			// ��Ч��λ�źű���ʱ��
	NCK2910_RSTN_HIGH();
	return TRUE;
#else
	uint8_t	u8PayloadBuf[2];

	u8PayloadBuf[0]	= OM_RESET;
	u8PayloadBuf[1]	= 0x00;
	if(_RciCmd_OS_SET_MODE(u8PayloadBuf, 0x02)) {
		return TRUE;
	}
	return FALSE;
#endif
}

/*********************************************************************************************************
** Function name:       NCK2910_Standby
** Descriptions:        NCK2910����Standbyģʽ��Standbyģʽ�£�CPU״̬��RAM�����ݱ��֣�polling timer��
**						watchdog��Ч
** input parameters:    u8StandbyTime : ͣ����Standby״̬�µ�ʱ��
** output parameters:
** Returned value:
*********************************************************************************************************/
bool_t NCK2910_Standby(uint8_t u8StandbyTime)
{
	uint8_t	u8PayloadBuf[2];

	u8PayloadBuf[0]	= OM_STANDBY;
	u8PayloadBuf[1]	= u8StandbyTime;
	if(_RciCmd_OS_SET_MODE(u8PayloadBuf, 0x02)) {
		return TRUE;
	}
	return FALSE;
}

/*********************************************************************************************************
** Function name:       NCK2910_Idle
** Descriptions:        NCK2910����idleģʽ
** input parameters:    idleģʽ�£�RX�������رգ�RCI����������Ӧ
** output parameters:
** Returned value:
*********************************************************************************************************/
bool_t NCK2910_Idle(void)
{
	uint8_t	u8PayloadBuf[2];

	u8PayloadBuf[0]	= OM_IDLE;
	u8PayloadBuf[1]	= 0x00;
	if (_RciCmd_OS_SET_MODE(u8PayloadBuf, 0x02)) {
		return TRUE;
	}
	return FALSE;
}

/*********************************************************************************************************
** Function name:       NCK2910_Sleep
** Descriptions:        NCK2910����Sleepģʽ��Sleepģʽ�£�CPU״̬��RAM�����ݱ��֣�polling timer��
**						watchdog��Ч��֧�ַ�ƵXO Clock���
** input parameters:    u8SleepTime : ͣ����sleepģʽ�µ�ʱ��
** output parameters:
** Returned value:
*********************************************************************************************************/
bool_t NCK2910_Sleep(uint8_t u8SleepTime)
{
	uint8_t	u8PayloadBuf[2];

	u8PayloadBuf[0]	= OM_SLEEP;
	u8PayloadBuf[1]	= u8SleepTime;
	if (_RciCmd_OS_SET_MODE(u8PayloadBuf, 0x02)) {
		return TRUE;
	}
	return FALSE;
}

/*********************************************************************************************************
** Function name:       NCK2910_ConRx
** Descriptions:        NCK2910������������ģʽ
** input parameters:    u8RxCfgIndex : �����������ڼ䣬���õ����ĸ�RF RX���������գ������õ�������
** output parameters:
** Returned value:
*********************************************************************************************************/
bool_t NCK2910_ConRx(uint8_t u8RxCfgIndex)
{
	uint8_t	u8PayloadBuf[2];

	u8PayloadBuf[0]	= OM_CON_RCV;
	u8PayloadBuf[1]	= u8RxCfgIndex;
	if (_RciCmd_OS_SET_MODE(u8PayloadBuf, 0x02)) {
		return TRUE;
	}
	return FALSE;
}

/*********************************************************************************************************
** Function name:       NCK2910_Polling
** Descriptions:        NCK2910����Pollingģʽ
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
bool_t NCK2910_Polling(void)
{
	uint8_t	u8PayloadBuf[2];

	u8PayloadBuf[0]	= OM_POLLING;
	u8PayloadBuf[1]	= 0x00;
	if (_RciCmd_OS_SET_MODE(u8PayloadBuf, 0x02)) {
		return TRUE;
	}
	return FALSE;
}

/*********************************************************************************************************
** Function name:       NCK2910_OS_GetStatus
** Descriptions:
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
uint8_t NCK2910_OS_GetStatus(void)
{
	uint8_t os_status = 0xFF;
	if(TRUE == _RciCmd_OS_STATUS(NULL, 0))
	{
		if(Gu8Nck2910RciRxBuf[0] == 0x0A && Gu8Nck2910RciRxBuf[1] == 0x06 && Gu8Nck2910RciRxBuf[2] == 0x00 && Gu8Nck2910RciRxBuf[5] == 0x00)
		{
			os_status = Gu8Nck2910RciRxBuf[6];
		}
	}
	return os_status;
}

/*********************************************************************************************************
** Function name:       NCK2910_ADC_Conf
** Descriptions:
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
bool_t NCK2910_ADC_Conf(void)
{
	uint8_t	u8PayloadBuf[2];

	u8PayloadBuf[0]	= 0x00;
	if (_RciCmd_OBM_CONF(u8PayloadBuf, 0x01)) {
		return TRUE;
	}
	return FALSE;
}

/*********************************************************************************************************
** Function name:       NCK2910_ADC_Read
** Descriptions:
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
bool_t NCK2910_ADC_Read(void)
{
	uint8_t	u8PayloadBuf[2];

	u8PayloadBuf[0]	= 3;   //amount of measurements
	u8PayloadBuf[1]	= 40;  //Sample time Tsample  2.4ms
	if (_RciCmd_OBM_READ(u8PayloadBuf, 0x02)) {
		return TRUE;
	}
	return FALSE;
}

/*********************************************************************************************************
** Function name:       NCK2910_PowerOff
** Descriptions:        NCK2910����PowerOffģʽ��PowerOffģʽ�£�polling timer��watchdog��Ч
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
bool_t NCK2910_PowerOff(void)
{
	uint8_t	u8PayloadBuf[2];

	u8PayloadBuf[0]	= OM_TRANSPORT;
	u8PayloadBuf[1]	= 0x01;				// �ò���Ϊ1������poweroffģʽ���ò���Ϊ0������transportģʽ
	if (_RciCmd_OS_SET_MODE(u8PayloadBuf, 0x02)) {
		return TRUE;
	}
	return FALSE;
}

/*********************************************************************************************************
** Function name:       NCK2910_Transport
** Descriptions:        NCK2910����Transportģʽ��Transportģʽ�£���ģʽ����͹��ģ���ҪRST_N������
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
bool_t NCK2910_Transport(void)
{
	uint8_t	u8PayloadBuf[2];

	u8PayloadBuf[0]	= OM_TRANSPORT;
	u8PayloadBuf[1]	= 0x00;				// �ò���Ϊ1������poweroffģʽ���ò���Ϊ0������transportģʽ
	if (_RciCmd_OS_SET_MODE(u8PayloadBuf, 0x02)) {
		return TRUE;
	}
	return FALSE;
}

/*********************************************************************************************************
** Function name:       NCK2910_ErrorHandle
** Descriptions:        NCK2910���ϴ���
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
void NCK2910_ErrorHandle(void)
{

}

bool_t NCK2910_CfgAndSetPolling(void)
{
	bool_t ret = TRUE;
#if SUPPORT_RECV_RATE_TEST && CONTINOUS_RECEPTION_FSK_TEST_EN

	rf_dbg_Printf("Enter TPMS ConRx.\r\n");
	NCK2910_ConRx(1); /* 0是ChargePile, 1是TPMS */
	return ret;

#elif SUPPORT_RECV_RATE_TEST && CONTINOUS_RECEPTION_ASK_TEST_EN

	rf_dbg_Printf("Enter ChargePile ConRx.\r\n");
	NCK2910_ConRx(0); /* 0是ChargePile, 1是TPMS */
	return ret;

#else
#if SUPPORT_RECV_RATE_TEST
	if(TPMS_IDs[2] == 0x00000001)
	{
		rf_dbg_Printf("Enter TPMS ConRx.\r\n");
		NCK2910_ConRx(1); /* 0是ChargePile, 1是TPMS */
		return ret;
	}
	else if(TPMS_IDs[2] == 0x00000002)
	{
		rf_dbg_Printf("Enter ChargePile ConRx.\r\n");
		NCK2910_ConRx(0); /* 0是ChargePile, 1是TPMS */
		return ret;
	}
#endif
	VehStateDetailed_t vehicleStateDetailed = ZONE_13C_t.msg.sig.VehStateDetailed;
	if(vehicleStateDetailed == VEHSTDRIVING_13)
	{
		ret = _RciCmd_OS_SET_POLLING((uint8_t *)c_u8PollingCfgBufOnlyTpms, sizeof(c_u8PollingCfgBufOnlyTpms));
		if(ret)
		{
			rf_dbg_Printf("Enter TPMS Polling.\r\n");
			ret = NCK2910_Polling();
		}
	}
	else
	{
		ret = _RciCmd_OS_SET_POLLING((uint8_t *)c_u8PollingCfgBuf, sizeof(c_u8PollingCfgBuf));
		if(ret)
		{
			rf_dbg_Printf("Enter TPMS&ChargePile Polling.\r\n");
			ret = NCK2910_Polling();
		}
	}
#endif
	return ret;
}

void NCK2910_GetPrintOSStatus(void)
{
#if RF_DEBUG
	rf_dbg_Printf("#OS_Status = %s, 0x%02X\r\n", OS_STATUS_VALUE_CONVERT_STRING(NCK2910_OS_GetStatus(), current_os_status));
	if(NCK2910_OS_STATUS_POLLING == current_os_status)
	{
		received_frame_count = 0;
	}
#endif
}

static uint8_t TpmsID_SaveInfo[16] = {0}, TpmsID_SaveInfoTemp[16] = {0};
void RF_TpmsID_Init(void)
{
	/* 读取flash数据16个字节 */
	uds_write_flash_data_un did_flash_data_un;
	if(HAL_RESULT_SUCCESS == Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,did_flash_data_un.write_buffer,DID_WRITE_DATA_LEN))
	{
		uint8_t ind = 0;
		memcpy(TpmsID_SaveInfo, did_flash_data_un.sig.TPMS_ID, sizeof(TpmsID_SaveInfo));
		TPMS_IDs[0] = (TpmsID_SaveInfo[ind++] << 24) | (TpmsID_SaveInfo[ind++] << 16) | (TpmsID_SaveInfo[ind++] << 8) | TpmsID_SaveInfo[ind++];
		TPMS_IDs[1] = (TpmsID_SaveInfo[ind++] << 24) | (TpmsID_SaveInfo[ind++] << 16) | (TpmsID_SaveInfo[ind++] << 8) | TpmsID_SaveInfo[ind++];
		TPMS_IDs[2] = (TpmsID_SaveInfo[ind++] << 24) | (TpmsID_SaveInfo[ind++] << 16) | (TpmsID_SaveInfo[ind++] << 8) | TpmsID_SaveInfo[ind++];
		TPMS_IDs[3] = (TpmsID_SaveInfo[ind++] << 24) | (TpmsID_SaveInfo[ind++] << 16) | (TpmsID_SaveInfo[ind++] << 8) | TpmsID_SaveInfo[ind++];
		rf_dbg_Printf("Read TpmsID_SaveInfo is success! \r\n");
		rf_dbg_Printf("TPMS_IDs[0] = %08X\r\n", TPMS_IDs[0]);
		rf_dbg_Printf("TPMS_IDs[1] = %08X\r\n", TPMS_IDs[1]);
		rf_dbg_Printf("TPMS_IDs[2] = %08X\r\n", TPMS_IDs[2]);
		rf_dbg_Printf("TPMS_IDs[3] = %08X\r\n", TPMS_IDs[3]);
	}
	else
	{
		memset(TpmsID_SaveInfo, 0, sizeof(TpmsID_SaveInfo));
		TPMS_IDs[0] = 0;
		TPMS_IDs[1] = 0;
		TPMS_IDs[2] = 0;
		TPMS_IDs[3] = 0;
		rf_dbg_Printf("Error: Read TpmsID Information is fail! Default all zero.\r\n");
	}
}

uint8_t RF_RecvTpmsIdFlag = 0;
void RF_TpmsID_Check(void)
{
	if(RF_RecvTpmsIdFlag)
	{
		uint32_t TPMS_IDsTemp[4];
		uint8_t ind = 0;

		RF_RecvTpmsIdFlag = 0;

		OSA_InterruptDisable();
		memcpy(TpmsID_SaveInfoTemp, &ZONE_L_TPMS_01_t.msg.data[18], sizeof(TpmsID_SaveInfoTemp));
		OSA_InterruptEnable();
		TPMS_IDsTemp[0] = (TpmsID_SaveInfoTemp[ind++] << 24) | (TpmsID_SaveInfoTemp[ind++] << 16) | (TpmsID_SaveInfoTemp[ind++] << 8) | TpmsID_SaveInfoTemp[ind++];
		TPMS_IDsTemp[1] = (TpmsID_SaveInfoTemp[ind++] << 24) | (TpmsID_SaveInfoTemp[ind++] << 16) | (TpmsID_SaveInfoTemp[ind++] << 8) | TpmsID_SaveInfoTemp[ind++];
		TPMS_IDsTemp[2] = (TpmsID_SaveInfoTemp[ind++] << 24) | (TpmsID_SaveInfoTemp[ind++] << 16) | (TpmsID_SaveInfoTemp[ind++] << 8) | TpmsID_SaveInfoTemp[ind++];
		TPMS_IDsTemp[3] = (TpmsID_SaveInfoTemp[ind++] << 24) | (TpmsID_SaveInfoTemp[ind++] << 16) | (TpmsID_SaveInfoTemp[ind++] << 8) | TpmsID_SaveInfoTemp[ind++];
		if(TPMS_IDs[0] != TPMS_IDsTemp[0] \
		|| TPMS_IDs[1] != TPMS_IDsTemp[1] \
		|| TPMS_IDs[2] != TPMS_IDsTemp[2] \
		|| TPMS_IDs[3] != TPMS_IDsTemp[3] \
		)
		{
			OSA_InterruptDisable();
			TPMS_IDs[0] = TPMS_IDsTemp[0];
			TPMS_IDs[1] = TPMS_IDsTemp[1];
			TPMS_IDs[2] = TPMS_IDsTemp[2];
			TPMS_IDs[3] = TPMS_IDsTemp[3];
			OSA_InterruptEnable();

			/* 存储flash数据16个字节 */
			uds_write_flash_data_un did_flash_data_un;
			if(HAL_RESULT_SUCCESS == Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,did_flash_data_un.write_buffer,DID_WRITE_DATA_LEN))
			{
				memcpy(did_flash_data_un.sig.TPMS_ID, TpmsID_SaveInfoTemp, sizeof(TpmsID_SaveInfoTemp));
				if(HAL_RESULT_SUCCESS != Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID,did_flash_data_un.write_buffer,DID_WRITE_DATA_LEN))
				{
					rf_dbg_Printf("Error: Save TpmsID Information is fail! \r\n");
				}
				else
				{
					rf_dbg_Printf("Save TpmsID Information is success! \r\n");
					rf_dbg_Printf("TPMS_IDsTemp[0] = %08X\r\n", TPMS_IDsTemp[0]);
					rf_dbg_Printf("TPMS_IDsTemp[1] = %08X\r\n", TPMS_IDsTemp[1]);
					rf_dbg_Printf("TPMS_IDsTemp[2] = %08X\r\n", TPMS_IDsTemp[2]);
					rf_dbg_Printf("TPMS_IDsTemp[3] = %08X\r\n", TPMS_IDsTemp[3]);
				}
			}
			else
			{
				rf_dbg_Printf("Error: Read did_flash_data is fail! Ready to save TpmsID Information.\r\n");
			}
		}
	}
}

static void NCK2910_RfEventRead(void)
{
	uint8_t LEN, EventCode, EventSubCode;
	uint32_t wait_timeout = 0;
	while ((HAL_PIN_LEVEL_LOW == hal_pin_if_read(NCK2910_INT_N_PIN)) && (wait_timeout < RF_WAIT_TIMEOUT_TRYCNT))
	{
		if(HAL_SPI_RESULT_SUCCESS == NCK2910_SpiRead(Gu8Nck2910RciRxBuf))
		{
			wait_timeout = 0;
			LEN = Gu8Nck2910RciRxBuf[0];
			EventCode = Gu8Nck2910RciRxBuf[1];
			EventSubCode = Gu8Nck2910RciRxBuf[2];
			switch(EventCode)
			{
				case 0x08:
					rf_dbg_Printf("RfEventRead: ADC event, EventCode = 0x%02X\r\n", EventCode);
					break;

				case 0x0F:
					rf_dbg_Printf("RfEventRead: received data:");
					rf_dbg_Printf("%02X  ", Gu8Nck2910RciRxBuf[0]);

					rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[1]);
					rf_dbg_Printf("%02X  ", Gu8Nck2910RciRxBuf[2]);

					rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[3]);
					rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[4]);
					rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[5]);
					rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[6]);
					rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[7]);
					rf_dbg_Printf("%02X     ", Gu8Nck2910RciRxBuf[8]);
					for(uint8_t i=9; i<LEN; i++) rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[i]);
					rf_dbg_Printf("     ");
					rf_dbg_Printf("%02X ---------->> %d", Gu8Nck2910RciRxBuf[LEN], ++received_frame_count);
					rf_dbg_Printf("\r\n");
					break;

				case 0x85:
					rf_dbg_Printf("RfEventRead: RSSI event.\r\n");
					break;

				case 0x84:
					if(0xF4 == EventSubCode)
					{
						rf_dbg_Printf("RfEventRead: PLL unlock\r\n");
					}
					else
					{
						rf_dbg_Printf("RfEventRead: EventCode = 0x%02X, EventSubCode = 0x%02X\r\n", EventCode, EventSubCode);
					}
					break;

				case 0x80:
					rf_dbg_Printf("RfEventRead: Reboot event, Reason = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6]);
					break;

				case 0x81:
					rf_dbg_Printf("RfEventRead: Heartbeat event.\r\n");
					break;

				case 0xFF:
					rf_dbg_Printf("RfEventRead: Communication error, Status = 0x%02X\r\n", Gu8Nck2910RciRxBuf[5]);
					break;

				default:
					rf_dbg_Printf("RfEventRead: Unknown event! EventCode = 0x%02X, EventSubCode = 0x%02X, LEN = %d, Status = 0x%02X\r\n", EventCode, EventSubCode, LEN, Gu8Nck2910RciRxBuf[5]);
					break;
			}
		}
		else
		{
			wait_timeout ++;
		}
	}
}

void NCK2910_RfEventProc(void)
{
	uint32_t wait_timeout = 0;
	uint8_t LEN, EventCode, EventSubCode;
	static uint8_t RC_value = 0;
	static BtaOutputRfData_msg rfdata_msg;
	static uint32_t rf_proc_time_point;

	rf_task_timeout_timer = 0;
	NCK2910_TmpCnt = 0;
	RfEventProcCnt = 0;
	while ((HAL_PIN_LEVEL_LOW == hal_pin_if_read(NCK2910_INT_N_PIN)) && (wait_timeout < RF_WAIT_TIMEOUT_TRYCNT))
	{
		NCK2910_ProcessFlag = 1;
		RfEventProcCnt ++;
		recv_data_cha = 0xff;
		if(HAL_SPI_RESULT_SUCCESS == NCK2910_SpiRead(Gu8Nck2910RciRxBuf))
		{
			Nm_StateType nmstate;
			Nm_ModeType nmmode;
			wait_timeout = 0;
			if(!recv_cnt_zero_flag) RfEventSuccProcCnt ++;
			LEN = Gu8Nck2910RciRxBuf[0];
			EventCode = Gu8Nck2910RciRxBuf[1];
			EventSubCode = Gu8Nck2910RciRxBuf[2];
			rf_proc_time_point = SysTimerGetCurrentTime(); //用于计算293处理时间的起始位置
			switch(EventCode)
			{
				case 0x08:
					if(0x02 == EventSubCode && 0x09 == LEN)
					{
						if(0x00 == Gu8Nck2910RciRxBuf[5])
						{
							//600mV at -40°C, 1050mV at 125 °C
							NCK2910_ADCValue = (Gu8Nck2910RciRxBuf[8] << 8) | Gu8Nck2910RciRxBuf[7];
							rf_dbg_Printf("New NCK2910_ADCValue = %d, Last_NCK2910_ADCValue = %d\r\n", NCK2910_ADCValue, Last_NCK2910_ADCValue);
							if(0 == Last_NCK2910_ADCValue)
							{
								Last_NCK2910_ADCValue = NCK2910_ADCValue;
							}
							if(Last_NCK2910_ADCValue != NCK2910_ADCValue)
							{
								uint16_t Diff = 0;
								if(Last_NCK2910_ADCValue > NCK2910_ADCValue) Diff = Last_NCK2910_ADCValue - NCK2910_ADCValue;
								else Diff = NCK2910_ADCValue -Last_NCK2910_ADCValue ;

								if(Diff > 200) //To be define!!!
								{

									rf_dbg_Printf("----------------------Be about to Re-Calibration on CONT_RX!\r\n");
								}
								Last_NCK2910_ADCValue = NCK2910_ADCValue;
							}
						}
						else
						{
							rf_dbg_Printf("Read NCK2910_ADCValue return status is 0x%02x\r\n", Gu8Nck2910RciRxBuf[5]);
						}
					}
					else
					{
						rf_dbg_Printf("Read NCK2910_ADCValue return EventSubCode = 0x%02x, LEN = 0x%02x\r\n", EventSubCode, LEN);
					}
					break;
				case 0x0F:
#if RF_DEBUG
					rf_dbg_Printf("received data:");
					rf_dbg_Printf("%02X  ", Gu8Nck2910RciRxBuf[0]);

					rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[1]);
					rf_dbg_Printf("%02X  ", Gu8Nck2910RciRxBuf[2]);

					rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[3]);
					rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[4]);
					rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[5]);
					rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[6]);
					rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[7]);
					rf_dbg_Printf("%02X     ", Gu8Nck2910RciRxBuf[8]);
					for(uint8_t i=9; i<LEN; i++) rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[i]);
					rf_dbg_Printf("     ");
					rf_dbg_Printf("%02X ---------->> %d", Gu8Nck2910RciRxBuf[LEN], ++received_frame_count);
					rf_dbg_Printf("\r\n");
#endif
					if(!recv_cnt_zero_flag) Total_Recv_FrameCnt ++;
					if(0x66 == Gu8Nck2910RciRxBuf[9] && 0x14 == LEN && EventSubCode == 0x80) //TPMS
					{
						static uint8_t line_feedcnt = 0;

#if SUPPORT_RECV_RATE_TEST
						measurementTpmsPackageRCnt ++;
						measurementTpmsPackageTCnt = 0;
						if(measurementTpmsPackageRCnt < 3)
						{
							if(measurementTpmsPackageRCnt == 1)
							{
								measurementTpmsPackageStartTime = SysTimerGetCurrentTime();
							}
						}
						else
						{
							measurementTpmsPackageRCnt = 0;
							measurementTpmsPackageEndTime = SysTimerGetCurrentTime();
							measurementTpmsPackageTime = measurementTpmsPackageEndTime - measurementTpmsPackageStartTime;
						}
#endif

						if(!recv_cnt_zero_flag) TPMS_Frame_RecvCnt ++;
						recv_data_cha = 1;

						/*
						TPMS_IDs[0] = (ZONE_L_TPMS_01_t.msg.data[18] << 24) | (ZONE_L_TPMS_01_t.msg.data[19] << 16) | (ZONE_L_TPMS_01_t.msg.data[20] << 8) | ZONE_L_TPMS_01_t.msg.data[21];
						TPMS_IDs[1] = (ZONE_L_TPMS_01_t.msg.data[22] << 24) | (ZONE_L_TPMS_01_t.msg.data[23] << 16) | (ZONE_L_TPMS_01_t.msg.data[24] << 8) | ZONE_L_TPMS_01_t.msg.data[25];
						TPMS_IDs[2] = (ZONE_L_TPMS_01_t.msg.data[26] << 24) | (ZONE_L_TPMS_01_t.msg.data[27] << 16) | (ZONE_L_TPMS_01_t.msg.data[28] << 8) | ZONE_L_TPMS_01_t.msg.data[29];
						TPMS_IDs[3] = (ZONE_L_TPMS_01_t.msg.data[30] << 24) | (ZONE_L_TPMS_01_t.msg.data[31] << 16) | (ZONE_L_TPMS_01_t.msg.data[32] << 8) | ZONE_L_TPMS_01_t.msg.data[33];
						*/

						uint8_t Tpms_data[11] = {0};
						memcpy(Tpms_data, &Gu8Nck2910RciRxBuf[9], sizeof(Tpms_data));
						uint32_t Tpms_ID  = (Tpms_data[4] << 24) | (Tpms_data[5] << 16) | (Tpms_data[6] << 8) | Tpms_data[7];

#if !RF_CLOSE_ALL_PRINTF
						if(recv_cnt_zero_flag)
						{
							NCK2910_IntCnt = 1;
							RfTaskWakeupNoProcCnt = 0;
							RfEventSuccProcCnt = 1;
							TpmsError_CRC8_Cnt = 0;
							ChargeError_CRC8_Cnt = 0;
							Rf_293Msg_SendCnt = 0;
							Rf_297Msg_SendCnt = 0;

							TPMS_Frame_RecvCnt = 1;
							TpmsCorrectRecvNum = 0;
							TPMS_Frame_RecvNoMatch_ID = 0;
							ChargingPile_Frame_RecvCnt = 0;
							Total_Recv_FrameCnt = 1;
							Error_Frame_RecvCnt = 0;
							CMsgReadyReSendCnt = 0;
							C293MsgReadyReSendCnt = 0;

							recv_cnt_zero_flag = 0;
						}
#endif

						bool isIDValid = false;
						CanNm_GetState(0,&nmstate,&nmmode);
						if((nmmode == NM_BUS_SLEEP) || (nmmode == NM_PREPAREBUSSLEEP)) /* SyRS_819, 网络休眠状态(Bus-Sleep或Prepare Bus-Sleep)下，需要进行ID匹配 */
						{
							rf_dbg_Printf("NM is sleep or presleep, assert Tpms ID, ");

							/* SyRS_819, (1-1) */
							if((0 == Tpms_ID) || (0 == TPMS_IDs[0] && 0 == TPMS_IDs[1] && 0 == TPMS_IDs[2] && 0 == TPMS_IDs[3]))
							{
								/* Data discard */
								if(0 == Tpms_ID)
								{
									rf_dbg_Printf("Received TPMS_ID is zero, data discard!\r\n");
								}
								else
								{
									rf_dbg_Printf("Stored TPMS_ID is all zero, data discard!\r\n");
								}
								break;
							}

							for (uint8_t i = 0; i < 4; i++)
							{
								if (Tpms_ID == TPMS_IDs[i])
								{
									rf_dbg_Printf("Match valid Tpms ID! 0x%08X ---------->> %d\r\n", Tpms_ID, TPMS_Frame_RecvCnt);
									isIDValid = true;
									break;
								}
							}
						}
						else
						{
#if SUPPORT_RECV_RATE_TEST
							static uint8_t str_buf[64], CommaSymbolRecvNum = 0;
							str_buf[0] = 0;
							if(TPMS_Frame_RecvCnt == 1) CommaSymbolRecvNum = 0, CorrectRecvNum = 0, Nck2910ShortTermIntNum = 0, line_feedcnt = 0;
							if(TPMS_IDs[3] != 0x10000001)
							{
								/* 2s 3 frame*/
								if(TmpsRecvDelayTimer >= 12 && TmpsRecvDelayTimer <= 100) sprintf(str_buf, "##%d##", TmpsRecvDelayTimer); /* 帧延迟 */
								else if(TmpsRecvDelayTimer > 100 && TmpsRecvDelayTimer < 200) /* 丢帧或多帧数或正常 */
								{
									if(CommaSymbolRecvNum < 3) sprintf(str_buf + strlen(str_buf), "%c%d-%d", CorrectRecvNum?'$':'&', CommaSymbolRecvNum, Nck2910ShortTermIntNum); /* 少帧-丢帧或干扰 */
									else if(CommaSymbolRecvNum > 3) sprintf(str_buf + strlen(str_buf), "%c%d-%d", CorrectRecvNum != 3?'$':'&', CommaSymbolRecvNum, Nck2910ShortTermIntNum); /* 多帧-丢帧和干扰 */
									else if(CorrectRecvNum != 3) sprintf(str_buf + strlen(str_buf), "%c%d-%d", '$', CommaSymbolRecvNum, Nck2910ShortTermIntNum); /* 丢帧 */
									sprintf(str_buf + strlen(str_buf), ",");
									CommaSymbolRecvNum = 0;
									CorrectRecvNum = 0;
									Nck2910ShortTermIntNum = 0;

									measurementTpmsPackageRCnt = 1;
									measurementTpmsPackageTCnt = 0;
									measurementTpmsPackageStartTime = SysTimerGetCurrentTime();
								}
								else if(TmpsRecvDelayTimer >= 200 && TmpsRecvDelayTimer < 500) CommaSymbolRecvNum = 0, CorrectRecvNum = 0, Nck2910ShortTermIntNum = 0, sprintf(str_buf + strlen(str_buf), "@@"); /* 丢包 */
								else if(TmpsRecvDelayTimer >= 500) CommaSymbolRecvNum = 0, CorrectRecvNum = 0, Nck2910ShortTermIntNum = 0, sprintf(str_buf + strlen(str_buf), "#!!#"); /* 超时 */
							}
							else
							{
								/* 1s 3 frame*/
								if(TmpsRecvDelayTimer >= 12 && TmpsRecvDelayTimer <= 50) sprintf(str_buf, "##%d##", TmpsRecvDelayTimer); /* 帧延迟 */
								else if(TmpsRecvDelayTimer > 50 && TmpsRecvDelayTimer < 100) /* 丢帧或多帧数或正常 */
								{
									if(CommaSymbolRecvNum < 3) sprintf(str_buf + strlen(str_buf), "%c%d-%d", CorrectRecvNum?'$':'&', CommaSymbolRecvNum, Nck2910ShortTermIntNum); /* 少帧-丢帧或干扰 */
									else if(CommaSymbolRecvNum > 3) sprintf(str_buf + strlen(str_buf), "%c%d-%d", CorrectRecvNum != 3?'$':'&', CommaSymbolRecvNum, Nck2910ShortTermIntNum); /* 多帧-丢帧和干扰 */
									else if(CorrectRecvNum != 3) sprintf(str_buf + strlen(str_buf), "%c%d-%d", '$', CommaSymbolRecvNum, Nck2910ShortTermIntNum); /* 丢帧 */
									sprintf(str_buf + strlen(str_buf), ",");
									CommaSymbolRecvNum = 0;
									CorrectRecvNum = 0;
									Nck2910ShortTermIntNum = 0;

									measurementTpmsPackageRCnt = 1;
									measurementTpmsPackageTCnt = 0;
									measurementTpmsPackageStartTime = SysTimerGetCurrentTime();
								}
								else if(TmpsRecvDelayTimer >= 100 && TmpsRecvDelayTimer < 300) CommaSymbolRecvNum = 0, CorrectRecvNum = 0, Nck2910ShortTermIntNum = 0, sprintf(str_buf + strlen(str_buf), "@@"); /* 丢包 */
								else if(TmpsRecvDelayTimer >= 300) CommaSymbolRecvNum = 0, CorrectRecvNum = 0, Nck2910ShortTermIntNum = 0, sprintf(str_buf + strlen(str_buf), "#!!#"); /* 超时 */
							}
							CommaSymbolRecvNum ++;
							sprintf(str_buf + strlen(str_buf), " %d ", TPMS_Frame_RecvCnt);
							if(++ line_feedcnt >= 30) line_feedcnt = 0, sprintf(str_buf + strlen(str_buf), "\r\n");
							rf_dbg_Printf("%s", str_buf);
#else
							rf_dbg_Printf("NM is wakeup, not assert Tpms ID ---------->> %d\r\n", TPMS_Frame_RecvCnt);
#endif

							isIDValid = true; /* SyRS_816, 网络不休眠状态下，不需要进行ID匹配，直接转发 */

#if SUPPORT_RECV_RATE_TEST
							/*
							if(Tpms_ID != TPMS_SENSOR_ID1 \
							&& Tpms_ID != TPMS_SENSOR_ID2 \
							&& Tpms_ID != TPMS_SENSOR_ID3 \
							&& Tpms_ID != TPMS_SENSOR_ID4 \
							*/
							if(Tpms_ID != TPMS_IDs[0] \
							&& Tpms_ID != TPMS_IDs[1] \
							&& Tpms_ID != TPMS_IDs[2] \
							&& Tpms_ID != TPMS_IDs[3] \
							)
							{
								rf_dbg_Printf("Not valid Tpms ID = 0x%08X ", Tpms_ID);
								rf_dbg_Printf("Incorrect Package Data:");
								for(uint8_t i = 0; i < sizeof(Tpms_data); i ++) rf_dbg_Printf("%02X ", Tpms_data[i]);
								rf_dbg_Printf("\r\n");
								line_feedcnt = 0;
								TPMS_Frame_RecvNoMatch_ID ++;
								break;
							}
#endif
						}
						if (true != isIDValid) /* SyRS_819, (1-3) */
						{
							/* Data discard */
							rf_dbg_Printf("No valid Tpms ID! Need ID: 0x%08X---------->> %d\r\n", Tpms_ID, TPMS_Frame_RecvCnt);
							break;
						}
						uint8_t Tmps_crc8 = TmpsCalculateCRC8(Tpms_data, 10, 0x1D, 0xFF);
						if (Tpms_data[10] != Tmps_crc8)
						{
							/* SyRS_947, checksum is fail, data discard */
							RF_Dat_Info.RF_Error_Info = 1;
							RF_Dat_Info.RF_Recv_State = 1;
							RF_Dat_Info.RF_DataErr_Cnt++;
							rf_dbg_Printf("TPMS data CRC8 fail! cal crc8 = 0x%02X ", Tmps_crc8);
							rf_dbg_Printf("Error Package Data:");
							for(uint8_t i = 0; i < sizeof(Tpms_data); i ++) rf_dbg_Printf("%02X ", Tpms_data[i]);
							rf_dbg_Printf("\r\n");
							line_feedcnt = 0;
							TpmsError_CRC8_Cnt ++;
							break;
						}
						else
						{
							CorrectRecvNum ++;
							TpmsCorrectRecvNum ++;
							TmpsRecvDelayTimer = 0;
							TmpsRecvCorrectDelayTimer = 0;

							/* Head byte */
							rfdata_msg.msg.sig.Handbyte		= Tpms_data[0];
							/* BAL status */
							rfdata_msg.msg.sig.BalLbtChoose	= (Tpms_data[1] & 0x07);
							rfdata_msg.msg.sig.Lef_Right	= ((Tpms_data[1] & 0x18) >> 3);
							rfdata_msg.msg.sig.FrameNumber	= ((Tpms_data[1] & 0x60) >> 5);
							rfdata_msg.msg.sig.SensorBALSts	= ((Tpms_data[1] & 0x80) >> 7);
							/* Function code */
							rfdata_msg.msg.sig.WorkMode		= (Tpms_data[2] & 0x0F);
							rfdata_msg.msg.sig.AlartSts		= ((Tpms_data[2] & 0x10) >> 4);
							rfdata_msg.msg.sig.LowBattery	= ((Tpms_data[2] & 0x20) >> 5);
							rfdata_msg.msg.sig.AdvanceFunctionCode = ((Tpms_data[2] & 0xC0) >> 6);
							/* Advance Function code */
							rfdata_msg.msg.sig.AdvFunc0 		= Tpms_data[3];
							/* TPMS ID */
							rfdata_msg.msg.sig.sensorID3 		= Tpms_data[4];
							rfdata_msg.msg.sig.sensorID2		= Tpms_data[5];
							rfdata_msg.msg.sig.sensorID1		= Tpms_data[6];
							rfdata_msg.msg.sig.SensorID0		= Tpms_data[7];
							/* Tire Pressure */
							rfdata_msg.msg.sig.CurrentTPMSP_phy = Tpms_data[8];
							/* Tire Temperature */
							rfdata_msg.msg.sig.CurrentTPMST_phy = Tpms_data[9];

							rfdata_msg.msg.sig.RFSignalRC 	= RC_value%3;
							wol2min_counter = 0;

							CanNm_GetState(0,&nmstate,&nmmode);
							if((nmmode == NM_BUS_SLEEP) || (nmmode == NM_PREPAREBUSSLEEP))
							{
								Nm_LocalActive(); //唤醒网络
								rf_dbg_Printf("Wake SRF !\r\n");
								wakenmFlag = 1;
								RFReleaseflag = 1;
								wol2min_counter = 0;
								msg293_sendTime += 500;
								tim1_cnt = 0;
							}///2024.10.17 经与系统需求文档核对及相关人员讨论确认,ready sleep模式下不需要唤醒网络和做2min延迟释放网络
							else if(nmstate == NM_READYSLEEP)
							{
								Nm_LocalActive(); //唤醒网络
								RFReleaseflag = 1;
								wol2min_counter = 0;
							}

							rfdata_msg.MsgTimeOut_PreT = rf_proc_time_point;
							if (false == RFData_if_QueuePush(&RF_DataQueue, rfdata_msg))
							{
								rf_dbg_Printf("RF Data Push Queue is Fail!\r\n");
							}
							else
							{
								RC_value++;
							}
							Rf_293Msg_SendCnt ++;
						}
					}
					else if(0x55 == Gu8Nck2910RciRxBuf[9] && 0x19 == LEN  && EventSubCode == 0x80) //Charging Pile
					{
						ChargingPile_Frame_RecvCnt ++;
						recv_data_cha = 0;

#if SUPPORT_RECV_RATE_TEST
						measurementChargePackageRCnt ++;
						measurementChargePackageTCnt = 0;
						if(measurementChargePackageRCnt < 4)
						{
							if(measurementChargePackageRCnt == 1)
							{
								measurementChargePackageStartTime = SysTimerGetCurrentTime();
							}
						}
						else
						{
							measurementChargePackageRCnt = 0;
							measurementChargePackageEndTime = SysTimerGetCurrentTime();
							measurementChargePackageTime = measurementChargePackageEndTime - measurementChargePackageStartTime;
						}
#endif

#if SUPPORT_RECV_RATE_TEST
						static uint8_t str_buf[64], line_feedcnt = 0, CommaSymbolRecvNum = 0;
						str_buf[0] = 0;
						if(ChargingPile_Frame_RecvCnt == 1) CommaSymbolRecvNum = 0, line_feedcnt = 0;
						/* 1s 3 frame*/
						if(ChargeRecvDelayTimer >= 12 && ChargeRecvDelayTimer <= 50) sprintf(str_buf, "##%d##", ChargeRecvDelayTimer);
						else if(ChargeRecvDelayTimer > 50 && ChargeRecvDelayTimer < 100)
						{
							if(CommaSymbolRecvNum < 3) sprintf(str_buf + strlen(str_buf), "$%d",CommaSymbolRecvNum);
							else if(CommaSymbolRecvNum > 4) sprintf(str_buf + strlen(str_buf), "$$%d", CommaSymbolRecvNum);
							sprintf(str_buf + strlen(str_buf), ",");
							CommaSymbolRecvNum = 0;

							measurementChargePackageRCnt = 1;
							measurementChargePackageTCnt = 0;
							measurementChargePackageStartTime = SysTimerGetCurrentTime();
						}
						else if(ChargeRecvDelayTimer >= 100 && ChargeRecvDelayTimer < 300) sprintf(str_buf + strlen(str_buf), "@@");
						else if(ChargeRecvDelayTimer >= 300) sprintf(str_buf + strlen(str_buf), "#!!#");

						sprintf(str_buf + strlen(str_buf), " %d ", ChargingPile_Frame_RecvCnt);
						if(++ line_feedcnt >= 51) line_feedcnt = 0, sprintf(str_buf + strlen(str_buf), "\r\n");
						rf_dbg_Printf("%s", str_buf);
						ChargeRecvDelayTimer = 0;
#endif

						VehStateDetailed_t vehicleStateDetailed = ZONE_13C_t.msg.sig.VehStateDetailed;
						if(vehicleStateDetailed != VEHSTDRIVING_13)
						{
							uint8_t ChargPile_data[16] = {0};
							memcpy(ChargPile_data, &Gu8Nck2910RciRxBuf[9], sizeof(ChargPile_data));
							uint8_t Charge_crc8 = ChargeCalculateCRC8(ChargPile_data, 15);//读取充电桩的CRC校验值
							if (ChargPile_data[15] != Charge_crc8)//CRC校验
							{
								/* SyRS_948, checksum is fail, data discard */
								RF_Dat_Info.RF_Error_Info = 1;
								RF_Dat_Info.RF_Recv_State = 1;
								RF_Dat_Info.RF_DataErr_Cnt++;
								rf_dbg_Printf("ChargePile data CRC8 fail! cal crc8 = 0x%02X ", Charge_crc8);
								rf_dbg_Printf("Error Package Data:");
								for(uint8_t i = 0; i < sizeof(ChargPile_data); i ++) rf_dbg_Printf("%02X ", ChargPile_data[i]);
								rf_dbg_Printf("\r\n");
								ChargeRecvDelayTimer = 0;
								ChargeError_CRC8_Cnt ++;
								break;
							}
							else
							{
								#define ChrgrGunGateOpenReq_On_KEEP_TIME (2*100U) /* 10ms once decrease. */
								uint8_t ind = 1, chargeBID;
								//bool privtID;

								CanNm_GetState(0,&nmstate,&nmmode);
								if((nmmode == NM_BUS_SLEEP) || (nmmode == NM_PREPAREBUSSLEEP))
								{
									/* SyRS_1344, NM state is Bus-Sleep or Prepare Bus-Sleep, data discard */
									rf_dbg_Printf("NM state is Bus-Sleep or Prepare Bus-Sleep, data discard!\r\n");
									break;
								}

								if(!RF_PrivtVINReadFlag)
								{
									extern void UDS_itf_readDid_F190(uint8_t *i_pDataBufPtr);
									uint8_t sys_vin[DID_F190_LENGTH] = {0};
									UDS_itf_readDid_F190(sys_vin);
									rf_dbg_Printf("First Read Vehicle VIN Code: %s\r\n", sys_vin);
									privtVIN = sys_vin[13]<<24 | sys_vin[14]<<16 | sys_vin[15]<<8 | sys_vin[16]<<0;
									rf_dbg_Printf("privtVIN = 0x%08X\r\n", privtVIN);
									RF_PrivtVINReadFlag = 1;
								}

#if SUPPORT_RECV_RATE_TEST
								uint8_t _0x297MsgBuf[16] = {0};
								memcpy(_0x297MsgBuf, ChargPile_data, sizeof(_0x297MsgBuf));
								if(CANBUS_SUCCESS != canbusTransSendMsgFrame(0x297, sizeof(_0x297MsgBuf), _0x297MsgBuf))
								{
									rf_dbg_Printf("Send 0x297 Msg is Fail!\r\n");
								}
								Rf_297Msg_SendCnt ++;
#endif

								Charge_IDE[0] = ChargPile_data[ind++]<<24 | ChargPile_data[ind++]<<16 | ChargPile_data[ind++]<<8 | ChargPile_data[ind++]<<0;
								Charge_IDE[1] = ChargPile_data[ind++]<<24 | ChargPile_data[ind++]<<16 | ChargPile_data[ind++]<<8 | ChargPile_data[ind++]<<0;
								Charge_IDE[2] = ChargPile_data[ind++]<<24 | ChargPile_data[ind++]<<16 | ChargPile_data[ind++]<<8 | ChargPile_data[ind++]<<0;
								chargeBID = ChargPile_data[13];//将充电桩RF数据的第14位赋值给BID
								if(chargeBID == 0x01 || chargeBID == 0x03)//01表示打开交流充电口、03表示打开直流充电口
								{
									if(Charge_IDE[0] == 0xAAAAAAAA && Charge_IDE[1] == 0x00000000 && Charge_IDE[2] == 0x00000000)//先判断是否为广播信号
									{
										BTA_SRF_295_t.msg.sig.ChrgrGunGateOpenReq = BASICONOFFSWTSTSON_493;
										ChrgrGunGateOpenReq_On_KEEP_TIME_cnt = ChrgrGunGateOpenReq_On_KEEP_TIME; /* SyRS_823 */
#if !SUPPORT_RECV_RATE_TEST
										rf_dbg_Printf("Broast Charging Pile Msg, Set Basicon On! ---------->> %d\r\n", ChargingPile_Frame_RecvCnt);
#endif
									}
									else
									{
										uint8_t i;
										for (i = 0; i < 3; i++)
										{
											if (privtVIN == Charge_IDE[i])
											{
												//privtID = true;//私有充电桩码匹配正确
												//匹配正确，发送报文
												BTA_SRF_295_t.msg.sig.ChrgrGunGateOpenReq = BASICONOFFSWTSTSON_493;
												ChrgrGunGateOpenReq_On_KEEP_TIME_cnt = ChrgrGunGateOpenReq_On_KEEP_TIME; /* SyRS_823 */
#if !SUPPORT_RECV_RATE_TEST
												rf_dbg_Printf("Private Charging Pile Msg, Set Basicon On! ---------->> %d\r\n", ChargingPile_Frame_RecvCnt);
#endif
												break;
											}
										}
										if(i >= 3)
										{
											/* SyRS_811, Private RF msg VIN is not match, data discard */
											rf_dbg_Printf("Private Charging Pile Msg, VIN not match! ---------->> %d\r\n", ChargingPile_Frame_RecvCnt);
											rf_dbg_Printf("privtVIN = 0x%08X, Charge_IDE[0] = 0x%08X, Charge_IDE[1] = 0x%08X, Charge_IDE[2] = 0x%08X\r\n", privtVIN, Charge_IDE[0], Charge_IDE[1], Charge_IDE[2]);
										}
									}
								}
								else if(chargeBID == 0x02 || chargeBID == 0x04)//02表示关闭交流充电口、04表示关闭直流充电口
								{
									if(Charge_IDE[0] == 0xAAAAAAAA && Charge_IDE[1] == 0x00000000 && Charge_IDE[2] == 0x00000000)//先判断是否为广播信号
									{
										BTA_SRF_295_t.msg.sig.ChrgrGunGateOpenReq = BASICONOFFSWTSTSOFF_492;
#if !SUPPORT_RECV_RATE_TEST
										rf_dbg_Printf("Broast Charging Pile Msg, Set Basicon Off! ---------->> %d\r\n", ChargingPile_Frame_RecvCnt);
#endif
									}
									else
									{
										uint8_t i;
										for (i = 0; i < 3; i++)
										{
											if (privtVIN == Charge_IDE[i])
											{
												//privtID = true;//私有充电桩码匹配正确
												//匹配正确，发送打开充电口的报文
												BTA_SRF_295_t.msg.sig.ChrgrGunGateOpenReq = BASICONOFFSWTSTSOFF_492;
#if !SUPPORT_RECV_RATE_TEST
												rf_dbg_Printf("Private Charging Pile Msg, Set Basicon Off! ---------->> %d\r\n", ChargingPile_Frame_RecvCnt);
#endif
												break;
											}
										}
										if(i >= 3)
										{
											/* SyRS_811, Private RF msg VIN is not match, data discard */
											rf_dbg_Printf("Private Charging Pile Msg, VIN not match! ---------->> %d\r\n", ChargingPile_Frame_RecvCnt);
											rf_dbg_Printf("privtVIN = 0x%08X, Charge_IDE[0] = 0x%08X, Charge_IDE[1] = 0x%08X, Charge_IDE[2] = 0x%08X\r\n", privtVIN, Charge_IDE[0], Charge_IDE[1], Charge_IDE[2]);
										}
									}
								}
								else
								{
									/* chargeBID is incorrect, data discard */
									rf_dbg_Printf("Unknown chargeBID, chargeBID = 0x%02X\r\n", chargeBID);
								}
							}
						}
						else
						{
							rf_dbg_Printf("Vehicle State don't meet the condition, Msg0x13C.vehicleStateDetailed == VEHSTDRIVING, Should not receive Charging Pile Data.\r\n");
						}
					}
					else
					{
						Error_Frame_RecvCnt ++;
						/* data discard */
#if !USE_OLD_CFG
						rf_dbg_Printf("unknown data, EventSubCode = 0x%02X, LEN = 0x%02X, Gu8Nck2910RciRxBuf[9] = 0x%02X\r\n", EventSubCode, LEN, Gu8Nck2910RciRxBuf[9]);
#endif
					}
					break;

				case 0x85:
					rf_dbg_Printf("RSSI event.\r\n");
					if(0xA3 == EventSubCode && LEN == 0x0B && Gu8Nck2910RciRxBuf[5] == 0x00)
					{
						int16_t RSSI = Gu8Nck2910RciRxBuf[10] << 8 | Gu8Nck2910RciRxBuf[9];
						rf_dbg_Printf("RSSI averaging event, Ind = %d, RSSI = %d, Gu8Nck2910RciRxBuf[10]=0x%02x, Gu8Nck2910RciRxBuf[9]=0x%02x\r\n", Gu8Nck2910RciRxBuf[8], RSSI, Gu8Nck2910RciRxBuf[10], Gu8Nck2910RciRxBuf[9]);
#if SUPPORT_RECV_RATE_TEST
						RF_SendRSSI_Info(Gu8Nck2910RciRxBuf[8], Gu8Nck2910RciRxBuf[10], Gu8Nck2910RciRxBuf[9]);
#endif
					}
					else
					{
						rf_dbg_Printf("RSSI Package Data:");
						for(uint8_t i = 0; i <= LEN; i ++) rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[i]);
						rf_dbg_Printf("\r\n");
					}
					break;

				case 0x84:
					rf_dbg_Printf("RF event. EventSubCode = %d, RSSI = %d, RSSI_Peak = %d, Reception Config Index = %d\r\n", EventSubCode, Gu8Nck2910RciRxBuf[8], Gu8Nck2910RciRxBuf[9], Gu8Nck2910RciRxBuf[10]);
					if(0xF4 == EventSubCode) /* PLL unlock detected (unmaskable). */
					{
						rf_dbg_Printf("PLL unlock event detected,\r\n");
						rf_dbg_Printf("Call NCK2910_InitFunc() ReInit!\r\n");
						NCK2910_InitFunc();
					}
					break;

				case 0x80:
					rf_dbg_Printf("Reboot event, Reason = 0x%02X\r\n", Gu8Nck2910RciRxBuf[6]);
					rf_dbg_Printf("Call NCK2910_InitFunc() ReInit!\r\n");
					NCK2910_InitFunc();
					break;

				case 0x81:
					rf_dbg_Printf("Heartbeat event.\r\n");
					break;

				case 0xFF:
					rf_dbg_Printf("Communication error, Status = 0x%02X\r\n", Gu8Nck2910RciRxBuf[5]);
					rf_dbg_Printf("Call NCK2910_Reset(), NCK2910_InitFunc() ReBoot!\r\n");
					NCK2910_Reset();
					NCK2910_InitFunc();
					break;

				default:
					rf_dbg_Printf("Unknown event! EventCode = 0x%02X, EventSubCode = 0x%02X, LEN = %d, Status = 0x%02X\r\n", EventCode, EventSubCode, LEN, Gu8Nck2910RciRxBuf[5]);
					uint8_t os_status = NCK2910_OS_GetStatus();
					if(NCK2910_OS_STATUS_CONTRX != os_status && NCK2910_OS_STATUS_POLLING != os_status)
					{
						rf_dbg_Printf("Current OS_Status = %s, 0x%02X, re-entry Polling!\r\n", OS_STATUS_VALUE_CONVERT_STRING(os_status, os_status));
						NCK2910_Idle();
						NCK2910_CfgAndSetPolling();
					}
					break;
			}
		}
		else
		{
			wait_timeout ++;
		}
#if RF_DEBUG
		NCK2910_GetPrintOSStatus();
#endif
#if PERIOD_RE_CALIBRATION_EN
		if(NCK2910_OS_STATUS_CONTRX == NCK2910_OS_GetStatus()) /* assert is Continuous reception */
		{
			NCK2910_ContTimeoutCnt = 0;
			NCK2910_ReCfgPollingTimer = 0;
			if(NCK2910_ReCalibrationCnt >= NCK2910_RE_CALIBRATION_TIME)
			{
				NCK2910_ReCaliTimerCntFlag = 1;
				NCK2910_ReCaliTimer = 0;
			}
			#if 0
			if(NCK2910_ReCalibrationCnt >= NCK2910_RE_CALIBRATION_TIME) /* time re-calibration */
			{
#if RF_DEBUG
				rf_dbg_Printf("######################Re-Calibration on CONT_RX! Cha = %d\r\n", recv_data_cha);
#endif
				if(0 == recv_data_cha || 1 == recv_data_cha)
				{
					NCK2910_Idle();
					NCK2910_CfgAndSetPolling();
				}
				NCK2910_ReCalibrationCnt = 0;
			}
			#endif
		}
		else
		{
			NCK2910_ReCalibrationFlag = 0;
			NCK2910_ReCalibrationCnt = 0;
		}
#endif
		NCK2910_ProcessFlag = 0;
	}


	if(0 == RfEventProcCnt)
	{
		if(NCK2910_IntCnt > 0 && !recv_cnt_zero_flag) RfTaskWakeupNoProcCnt ++;
#if PERIOD_RE_CALIBRATION_EN
		if(NCK2910_ReCalibrationCnt >= NCK2910_RE_CALIBRATION_TIME)
		{
			if(NCK2910_ReCaliTimer > NCK2910_INACTIVE_DELAY_RE_CALIBRATION_TIME)
			{
				if(NCK2910_ReCalibrationFlag)
				{
#if 1
					rf_dbg_Printf("######################Re-Calibration on CONT_RX!\r\n");
					NCK2910_Idle();
					NCK2910_CfgAndSetPolling();
#else
					rf_dbg_Printf("----------------------Check ADC Diff on CONT_RX! ");
					if(TRUE == NCK2910_ADC_Conf())
					{
						NCK2910_ADC_Read();
						rf_dbg_Printf("read ADC~\r\n");
					}
#endif
					NCK2910_ReCalibrationFlag = 0;
					NCK2910_ReCalibrationCnt = 0;
					NCK2910_ReCaliTimer = 0;
					NCK2910_ReCaliTimerCntFlag = 0;
				}
			}
		}
#endif
		if(driving_stat_change)
		{
			if(NCK2910_ReCfgPollingTimer > NCK2910_INACTIVE_DELAY_RE_CFG_POLLING_TIME)
			{
				rf_dbg_Printf("@@@@@@@@@@@@@@@@@@@@@@@Re-Config Polling! %s\r\n", ZONE_13C_t.msg.sig.VehStateDetailed == VEHSTDRIVING_13?"TPMS Only":"TPM & CHARG");
				NCK2910_Idle();
				NCK2910_CfgAndSetPolling();
				NCK2910_ReCfgPollingTimer = 0;
				driving_stat_change = 0;
			}
		}
	}

#if SUPPORT_RECV_RATE_TEST
	//FSK TPMS
	RF_SendFSK_RecvInfo();
	/* ASK CHARGING PILE */
	RF_SendASK_RecvInfo();
#endif
}

void NCK2910_Rf10msProc(void)
{
#if (defined(RF_DEBUG) && (RF_DEBUG == 1))
	extern uint32_t NCK2910_IntCnt, NCK2910_TmpCnt;
	if(NCK2910_TmpCnt < 100) NCK2910_TmpCnt ++;
	else
	{
		NCK2910_TmpCnt = 0;
		NCK2910_IntCnt = 0;
		NCK2910_GetPrintOSStatus();
	}
#endif
#if PERIOD_RE_CALIBRATION_EN
	extern uint32_t NCK2910_ReCalibrationCnt, NCK2910_ContTimeoutCnt, NCK2910_ReCaliTimer;
	extern uint8_t NCK2910_ReCalibrationFlag, NCK2910_ReCaliTimerCntFlag;
	extern osa_semaphore_handle_t semaphoreHandle;
	if(NCK2910_ContTimeoutCnt < NCK2910_CONT_RECP_TIMEOUT_TIME) /* Continous Reception Timeout Time */
	{
		NCK2910_ContTimeoutCnt ++;
		NCK2910_ReCalibrationCnt ++;
		if(NCK2910_ReCalibrationCnt >= NCK2910_RE_CALIBRATION_TIME) /* time re-calibration */
		{
			if(NCK2910_ReCaliTimerCntFlag)
			{
				NCK2910_ReCaliTimer ++;
				if(NCK2910_ReCaliTimer > NCK2910_INACTIVE_DELAY_RE_CALIBRATION_TIME) /* RF inactive delay time re-calibration */
				{
					NCK2910_ReCalibrationFlag = 1;
					OSA_SemaphorePost(semaphoreHandle);
				}
			}
		}
		else
		{
			NCK2910_ReCaliTimerCntFlag = 0;
		}
	}
	else
	{
		NCK2910_ReCalibrationFlag = 0;
		NCK2910_ReCalibrationCnt = 0;
		NCK2910_ReCaliTimer = 0;
		NCK2910_ReCaliTimerCntFlag = 0;
	}
#endif
	extern uint8_t driving_stat_change;
	extern uint32_t NCK2910_ReCfgPollingTimer;
	static VehStateDetailed_t last_vehicleStateDetailed, current_vehicleStateDetailed;
	current_vehicleStateDetailed = ZONE_13C_t.msg.sig.VehStateDetailed;
	if(last_vehicleStateDetailed != current_vehicleStateDetailed)
	{
		if(current_vehicleStateDetailed == VEHSTDRIVING_13 || last_vehicleStateDetailed == VEHSTDRIVING_13)
		{
			NCK2910_ReCfgPollingTimer = 0;
			driving_stat_change = 1;
		}
		last_vehicleStateDetailed = current_vehicleStateDetailed;
	}

	if(rfInitSuccFlag)
	{
		if(driving_stat_change)
		{
			NCK2910_ReCfgPollingTimer ++;
			if(NCK2910_ReCfgPollingTimer > NCK2910_INACTIVE_DELAY_RE_CFG_POLLING_TIME)
			{
				OSA_SemaphorePost(semaphoreHandle);
			}
		}
	}

#if UNDER_VOLTAGE_IDLE_MODE_EN
	if(Get_PMState() == PMS_UNDER)
	{
		if(0 == NCK2910_ReInitFlag)
		{
			if(!NCK2910_DealLock)
			{
				rf_dbg_Printf("PMState = UNDER VOLTAGE, Call NCK2910_Reset()!\r\n");
				NCK2910_Reset();
				NCK2910_ReInitFlag = 1;
			}
		}
	}
	else
	{
		if(NCK2910_ReInitFlag)
		{
			if(!NCK2910_DealLock)
			{
				rf_dbg_Printf("PMState != UNDER VOLTAGE, Call NCK2910_InitFunc() ReInit!\r\n");
				NCK2910_InitFunc();
				NCK2910_ReInitFlag = 0;
			}
		}
	}
#endif

	static uint32_t NCK2910_Int_ChkDelayCnt = 0;
	if(rfInitSuccFlag)
	{
		if(!NCK2910_ProcessFlag)
		{
			if(HAL_PIN_LEVEL_LOW == hal_pin_if_read(NCK2910_INT_N_PIN))
			{
				NCK2910_Int_ChkDelayCnt ++;
				if(NCK2910_Int_ChkDelayCnt > 5)
				{
					rf_dbg_Printf("RF INT PIN IS LOW NO DEEL!\r\n");
					OSA_SemaphorePost(semaphoreHandle);
					NCK2910_Int_ChkDelayCnt = 0;
				}
			}
		}
		else
		{
			NCK2910_Int_ChkDelayCnt = 0;
		}
	}

#if SUPPORT_RECV_RATE_TEST
	if(NCK2910_IntCnt != 0)
	{
		if(0 == recv_cnt_zero_flag)
		{
			rf_task_timeout_timer ++;
			if(rf_task_timeout_timer > 1000) /* 10s */
			{
#if !CONTINOUS_RECEPTION_FSK_TEST_EN && !CONTINOUS_RECEPTION_ASK_TEST_EN
				uint8_t cur_os_status;
				if(TPMS_IDs[2] == 0x00000001 || TPMS_IDs[2] == 0x00000002)
				{
					cur_os_status = NCK2910_OS_STATUS_POLLING;
				}
				else
				{
					cur_os_status = NCK2910_OS_GetStatus();
				}
				if(NCK2910_OS_STATUS_POLLING == cur_os_status)
#endif
				{
#if 0
					NCK2910_IntCnt = 0;
					RfTaskWakeupNoProcCnt = 0;
					RfEventSuccProcCnt = 0;
					TpmsError_CRC8_Cnt = 0;
					ChargeError_CRC8_Cnt = 0;
					Rf_293Msg_SendCnt = 0;
					Rf_297Msg_SendCnt = 0;

					TPMS_Frame_RecvCnt = 0;
					TpmsCorrectRecvNum = 0;
					TPMS_Frame_RecvNoMatch_ID = 0;
					ChargingPile_Frame_RecvCnt = 0;
					Total_Recv_FrameCnt = 0;
					Error_Frame_RecvCnt = 0;
					CMsgReadyReSendCnt = 0;
					C293MsgReadyReSendCnt = 0;
#else
					NCK2910_IntCnt --;
					recv_cnt_zero_flag = 1;
#endif
				}
			}
		}
	}
	static uint16_t rf_report_msg_cnt;
	rf_report_msg_cnt ++;
	if(rf_report_msg_cnt > 100)
	{
		uint8_t i;
		rf_report_msg_cnt = 0;

		//FSK TPMS
		RF_SendFSK_RecvInfo();
		/* ASK CHARGING PILE */
		RF_SendASK_RecvInfo();
		/* Error Info */
		RF_SendError_Info();
	}
	TmpsRecvDelayTimer ++;
	if(TmpsRecvCorrectDelayTimer <= 200) TmpsRecvCorrectDelayTimer ++;
	else
	{
		if(TmpsRecvCorrectDelayTimer == 201)
		{
			rf_dbg_Printf(" ~~~%d~~~%d \r\n", TpmsCorrectRecvNum, Nck2910ShortTermIntNum);
			TmpsRecvCorrectDelayTimer ++;
		}
	}

	if(measurementTpmsPackageTCnt <= 200) measurementTpmsPackageTCnt ++;
	else
	{
		if(measurementTpmsPackageTCnt == 201)
		{
			measurementTpmsPackageRCnt = 0;
			measurementTpmsPackageTCnt ++;
		}
	}

	if(measurementChargePackageTCnt <= 200) measurementChargePackageTCnt ++;
	else
	{
		if(measurementChargePackageTCnt == 201)
		{
			measurementChargePackageRCnt = 0;
			measurementChargePackageTCnt ++;
		}
	}

#endif
}

#if SUPPORT_RECV_RATE_TEST
void RF_SendFSK_RecvInfo(void)
{
	static uint8_t msg_123[32];
	static uint8_t i, sf = 0;

	if(sf) return;
	sf  = 1;

	//FSK TPMS
	i = 0;
	/* NCK2910总得中断次数计数 */
	msg_123[i++] = NCK2910_IntCnt >> 24;
	msg_123[i++] = NCK2910_IntCnt >> 16;
	msg_123[i++] = NCK2910_IntCnt >> 8;
	msg_123[i++] = NCK2910_IntCnt >> 0;

	/* RF任务呗唤醒后未处理任何RF事件次数计数*/
	msg_123[i++] = RfTaskWakeupNoProcCnt >> 24;
	msg_123[i++] = RfTaskWakeupNoProcCnt >> 16;
	msg_123[i++] = RfTaskWakeupNoProcCnt >> 8;
	msg_123[i++] = RfTaskWakeupNoProcCnt >> 0;

	/* RF任务成功处理NCK2910事件的次数计数/RF任务处理NCK2910成功读取到数据的次数计数 */
	msg_123[i++] = RfEventSuccProcCnt >> 24;
	msg_123[i++] = RfEventSuccProcCnt >> 16;
	msg_123[i++] = RfEventSuccProcCnt >> 8;
	msg_123[i++] = RfEventSuccProcCnt >> 0;

	/* 处理NCK2910接收帧事件数计数 */
	msg_123[i++] = Total_Recv_FrameCnt >> 24;
	msg_123[i++] = Total_Recv_FrameCnt >> 16;
	msg_123[i++] = Total_Recv_FrameCnt >> 8;
	msg_123[i++] = Total_Recv_FrameCnt >> 0;

	/*****************************************************
	------------------------------------------------------
	 *****************************************************/

	/* 处理TPMS数据帧计数 */
	msg_123[i++] = TPMS_Frame_RecvCnt >> 24;
	msg_123[i++] = TPMS_Frame_RecvCnt >> 16;
	msg_123[i++] = TPMS_Frame_RecvCnt >> 8;
	msg_123[i++] = TPMS_Frame_RecvCnt >> 0;

	/* 非TPMS或Charge Pile数据帧计数 */
	msg_123[i++] = Error_Frame_RecvCnt >> 24;
	msg_123[i++] = Error_Frame_RecvCnt >> 16;
	msg_123[i++] = Error_Frame_RecvCnt >> 8;
	msg_123[i++] = Error_Frame_RecvCnt >> 0;

	/* TPMS数据帧CRC8错误计数 */
	msg_123[i++] = TpmsError_CRC8_Cnt >> 24;
	msg_123[i++] = TpmsError_CRC8_Cnt >> 16;
	msg_123[i++] = TpmsError_CRC8_Cnt >> 8;
	msg_123[i++] = TpmsError_CRC8_Cnt >> 0;

	/* TPMS转发0x293报文计数 */
	msg_123[i++] = Rf_293Msg_SendCnt >> 24;
	msg_123[i++] = Rf_293Msg_SendCnt >> 16;
	msg_123[i++] = Rf_293Msg_SendCnt >> 8;
	msg_123[i++] = Rf_293Msg_SendCnt >> 0;

	canbusTransSendMsgFrame(0x123, sizeof(msg_123), msg_123);

	sf  = 0;
}
void RF_SendASK_RecvInfo(void)
{
	static uint8_t msg_456[32];
	static uint8_t i, sf = 0;

	if(sf) return;
	sf  = 1;

	/* ASK CHARGING PILE */
	i = 0;
	/* NCK2910总得中断次数计数 */
	msg_456[i++] = NCK2910_IntCnt >> 24;
	msg_456[i++] = NCK2910_IntCnt >> 16;
	msg_456[i++] = NCK2910_IntCnt >> 8;
	msg_456[i++] = NCK2910_IntCnt >> 0;

	/* RF任务呗唤醒后未处理任何RF事件次数计数*/
	msg_456[i++] = RfTaskWakeupNoProcCnt >> 24;
	msg_456[i++] = RfTaskWakeupNoProcCnt >> 16;
	msg_456[i++] = RfTaskWakeupNoProcCnt >> 8;
	msg_456[i++] = RfTaskWakeupNoProcCnt >> 0;

	/* RF任务成功处理NCK2910事件的次数计数/RF任务处理NCK2910成功读取到数据的次数计数 */
	msg_456[i++] = RfEventSuccProcCnt >> 24;
	msg_456[i++] = RfEventSuccProcCnt >> 16;
	msg_456[i++] = RfEventSuccProcCnt >> 8;
	msg_456[i++] = RfEventSuccProcCnt >> 0;

	/* 处理NCK2910接收帧事件数计数 */
	msg_456[i++] = Total_Recv_FrameCnt >> 24;
	msg_456[i++] = Total_Recv_FrameCnt >> 16;
	msg_456[i++] = Total_Recv_FrameCnt >> 8;
	msg_456[i++] = Total_Recv_FrameCnt >> 0;

	/*****************************************************
	------------------------------------------------------
	 *****************************************************/

	/* 处理Charge Pile数据帧计数 */
	msg_456[i++] = ChargingPile_Frame_RecvCnt >> 24;
	msg_456[i++] = ChargingPile_Frame_RecvCnt >> 16;
	msg_456[i++] = ChargingPile_Frame_RecvCnt >> 8;
	msg_456[i++] = ChargingPile_Frame_RecvCnt >> 0;

	/* 非TPMS或Charge Pile数据帧计数 */
	msg_456[i++] = Error_Frame_RecvCnt >> 24;
	msg_456[i++] = Error_Frame_RecvCnt >> 16;
	msg_456[i++] = Error_Frame_RecvCnt >> 8;
	msg_456[i++] = Error_Frame_RecvCnt >> 0;

	/* Charge Pile数据帧CRC8错误计数 */
	msg_456[i++] = ChargeError_CRC8_Cnt >> 24;
	msg_456[i++] = ChargeError_CRC8_Cnt >> 16;
	msg_456[i++] = ChargeError_CRC8_Cnt >> 8;
	msg_456[i++] = ChargeError_CRC8_Cnt >> 0;

	/* Charge Pile转发0x297报文计数 */
	msg_456[i++] = Rf_297Msg_SendCnt >> 24;
	msg_456[i++] = Rf_297Msg_SendCnt >> 16;
	msg_456[i++] = Rf_297Msg_SendCnt >> 8;
	msg_456[i++] = Rf_297Msg_SendCnt >> 0;

	canbusTransSendMsgFrame(0x456, sizeof(msg_456), msg_456);

	sf  = 0;
}
void RF_SendError_Info(void)
{
	static uint8_t msg_789[32];
	static uint8_t i, sf = 0;

	if(sf) return;
	sf  = 1;

	i = 0;
	/* CAN报文准备重复次数记录 */
	msg_789[i++] = CMsgReadyReSendCnt >> 24;
	msg_789[i++] = CMsgReadyReSendCnt >> 16;
	msg_789[i++] = CMsgReadyReSendCnt >> 8;
	msg_789[i++] = CMsgReadyReSendCnt >> 0;

	/* 293报文准备重复次数记录 */
	msg_789[i++] = C293MsgReadyReSendCnt >> 24;
	msg_789[i++] = C293MsgReadyReSendCnt >> 16;
	msg_789[i++] = C293MsgReadyReSendCnt >> 8;
	msg_789[i++] = C293MsgReadyReSendCnt >> 0;

	/* 预留 */
	msg_789[i++] = 0 >> 24;
	msg_789[i++] = 0 >> 16;
	msg_789[i++] = 0 >> 8;
	msg_789[i++] = 0 >> 0;

	/* 预留 */
	msg_789[i++] = 0 >> 24;
	msg_789[i++] = 0 >> 16;
	msg_789[i++] = 0 >> 8;
	msg_789[i++] = 0 >> 0;

	/*****************************************************
	------------------------------------------------------
	 *****************************************************/

	/* 预留 */
	msg_789[i++] = 0 >> 24;
	msg_789[i++] = 0 >> 16;
	msg_789[i++] = 0 >> 8;
	msg_789[i++] = 0 >> 0;

	/* Tpms接收3包时间 */
	msg_789[i++] = measurementTpmsPackageTime >> 24;
	msg_789[i++] = measurementTpmsPackageTime >> 16;
	msg_789[i++] = measurementTpmsPackageTime >> 8;
	msg_789[i++] = measurementTpmsPackageTime >> 0;

	/* ChargePile接收4包时间 */
	msg_789[i++] = measurementChargePackageTime >> 24;
	msg_789[i++] = measurementChargePackageTime >> 16;
	msg_789[i++] = measurementChargePackageTime >> 8;
	msg_789[i++] = measurementChargePackageTime >> 0;

	/* 接收到TPMS不匹配的ID报文 */
	msg_789[i++] = TPMS_Frame_RecvNoMatch_ID >> 24;
	msg_789[i++] = TPMS_Frame_RecvNoMatch_ID >> 16;
	msg_789[i++] = TPMS_Frame_RecvNoMatch_ID >> 8;
	msg_789[i++] = TPMS_Frame_RecvNoMatch_ID >> 0;

	canbusTransSendMsgFrame(0x789, sizeof(msg_789), msg_789);

	sf  = 0;
}
void RF_SendRSSI_Info(uint8_t recv_ind, uint8_t rssi_hi, uint8_t rssi_lo)
{
	static uint8_t msg_777[16];
	static uint8_t i, sf = 0;

	if(sf) return;
	sf  = 1;

	i = 0;
	/* RSSI */
	msg_777[i++] = recv_ind;
	msg_777[i++] = 0;
	msg_777[i++] = rssi_hi; //RSSI hi
	msg_777[i++] = rssi_lo; //RSSI lo

	/* 预留 */
	msg_777[i++] = 0 >> 24;
	msg_777[i++] = 0 >> 16;
	msg_777[i++] = 0 >> 8;
	msg_777[i++] = 0 >> 0;

	/* 预留 */
	msg_777[i++] = 0 >> 24;
	msg_777[i++] = 0 >> 16;
	msg_777[i++] = 0 >> 8;
	msg_777[i++] = 0 >> 0;

	/* 预留 */
	msg_777[i++] = 0 >> 24;
	msg_777[i++] = 0 >> 16;
	msg_777[i++] = 0 >> 8;
	msg_777[i++] = 0 >> 0;

	canbusTransSendMsgFrame(0x777, sizeof(msg_777), msg_777);

	sf  = 0;
}
#endif

/*********************************************************************************************************
** Function name:       NCK2910_RfRcvDataDeal
** Descriptions:        ����NCK2910���յ���RF����
** input parameters:
** output parameters:
** Returned value:
*********************************************************************************************************/
void NCK2910_RfRcvDataDeal(void)
{
#if UNDER_VOLTAGE_IDLE_MODE_EN
	NCK2910_DealLock = 1;
	if(!NCK2910_ReInitFlag)
	{
#endif
	NCK2910_RfEventProc();
#if UNDER_VOLTAGE_IDLE_MODE_EN
	}
	NCK2910_DealLock = 0;
#endif
#if 0
    uint8_t Tpms_data[11];
	uint32_t Tpms_ID = 0;
	int8_t Tier_Temperature = 0;
	uint16_t Tier_Pressure = 0;
	uint8_t Tmps_crc8 = 0;
	uint8_t bus_nmState;
	static uint8_t RC_value = 0;
	BtaOutputRfData_msg rfdata_msg;
	uint8_t RC_counter;
	bool isIDValid = false;
	uint32_t frameTime = 0;
	Nm_StateType nmstate;
	Nm_ModeType nmmode;


	memset(Tpms_data, 0 ,11);
	NCK2910_SpiRead(Gu8Nck2910RciRxBuf);
	rf_dbg_Printf("22\r\n");


	TPMS_IDs[0] = (ZONE_L_TPMS_01_t.msg.data[18] << 24) | (ZONE_L_TPMS_01_t.msg.data[19] << 16) | (ZONE_L_TPMS_01_t.msg.data[20] << 8) | ZONE_L_TPMS_01_t.msg.data[21];
	TPMS_IDs[1] = (ZONE_L_TPMS_01_t.msg.data[22] << 24) | (ZONE_L_TPMS_01_t.msg.data[23] << 16) | (ZONE_L_TPMS_01_t.msg.data[24] << 8) | ZONE_L_TPMS_01_t.msg.data[25];
	TPMS_IDs[2] = (ZONE_L_TPMS_01_t.msg.data[26] << 24) | (ZONE_L_TPMS_01_t.msg.data[27] << 16) | (ZONE_L_TPMS_01_t.msg.data[28] << 8) | ZONE_L_TPMS_01_t.msg.data[29];
	TPMS_IDs[3] = (ZONE_L_TPMS_01_t.msg.data[30] << 24) | (ZONE_L_TPMS_01_t.msg.data[31] << 16) | (ZONE_L_TPMS_01_t.msg.data[32] << 8) | ZONE_L_TPMS_01_t.msg.data[33];
	//TPMS_IDs[0] = 0xDC010174;

	switch (Gu8Nck2910RciRxBuf[9])
	{
		case 0x55:	// 充电桩HEAD
		{
			break;
		}

		case 0x66:	// 胎压传感器HEAD
		{
			memcpy(Tpms_data, &Gu8Nck2910RciRxBuf[9], sizeof(Tpms_data));//从Gu8Nck2910RciRxBuf[]复制n个数给Tpms_data[]
			Tpms_ID  = (Tpms_data[4] << 24) | (Tpms_data[5] << 16) | (Tpms_data[6] << 8) | Tpms_data[7];
			for (uint8_t i = 0; i < 4; i++)
			{
				if (Tpms_ID == TPMS_IDs[i])
				{
					isIDValid = true;
					break;
				}
			}
			if (true != isIDValid)
			{
				return;
			}
			Tmps_crc8 = TmpsCalculateCRC8(Tpms_data, 10, 0x1D, 0xFF);
			if (Tpms_data[10] != Tmps_crc8)
			{
				RF_Dat_Info.RF_Error_Info = 1;
				RF_Dat_Info.RF_Recv_State = 1;
				RF_Dat_Info.RF_DataErr_Cnt++;
//				hal_uart_if_writebyte(1, 0xCC);	// 串口调试使用，0xCC表CRC校验失败
				return;
			}
			else
			{
				rfdata_msg.msg.sig.Handbyte		= Tpms_data[0];
				rfdata_msg.msg.sig.BalLbtChoose	= (Tpms_data[1] & 0x07);
				rfdata_msg.msg.sig.Lef_Right		= ((Tpms_data[1] & 0x18) >> 3);
				rfdata_msg.msg.sig.FrameNumber	= ((Tpms_data[1] & 0x60) >> 5);
				rfdata_msg.msg.sig.SensorBALSts	= ((Tpms_data[1] & 0x80) >> 7);
				rfdata_msg.msg.sig.WorkMode		= (Tpms_data[2] & 0x0F);
				rfdata_msg.msg.sig.AlartSts		= ((Tpms_data[2] & 0x10) >> 4);
				rfdata_msg.msg.sig.LowBattery	= ((Tpms_data[2] & 0x20) >> 5);
				rfdata_msg.msg.sig.AdvanceFunctionCode = ((Tpms_data[2] & 0xC0) >> 6);
				rfdata_msg.msg.sig.AdvFunc0 		= Tpms_data[3];
				rfdata_msg.msg.sig.sensorID3 	= Tpms_data[4];
				rfdata_msg.msg.sig.sensorID2		= Tpms_data[5];
				rfdata_msg.msg.sig.sensorID1		= Tpms_data[6];
				rfdata_msg.msg.sig.SensorID0		= Tpms_data[7];
				rfdata_msg.msg.sig.CurrentTPMSP_phy = Tpms_data[8];
				rfdata_msg.msg.sig.CurrentTPMST_phy = Tpms_data[9];
				rfdata_msg.msg.sig.RFSignalRC 	= RC_value%3;
				wol2min_counter =0;
			//	uint32_t start_tick = SysTimerGetCurrentTime( ); //用于计算293处理时间的起始位置
			//	hal_uart_if_writebuff(1, Tpms_data, 11);	// 成功通过校验输出接收数据
				CanNm_GetState(0,&nmstate,&nmmode);

				if((nmmode == NM_BUS_SLEEP) || (nmmode == NM_PREPAREBUSSLEEP))
				{

					//CanNm_NetworkRequest(0);//唤醒网络
					//Nm_LocalActive();
					rf_dbg_Printf("Wake SRF !\r\n");
					wakenmFlag = 1;
					RFReleaseflag = 1;
					msg293_sendTime += 500;
					tim1_cnt = 0;
					if (false == RFData_if_QueuePush(&RF_DataQueue, rfdata_msg))
					{
						return;
					}

				}
				else if(nmstate == NM_READYSLEEP)
				{
					Nm_LocalActive();//唤醒网络
					RFReleaseflag = 1;
					if (false == RFData_if_QueuePush(&RF_DataQueue, rfdata_msg))
					{
						return;
					}
				}
				else
				{
					if (false == RFData_if_QueuePush(&RF_DataQueue, rfdata_msg))
					{
						rf_dbg_Printf("Recevie SRF !\r\n");
						return;
					}
					isIDValid = false;
					RC_value++;
					break;
				}
			}
		}

		default:
		{
			//异常报文or不符合NIO规定的射频信号head
			break;
		}

	}
	memset(Gu8Nck2910RciRxBuf, 0 ,256);
#endif
 }

/*********************************************************************************************************
** Function name:       NCK2910_SpiWrite
** Descriptions:        ͨ��SPI��������
** input parameters:    pu8WrBuf : SPIд����ָ��
**						WrBufLen : SPIд���峤�ȣ�unit(Byte)
** output parameters:
** Returned value:
*********************************************************************************************************/
void NCK2910_SpiWrite(uint8_t *pu8WrBuf, uint8_t WrBufLen)
{
	hal_spi_result_et res;
	uint8_t pu8RdBuf[64];

#if 0
	rf_dbg_Printf("NCK2910 Write data: ");
	rf_dbg_Printf("len=%d, data-->", pu8WrBuf[0]);
	for(uint8_t i = 0; i <= pu8WrBuf[0]; i ++)
	{
		rf_dbg_Printf("%02X ", pu8WrBuf[i]);
	}
	rf_dbg_Printf("\r\n");
#endif

	memset(pu8RdBuf, 0, 64);
	hal_pin_if_write(K2910_CS_INDEX,HAL_PIN_LEVEL_LOW);

	uint32_t wait_timeout = 0;
	while ((HAL_PIN_LEVEL_LOW != hal_pin_if_read(HAL_PIN_INDEX8) && (wait_timeout < RF_WAIT_TIMEOUT_CNT))) wait_timeout ++;
//	rf_dbg_Printf("wait_timeout = %d\r\n", wait_timeout);

#if _2910_SPI_COMM_DELAY_1MS
	platform_time_delay(1);
#endif
	res = hal_spi_if_write(NCK2910_SPI_CHANNEL, pu8WrBuf, NULL, WrBufLen);
	if (HAL_SPI_RESULT_SUCCESS != res)
	{
		rf_dbg_Printf("NCK2910 write LEN = %d byte data is fail\r\n", WrBufLen);
	}
	hal_pin_if_write(K2910_CS_INDEX,HAL_PIN_LEVEL_HIGH);

	wait_timeout = 0;
	while ((HAL_PIN_LEVEL_LOW == hal_pin_if_read(HAL_PIN_INDEX8)) && (wait_timeout < RF_WAIT_TIMEOUT_CNT)) wait_timeout ++;
//	rf_dbg_Printf("wait_timeout = %d\r\n", wait_timeout);
}

/*********************************************************************************************************
** Function name:       NCK2910_SpiRead
** Descriptions:        ͨ��SPI������
** input parameters:    pu8RdBuf : SPI������ָ��
** output parameters:
** Returned value:
*********************************************************************************************************/
void delay_us(uint32_t us)
{
	uint32_t loop = us *1000;
    while (loop > 0U)
    {
        __NOP();
        loop--;
    }
}

#if 1
hal_spi_result_et NCK2910_SpiRead(uint8_t *pu8RdBuf)
{
	hal_spi_result_et res;

	/* CS Active */
	hal_pin_if_write(K2910_CS_INDEX,HAL_PIN_LEVEL_LOW);
	/* 先读取一个字节，该字节表示报文长度(RCI CMD格式) */

	uint32_t wait_timeout = 0;
	while ((HAL_PIN_LEVEL_LOW != hal_pin_if_read(HAL_PIN_INDEX8)) && (wait_timeout < RF_WAIT_TIMEOUT_CNT)) wait_timeout ++;
//	rf_dbg_Printf("wait_timeout = %d\r\n", wait_timeout);

#if _2910_SPI_COMM_DELAY_1MS
	platform_time_delay(1);
#endif
	res = hal_spi_if_write(NCK2910_SPI_CHANNEL, NULL, &pu8RdBuf[0], 1);
	if (HAL_SPI_RESULT_SUCCESS == res)
	{
		if(pu8RdBuf[0] > 0 && pu8RdBuf[0] < 255)
		{
			/* 根据刚刚收到的字节长度，收取完整报文 */
//			while (HAL_PIN_LEVEL_LOW != hal_pin_if_read(HAL_PIN_INDEX8));
#if _2910_SPI_COMM_DELAY_1MS
			platform_time_delay(1);
#endif
			res = hal_spi_if_write(NCK2910_SPI_CHANNEL, NULL, &pu8RdBuf[1], pu8RdBuf[0]);
			if (HAL_SPI_RESULT_SUCCESS != res)
			{
				rf_dbg_Printf("NCK2910 read data byte is fail, read LEN = %d\r\n", pu8RdBuf[0]);
			}
		}
		else
		{
			rf_dbg_Printf("NCK2910 read first byte LEN error, LEN = %d\r\n", pu8RdBuf[0]);
			res = HAL_SPI_RESULT_FAIL;
		}
	}
	else
	{
		rf_dbg_Printf("NCK2910 read first LEN byte is fail\r\n");
	}

	/* CS Deactive */
	hal_pin_if_write(K2910_CS_INDEX,HAL_PIN_LEVEL_HIGH);

	wait_timeout = 0;
	while ((HAL_PIN_LEVEL_LOW == hal_pin_if_read(HAL_PIN_INDEX8)) && (wait_timeout < RF_WAIT_TIMEOUT_CNT)) wait_timeout ++;
//	rf_dbg_Printf("wait_timeout = %d\r\n", wait_timeout);

	if (HAL_SPI_RESULT_SUCCESS == res)
	{
		if(Gu8Nck2910RciRxBuf[Gu8Nck2910RciRxBuf[0]] != _CalculateCRC8(Gu8Nck2910RciRxBuf, Gu8Nck2910RciRxBuf[0]))
		{
			rf_dbg_Printf("NCK2910 read data CRC8 error.\r\n");
			rf_dbg_Printf("NCK2910 read data: ");
			rf_dbg_Printf("len=%d, data-->", Gu8Nck2910RciRxBuf[0]);
			for(uint8_t i = 0; i <= Gu8Nck2910RciRxBuf[0]; i ++)
			{
				rf_dbg_Printf("%02X ", Gu8Nck2910RciRxBuf[i]);
			}
			rf_dbg_Printf("\r\n");
			res = HAL_SPI_RESULT_FAIL;
		}
	}

#if 0
	rf_dbg_Printf("NCK2910 Read data: ");
	rf_dbg_Printf("len=%d, data-->", pu8RdBuf[0]);
	for(uint8_t i = 0; i <= pu8RdBuf[0]; i ++)
	{
		rf_dbg_Printf("%02X ", pu8RdBuf[i]);
	}
	rf_dbg_Printf("\r\n");
#endif

	return res;
}
#else
hal_spi_result_et NCK2910_SpiRead(uint8_t *pu8RdBuf)
{
	uint8_t	Rdspi_lenth	= 0;
	uint8_t rdFlag[64];
	memset(rdFlag, 0, 64);

//	//  1. cs = 0;
	hal_pin_if_write(K2910_CS_INDEX,HAL_PIN_LEVEL_LOW);
	platform_time_delay(1);
	if (HAL_PIN_LEVEL_LOW == hal_pin_if_read(HAL_PIN_INDEX8))
	{
		if (HAL_SPI_RESULT_SUCCESS != hal_spi_if_write(NCK2910_SPI_CHANNEL, &rdFlag[0], &pu8RdBuf[0], 1))
		{
			rf_dbg_Printf("err\r\n");
			return;
		}
	}
//	//  2.先读取一个字节，该字节表示报文长度(RCI CMD格式)
	platform_time_delay(1);
//	//  3.根据刚刚收到的字节长度，收取完整报文
	if (HAL_PIN_LEVEL_LOW == hal_pin_if_read(HAL_PIN_INDEX8))
	{
		if (HAL_SPI_RESULT_SUCCESS != hal_spi_if_write(NCK2910_SPI_CHANNEL, rdFlag, &pu8RdBuf[1], pu8RdBuf[0]))
		{
			rf_dbg_Printf("err2\r\n");
			return;
		}
	}
//	//	4.cs = 1;
	hal_pin_if_write(K2910_CS_INDEX,HAL_PIN_LEVEL_HIGH);

	return 0;
}
#endif

static void _NCK2910_Ready(void)
{
	if(rfInitSuccFlag)
	{
		OSA_SemaphorePost(semaphoreHandle);
		//rf_SetSleepFlag(0);
		//rf_dbg_Printf("11\r\n");

		if(!recv_cnt_zero_flag)	NCK2910_IntCnt ++;
		Nck2910ShortTermIntNum ++;
	}
}

/*********************************************************************************************************
  EoF
*********************************************************************************************************/


