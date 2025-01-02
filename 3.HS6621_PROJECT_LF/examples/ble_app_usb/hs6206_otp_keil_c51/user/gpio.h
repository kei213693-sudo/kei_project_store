//------------------------------------------------------------------------------------
// gpio.h
//------------------------------------------------------------------------------------
// Copyright (c) 2013, HunterSun Electronics Co., Ltd.
// Huiqing, Zheng
// 2013-12-23
//
// Program Description: Header file for GPIO.
//
//
//

#ifndef __GPIO_H__
#define __GPIO_H__

/*--------------------CFG_DFEN_P0, CFG_DFEN_P1, CFG_DFEN_P2-----------------------*/                
#define CFG_EN 1
#define CFG_DIS 0

#define GPIO_IN 1
#define GPIO_OUT 0

/* basic macro definations */
#define SET_GPIO_PIN_CFG(REG_NAME, PORT, X, value)      (REG_NAME##PORT |= (value) << X) /* set gpio cfg of port PORT pin X as value, register name is REG_NAME<PORT> */
#define CLR_GPIO_PIN_CFG(REG_NAME, PORT, X)             (REG_NAME##PORT &= ~(1 << X)) /* clear gpio cfg of port PORT pin X, register name is REG_NAME<PORT> */
#define GET_GPIO_PIN_CFG(REG_NAME, PORT, X)             ((REG_NAME##PORT >> X) & 0x1) /* get gpio cfg of port PORT pin X, register name is REG_NAME<PORT> */
#define SET_GPIO_PIN_CFG_INST(REG_NAME, PORT, X, value) SET_GPIO_PIN_CFG(REG_NAME, PORT, X, value) /* set gpio cfg of port PORT pin X as value, register name is REG_NAME<PORT> */
#define CLR_GPIO_PIN_CFG_INST(REG_NAME, PORT, X)        CLR_GPIO_PIN_CFG(REG_NAME, PORT, X) /* clear gpio cfg of port PORT pin X, register name is REG_NAME<PORT> */
#define GET_GPIO_PIN_CFG_INST(REG_NAME, PORT, X)        GET_GPIO_PIN_CFG(REG_NAME, PORT, X) /* get gpio cfg of port PORT pin X, register name is REG_NAME<PORT> */

#define SET_GPIO_PIN_CFG2(REG_NAME_P, REG_NAME_S, PORT, X, value)    (REG_NAME_P##PORT##REG_NAME_S |= (value) << X) /* set gpio cfg of port PORT pin X as value, register name is REG_NAME_P<PORT>REG_NAME_S */
#define CLR_GPIO_PIN_CFG2(REG_NAME_P, REG_NAME_S, PORT, X)           (REG_NAME_P##PORT##REG_NAME_S &= ~(1 << X)) /* clear gpio cfg of port PORT pin X, register name is REG_NAME_P<PORT>REG_NAME_S */
#define GET_GPIO_PIN_CFG2(REG_NAME_P, REG_NAME_S, PORT, X)    ((REG_NAME_P##PORT##REG_NAME_S >> X) & 0x1) /* get gpio cfg of port PORT pin X, register name is REG_NAME_P<PORT>REG_NAME_S */

/*--------------------CFG_DFEN_P0/1/2-----------------------*/                
/* Control digital filter registers */
#define SET_GPIO_CFG_DFEN_PIN(PORT, X, CFG_SET)         (SET_GPIO_PIN_CFG(CFG_DFEN_P, PORT, X, CFG_SET))  /* set gpio CFG_DFEN, port is PORT, pin is X, cfg value is CFG_SET */
#define CLR_GPIO_CFG_DFEN_PIN(PORT, X)                  (CLR_GPIO_PIN_CFG(CFG_DFEN_P, PORT, X))  /* clear gpio CFG_DFEN, port is PORT, pin is X */
#define GET_GPIO_CFG_DFEN_PIN(PORT, X)                  (GET_GPIO_PIN_CFG(CFG_DFEN_P, PORT, X))  /* et gpio CFG_DFEN, port is PORT, pin is X */

/*--------------------P0/1/2DIR-----------------------*/            
/* Direction control registers */
#define SET_GPIO_DIR_PIN(PORT, X, GPIO_DIR)             (SET_GPIO_PIN_CFG2(P, DIR, PORT, X, GPIO_DIR))  /* set gpio direct register port PORT pin X */
#define CLR_GPIO_DIR_PIN(PORT, X)                       (CLR_GPIO_PIN_CFG2(P, DIR, PORT, X))  /* clear gpio direct register port PORT pin X */
#define GET_GPIO_DIR_PIN(PORT, X)                       (GET_GPIO_PIN_CFG2(P, DIR, PORT, X))  /* get gpio direct register port PORT pin X */

/*--------------------P0/1/2CON-----------------------*/            
/* input and output options registers */
#define GPIO_PCON_PINMODE         0x70
#define GPIO_PCON_PINMODE_SFT     5
#define GPIO_PCON_INOUT           0x10
#define GPIO_PCON_READADDR        0x08
#define GPIO_PCON_BITADDR         0x7
#define GPIO_PCON_BITADDR_SFT     0

#define GPIO_PCON_PINMODE_IN_BUFON_NO        0x00
#define GPIO_PCON_PINMODE_IN_BUFON_DOWN      0x01
#define GPIO_PCON_PINMODE_IN_BUFON_UP_30K    0x02
#define GPIO_PCON_PINMODE_IN_BUFON_UP_150K   0x03
#define GPIO_PCON_PINMODE_IN_BUFOFF          0x07
#define GPIO_PCON_PINMODE_OUT_NORM           0x00
#define GPIO_PCON_PINMODE_OUT_HIGH           0x03

#define GPIO_WRITE_REG(PORT, pinmode, inout, addr)  (P##PORT##CON = (pinmode << GPIO_PCON_PINMODE_SFT) | (inout * GPIO_PCON_INOUT) | (addr << GPIO_PCON_BITADDR_SFT))
#define GPIO_READ_REG(PORT, inout, addr)            (P##PORT##CON = (inout * GPIO_PCON_INOUT) | GPIO_PCON_READADDR | (addr << GPIO_PCON_BITADDR_SFT))
#define GET_PCON_MODE(PORT)                         ((P##PORT##CON & GPIO_PCON_PINMODE) >> GPIO_PCON_PINMODE_SFT)

/* initial values of GPIO registers */
#define CFG_DFEN_P0_INIT_VALUE           0
#define CFG_DFEN_P1_INIT_VALUE           0
#define CFG_DFEN_P2_INIT_VALUE           0
#define DBGSEL0_INIT_VALUE               0
#define DBGSEL1_INIT_VALUE               0
#define P0DIR_INIT_VALUE                 0xF7   // uart TXD
#define P1DIR_INIT_VALUE                 0xFF
#define P2DIR_INIT_VALUE                 0x03
#define P0CON_INIT_VALUE                 0
#define P1CON_INIT_VALUE                 0
#define P2CON_INIT_VALUE                 0

#define get_init_value(REG)         (REG != REG##_INIT_VALUE)

#endif  /* __GPIO_H__ */

/*-------------------------------------------End Of File---------------------------------------------*/
