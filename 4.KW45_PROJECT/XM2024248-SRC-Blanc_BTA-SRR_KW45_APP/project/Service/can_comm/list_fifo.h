/*
***************************************************************************************
* FileName: list_fifo.h
* Description:
* Deta:
***************************************************************************************
*/

#ifndef LIST_FIFO_H__
#define LIST_FIFO_H__

#include <stdint.h>

typedef struct LIST_FIFO_STRUCT
{
    uint8_t index;
    uint8_t fullfg;

    struct LIST_FIFO_STRUCT *prve;
    struct LIST_FIFO_STRUCT *next;

    void* para;
}LIST_FIFO_STRUCT_TYPEDEF;


void ListFifo_Init(LIST_FIFO_STRUCT_TYPEDEF **ListHead,LIST_FIFO_STRUCT_TYPEDEF *ListBuff,uint8_t num);

void ListFifo_PutHeadToEnd(LIST_FIFO_STRUCT_TYPEDEF **head);

void* ListFifo_GetPara(LIST_FIFO_STRUCT_TYPEDEF *head);

LIST_FIFO_STRUCT_TYPEDEF *ListFifo_GetEmpty(LIST_FIFO_STRUCT_TYPEDEF *head);

void ListFifo_ClearAll(LIST_FIFO_STRUCT_TYPEDEF **head);

#endif









