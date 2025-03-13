/*
 * bleMessage_cfg.h
 *
 *  Created on: 2024年6月14日
 *      Author: F704012276_
 */

#ifndef DRV_BLE_BLEMESSAGE_BLEMESSAGE_CFG_H_
#define DRV_BLE_BLEMESSAGE_BLEMESSAGE_CFG_H_

#include "signals_api.h"

#define BLE_MESSAGE_DV_Length		2		/* change according to the num of BLE message*/

typedef enum
{
/*	enum_bleMsg_0x04_0x01 = 0,
	enum_bleMsg_0x04_0x03,
	enum_bleMsg_0x04_0x05,
	enum_bleMsg_0x04_0x07,
	enum_bleMsg_0x04_0x09,
	enum_bleMsg_0x04_0xB1,

	enum_bleMsg_0x03_0x11,

	enum_bleMsg_0x20_0x00,
	enum_bleMsg_0x20_0x02,

	enum_bleMsg_0x01_0x01,
	enum_bleMsg_0x01_0x03,
	enum_bleMsg_0x40_0x00,

	enum_bleMsg_0x04_0xF1,
	enum_bleMsg_0x04_0xF3,
	enum_bleMsg_0x04_0xF5,
	enum_bleMsg_0x04_0xF7,

	enum_bleMsg_0x84_0x02,
	enum_bleMsg_0x84_0x04,
	enum_bleMsg_0x84_0x06,
	enum_bleMsg_0x84_0x08,*/
	enum_bleMsg_0x84_0x0A,

	enum_bleMsgTx_total_num,

}enum_TxbleMsg_t;

typedef enum
{
/*	enum_bleMsg_0x04_0x02 = 0,	Phone -> BLE
	enum_bleMsg_0x04_0x04,
	enum_bleMsg_0x04_0x06,
	enum_bleMsg_0x04_0x08,
	enum_bleMsg_0x04_0x0A,*/
	enum_bleMsg_0x04_0x0B,
/*	enum_bleMsg_0x04_0xB0,

	enum_bleMsg_0x01_0x01,
	enum_bleMsg_0x01_0x02,		FOB -> BLE */
	enum_bleMsg_0x01_0x03,
/*	enum_bleMsg_0x01_0x04,
	enum_bleMsg_0x04_0xF2,
	enum_bleMsg_0x04_0xF4,
	enum_bleMsg_0x04_0xF6,
	enum_bleMsg_0x04_0xF8,
	enum_bleMsg_0x1A_0x00,
	enum_bleMsg_0x20_0x01,
	enum_bleMsg_0x20_0x03,
*/
	enum_bleMsg_0x40_0x01,

	enum_bleMsg_0x84_0x01,		/*VDF -> BLE through CAN*/
	enum_bleMsg_0x84_0x03,
	enum_bleMsg_0x84_0x05,
	enum_bleMsg_0x84_0x07,
	enum_bleMsg_0x84_0x09,

	enum_bleMsg_total_num,

}enum_RxbleMsg_t;

typedef struct
{
	uint8_t device_addr[6];
}device_addr_st;

typedef struct
{
	uint8_t device_addr[6];
	uint8_t device_id[16];
	uint8_t device_status[1];
}device_info_st;

typedef struct
{
	uint8_t bleMsg_MType_0x01_MID_0x01[1];
}bleMsg_MType_0x01_MID_0x01_t;

