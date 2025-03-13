/*
***************************************************************************************
* FileName: ringbuff.c
* Description: ���λ���������Դ�ļ�
* Deta: 2020-11-13   ZXE
***************************************************************************************
*/

#include "fsl_os_abstraction.h"
#include "ringbuff.h"

/*
*****************************************************************
*name��RingBuff_Init
*descrip�����λ�������ʼ��
*params�� pRingBuff - ���λ�����ָ�룬 pBuff - ��������ַ�� size - ��������С
*return�� ��
*****************************************************************
*/
void RingBuff_Init(RING_BUFF_STRUCT_TYPEDEF *pRingBuff, uint8_t *pBuff, uint16_t size)
{
	pRingBuff->capacity = size;
	pRingBuff->head = 0;
	pRingBuff->tail = 0;
	pRingBuff->length = 0;
	pRingBuff->pBuffer = pBuff;
}

/*
*****************************************************************
*name: RingBuff_Write
*descrip: �����λ�����д��uint8_t���͵�����
*params��pRingBuff - ���λ�����ָ�룬 data - ��Ҫд�������
*return��FLASE:���λ�����������д��ʧ��; TRUE:д��ɹ�
*****************************************************************
*/
uint8_t RingBuff_Write(RING_BUFF_STRUCT_TYPEDEF *pRingBuff, uint8_t data)
{
	if(pRingBuff->length >= pRingBuff->capacity)	//�жϻ������Ƿ�����
	{
		return -1;
	}
	pRingBuff->pBuffer[pRingBuff->tail] = data;
	pRingBuff->tail = (pRingBuff->tail + 1) % pRingBuff->capacity;	//��ֹԽ��Ƿ�����
	pRingBuff->length ++;
	return 0;
}

/*
*****************************************************************
*name: RingBuff_WriteData
*descrip: �����λ�����д��ָ�����ȵ�����
*params��pRingBuff - ���λ�����ָ�룬 data - ��Ҫд���������ʼ��ַ�� length - д������ݳ���
*return��д�뻷�λ������е����ݳ���
*****************************************************************
*/
uint16_t RingBuff_WriteData(RING_BUFF_STRUCT_TYPEDEF *pRingBuff, uint8_t *pdata, uint16_t length)
{
	uint16_t writelen = 0;
	while((pRingBuff->length < pRingBuff->capacity) && (writelen < length)) //������δ�� ���� ��д�����ݼ���С��Ҫд�����ݳ���
	{
		pRingBuff->pBuffer[pRingBuff->tail] = *pdata;
		pRingBuff->tail = (pRingBuff->tail + 1) % pRingBuff->capacity;	//��ֹԽ��Ƿ�����
		pRingBuff->length ++;

		pdata ++;    //Դ���ݵ�ַָ������
		writelen ++; //д�뻺�������ݼ���
	}
	return writelen; //����д��ĸ���
}

/*
*****************************************************************
*name: RingBuff_Read
*descrip: �ӻ��λ�������ȡһ��uint8_t���͵�����
*params��pRingBuff - ���λ�����ָ�룬 *rData - ���ڱ����ȡ������
*return: FLASE:���λ�����û�����ݣ���ȡʧ��; TRUE:��ȡ�ɹ�
*****************************************************************
*/
uint8_t RingBuff_Read(RING_BUFF_STRUCT_TYPEDEF *pRingBuff, uint8_t *rData)
{
	if(pRingBuff->length == 0) //�жϿ�
	{
		return -1;
	}
	*rData = pRingBuff->pBuffer[pRingBuff->head]; //�Ƚ��ȳ�FIFO���ӻ�����ͷ��
	pRingBuff->head = (pRingBuff->head + 1) % pRingBuff->capacity;	//��ֹԽ��Ƿ�����
	pRingBuff->length --;
	return 0;
}

/*
*****************************************************************
*name: RingBuff_ReadData
*descrip: �ӻ��λ�������ȡָ�����ȵ�����
*params��pRingBuff - ���λ�����ָ�룬 *rData - ���ڱ����ȡ��������ʼ��ַ�� length - ��ȡ�����ݳ���
*return: �ӻ��λ�������ȡ�������ݳ���
*****************************************************************
*/
uint16_t RingBuff_ReadData(RING_BUFF_STRUCT_TYPEDEF *pRingBuff, uint8_t *rData, uint16_t length)
{
	uint16_t readlen = 0;
	while((pRingBuff->length > 0) && (readlen < length)) //������������ ���� �Ѷ�ȡ���ݼ���С��Ҫ��ȡ���ݳ���
	{
		*rData = pRingBuff->pBuffer[pRingBuff->head]; //�Ƚ��ȳ�FIFO���ӻ�����ͷ��
		if(*rData == 0)
		{
			break;
		}
		pRingBuff->head = (pRingBuff->head + 1) % pRingBuff->capacity;	//��ֹԽ��Ƿ�����
		pRingBuff->length --;

		rData ++;	//Ŀ�����ݵ�ַָ������
		readlen ++; //��ȡ���������ݼ���
	}
	return readlen; //���ض����ĸ���
}

/*
*****************************************************************
*name: RingBuff_GetDataLength
*descrip: ��ȡ���λ������е����ݳ���
*params��pRingBuff - ���λ�����ָ��
*return: �������е����ݳ���
*****************************************************************
*/
uint16_t RingBuff_GetDataLength(RING_BUFF_STRUCT_TYPEDEF *pRingBuff)
{
	return pRingBuff->length;
}

/*
*****************************************************************
*name: RingBuff_GetRemainSize
*descrip: ��ȡ���λ�������ʣ��ռ�Ĵ�С
*params��pRingBuff - ���λ�����ָ��
*return: ��������ʣ��ռ�Ĵ�С
*****************************************************************
*/
uint16_t RingBuff_GetRemainSize(RING_BUFF_STRUCT_TYPEDEF *pRingBuff)
{
	return (pRingBuff->capacity - pRingBuff->length);
}
