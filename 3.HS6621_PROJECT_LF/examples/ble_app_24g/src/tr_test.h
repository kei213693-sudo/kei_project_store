//------------------------------------------------------------------------------------
// app_ble.h
//------------------------------------------------------------------------------------
// Copyright 2019, HunterSun Electronics Co., Ltd.
// crazy_code@yeah.net
// 2019-07-18
//
// Program Description:
//
//
//
#ifndef USER_TR_TEST_H
#define USER_TR_TEST_H

/* BLE help */
#define CLI_Cmd_TRTEST \
    "[trTest]\r\n"\
    " * tr 0: interrupt test\r\n"\
    " * tr 1: reciver test (no ack, nocrc, spl)\r\n"\
        "   -> HS6206 FPGA:  CMD> rf 09 08 \r\n"\
    " * tr 2: send test (no ack, nocrc, spl)\r\n"\
        "   -> HS6206 FPGA:  CMD> rf 08 08 \r\n"\
    " * tr 3: reciver test (ack, crc, dpl)\r\n"\
        "   -> HS6206 FPGA:  CMD> rf 8e 08 \r\n"\
    " * tr 4: send test (ack, crc, dpl)\r\n"\
        "   -> HS6206 FPGA:  CMD> rf 8f 08 \r\n"\
    "\r\n"


extern uint8_t CLI_TRTest(void *para, uint8_t len);


#endif
