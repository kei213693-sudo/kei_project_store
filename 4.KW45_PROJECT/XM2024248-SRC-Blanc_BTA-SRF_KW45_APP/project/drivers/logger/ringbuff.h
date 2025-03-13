/*
***************************************************************************************
* FileName: ringbuff.h
* Description: ���λ���������ͷ�ļ�
* Deta: 2020-11-13   ZXE
***************************************************************************************
*/

#ifndef RING_BUFF_H__
#define RING_BUFF_H__

/* ���λ��������ݽṹ */
typedef struct RING_BUFF_STRUCT
{
	uint16_t capacity;
    uint16_t head;
    uint16_t tail;
	uint16_t length;
	uint8_t *pBuffer;
}RING_BUFF_STRUCT_TYPEDEF;

/* ���λ�������ʼ�� */
void RingBuff_Init(RING_BUFF_STRUCT_TYPEDEF *pRingBuff, uint8_t *pBuff, uint16_t size);

/* �����λ�����д��uint8_t���͵����� */
uint8_t RingBuff_Write(RING_BUFF_STRUCT_TYPEDEF *pRingBuff, uint8_t data);

/* �����λ�����д��ָ�����ȵ����� */
uint16_t RingBuff_WriteData(RING_BUFF_STRUCT_TYPEDEF *pRingBuff, uint8_t *pdata, uint16_t length);

/* �ӻ��λ�������ȡһ��uint8_t���͵����� */
uint8_t RingBuff_Read(RING_BUFF_STRUCT_TYPEDEF *pRingBuff, uint8_t *rData);

/* �ӻ��λ�������ȡָ�����ȵ����� */
uint16_t RingBuff_ReadData(RING_BUFF_STRUCT_TYPEDEF *pRingBuff, uint8_t *rData, uint16_t length);

/* ��ȡ���λ������е����ݳ��� */
uint16_t RingBuff_GetDataLength(RING_BUFF_STRUCT_TYPEDEF *pRingBuff);

/*  ��ȡ���λ�������ʣ��ռ�Ĵ�С */
uint16_t RingBuff_GetRemainSize(RING_BUFF_STRUCT_TYPEDEF *pRingBuff);


#endif
