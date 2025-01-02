#ifndef __M_KEYS_H__
#define __M_KEYS_H__

#if (APP_CUSTOMER_ID == 0 || APP_CUSTOMER_ID == 1 || APP_CUSTOMER_ID == 3)
#ifdef CONFIG_HS6621
//Keyboard key code macro define
#define KB_KEY_POWER                     0x66 //S1
#define KB_KEY_VOICE                     0x3E //S2
#define KB_KEY_UP                        0x52 //S3
#define KB_KEY_DOWN                      0x51 //S7
#define KB_KEY_LEFT                      0x50 //S6
#define KB_KEY_RIGHT                     0x4F //S4
#define KB_KEY_OK                        0x28 //S5
#define KB_KEY_HOME                      0x4A //S10
#define KB_KEY_BACK                      0xF1 //S9
#define KB_KEY_MENU                      0x65 //S8
#define KB_KEY_VOLUME_UP                 0x80 //S11
#define KB_KEY_VOLUME_DOWN               0x81 //S12

//Keyboard key code matrix
#define KEY_MATRIX_KEYBOARD              \
{                                        \
    KB_KEY_UP,        KB_KEY_RIGHT,       KB_KEY_POWER, KB_KEY_VOICE, \
    KB_KEY_DOWN,      KB_KEY_MENU,        KB_KEY_OK,    KB_KEY_LEFT,  \
    KB_KEY_VOLUME_UP, KB_KEY_VOLUME_DOWN, KB_KEY_BACK,  KB_KEY_HOME,  \
}

//Consumer key code macro define
#define CON_KEY_POWER                    0x0030 //S1
#define CON_KEY_VOICE                    0x0221 //S2
#define CON_KEY_UP                       0x0042 //S3
#define CON_KEY_DOWN                     0x0043 //S7
#define CON_KEY_LEFT                     0x0044 //S6
#define CON_KEY_RIGHT                    0x0045 //S4
#define CON_KEY_OK                       0x0041 //S5
#define CON_KEY_HOME                     0x0223 //S10
#define CON_KEY_BACK                     0x0224 //S9
#define CON_KEY_MENU                     0x0040 //S8
#define CON_KEY_VOLUME_UP                0x00E9 //S11
#define CON_KEY_VOLUME_DOWN              0x00EA //S12

//Consumer key matrix
#define KEY_MATRIX_CONSUMER              \
{                                        \
    CON_KEY_UP,        CON_KEY_RIGHT,       CON_KEY_POWER, CON_KEY_VOICE, \
    CON_KEY_DOWN,      CON_KEY_MENU,        CON_KEY_OK,    CON_KEY_LEFT,  \
    CON_KEY_VOLUME_UP, CON_KEY_VOLUME_DOWN, CON_KEY_BACK,  CON_KEY_HOME,  \
}

//IR key code macro
#define IR_KEY_POWER                     0x66 //S1
#define IR_KEY_VOICE                     0x3E //S2
#define IR_KEY_UP                        0x52 //S3
#define IR_KEY_DOWN                      0x51 //S7
#define IR_KEY_LEFT                      0x50 //S6
#define IR_KEY_RIGHT                     0x4F //S4
#define IR_KEY_OK                        0x28 //S5
#define IR_KEY_HOME                      0x4A //S10
#define IR_KEY_BACK                      0xF1 //S9
#define IR_KEY_MENU                      0x65 //S8
#define IR_KEY_VOLUME_UP                 0x80 //S11
#define IR_KEY_VOLUME_DOWN               0x81 //S12

//IR key code matrix
#define KEY_MATRIX_IR                    \
{                                        \
    IR_KEY_UP,        IR_KEY_RIGHT,       IR_KEY_POWER, IR_KEY_VOICE, \
    IR_KEY_DOWN,      IR_KEY_MENU,        IR_KEY_OK,    IR_KEY_LEFT,  \
    IR_KEY_VOLUME_UP, IR_KEY_VOLUME_DOWN, IR_KEY_BACK,  IR_KEY_HOME,  \
}
#else
//Keyboard key code macro define
#define KB_KEY_POWER                     0x66 //S5
#define KB_KEY_VOICE                     0x3E //S6
#define KB_KEY_UP                        0x52 //S7
#define KB_KEY_LEFT                      0x50 //S10
#define KB_KEY_OK                        0x28 //S9
#define KB_KEY_RIGHT                     0x4F //S8
#define KB_KEY_DOWN                      0x51 //S11
#define KB_KEY_HOME                      0x4A //S2
#define KB_KEY_BACK                      0xF1 //S1
#define KB_KEY_MENU                      0x65 //S12
#define KB_KEY_VOLUME_UP                 0x80 //S3
#define KB_KEY_VOLUME_DOWN               0x81 //S4


