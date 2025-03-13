/* This file was automatically generated */

#include "DidList.h"
#include "UdsItf_ReadDid.h"
#include "UdsItf_WriteDid.h"

uint8_t rub_DidDataOperBuf[UDS_C_DID_DATA_OPERATE_BUF_SIZE];

#ifdef UDS_ENABLE_READDATABYIDENTIFIER

const UDS_DidDatMngRecd_t Uds_g_DidDataTable[] =
{
    { 0xF186u, 1u  , 0x0A01, rub_DidDataOperBuf, UDS_itf_readDid_F186 }, /* Active Diag Session */
    { 0xF110u, 11u , 0x0303, rub_DidDataOperBuf, UDS_itf_readDid_F110 }, /* Vehicle Manufacture Hardware Part and Version Number */
    { 0xF18Au, 8u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_F18A }, /* ECU Supplier Identifier */
    { 0xF18Bu, 3u  , 0x1303, rub_DidDataOperBuf, UDS_itf_readDid_F18B }, /* ECU Manufacture date */
    { 0xF15Au, 12u , 0x4103, rub_DidDataOperBuf, UDS_itf_readDid_F15A }, /* Write Fingerprint */
    { 0xF15Bu, 12u , 0x0A01, rub_DidDataOperBuf, UDS_itf_readDid_F15B }, /* Read Fingerprint */
    { 0xF190u, 17u , 0x1243, rub_DidDataOperBuf, UDS_itf_readDid_F190 }, /* Vehicle Identification Number */
    { 0xF111u, 2u  , 0x0B03, rub_DidDataOperBuf, UDS_itf_readDid_F111 }, /* Vehicle Manufacture Hardware Baseline */
    { 0xF118u, 11u , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_F118 }, /* Vehicle Manufacture Software Part and Version Number */
    { 0xF119u, 2u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_F119 }, /* Vehicle Manufacture Software Baseline */
    { 0xF010u, 4u  , 0x0A01, rub_DidDataOperBuf, UDS_itf_readDid_F010 }, /* Reprogramming attempt counter */
    { 0xF19Eu, 3u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_F19E }, /* Diagnostic database version */
    { 0xF150u, 6u  , 0x0A01, rub_DidDataOperBuf, UDS_itf_readDid_F150 }, /* Bootloader version number */
    { 0xF18Cu, 19u , 0x1343, rub_DidDataOperBuf, UDS_itf_readDid_F18C }, /* Serial number */
    { 0xF193u, 3u  , 0x0303, rub_DidDataOperBuf, UDS_itf_readDid_F193 }, /* System Supplier Hardware Version Number */
    { 0xF195u, 6u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_F195 }, /* System Supplier Software Version Number */
    { 0xF130u, 11u , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_F130 }, /* Vehicle Manufacture Calibration Part and Version Number */
    { 0xF131u, 2u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_F131 }, /* Vehicle Manufacturer Calibration Baseline */
    { 0xF151u, 6u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_F151 }, /* 29D6 Version nummber */
    { 0x4A00u, 16u , 0x1243, rub_DidDataOperBuf, UDS_itf_readDid_4A00 }, /* Vehicle id */
    { 0x4A01u, 16u , 0x1243, rub_DidDataOperBuf, UDS_itf_readDid_4A01 }, /* Keyfob device id */
    { 0x4A02u, 6u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A02 }, /* BTA BLE Mac Address */
    { 0x4A03u, 1u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A03 }, /* BAT_SRF Battery_Voltage */
    { 0x4A43u, 1u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A43 }, /* BAT_FB Battery_Voltage */
    { 0x4A33u, 1u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A33 }, /* BAT_SRR Battery_Voltage */
    { 0x4A13u, 1u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A13 }, /* BAT_L Battery_Voltage */
    { 0x4A23u, 1u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A23 }, /* BAT_R Battery_Voltage */
    { 0x4A04u, 2u  , 0x1243, rub_DidDataOperBuf, UDS_itf_readDid_4A04 }, /* BTA_Adv_Interval */
    { 0x4A05u, 2u  , 0x1243, rub_DidDataOperBuf, UDS_itf_readDid_4A05 }, /* BTA_Conn_Interval */
    { 0x4A06u, 2u  , 0x1243, rub_DidDataOperBuf, UDS_itf_readDid_4A06 }, /* BTA_Slave_Latency */
    { 0x4A08u, 120u, 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A08 }, /* BTA_White_List */
    { 0x4A09u, 110u, 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A09 }, /* BTA_CurConnDevice_Info */
    { 0x4A50u, 10u , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A50 }, /* Supplier APP software test version */
    { 0x4A51u, 8u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A51 }, /* ECU part number */
    { 0x4A52u, 10u , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A52 }, /* Fulda assembly part number */
    { 0x4A53u, 10u , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A53 }, /* Supplier APP2 software test version */
    { 0x4A54u, 6u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A54 }, /* Supplier APP2 software release version */
    { 0x4A55u, 6u  , 0x0201, rub_DidDataOperBuf, UDS_itf_readDid_4A55 }, /* Supplier BOOTLOADER2 software version */
    { 0x4A1Fu, 3u  , 0x1243, rub_DidDataOperBuf, UDS_itf_readDid_4A1F }, /* NFC_ECP_TCI */
    {UDS_C_DID_END_OF_TBL_FFFF, 0x00u, 0x0000u, NULL, NULL}/*This Line indicate the end of the data table */
};
#endif /* #ifdef UDS_ENABLE_READDATABYIDENTIFIER */

#ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER

const UDS_DidEepromDatMngRecd_t Uds_g_DidEepromDataTable[] =
{
    { 0xF110u, 11u , 0xA203, 0, rub_DidDataOperBuf, UDS_itf_writeCheck_F110, UDS_itf_writeDid_F110 }, /* Vehicle Manufacture Hardware Part and Version Number */
    { 0xF18Bu, 3u  , 0xA203, 0, rub_DidDataOperBuf, UDS_itf_writeCheck_F18B, UDS_itf_writeDid_F18B }, /* ECU Manufacture date */
    { 0xF15Au, 12u , 0xA203, 0, rub_DidDataOperBuf, UDS_itf_writeCheck_F15A, UDS_itf_writeDid_F15A }, /* Write Fingerprint */
    { 0xF190u, 17u , 0x1243, 0, rub_DidDataOperBuf, UDS_itf_writeCheck_F190, UDS_itf_writeDid_F190 }, /* Vehicle Identification Number */
    { 0xF111u, 2u  , 0xA203, 0, rub_DidDataOperBuf, UDS_itf_writeCheck_F111, UDS_itf_writeDid_F111 }, /* Vehicle Manufacture Hardware Baseline */
    { 0xF18Cu, 19u , 0xA203, 0, rub_DidDataOperBuf, UDS_itf_writeCheck_F18C, UDS_itf_writeDid_F18C }, /* Serial number */
    { 0xF193u, 3u  , 0xA203, 0, rub_DidDataOperBuf, UDS_itf_writeCheck_F193, UDS_itf_writeDid_F193 }, /* System Supplier Hardware Version Number */
    { 0x4A00u, 16u , 0x1243, 0, rub_DidDataOperBuf, UDS_itf_writeCheck_4A00, UDS_itf_writeDid_4A00 }, /* Vehicle id */
    { 0x4A01u, 16u , 0x1243, 0, rub_DidDataOperBuf, UDS_itf_writeCheck_4A01, UDS_itf_writeDid_4A01 }, /* Keyfob device id */
    { 0x4A04u, 2u  , 0x1243, 0, rub_DidDataOperBuf, UDS_itf_writeCheck_4A04, UDS_itf_writeDid_4A04 }, /* BTA_Adv_Interval */
    { 0x4A05u, 2u  , 0x1243, 0, rub_DidDataOperBuf, UDS_itf_writeCheck_4A05, UDS_itf_writeDid_4A05 }, /* BTA_Conn_Interval */
    { 0x4A06u, 2u  , 0x1243, 0, rub_DidDataOperBuf, UDS_itf_writeCheck_4A06, UDS_itf_writeDid_4A06 }, /* BTA_Slave_Latency */
    { 0x4A1Fu, 3u  , 0x1243, 0, rub_DidDataOperBuf, UDS_itf_writeCheck_4A1F, UDS_itf_writeDid_4A1F }, /* NFC_ECP_TCI */
    {UDS_C_DID_END_OF_TBL_FFFF, 0x00u, 0x0000u, 0x00u, NULL, NULL, NULL}/*This Line indicate the end of the data table */
};
#endif /* #ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER */
