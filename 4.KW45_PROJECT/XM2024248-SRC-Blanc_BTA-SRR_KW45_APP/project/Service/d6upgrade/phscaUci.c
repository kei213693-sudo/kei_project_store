/*
 (c) NXP B.V. 2009-2021. All rights reserved.

 Disclaimer
 1. The NXP Software/Source Code is provided to Licensee "AS IS" without any
 warranties of any kind. NXP makes no warranties to Licensee and shall not
 indemnify Licensee or hold it harmless for any reason related to the NXP
 Software/Source Code or otherwise be liable to the NXP customer. The NXP
 customer acknowledges and agrees that the NXP Software/Source Code is
 provided AS-IS and accepts all risks of utilizing the NXP Software under
 the conditions set forth according to this disclaimer.

 2. NXP EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT OF INTELLECTUAL PROPERTY
 RIGHTS. NXP SHALL HAVE NO LIABILITY TO THE NXP CUSTOMER, OR ITS
 SUBSIDIARIES, AFFILIATES, OR ANY OTHER THIRD PARTY FOR ANY DAMAGES,
 INCLUDING WITHOUT LIMITATION, DAMAGES RESULTING OR ALLEGDED TO HAVE
 RESULTED FROM ANY DEFECT, ERROR OR OMMISSION IN THE NXP SOFTWARE/SOURCE
 CODE, THIRD PARTY APPLICATION SOFTWARE AND/OR DOCUMENTATION, OR AS A
 RESULT OF ANY INFRINGEMENT OF ANY INTELLECTUAL PROPERTY RIGHT OF ANY
 THIRD PARTY. IN NO EVENT SHALL NXP BE LIABLE FOR ANY INCIDENTAL,
 INDIRECT, SPECIAL, EXEMPLARY, PUNITIVE, OR CONSEQUENTIAL DAMAGES
 (INCLUDING LOST PROFITS) SUFFERED BY NXP CUSTOMER OR ITS SUBSIDIARIES,
 AFFILIATES, OR ANY OTHER THIRD PARTY ARISING OUT OF OR RELATED TO THE NXP
 SOFTWARE/SOURCE CODE EVEN IF NXP HAS BEEN ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGES.

 3. NXP reserves the right to make changes to the NXP Software/Sourcecode any
 time, also without informing customer.

 4. Licensee agrees to indemnify and hold harmless NXP and its affiliated
 companies from and against any claims, suits, losses, damages,
 liabilities, costs and expenses (including reasonable attorney's fees)
 resulting from Licensee's and/or Licensee customer's/licensee's use of the
 NXP Software/Source Code.

 */

/*
 *    @file: phscaUci.c
 *   @brief: UCI communication interface
 */

/* =============================================================================
 * External Includes
 * ========================================================================== */

/* =============================================================================
 * Internal Includes
 * ========================================================================== */
#define PHSCAUCI_EXTERN_GUARD
#include "phscaUci_Cfg.h"
#include "phscaUci.h"
#include <string.h>
#undef PHSCAUCI_EXTERN_GUARD

/* =============================================================================
 * Private Symbol Defines
 * ========================================================================== */
#define PHSCAUCI_u16_FRAME_BUFFER_SIZE                         (uint16_t)(1000u)

/* =============================================================================
 * Private Function-like Macros
 * ========================================================================== */

/* =============================================================================
 * Private Type Definitions
 * ========================================================================== */

/* =============================================================================
 * Private Function Prototypes
 * ========================================================================== */
static void phscaUci_RspNtfReceivedCallback(void);

/* =============================================================================
 * Private Module-wide Visible Variables
 * ========================================================================== */
static uint8_t m_u8arr_CommandBuffer[PHSCAUCI_u16_FRAME_BUFFER_SIZE];
static uint8_t m_u8arr_ResponseBuffer[PHSCAUCI_u16_FRAME_BUFFER_SIZE];
static phscaUci_pf_RspNtfReceivedCallback_t m_pf_RspNtfReceivedCallback = PHSCATYPES_pv_NULLPTR;

static uint8_t uciSta = 0;
const uint8_t intfRsp[] = {0x6Fu, 0x01u, 0x00u, 0x01u};

/* =============================================================================
 * Function Definitions
 * ========================================================================== */