//Keyboard key code matrix
#define KEY_MATRIX_KEYBOARD              \
{                                        \
    KB_KEY_BACK,  KB_KEY_HOME,  KB_KEY_VOLUME_UP, KB_KEY_VOLUME_DOWN, \
    KB_KEY_POWER, KB_KEY_VOICE, KB_KEY_UP,        KB_KEY_RIGHT,       \
    KB_KEY_OK,    KB_KEY_LEFT,  KB_KEY_DOWN,      KB_KEY_MENU,        \
}

//Consumer key code macro define
#define CON_KEY_POWER                    0x0030 //S5
#define CON_KEY_VOICE                    0x0221 //S6
#define CON_KEY_UP                       0x0042 //S7
#define CON_KEY_LEFT                     0x0044 //S10
#define CON_KEY_OK                       0x0041 //S9
#define CON_KEY_RIGHT                    0x0045 //S8
#define CON_KEY_DOWN                     0x0043 //S11
#define CON_KEY_HOME                     0x0223 //S2
#define CON_KEY_BACK                     0x0224 //S1
#define CON_KEY_MENU                     0x0040 //S12
#define CON_KEY_VOLUME_UP                0x0042 //S3
#define CON_KEY_VOLUME_DOWN              0x0045 //S4

//Consumer key matrix
#define KEY_MATRIX_CONSUMER              \
{                                        \
    CON_KEY_BACK,  CON_KEY_HOME,  CON_KEY_VOLUME_UP, CON_KEY_VOLUME_DOWN, \
    CON_KEY_POWER, CON_KEY_VOICE, CON_KEY_UP,        CON_KEY_RIGHT,       \
    CON_KEY_OK,    CON_KEY_LEFT,  CON_KEY_DOWN,      CON_KEY_MENU,        \
}

//IR key code macro
#define IR_KEY_POWER                     0x01 //S5
#define IR_KEY_VOICE                     0x02 //S6
#define IR_KEY_UP                        0x03 //S7
#define IR_KEY_LEFT                      0x04 //S10
#define IR_KEY_OK                        0x05 //S9
#define IR_KEY_RIGHT                     0x06 //S8
#define IR_KEY_DOWN                      0x07 //S11
#define IR_KEY_HOME                      0x08 //S2
#define IR_KEY_BACK                      0x09 //S1
#define IR_KEY_MENU                      0x0A //S12
#define IR_KEY_VOLUME_UP                 0x0B //S3
#define IR_KEY_VOLUME_DOWN               0x0C //S4


//IR key code matrix
#define KEY_MATRIX_IR                    \
{                                        \
    IR_KEY_BACK,  IR_KEY_HOME,  IR_KEY_VOLUME_UP, IR_KEY_VOLUME_DOWN, \
    IR_KEY_POWER, IR_KEY_VOICE, IR_KEY_UP,        IR_KEY_RIGHT,       \
    IR_KEY_OK,    IR_KEY_LEFT,  IR_KEY_DOWN,      IR_KEY_MENU,        \
}
#endif

#else

//Keyboard key code macro define
#define KB_KEY_POWER                     0x66 //S1
#define KB_KEY_VOICE                     0x3E //S2
#define KB_KEY_UP                        0x52 //S3
#define KB_KEY_DOWN                      0x51 //S7
#define KB_KEY_LEFT                      0x50 //S6
#define KB_KEY_RIGHT                     0x4F //S4
#define KB_KEY_OK                        0x28 //S5
#define KB_KEY_HOME                      0x4A //S10
#define KB_KEY_BACK                      0xF1 //S9
#define KB_KEY_MENU                      0x65 //S8
#define KB_KEY_VOLUME_UP                 0x80 //S11
#define KB_KEY_VOLUME_DOWN               0x81 //S12

