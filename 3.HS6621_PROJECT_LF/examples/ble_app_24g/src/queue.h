/**
  ******************************************************************************
  * @file:      sys_queue.h
  * @author:    Cat
  * @version:   V1.0
  * @date:      2018-1-18
  * @brief:     queue
  * @attention:
  ******************************************************************************
  */


#ifndef __SYS_QUEUE_H
#define __SYS_QUEUE_H

#include <stdint.h>
//#include "global.h"
#ifndef     NULL
#define     NULL            (0)
#endif
#define     TRUE            (1)
#define     FALSE           (0)

#define ENABLE_ALL_IRQ()    __enable_irq()     /* disable all interrupt */
#define DISABLE_ALL_IRQ()   __disable_irq()    /* enable all interrupt */


extern uint8_t queue_init(uint16_t *front, uint16_t *rear, uint8_t *pbase, uint16_t Len);
extern uint8_t queue_full(uint16_t *front, uint16_t *rear, uint8_t *pbase, uint16_t Len);
extern uint8_t queue_empty(uint16_t *front, uint16_t *rear, uint8_t *pbase, uint16_t Len);
extern uint8_t queue_in(uint16_t *front, uint16_t *rear, uint8_t *pbase, uint16_t Len, uint8_t *pdata);
extern uint8_t queue_out(uint16_t *front, uint16_t *rear, uint8_t *pbase, uint16_t Len, uint8_t *pdata);

//queue init.
#define QUEUE_INIT(q)       queue_init((uint16_t *) &(q.front),(uint16_t *) &(q.rear),(uint8_t *) &(q.pbase[0]),(uint16_t) (sizeof(q.pbase)))
//queue full?
#define QUEUE_FULL(q)       queue_full((uint16_t *) &(q.front),(uint16_t *) &(q.rear),(uint8_t *) &(q.pbase[0]),(uint16_t) (sizeof(q.pbase)))
//queue empty?
#define QUEUE_EMPTY(q)      queue_empty((uint16_t *) &(q.front),(uint16_t *) &(q.rear),(uint8_t *) &(q.pbase[0]),(uint16_t) (sizeof(q.pbase)))
//put pdata in queue
#define QUEUE_IN(q, pdata)  queue_in((uint16_t *) &(q.front),(uint16_t *) &(q.rear),(uint8_t *) &(q.pbase[0]),(uint16_t) (sizeof(q.pbase)),(uint8_t *) &pdata)
//get byte from queue
#define QUEUE_OUT(q, pdata) queue_out((uint16_t *) &(q.front),(uint16_t *) &(q.rear),(uint8_t *) &(q.pbase[0]),(uint16_t) (sizeof(q.pbase)),(uint8_t *) &pdata)

//8 Bytes Queue Struct
typedef struct __attribute__ ((__packed__)) queue8 {
    uint16_t    front;
    uint16_t    rear;
    uint8_t     pbase[8 + 1];
} QUEUE8_S;

//16 Bytes Queue Struct
typedef struct __attribute__ ((__packed__)) queue16 {
    uint16_t    front;
    uint16_t    rear;
    uint8_t     pbase[16 + 1];
} QUEUE16_S;

//32 Bytes Queue Struct
typedef struct __attribute__ ((__packed__)) queue32 {
    uint16_t    front;
    uint16_t    rear;
    uint8_t     pbase[32 + 1];
} QUEUE32_S;

//64 Bytes Queue Struct
typedef struct __attribute__ ((__packed__)) queue64 {
    uint16_t    front;
    uint16_t    rear;
    uint8_t     pbase[64 + 1];
} QUEUE64_S;

//128 Bytes Queue Struct
typedef struct __attribute__ ((__packed__)) queue128 {
    uint16_t    front;
    uint16_t    rear;
    uint8_t     pbase[128 + 1];
} QUEUE128_S;

//256 Bytes Queue Struct
typedef struct __attribute__ ((__packed__)) queue256 {
    uint16_t    front;
    uint16_t    rear;
    uint8_t     pbase[256 + 1];
} QUEUE256_S;

typedef struct __attribute__ ((__packed__)) queue1024 {
    uint16_t    front;
    uint16_t    rear;
    uint8_t     pbase[1024 + 1];
} QUEUE1024_S;

typedef struct __attribute__ ((__packed__)) queue2048 {
    uint16_t    front;
    uint16_t    rear;
    uint8_t     pbase[2048 + 1];
} QUEUE2048_S;


#endif /* __SYS_QUEUE_H */