void phscaUci_Init(const phscaUci_pf_RspNtfReceivedCallback_t pf_RspNtfReceivedCallback)
{
	m_pf_RspNtfReceivedCallback = pf_RspNtfReceivedCallback;
	phscaUci_InitDevice(phscaUci_RspNtfReceivedCallback);
}

static void phscaUci_RspNtfReceivedCallback(void)
{
	uint32_t u32_UciResponseLength = PHSCATYPES_u32_MIN_U32;
	phscaUci_en_MessageType_t en_MessageType = PHSCAUCI_MESSAGETYPE_COMMAND;

	if(m_pf_RspNtfReceivedCallback != PHSCATYPES_pv_NULLPTR)
	{
	    u32_UciResponseLength = phscaUci_GetResponse(m_u8arr_ResponseBuffer);

		en_MessageType = (phscaUci_en_MessageType_t)(PHSCAUCI_u8_READ_BYTE_UCI_MESSAGE_TYPE(m_u8arr_ResponseBuffer[0u]));
		m_pf_RspNtfReceivedCallback(en_MessageType, m_u8arr_ResponseBuffer[0u], m_u8arr_ResponseBuffer[1u], (uint32_t)(u32_UciResponseLength - PHSCAUCI_u8_UCI_HEADER_SIZE_BYTES), m_u8arr_ResponseBuffer);
	}
	else
	{
		/* Do nothing. */
	}
}

uint16_t crc16_xmodem(uint8_t *data_p, uint16_t length)
{
    uint16_t crc = 0; // 初始CRC值为0
    uint8_t i;

    while (length--) {
        crc ^= *data_p++ << 8; // 将数据字节与CRC的高8位进行异或

        for (i = 0; i < 8; ++i) {
            if (crc & 0x8000) // 如果最高位是1
                crc = (crc << 1) ^ 0x1021; // 左移并异或多项式
            else
                crc = crc << 1; // 左移
        }
    }

    return crc & 0xFFFF; // 返回低16位作为CRC值
}

uint8_t phscaUci_GetSta(void)
{
	return uciSta;
}

void phscaUci_SendCommand(const uint8_t u8_BytesToTransmit[], const uint32_t u32_DataLengthBytes)
{
	uint32_t u32_ByteLoopIndex = PHSCATYPES_u32_MIN_U32;
	bool b_IsResponseAvailable = PHSCATYPES_b_FALSE;
	uint16_t crc = 0u;
	do
	{
		b_IsResponseAvailable = phscaUci_IsResponseAvailable();
		if(b_IsResponseAvailable == PHSCATYPES_b_TRUE) /** 如果有缓存数据，先读出来 */
		{
			phscaUci_GetResponse(m_u8arr_ResponseBuffer);
		}
	} while (b_IsResponseAvailable);


	/* Copy input bytes to internal buffer */
	for(u32_ByteLoopIndex = PHSCATYPES_u8_MIN_U8; u32_ByteLoopIndex < (u32_DataLengthBytes + PHSCAUCI_u8_UCI_HEADER_SIZE_BYTES); u32_ByteLoopIndex++)
	{
		m_u8arr_CommandBuffer[u32_ByteLoopIndex] = u8_BytesToTransmit[u32_ByteLoopIndex];
	}
	/** 添加CRC */
	crc = crc16_xmodem(m_u8arr_CommandBuffer, u32_DataLengthBytes+PHSCAUCI_u8_UCI_HEADER_SIZE_BYTES);
	m_u8arr_CommandBuffer[u32_ByteLoopIndex] = (uint8_t)(crc&0xff);
	m_u8arr_CommandBuffer[u32_ByteLoopIndex+1] = (uint8_t)((crc>>8)&0xff);

	phscaUci_StartCommandTx();
	phscaUci_Transceive((u32_DataLengthBytes + PHSCAUCI_u8_UCI_HEADER_SIZE_BYTES + PHSCAUCI_u8_UCI_CRC_SIZE_BYTES), m_u8arr_CommandBuffer, m_u8arr_ResponseBuffer);
	phscaUci_StopCommandTx();
}