typedef struct
{
	uint8_t bleMsg_MType_0x01_MID_0x02[1];
}bleMsg_MType_0x01_MID_0x02_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x01_MID_0x03[2];
		struct
		{
			uint8_t SKF_Auth_Result[1];
			uint8_t error_code[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x01_MID_0x03_t;
#define KEYFOB_AUTHEN_SUCCESS	0
#define KEYFOB_AUTHEN_FAILED	1
#define KEYFOB_AUTHEN_CODE_NOErrors	0
#define KEYFOB_AUTHEN_CODE_RevVKTimeout	1
#define KEYFOB_AUTHEN_CODE_DecryptionFailed	2
#define KEYFOB_AUTHEN_CODE_VehicleIdNotMatched	3
#define KEYFOB_AUTHEN_CODE_RandomNotMatched	4
#define KEYFOB_AUTHEN_CODE_LatencyLonger2s	5

typedef struct
{
	uint8_t bleMsg_MType_0x01_MID_0x04[1];
}bleMsg_MType_0x01_MID_0x04_t;

typedef struct
{
	uint8_t bleMsg_MType_0x40_MID_0x00[1];
}bleMsg_MType_0x40_MID_0x00_t;

typedef struct
{
	uint8_t bleMsg_MType_0x40_MID_0x01[1];
}bleMsg_MType_0x40_MID_0x01_t;

typedef struct
{
	uint8_t bleMsg_MType_0x04_MID_0xF1[1];
}bleMsg_MType_0x04_MID_0xF1_t;
typedef struct
{
	uint8_t bleMsg_MType_0x04_MID_0xF2[1];
}bleMsg_MType_0x04_MID_0xF2_t;
typedef struct
{
	uint8_t bleMsg_MType_0x04_MID_0xF3[1];
}bleMsg_MType_0x04_MID_0xF3_t;
typedef struct
{
	uint8_t bleMsg_MType_0x04_MID_0xF4[1];
}bleMsg_MType_0x04_MID_0xF4_t;
typedef struct
{
	uint8_t bleMsg_MType_0x04_MID_0xF5[1];
}bleMsg_MType_0x04_MID_0xF5_t;
typedef struct
{
	uint8_t bleMsg_MType_0x04_MID_0xF6_t[1];
}bleMsg_MType_0x04_MID_0xF6_t;
typedef struct
{
	uint8_t bleMsg_MType_0x04_MID_0xF7[1];
}bleMsg_MType_0x04_MID_0xF7_t;

typedef struct
{
	uint8_t bleMsg_MType_0x04_MID_0xF8[1];
}bleMsg_MType_0x04_MID_0xF8_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x04_MID_0x01[72];
		struct
		{
			uint8_t s_random[16];
			uint8_t key_id[16];
			uint8_t read_ts[8];
			uint8_t realtime_cipher[32];
		}Msg;
	}Msg;
}bleMsg_MType_0x04_MID_0x01_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x04_MID_0x02[42];
		struct
		{
			uint8_t s_random[16];
			uint8_t vehicle_id[32];
			uint8_t read_ts[8];
			uint8_t device_type[1];
			uint8_t rssi[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x04_MID_0x02_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x04_MID_0x03[2];
		struct
		{
			uint8_t SKF_Auth_Result[1];
			uint8_t error_code[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x04_MID_0x03_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x04_MID_0x04[1];
		struct
		{
			uint8_t Key_Fob_NRC[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x04_MID_0x04_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x04_MID_0x05[1];
		struct
		{
			uint8_t Key_Fob_NRC[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x04_MID_0x05_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x04_MID_0x06[1];
		struct
		{
			uint8_t Key_Fob_NRC[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x04_MID_0x06_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x04_MID_0x07[1];
		struct
		{
			uint8_t Key_Fob_NRC[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x04_MID_0x07_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x04_MID_0x08[1];
		struct
		{
			uint8_t Key_Fob_NRC[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x04_MID_0x08_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x04_MID_0x09[1];
		struct
		{
			uint8_t Key_Fob_NRC[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x04_MID_0x09_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x04_MID_0x0A[1];
		struct
		{
			uint8_t Key_Fob_NRC[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x04_MID_0x0A_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x04_MID_0x0B[1];
		struct
		{
			uint8_t Key_Fob_NRC[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x04_MID_0x0B_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x04_MID_0xB0[1];
		struct
		{
			uint8_t Key_Fob_NRC[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x04_MID_0xB0_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x04_MID_0xB1[1];
		struct
		{
			uint8_t Key_Fob_NRC[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x04_MID_0xB1_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x03_MID_0x11[1];
	}Msg;
}bleMsg_MType_0x03_MID_0x11_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x20_MID_0x00[42];
		struct
		{
			uint8_t random_str_another[16];
			uint8_t instruction[2];
			uint8_t instruction_ts[8];
			uint8_t key_id[16];
		}Msg;
	}Msg;
}bleMsg_MType_0x20_MID_0x00_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x20_MID_0x01[60];
		struct
		{
			uint8_t vehicle_id[16];
			uint8_t mode[1];
			uint8_t device_id[16];
			uint8_t random_str_another[16];
			uint8_t rssi[1];
			uint8_t instruction[2];
			uint8_t instruction_ts[8];
		}Msg;
	}Msg;
}bleMsg_MType_0x20_MID_0x01_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x20_MID_0x02[1];
		struct
		{
			uint8_t KeyFob_IMMO_Errort[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x20_MID_0x02_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x20_MID_0x03[1];
	}Msg;
}bleMsg_MType_0x20_MID_0x03_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x1A_MID_0x00[1];
	}Msg;
}bleMsg_MType_0x1A_MID_0x00_t;


typedef struct
{
	union{
		uint8_t bleMsg_MType_0x84_MID_0x01[1];
		struct
		{
			uint8_t device_addr[6];
		}Msg;
	}Msg;
}bleMsg_MType_0x84_MID_0x01_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x84_MID_0x02[1];
		struct
		{
			uint8_t device_addr[6];
			uint8_t result[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x84_MID_0x02_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x84_MID_0x03[8];
		struct
		{
			uint8_t action_id[1];
			uint8_t device_type[1];
			uint8_t device_addr[6];
		}Msg;
	}Msg;
}bleMsg_MType_0x84_MID_0x03_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x84_MID_0x04[9];
		struct
		{
			uint8_t action_id[1];
			uint8_t device_type[1];
			uint8_t device_addr[6];
			uint8_t result[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x84_MID_0x04_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x84_MID_0x05[22];
		struct
		{
			uint8_t device_address[6];
			uint8_t device_id[16];
		}Msg;
	}Msg;
}bleMsg_MType_0x84_MID_0x05_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x84_MID_0x06[7];
		struct
		{
			uint8_t device_addr[6];
			uint8_t result[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x84_MID_0x06_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x84_MID_0x07[1];
		struct
		{
			uint8_t reserved[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x84_MID_0x07_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x84_MID_0x08[50];
		struct
		{
			uint8_t result[1];
			uint8_t device_count[1];
			device_addr_st device_addr0;
			device_addr_st device_addr1;
			device_addr_st device_addr2;
			device_addr_st device_addr3;
			device_addr_st device_addr4;
			device_addr_st device_addr5;
			device_addr_st device_addr6;
			device_addr_st device_addr7;
		}Msg;
	}Msg;
}bleMsg_MType_0x84_MID_0x08_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x84_MID_0x09[1];
		struct
		{
			uint8_t reserved[1];
		}Msg;
	}Msg;
}bleMsg_MType_0x84_MID_0x09_t;

typedef struct
{
	union{
		uint8_t bleMsg_MType_0x84_MID_0x0A[186];
		struct
		{
			uint8_t result[1];
			uint8_t device_count[1];
			device_info_st deviceInfo_0;
			device_info_st deviceInfo_1;
			device_info_st deviceInfo_2;
			device_info_st deviceInfo_3;
			device_info_st deviceInfo_4;
			device_info_st deviceInfo_5;
			device_info_st deviceInfo_6;
			device_info_st deviceInfo_7;
		}Msg;
	}Msg;
}bleMsg_MType_0x84_MID_0x0A_t;

typedef struct phoneMsgMID_0x04_tag
{
	uint8_t MID;
	union{
		bleMsg_MType_0x04_MID_0x02_t bleMsg_0x04_0x02;
		bleMsg_MType_0x04_MID_0x04_t bleMsg_0x04_0x04;
		bleMsg_MType_0x04_MID_0x06_t bleMsg_0x04_0x06;
		bleMsg_MType_0x04_MID_0x08_t bleMsg_0x04_0x08;
		bleMsg_MType_0x04_MID_0x0A_t bleMsg_0x04_0x0A;
		bleMsg_MType_0x04_MID_0x0B_t bleMsg_0x04_0x0B;
		bleMsg_MType_0x04_MID_0xB0_t bleMsg_0x04_0xB0;
	}phoneMsg;
}phoneMsgMT_0x04_t;

typedef struct phoneDKMessage_tag
{
	uint8_t MType;
	union{
		phoneMsgMT_0x04_t				bleMsg_0x04;
	}phoneMsgMT;
}phoneDKMessage_t;

typedef struct canMsgMT_0x01_tag
{
	uint8_t MID;
	union{
		bleMsg_MType_0x01_MID_0x02_t bleMsg_0x01_0x02;
		bleMsg_MType_0x01_MID_0x04_t bleMsg_0x01_0x04;
	}canMsg;
}canMsgMT_0x01_t;

typedef struct canMsgMT_0x04_tag
{
	uint8_t MID;
	union{
		bleMsg_MType_0x04_MID_0xF2_t bleMsg_0x04_0xF2;
		bleMsg_MType_0x04_MID_0xF4_t bleMsg_0x04_0xF4;
		bleMsg_MType_0x04_MID_0xF6_t bleMsg_0x04_0xF6;
		bleMsg_MType_0x04_MID_0xF8_t bleMsg_0x04_0xF8;
	}canMsg;
}canMsgMT_0x04_t;

typedef struct canMsgMT_0x1A_tag
{
	uint8_t MID;
	union{
		bleMsg_MType_0x1A_MID_0x00_t bleMsg_0x1A_0x00;
	}canMsg;
}canMsgMT_0x1A_t;

typedef struct canMsgMT_0x20_tag
{
	uint8_t MID;
	union{
		bleMsg_MType_0x20_MID_0x01_t bleMsg_0x20_0x01;
		bleMsg_MType_0x20_MID_0x03_t bleMsg_0x20_0x03;
	}canMsg;
}canMsgMT_0x20_t;

typedef struct canMsgMT_0x40_tag
{
	uint8_t MID;
	union{
		bleMsg_MType_0x40_MID_0x01_t bleMsg_0x40_0x01;
	}canMsg;
}canMsgMT_0x40_t;

typedef struct canDKMessage_tag
{
	uint8_t MType;
	union{
		canMsgMT_0x01_t				bleMsg_0x01;
		canMsgMT_0x04_t				bleMsg_0x04;
		canMsgMT_0x1A_t				bleMsg_0x1A;
		canMsgMT_0x20_t				bleMsg_0x20;
		canMsgMT_0x40_t				bleMsg_0x40;
	}canMsgMT;
}canDKMessage_t;


typedef struct fobMsgMT_0x84_tag
{
	uint8_t MID;
	union{
		bleMsg_MType_0x84_MID_0x01_t bleMsg_0x84_0x01;
		bleMsg_MType_0x84_MID_0x03_t bleMsg_0x84_0x03;
		bleMsg_MType_0x84_MID_0x05_t bleMsg_0x84_0x05;
		bleMsg_MType_0x84_MID_0x07_t bleMsg_0x84_0x07;
		bleMsg_MType_0x84_MID_0x09_t bleMsg_0x84_0x09;
	}fobMsg;
}fobMsgMT_0x84_t;

typedef struct fobDKMessage_tag
{
	uint8_t MType;
	union{
		fobMsgMT_0x84_t				bleMsg_0x84;
	}fobMsgMT;
}fobDKMessage_t;

typedef struct appDKMessage_tag
{
    uint8_t		deviceId;
    uint8_t   	handle;
    union{
    	phoneDKMessage_t phoneMsg;
    	canDKMessage_t canMsg;
    	fobDKMessage_t fobMsg;
    }Msg;
} appRevDKMessage_t;

typedef uint8_t   (*ApplIndicationFunc)      (uint8_t deviceId, void* pBleMsg);
typedef uint8_t   (*ApplPreCopyFunc)      (uint8_t deviceId, void* pBleMsg);

#endif /* DRV_BLE_BLEMESSAGE_BLEMESSAGE_CFG_H_ */
