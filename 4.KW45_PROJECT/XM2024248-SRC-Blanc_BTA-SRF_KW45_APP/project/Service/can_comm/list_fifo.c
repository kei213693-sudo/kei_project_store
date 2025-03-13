/*
***************************************************************************************
* FileName: list_fifo.c
* Description:
* Deta:
***************************************************************************************
*/
#include "list_fifo.h"

void ListFifo_Init(LIST_FIFO_STRUCT_TYPEDEF **ListHead,LIST_FIFO_STRUCT_TYPEDEF *ListBuff,uint8_t num)
{
    uint8_t i;

    if((ListHead == 0)||(ListBuff == 0)||(num == 0U))
    {
        ;;;
    }
    else
    {
        for(i=0;i<num;i++)
        {
            (ListBuff+i)->index   = i;
            (ListBuff+i)->fullfg  = 0;

            (ListBuff+i)->prve    = 0;
            (ListBuff+i)->next    = 0;

            (ListBuff+i)->para    = 0;

        }
        *ListHead = ListBuff;

        for(i=0;i<(num-1U);i++)
        {
            (ListBuff+i)->next    = ListBuff+i+1;
            (ListBuff+i+1)->prve  = ListBuff+i;
        }
    }
}

void ListFifo_PutHeadToEnd(LIST_FIFO_STRUCT_TYPEDEF **head)
{
    LIST_FIFO_STRUCT_TYPEDEF *end;
    LIST_FIFO_STRUCT_TYPEDEF *item;

    if((head == 0)||(*head == 0))
    {
        ;;;
    }
    else
    {
        item = *head;
        end = item;
        while(end->next!=0) {end = end->next; }
		if(end == item)
		{
	        item->fullfg  = 0;
		}
		else
		{
	        item->next->prve = 0;
	        *head = item->next;

	        item->prve    = end;
	        item->next    = 0;
	        item->fullfg  = 0;
	        end->next     = item;
		}
    }
}

void* ListFifo_GetPara(LIST_FIFO_STRUCT_TYPEDEF *head)
{
    void* para = 0;

    if(head == 0)
    {
        para = 0;
    }
    else
    {
        if(head->fullfg==1U) {para = head->para;}
    }

    return para;
}

LIST_FIFO_STRUCT_TYPEDEF *ListFifo_GetEmpty(LIST_FIFO_STRUCT_TYPEDEF *head)
{
    if(head == 0)
    {
        ;;;
    }
    else
    {
        while(head->fullfg==1U)
        {
            head = head->next;
            if(head==0){break;}
        }
    }

    return head;
}

void ListFifo_ClearAll(LIST_FIFO_STRUCT_TYPEDEF **head)
{
    LIST_FIFO_STRUCT_TYPEDEF *item;

    if((head == 0)||(*head == 0))
    {
        ;;;
    }
    else
    {
        item = *head;
        do
        {
            item->fullfg = 0;
            item = item->next;
        }
        while(item!=0);
    }
}











