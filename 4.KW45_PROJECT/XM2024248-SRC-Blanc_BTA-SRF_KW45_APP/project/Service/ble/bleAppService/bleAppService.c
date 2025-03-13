/*
 * bleAppService.c
 *
 *  Created on: 2024年8月28日
 *      Author: F704012276_
 */

#include "signals_api.h"
#include "bleApp1_api.h"
#include "bleInstallCallback_api.h"
#include "bleTP_api.h"

void Init_BleService(void)
{
    Tp_Init();
}

void BleService_HandleMessage(void)
{
#ifdef SDK_OS_FREE_RTOS
    osa_event_flags_t event = 0U;
    (void)OSA_EventWait((osa_event_handle_t)mBleAppEvent,
                        osaEventFlagsAll_c,
                        FALSE,
                        gAppTaskWaitTimeout_ms_c ,
                        &event);
#endif /* SDK_OS_FREE_RTOS */

    /* Check for existing messages in queue */
    if (MSG_QueueGetHead(&mAppEventQueue) != NULL)
    {
        /* Pointer for storing the callback messages. */
        appMsgCallback_t *pMsgIn = MSG_QueueGetHead(&mAppEventQueue);

        if (pMsgIn != NULL)
        {
            /* Execute callback handler */
            if (pMsgIn->handler != NULL)
            {
                pMsgIn->handler(pMsgIn->param);
            }

            /* Messages must always be freed. */
            (void)MSG_Free(pMsgIn);
        }
    }

    /* Check for existing messages in queue */
    if (MSG_QueueGetHead(&mAppMessageQueue) != NULL)
    {
        /* Pointer for storing the callback messages. */
        appMsgCallback_t *pMsgIn = MSG_QueueGetHead(&mAppMessageQueue);

        if (pMsgIn != NULL)
        {
            /* Execute callback handler */
            if (pMsgIn->handler != NULL)
            {
                pMsgIn->handler(pMsgIn->param);
            }

            /* Messages must always be freed. */
            (void)MSG_Free(pMsgIn);
        }
    }

#ifdef SDK_OS_FREE_RTOS
    /* Signal the main_thread again if there are more messages pending */
    event = (MSG_QueueGetHead(&mAppEventQueue) != NULL) ? gAppEvtService_c : 0U;
    event |= (MSG_QueueGetHead(&mAppMessageQueue) != NULL) ? gAppEvtMsgService_c : 0U;
    if (event != 0U)
    {
    	(void)OSA_EventSet((osa_event_handle_t)mBleAppEvent, gAppEvtMsgService_c);
    }
#endif /* SDK_OS_FREE_RTOS */
}