//Keyboard key code matrix
#define KEY_MATRIX_KEYBOARD              \
{                                        \
    KB_KEY_UP,        KB_KEY_RIGHT,       KB_KEY_POWER, KB_KEY_VOICE, \
    KB_KEY_DOWN,      KB_KEY_MENU,        KB_KEY_OK,    KB_KEY_LEFT,  \
    KB_KEY_VOLUME_UP, KB_KEY_VOLUME_DOWN, KB_KEY_BACK,  KB_KEY_HOME,  \
}

//Consumer key code macro define
#define CON_KEY_POWER                    0x0030 //S1
#define CON_KEY_HOME                     0x0223 //S2
#define CON_KEY_MOUSE                    0xFFFF //S3 res
#define CON_KEY_MENU                     0xFFFF //S4 res
#define CON_KEY_MUTE                     0x00E2 //S5 
#define CON_KEY_UP                       0x0042 //S6
#define CON_KEY_PG_N                     0x004E //S7
#define CON_KEY_PG_P                     0x004B //S8
#define CON_KEY_LEFT                     0x0044 //S9
#define CON_KEY_VOL_P                    0x00E9 //S10
#define CON_KEY_DOWN                     0x0043 //S11
#define CON_KEY_SIGNAL                   0xFFFF //S12 res
#define CON_KEY_VOL_N                    0x00EA //S13
#define CON_KEY_VOICE                    0x0221 //S14
#define CON_KEY_RIGHT                    0x0045 //S15
#define CON_KEY_BACK                     0x0224 //S16
#define CON_KEY_OK                       0x0041 //S17

//Consumer key matrix
#define KEY_MATRIX_CONSUMER \
{                           \
       /*18*/         /*19*/          /*25*/         /*26*/         \
/*02*/ CON_KEY_MOUSE, CON_KEY_MENU,   CON_KEY_POWER, CON_KEY_HOME,  \
/*20*/ CON_KEY_RIGHT, CON_KEY_BACK,   CON_KEY_VOL_N, CON_KEY_VOICE, \
/*21*/ 0xFFFF,        0xFFF,          CON_KEY_OK,    0xFFFF,        \
/*22*/ CON_KEY_PG_N,  CON_KEY_PG_P,   CON_KEY_MUTE,  CON_KEY_UP,    \
/*24*/ CON_KEY_DOWN,  CON_KEY_SIGNAL, CON_KEY_LEFT,  CON_KEY_VOL_P, \
}

//IR key code macro
#define IR_KEY_POWER                     0x0A //S1
#define IR_KEY_HOME                      0x0C //S2
#define IR_KEY_MOUSE                     0xFF //S3 res
#define IR_KEY_MENU                      0xFF //S4 res
#define IR_KEY_MUTE                      0x0D //S5 
#define IR_KEY_UP                        0x00 //S6
#define IR_KEY_PG_N                      0x52 //S7
#define IR_KEY_PG_P                      0x51 //S8
#define IR_KEY_LEFT                      0x03 //S9
#define IR_KEY_VOL_P                     0x58 //S10
#define IR_KEY_DOWN                      0x01 //S11
#define IR_KEY_SIGNAL                    0xFF //S12 res
#define IR_KEY_VOL_N                     0x5D //S13
#define IR_KEY_VOICE                     0x74 //S14
#define IR_KEY_RIGHT                     0x02 //S15
#define IR_KEY_BACK                      0x1C //S16
#define IR_KEY_OK                        0x1F //S17

//IR key code matrix
#define KEY_MATRIX_IR \
{                     \
       /*18*/        /*19*/         /*25*/        /*26*/        \
/*02*/ IR_KEY_MOUSE, IR_KEY_MENU,   IR_KEY_POWER, IR_KEY_HOME,  \
/*20*/ IR_KEY_RIGHT, IR_KEY_BACK,   IR_KEY_VOL_N, IR_KEY_VOICE, \
/*21*/ 0xFFFF,       0xFFF,         IR_KEY_OK,    0xFFFF,       \
/*22*/ IR_KEY_PG_N,  IR_KEY_PG_P,   IR_KEY_MUTE,  IR_KEY_UP,    \
/*24*/ IR_KEY_DOWN,  IR_KEY_SIGNAL, IR_KEY_LEFT,  IR_KEY_VOL_P, \
}
#endif
typedef struct {
    uint8_t  state;
    uint8_t  row;
    uint8_t  col;
    uint16_t val;
} app_key_t;

void key_init(void);
void key_scan(app_key_t *key);

#endif
