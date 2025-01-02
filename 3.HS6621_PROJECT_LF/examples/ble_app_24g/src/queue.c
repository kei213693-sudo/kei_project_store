/**
  ******************************************************************************
  * @file:      sys_queue.c
  * @author:    Cat
  * @version:   V1.0
  * @date:      2018-1-18
  * @brief:     queue
  * @attention:
  ******************************************************************************
  */


#include "queue.h"


/**
 * @brief  queue_init
 * @param  front , rear , pbase , Len
 * @retval True
 */
uint8_t queue_init(uint16_t *front, uint16_t *rear, uint8_t *pbase, uint16_t Len)
{
    uint16_t index;

    for(index = 0; index < Len; index++)
    {
        pbase[index] = NULL;
    }

    *front = *rear = 0;
    return TRUE;
}


/**
 * @brief  queue_full
 * @param  front , rear , pbase , Len
 * @retval Result of Queue Operation as Enum
 */
uint8_t queue_full(uint16_t *front, uint16_t *rear, uint8_t *pbase, uint16_t Len)
{
    if((((*rear) + 1) % Len) == *front)
    {
        return TRUE;
    } else
    {
        return FALSE;
    }
}

/**
 * @brief  queue_empty
 * @param  front , rear , pbase , Len
 * @retval Result of Queue Operation as Enum
 */
uint8_t queue_empty(uint16_t *front, uint16_t *rear, uint8_t *pbase, uint16_t Len)
{
    if(*front == *rear)
    {
        return TRUE;
    } else
    {
        return FALSE;
    }
}


/**
 * @brief  queue_in
 * @param  front , rear , pbase , Len
 * @retval Result of Queue Operation as Enum
 */
uint8_t queue_in(uint16_t *front, uint16_t *rear, uint8_t *pbase, uint16_t Len, uint8_t *pdata)
{
    //DISABLE_ALL_IRQ();

    if(queue_full(front, rear, pbase, Len)) 
    {
        return FALSE;
    }

    pbase[*rear] = *pdata;
    *rear = ((*rear) + 1) % Len;

    //ENABLE_ALL_IRQ();

    return TRUE;
}


/**
 * @brief  queue_out
 * @param  front , rear , pbase , Len
 * @retval Result of Queue Operation as Enum
 */

uint8_t queue_out(uint16_t *front, uint16_t *rear, uint8_t *pbase, uint16_t Len, uint8_t *pdata)
{
    //DISABLE_ALL_IRQ();

    if(queue_empty(front, rear, pbase, Len))
    {
        return FALSE;
    }

    *pdata = pbase[*front];
    *front = ((*front) + 1) % Len;

    //ENABLE_ALL_IRQ();

    return TRUE;
}


