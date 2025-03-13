
#include "string.h"
#include "uds_confpriv.h"
#include "E2Para.h"
#include "udscan_confdefs.h"
/* Active Diag Session */
void UDS_itf_readDid_F186(uint8_t *i_pDataBufPtr)
{
    i_pDataBufPtr[0] = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION();
}
/* Vehicle Manufacture Hardware Part and Version Number */
/* Vehicle Manufacture Hardware Part and Version Number */
void UDS_itf_readDid_F110(uint8_t *i_pDataBufPtr)
{
   did_F110_set_value(i_pDataBufPtr);
}

/* ECU Supplier Identifier */
void UDS_itf_readDid_F18A(uint8_t *i_pDataBufPtr)
{
    did_F18A_set_value(i_pDataBufPtr);
}

/* ECU Manufacture date */
void UDS_itf_readDid_F18B(uint8_t *i_pDataBufPtr)
{
     did_F18B_set_value(i_pDataBufPtr);
}

/* Write Fingerprint */
void UDS_itf_readDid_F15A(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
}

/* Read Fingerprint */
void UDS_itf_readDid_F15B(uint8_t *i_pDataBufPtr)
{
    did_F15B_set_value(i_pDataBufPtr);
}

/* Vehicle Identification Number */
void UDS_itf_readDid_F190(uint8_t *i_pDataBufPtr)
{
     did_F190_set_value(i_pDataBufPtr);
}

/* Vehicle Manufacture Hardware Baseline */
void UDS_itf_readDid_F111(uint8_t *i_pDataBufPtr)
{
     did_F111_set_value(i_pDataBufPtr);
}

/* Vehicle Manufacture Software Part and Version Number */
void UDS_itf_readDid_F118(uint8_t *i_pDataBufPtr)
{
     did_F118_set_value(i_pDataBufPtr);
}

/* Vehicle Manufacture Software Baseline */
void UDS_itf_readDid_F119(uint8_t *i_pDataBufPtr)
{
    did_F119_set_value(i_pDataBufPtr);
}

/* Reprogramming attempt counter */
void UDS_itf_readDid_F010(uint8_t *i_pDataBufPtr)
{
     did_F010_set_value(i_pDataBufPtr);
}

/* Diagnostic database version */
void UDS_itf_readDid_F19E(uint8_t *i_pDataBufPtr)
{
     did_F19E_set_value(i_pDataBufPtr);
}

/* Bootloader version number */
void UDS_itf_readDid_F150(uint8_t *i_pDataBufPtr)
{
   did_F150_set_value(i_pDataBufPtr);
}

/* Serial number */
void UDS_itf_readDid_F18C(uint8_t *i_pDataBufPtr)
{
   did_F18C_set_value(i_pDataBufPtr);
}

/* System Supplier Hardware Version Number */
void UDS_itf_readDid_F193(uint8_t *i_pDataBufPtr)
{
    did_F193_set_value(i_pDataBufPtr);
}

/* System Supplier Software Version Number */
void UDS_itf_readDid_F195(uint8_t *i_pDataBufPtr)
{
    did_F195_set_value(i_pDataBufPtr);
}

/* Vehicle Manufacture Calibration Part and Version Number */
void UDS_itf_readDid_F130(uint8_t *i_pDataBufPtr)
{
#if ((UdsApi_BTA_SRF_FUN_EN == 1) || (UdsApi_BTA_SRR_FUN_EN == 1))
	did_F130_set_value(i_pDataBufPtr);
#endif
}

/* Vehicle Manufacturer Calibration Baseline */
void UDS_itf_readDid_F131(uint8_t *i_pDataBufPtr)
{
#if ((UdsApi_BTA_SRF_FUN_EN == 1) || (UdsApi_BTA_SRR_FUN_EN == 1))
	did_F131_set_value(i_pDataBufPtr);
#endif
}

/* 29D6 Version nummber */
void UDS_itf_readDid_F151(uint8_t *i_pDataBufPtr)
{
#if ((UdsApi_BTA_SRF_FUN_EN == 1) || (UdsApi_BTA_SRR_FUN_EN == 1))
	did_F151_set_value(i_pDataBufPtr);
#endif
}


/* Vehicle id */
void UDS_itf_readDid_4A00(uint8_t *i_pDataBufPtr)
{
    	did_4A00_set_value(i_pDataBufPtr);
}

