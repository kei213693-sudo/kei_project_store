/***
 * @Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * @All rights reserved.
 * @FilePath: hal_can_type.h
 * @abstract:
 * @Current_Version:
 * @Date:
 * @History_Version: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#ifndef HAL_CAN_TYPE_H
#define HAL_CAN_TYPE_H
#include "stdint.h"

#define HAL_CAN_FDFRAME     1

/**
 * 通道枚举类型定义
 * 本枚举是对实质can通道的映射
 * 后续方便初始化或设置相关参数
*/
typedef enum{
    /*To Do*/
    HAL_CAN0_CHANNEL = 0,
    HAL_CAN_CHANNEL_NUM
}hal_can_channel_et;

/**
 * ID类型枚举定义
 * HAL_CAN_ID_TYPE_StAND        CAN ID类型为标准型，此处定义为0，具体配置到寄存器值还需根据手册来。
 * HAL_CAN_ID_TYPE_EXTEND       CAN ID类型为扩展型，此处定义为1，具体配置到寄存器值还需根据手册来。
*/
typedef enum{
    HAL_CAN_ID_TYPE_StAND       = 0,
    HAL_CAN_ID_TYPE_EXTEND,
}hal_can_id_type_et;
/**
 * 帧类型枚举定义
 * HAL_CAN_FRAME_TYPE_CAN       帧类型为经典CAN
 * HAL_CAN_FRAME_TYPE_CANFD     帧类型为CANFD
 *
*/
typedef enum{
    HAL_CAN_FRAME_TYPE_CAN       = 0,
    HAL_CAN_FRAME_TYPE_CANFD,
}hal_can_frame_type_et;

typedef enum{
    HAL_CAN_BRS_NOT_CHANGE       = 0,
    HAL_CAN_BRS_CHANGE,
}hal_can_brs_et;
/**
 * 传输模式定义，具体寄存器配置还请参考芯片数据手册
 * CANTRCV_TRCVMODE_NORMAL        CAN传输模式为 正常模式
 * CANTRCV_TRCVMODE_SLEEP         CAN传输模式为 睡眠模式
 * CANTRCV_TRCVMODE_STANDBY       CAN传输模式为 Standby模式
 *
*/
typedef enum
{
    CANTRCV_TRCVMODE_UNINIT = 0,
    CANTRCV_TRCVMODE_NORMAL,
    CANTRCV_TRCVMODE_SLEEP,
    CANTRCV_TRCVMODE_STANDBY
}hal_can_trcvmode_et;
/**
 * 控制模式定义，具体寄存器配置还请参考芯片数据手册
 * ControllerMode_UNINIT        CAN控制模式为 CAN未初始化的时候定义为该模式
 * ControllerMode_STOPPED       CAN控制模式为 停止模式
 * ControllerMode_STARTED       CAN控制模式为 正常通信模式
 * ControllerMode_SLEEP         CAN控制模式为 睡眠模式
 * ControllerMode_BUSOFF        CAN控制模式为 busoff
 *
*/
typedef enum
{
    ControllerMode_UNINIT = 0,
    ControllerMode_STOPPED,
    ControllerMode_STARTED,
    ControllerMode_SLEEP,
    ControllerMode_BUSOFF
}hal_can_ctrmode_et;
/**
 * id 过滤配置
 * m_u32_code       过滤值
 * m_u32_mask       过滤掩码    1=相关位(必须与m_code值一致)    0=不相关
 * m_canidtype_e    过滤can类型 参考 hal_can_id_type_et[hal_can_cfg.h]
 * m_frametype_e    CAN帧类型   参考 hal_can_frame_type_et[hal_can_cfg.h]
 * 举例：
 *      1.如m_u32_code = 0x123  m_u32_mask = 0xffffffff 只允许ID=0x123
 *      2.如m_u32_code = 0x403  m_u32_mask = 0x111 可展开
 *                  code = 0000 0100 0000 0011
 *                  mask = 0000 0001 0001 0001
 *                  ID   = XXXX XXX0 XXX0 XXX1 符合该规则的ID允许通过
*/
typedef struct
{
    uint32_t m_u32_code;
    uint32_t m_u32_mask;
    hal_can_id_type_et      m_canidtype_e;
    hal_can_frame_type_et   m_frametype_e;

} hal_can_filter_st;


/**
 * CAN发送接收报文结构体
 * m_u8_pdata                   发送接收数据指针
 * m_u8_dlc                     发送接收数据的长度
 * m_id_type_e                  CAN ID类型，具体配置可参考hal_can_id_type_et[hal_can_cfg.h]
 * m_frame_type_e               帧类型，详见hal_can_frame_type_et
 * m_fd_brs_e                   CANFD是否配置成可变速率，芯片带CANFD才有此项，如不带CANFD则配置成
 *                              HAL_CAN_BRS_NOT_CHANGE即可
 * m_u32_canid                  CAN ID
 */
typedef struct
{
    uint8_t *m_u8_pdata;
    uint8_t m_u8_dlc;

    hal_can_id_type_et m_id_type_e;
#if (1 == HAL_CAN_FDFRAME)
    hal_can_frame_type_et m_frame_type_e;
    hal_can_brs_et m_fd_brs_e;
#endif
    uint32_t m_u32_canid;
} hal_can_pdu_st;


/**
 * CAN通信tq配置
 * m_u8_syncseg                  同步段tq，默认配置为1
 * m_u8_procseg                  传播段tq，数据传入为1、2、3、4...，实现的时候需要对照相应寄存器具体配置
 * m_u8_seg1                     相位缓冲段1，数据传入为1、2、3、4...，实现的时候需要对照相应寄存器具体配置
 * m_u8_seg2                     相位缓冲段2，数据传入为1、2、3、4...，实现的时候需要对照相应寄存器具体配置
 * m_u8_sjw                      SJW，数据传入为1、2、3、4...，实现的时候需要对照相应寄存器具体配置
 * m_u16_brp                     预分频，数据传入为1、2、3、4...，实现的时候需要对照相应寄存器具体配置
 */
typedef struct
{
    uint8_t m_u8_syncseg;
    uint8_t m_u8_procseg;
    uint8_t m_u8_seg1;
    uint8_t m_u8_seg2;
    uint8_t m_u8_sjw;
    uint16_t m_u16_brp;
} hal_can_tq_st;
/**
 *
 * CAN 接收回调函数重定义
*/
typedef void (*hal_can_recv_cbk_t)(const hal_can_pdu_st *);
/**
 *
 * CAN BUSOFF回调函数重定义
*/
typedef void (*hal_can_busoff_cbk_t)(void);
/**
 *
 * CAN 发送完成回调函数重定义
*/
typedef void (*hal_can_tx_cbk_t)(uint32_t id);


#endif