uint32_t phscaUci_GetResponse(uint8_t * const u8arr_ReceivedData)
{
  uint32_t u32_ByteLoopIndex = PHSCATYPES_u32_MIN_U32;
  bool b_IsResponseAvailable = PHSCATYPES_b_FALSE;
  uint32_t u32_UciResponsePayloadLength = PHSCATYPES_u32_MIN_U32;

  b_IsResponseAvailable = phscaUci_IsResponseAvailable();

  if(b_IsResponseAvailable == PHSCATYPES_b_TRUE)
  {
	  /* Get the UCI header (5 bytes = first bytes always zero + 4 bytes of standard UCI header) */
	  for(u32_ByteLoopIndex = PHSCATYPES_u32_MIN_U32; u32_ByteLoopIndex < (uint32_t)(PHSCAUCI_u8_UCI_HEADER_SIZE_BYTES); u32_ByteLoopIndex++)
	  {
		  m_u8arr_CommandBuffer[u32_ByteLoopIndex] = PHSCATYPES_u8_MIN_U8;
	  }

	  phscaUci_StartResponseTx();
	  phscaUci_Transceive((uint32_t)(PHSCAUCI_u8_UCI_HEADER_SIZE_BYTES), m_u8arr_CommandBuffer, m_u8arr_ResponseBuffer);

	  u32_UciResponsePayloadLength = (uint32_t)phscaTypes_ConvertU8toU16(m_u8arr_ResponseBuffer[PHSCAUCI_u8_UCI_PAYLOADLENGTH_BYTE_POS], m_u8arr_ResponseBuffer[PHSCAUCI_u8_UCI_PAYLOADLENGTH_HIGH_BYTE_POS]);

	  if((u32_UciResponsePayloadLength != PHSCATYPES_u32_MIN_U32) && (u32_UciResponsePayloadLength < PHSCAUCI_u16_FRAME_BUFFER_SIZE))
	  {
		  /* Get the payload according to the received response length from UCI header */
		  for(u32_ByteLoopIndex = PHSCATYPES_u32_MIN_U32; u32_ByteLoopIndex < (uint32_t)u32_UciResponsePayloadLength + PHSCAUCI_u8_CRC_SIZE_BYTES; u32_ByteLoopIndex++)
		  {
			  m_u8arr_CommandBuffer[u32_ByteLoopIndex] = PHSCATYPES_u8_MIN_U8;
		  }

		phscaUci_Transceive((uint32_t)u32_UciResponsePayloadLength + PHSCAUCI_u8_CRC_SIZE_BYTES, m_u8arr_CommandBuffer, m_u8arr_ResponseBuffer + (PHSCAUCI_u8_UCI_HEADER_SIZE_BYTES));
		phscaUci_StopResponseTx();
	  }
	  else
	  {
		  phscaUci_Transceive( PHSCAUCI_u8_CRC_SIZE_BYTES, m_u8arr_CommandBuffer, m_u8arr_ResponseBuffer + (PHSCAUCI_u8_UCI_HEADER_SIZE_BYTES));
			phscaUci_StopCs();
	  }

	  if(u8arr_ReceivedData != PHSCATYPES_pv_NULLPTR)
	  {
		  /* Copy complete response to application supplied buffer */
		  for(u32_ByteLoopIndex = PHSCATYPES_u16_MIN_U16; u32_ByteLoopIndex < (uint16_t)(PHSCAUCI_u8_UCI_HEADER_SIZE_BYTES + (uint16_t)u32_UciResponsePayloadLength + PHSCAUCI_u8_CRC_SIZE_BYTES); u32_ByteLoopIndex++)
		  {
			  u8arr_ReceivedData[u32_ByteLoopIndex] = m_u8arr_ResponseBuffer[u32_ByteLoopIndex];
		  }
			if (memcmp(intfRsp, m_u8arr_ResponseBuffer, 4) == 0)
			{
				uint8_t sta = m_u8arr_ResponseBuffer[4];
				if ((SWUP_STATE_ACTIVE == sta) || SWUP_STATE_TRANSFER == sta)
				{
					uciSta = sta;
				}
				else
				{
					uciSta = SWUP_STATE_ERROR;
				}
			}
	  }
	  else
	  {
		  /* Response not needed by caller. Do nothing and keep the response in m_u8arr_SpiResponseBuffer */
	  }

	  u32_UciResponsePayloadLength = (u32_UciResponsePayloadLength + PHSCAUCI_u8_UCI_HEADER_SIZE_BYTES + PHSCAUCI_u8_CRC_SIZE_BYTES);
  }
  else
  {
	  u32_UciResponsePayloadLength = PHSCATYPES_u32_MIN_U32;
  }

  return u32_UciResponsePayloadLength;
}