/* Keyfob device id */
void UDS_itf_readDid_4A01(uint8_t *i_pDataBufPtr)
{
    	did_4A01_set_value(i_pDataBufPtr);
}

/* BTA BLE Mac Address */
void UDS_itf_readDid_4A02(uint8_t *i_pDataBufPtr)
{
    	did_4A02_set_value(i_pDataBufPtr);
}

/* BAT_SRF Battery_Voltage */
void UDS_itf_readDid_4A03(uint8_t *i_pDataBufPtr)
{
    	did_4A03_set_value(i_pDataBufPtr);
}

/* BAT_FB Battery_Voltage */
void UDS_itf_readDid_4A43(uint8_t *i_pDataBufPtr)
{
    	did_4A43_set_value(i_pDataBufPtr);
}

/* BAT_SRR Battery_Voltage */
void UDS_itf_readDid_4A33(uint8_t *i_pDataBufPtr)
{
    	did_4A33_set_value(i_pDataBufPtr);
}

/* BAT_L Battery_Voltage */
void UDS_itf_readDid_4A13(uint8_t *i_pDataBufPtr)
{
    	did_4A13_set_value(i_pDataBufPtr);
}

/* BAT_R Battery_Voltage */
void UDS_itf_readDid_4A23(uint8_t *i_pDataBufPtr)
{
    	did_4A23_set_value(i_pDataBufPtr);
}

/* BTA_Adv_Interval */
void UDS_itf_readDid_4A04(uint8_t *i_pDataBufPtr)
{
   	did_4A04_set_value(i_pDataBufPtr);
}

/* BTA_Conn_Interval */
void UDS_itf_readDid_4A05(uint8_t *i_pDataBufPtr)
{
   	did_4A05_set_value(i_pDataBufPtr);
}

/* BTA_Slave_Latency */
void UDS_itf_readDid_4A06(uint8_t *i_pDataBufPtr)
{
    	did_4A06_set_value(i_pDataBufPtr);
}

/* BTA_White_List */
void UDS_itf_readDid_4A08(uint8_t *i_pDataBufPtr)
{
    	did_4A08_set_value(i_pDataBufPtr);
}

/* BTA_CurConnDevice_Info */
void UDS_itf_readDid_4A09(uint8_t *i_pDataBufPtr)
{
    	did_4A09_set_value(i_pDataBufPtr);
}

/* Supplier APP software test version */
void UDS_itf_readDid_4A50(uint8_t *i_pDataBufPtr)
{
    	did_4A50_set_value(i_pDataBufPtr);
}

/* ECU part number */
void UDS_itf_readDid_4A51(uint8_t *i_pDataBufPtr)
{
    did_4A51_set_value(i_pDataBufPtr);
}

/* Fulda assembly part number */
void UDS_itf_readDid_4A52(uint8_t *i_pDataBufPtr)
{
    did_4A52_set_value(i_pDataBufPtr);
}

/* Supplier APP2 software test version */
void UDS_itf_readDid_4A53(uint8_t *i_pDataBufPtr)
{
    did_4A53_set_value(i_pDataBufPtr);
}

/* Supplier APP2 software release version */
void UDS_itf_readDid_4A54(uint8_t *i_pDataBufPtr)
{
   did_4A54_set_value(i_pDataBufPtr);
}

/* Supplier BOOTLOADER2 software version */
void UDS_itf_readDid_4A55(uint8_t *i_pDataBufPtr)
{
    did_4A55_set_value(i_pDataBufPtr);
}

void UDS_itf_readDid_4A59(uint8_t *i_pDataBufPtr)
{
    //did_4A55_set_value(i_pDataBufPtr);
}


/* NFC_ECP_TCI */
void UDS_itf_readDid_4A1F(uint8_t *i_pDataBufPtr)
{
   did_4A1F_set_value(i_pDataBufPtr);
}
/* BTA_KeyFobEOLInfo */
void UDS_itf_readDid_4A0B(uint8_t *i_pDataBufPtr)
{
	did_4A0B_set_value(i_pDataBufPtr);
}

/* KW45 reset source */
void UDS_itf_readDid_4A0A(uint8_t *i_pDataBufPtr)
{
    did_4A0A_set_value(i_pDataBufPtr);
}

/* 29D6 reset source */
void UDS_itf_readDid_4A57(uint8_t *i_pDataBufPtr)
{
	 did_4A57_set_value(i_pDataBufPtr);
}
/* Turn on Bluetooth broadcasting */
void UDS_itf_readDid_4A56(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
}
