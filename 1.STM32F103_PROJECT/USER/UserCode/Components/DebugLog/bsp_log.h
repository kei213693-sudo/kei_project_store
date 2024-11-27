/*
 * bsp_log.h
 *
 *  Created on: 2024年7月22日
 *      Author: lj
 *	  Abstract: 日志模块，包含tx和rx两个部分，tx就是“模块化+标准化打印”，rx就是“调试指令处理逻辑”
 */
#ifndef _BSP_LOG_H_
#define _BSP_LOG_H_

/*******************************头文件*******************************/

/*******************************宏定义*******************************/
#define BSP_LOG_MAX_LEN          192//单次打印最大数据长度

#define hw_printf(format,arg...)     printf(format,##arg)//标准化打印，要根据实际的sdk决定

/*******************************结构体、枚举******************************/

typedef enum
{//开关枚举
    BSP_LOG_SWITCH_ON              = 0,//打印开
    BSP_LOG_SWITCH_OFF,                //打印关
    BSP_LOG_SWITCH_END//打印开
}BSP_LOG_SWITCH_E;

typedef enum
{//信息类型枚举
    BSP_LOG_TYPE_INFO             = 0,//通常内容
    BSP_LOG_TYPE_WARNING,             //告警内容
    BSP_LOG_TYPE_ERROR,               //错误内容

    BSP_LOG_TYPE_END
}BSP_LOG_TYPE_E;

typedef enum
{//模块类型枚举，根据实际工程扩减
    BSP_LOG_MODULE_NONE             = 0,//无模块，通用打印
    BSP_LOG_MODULE_LOG,                 //日志模块
    BSP_LOG_MODULE_LP,                  //低功耗模块
    BSP_LOG_MODULE_BLE,                 //蓝牙模块
    BSP_LOG_MODULE_BUTTON,              //按键模块
    BSP_LOG_MODULE_LED,                 //灯模块
    BSP_LOG_MODULE_COMM,                //信息队列模块
    BSP_LOG_MODULE_TASK,                //task管理模块
    BSP_LOG_MODULE_UPGRADE,             //程序升级模块

    BSP_LOG_MODULE_ALL//选中所有模块
}BSP_LOG_MODULE_E;

typedef struct bsp_log_ctl_t
{//模块信息结构体
    const char *name;//指向模块名称指针
    BSP_LOG_SWITCH_E en;//打印开关
}bsp_log_ctl_t;

typedef struct
{//提供给应用层接口的结构体
void (*bsp_log_switch_set)(BSP_LOG_MODULE_E module,BSP_LOG_SWITCH_E en);
void (*bsp_log_print)(BSP_LOG_MODULE_E module,BSP_LOG_TYPE_E type,const char *format, ...);
void (*bsp_log_print_hex)(BSP_LOG_MODULE_E module,const char *note,const uint8_t *pData,uint32_t u32len);
void (*bsp_log_cmd_handle)(const uint8_t *pData,uint32_t u32len);
}BSP_LOG_API_T;

/*******************************全局变量的外部声明extern*******************************/
extern const BSP_LOG_API_T g_BspLogApi;//外部调用该组件直接使用这个变量访问所有api

/*******************************全局函数的声明*******************************/
extern void bsp_log_switch_set(BSP_LOG_MODULE_E module,BSP_LOG_SWITCH_E en);
extern void bsp_log_print(BSP_LOG_MODULE_E module,BSP_LOG_TYPE_E type,const char *format, ...);
extern void bsp_log_print_hex(BSP_LOG_MODULE_E module,const char *note,const uint8_t *pData,uint32_t u32len);
extern void bsp_log_cmd_handle(const uint8_t *pData,uint32_t u32len);

#endif /* _BSP_LOG_H_ */
