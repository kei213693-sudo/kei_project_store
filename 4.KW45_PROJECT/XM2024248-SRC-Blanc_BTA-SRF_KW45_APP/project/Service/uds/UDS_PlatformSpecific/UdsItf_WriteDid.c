
#include "string.h"
#include "uds_confpriv.h"
#include "uds_api.h"
#include "E2Para.h"
/*DID结构体*/
uds_write_flash_data_un uds_write_data_un;

/* Vehicle Manufacture Hardware Part and Version Number */
uint8_t UDS_itf_writeCheck_F110(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    return UDS_POSITIVE_RESPONSE;
}

void UDS_itf_writeDid_F110(uint8_t *i_pDataBufPtr)
{
	  Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_write_data_un.write_buffer,DID_WRITE_DATA_LEN);
	  memcpy(uds_write_data_un.sig.f110_data, i_pDataBufPtr, DID_F110_LENGTH);
	  Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID, uds_write_data_un.write_buffer, DID_WRITE_DATA_LEN);
}

/* ECU Manufacture date */
uint8_t UDS_itf_writeCheck_F18B(uint8_t *i_pDataBufPtr)
{
	uint8_t year = 0;
	uint8_t month = 0;
	uint8_t days = 0;
	year = i_pDataBufPtr[0];
	month = i_pDataBufPtr[1];
	days = i_pDataBufPtr[2];
	if(((year >= 0) && (year <= 0x99)) && ((month >= 0x1) && (month <= 0x12)) &&
			((days >= 0x1) && (days <= 0x31)))
	{
    return UDS_POSITIVE_RESPONSE;
	}
	else
	{
		return UDS_NRC_REQUESTOUTOFRANGE;
	}
}

void UDS_itf_writeDid_F18B(uint8_t *i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_write_data_un.write_buffer,DID_WRITE_DATA_LEN);
    memcpy(uds_write_data_un.sig.f18B_data, i_pDataBufPtr, DID_F18B_LENGTH);
    Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID, uds_write_data_un.write_buffer, DID_WRITE_DATA_LEN);
}

/* Write Fingerprint */
uint8_t UDS_itf_writeCheck_F15A(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    return UDS_NRC_REQUESTOUTOFRANGE;
}

void UDS_itf_writeDid_F15A(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
}

/* Vehicle Identification Number */
uint8_t UDS_itf_writeCheck_F190(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_write_data_un.write_buffer,DID_WRITE_DATA_LEN);
    if(0x55 == uds_write_data_un.sig.f190_write_flag)
    {
    	return UDS_NRC_CONDITIONSNOTCORRECT;
    }
    else
    {
    return UDS_POSITIVE_RESPONSE;
    }
}

void UDS_itf_writeDid_F190(uint8_t *i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_write_data_un.write_buffer,DID_WRITE_DATA_LEN);
	if( uds_write_data_un.sig.f190_write_flag != 0x55)
	{
		memcpy(uds_write_data_un.sig.f190_data, i_pDataBufPtr, DID_F190_LENGTH);
		uds_write_data_un.sig.f190_write_flag = 0x55;
		Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID, uds_write_data_un.write_buffer, DID_WRITE_DATA_LEN);
	}
	else
	{
	}

}

/* Vehicle Manufacture Hardware Baseline */
uint8_t UDS_itf_writeCheck_F111(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    return UDS_POSITIVE_RESPONSE;
}

void UDS_itf_writeDid_F111(uint8_t *i_pDataBufPtr)
{
	/*读取flash内容*/
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_write_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(uds_write_data_un.sig.f111_data, i_pDataBufPtr, DID_F111_LENGTH);
	Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID, uds_write_data_un.write_buffer, DID_WRITE_DATA_LEN);
}

/* Serial number */
uint8_t UDS_itf_writeCheck_F18C(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    return UDS_POSITIVE_RESPONSE;
}

void UDS_itf_writeDid_F18C(uint8_t *i_pDataBufPtr)
{
	/*读取flash内容*/
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_write_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(uds_write_data_un.sig.f18C_data, i_pDataBufPtr, DID_F18C_LENGTH);
	Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID, uds_write_data_un.write_buffer, DID_WRITE_DATA_LEN);
}

/* System Supplier Hardware Version Number */
uint8_t UDS_itf_writeCheck_F193(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    return UDS_POSITIVE_RESPONSE;
}

void UDS_itf_writeDid_F193(uint8_t *i_pDataBufPtr)
{
   	/*读取flash内容*/
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_write_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(uds_write_data_un.sig.f193_data, i_pDataBufPtr, DID_F193_LENGTH);
	Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID, uds_write_data_un.write_buffer, DID_WRITE_DATA_LEN);
}

/* Vehicle id */
uint8_t UDS_itf_writeCheck_4A00(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    return UDS_POSITIVE_RESPONSE;
}

/* Vehicle id */
uint8_t UDS_itf_writeCheck_4A59(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    return UDS_POSITIVE_RESPONSE;
}


void UDS_itf_writeDid_4A00(uint8_t *i_pDataBufPtr)
{
	/*读取flash内容*/
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_write_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(uds_write_data_un.sig._4a00_data, i_pDataBufPtr, DID_4A00_LENGTH);
	Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID, uds_write_data_un.write_buffer, DID_WRITE_DATA_LEN);
}

void UDS_itf_writeDid_4A59(uint8_t *i_pDataBufPtr)
{
	canlog_Eflag = *i_pDataBufPtr;
}


/* Keyfob device id */
uint8_t UDS_itf_writeCheck_4A01(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    return UDS_POSITIVE_RESPONSE;
}

void UDS_itf_writeDid_4A01(uint8_t *i_pDataBufPtr)
{
	/*读取flash内容*/
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_write_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(uds_write_data_un.sig._4a01_data, i_pDataBufPtr, DID_4A01_LENGTH);
	Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID, uds_write_data_un.write_buffer, DID_WRITE_DATA_LEN);
}

/* BTA_Adv_Interval */
uint8_t UDS_itf_writeCheck_4A04(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    return UDS_POSITIVE_RESPONSE;
}

void UDS_itf_writeDid_4A04(uint8_t *i_pDataBufPtr)
{
	/*读取flash内容*/
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_write_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(uds_write_data_un.sig._4a04_data, i_pDataBufPtr, DID_4A04_LENGTH);
	Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID, uds_write_data_un.write_buffer, DID_WRITE_DATA_LEN);
}

/* BTA_Conn_Interval */
uint8_t UDS_itf_writeCheck_4A05(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    return UDS_POSITIVE_RESPONSE;
}

void UDS_itf_writeDid_4A05(uint8_t *i_pDataBufPtr)
{
	/*读取flash内容*/
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_write_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(uds_write_data_un.sig._4a05_data, i_pDataBufPtr, DID_4A05_LENGTH);
	Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID, uds_write_data_un.write_buffer, DID_WRITE_DATA_LEN);
}

/* BTA_Slave_Latency */
uint8_t UDS_itf_writeCheck_4A06(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    return UDS_POSITIVE_RESPONSE;
}

void UDS_itf_writeDid_4A06(uint8_t *i_pDataBufPtr)
{
	/*读取flash内容*/
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_write_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(uds_write_data_un.sig._4a06_data, i_pDataBufPtr, DID_4A06_LENGTH);
	Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID, uds_write_data_un.write_buffer, DID_WRITE_DATA_LEN);
}

/* NFC_ECP_TCI */
uint8_t UDS_itf_writeCheck_4A1F(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    return UDS_POSITIVE_RESPONSE;
}

void UDS_itf_writeDid_4A1F(uint8_t *i_pDataBufPtr)
{
	/*读取flash内容*/
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_write_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(uds_write_data_un.sig._4a1F_data, i_pDataBufPtr, DID_4A1F_LENGTH);
	Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID, uds_write_data_un.write_buffer, DID_WRITE_DATA_LEN);
}
/* Turn on Bluetooth broadcasting */
uint8_t UDS_itf_writeCheck_4A56(uint8_t *i_pDataBufPtr)
{
    (void) i_pDataBufPtr;
    return UDS_POSITIVE_RESPONSE;
}

void UDS_itf_writeDid_4A56(uint8_t *i_pDataBufPtr)
{
	uint8_t ble_state = 0;
	ble_state = *i_pDataBufPtr;
	UdsApi_set_BleTurnOnState(ble_state);
}
