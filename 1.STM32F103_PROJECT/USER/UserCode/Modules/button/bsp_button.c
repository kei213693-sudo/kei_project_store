/*
 * bsp_gpio.c
 *
 *  Created on: 2023年6月21日
 *	  Abstract: button处理模块，初始化io口参数，传递某一个按键操作（短按、长按、长按超时）传递给app层
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>

#include "bsp_button.h"
#include "hw_gpio.h"//hw硬件层代码，负责初始化gpio参数

/*******************************全局变量定义*******************************/
const BSP_BUTTON_API_T g_BspButtonApi =
{
    bsp_button_init,
    bsp_button_deinit,
    bsp_button_info_get
};

/*******************************内部变量定义*******************************/
static appButtonCB_t g_pButtonAppCB = NULL;// Pointer to application callback

static BSP_BUTTON_TAB_T g_BspButtonTab[] = 
{
{BSP_BUTTON_NAME_0,          HW_GPIO_INDEX_BUTTON_0,       0,0,0},//PC5，0有效
{BSP_BUTTON_NAME_1,          HW_GPIO_INDEX_BUTTON_1,       0,0,0},//PA15，0有效
{BSP_BUTTON_NAME_2,          HW_GPIO_INDEX_BUTTON_2,       1,0,0},//PA5，1有效
{BSP_BUTTON_NAME_3,          0,       0,0,0}//后续有按键在此依次扩充
};

//记录当前的按键信息，提供给app获取
static BSP_BUTTON_INFO_T g_stBspButtonInfo = {BSP_BUTTON_NAME_END,BSP_BUTTON_TYPE_END};

/*******************************内部函数的声明*******************************/
static void bsp_button_upload(BSP_BUTTON_NAME_E eButtonName,BSP_BUTTON_TYPE_E eButtonType);
static uint8_t bsp_button_level_get(uint16_t u16PinIndex);

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：button初始化
*函数参数 ：appButtonCB，应用层回调函数
*函数返回值 ：
*备注：
**************************************************************/
void bsp_button_init(appButtonCB_t pCallback)
{
    hw_gpio_button_init();

    //这里"消抖、长按、短按"等时间判定，复用周期性1ms产生定时中断的TIM3
    
    if(NULL != pCallback)
    {
        g_pButtonAppCB = pCallback;
    }
}

/**************************************************************
*函数功能描述 ：button去初始化
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void bsp_button_deinit(void)
{
    hw_gpio_button_deinit();
    g_pButtonAppCB = NULL;
}

/**************************************************************
*函数功能描述 ：传递单个按键操作给app
*函数参数 ：eButtonName，键值
            eButtonType，按键类型
*函数返回值 ：
*备注：本质还是调用app层提供给bsp的回调函数，让app的task来主动获取最新的单个按键操作
**************************************************************/
static void bsp_button_upload(BSP_BUTTON_NAME_E eButtonName,BSP_BUTTON_TYPE_E eButtonType)
{
    if(NULL != g_pButtonAppCB)
    {
        g_stBspButtonInfo.eButtonName = eButtonName;
        g_stBspButtonInfo.eButtonType = eButtonType;

        (*g_pButtonAppCB)(NULL);// Notify the application
    }
}

/**************************************************************
*函数功能描述 ：供app层获取当前按键参数
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void bsp_button_info_get(BSP_BUTTON_INFO_T *pStData)
{
    if(NULL != pStData)
    {
        memcpy(pStData,&g_stBspButtonInfo,sizeof(BSP_BUTTON_INFO_T));
    }
}

/**************************************************************
*函数功能描述 ：获取当前io电平
*函数参数 ：u16PinIndex,io口索引
*函数返回值 ：
*备注：
**************************************************************/
static uint8_t bsp_button_level_get(uint16_t u16PinIndex)
{
    uint8_t u8PinLevel = 0xff;
    switch(u16PinIndex)
    {
        case HW_GPIO_INDEX_BUTTON_0:
        {
            u8PinLevel = (uint8_t)HW_GPIO_LEVEL_BUTTON_0;
            break;
        }

        case HW_GPIO_INDEX_BUTTON_1:
        {
            u8PinLevel = (uint8_t)HW_GPIO_LEVEL_BUTTON_1;
            break;
        }

        case HW_GPIO_INDEX_BUTTON_2:
        {
            u8PinLevel = (uint8_t)HW_GPIO_LEVEL_BUTTON_2;
            break;
        }

        default:
            // Do nothing.
            break;
    }
    return u8PinLevel;
}

/**************************************************************
*函数功能描述 ：io中断回调处理，根据列表读取io电平，然后修改g_BspButtonTab对应标志位
*函数参数 ：u16PinIndex,io口索引
*函数返回值 ：
*备注：在IO中断服务函数中调用，上下双边沿中断触发，在这里兼容不同有效电平的gpio
**************************************************************/
void bsp_button_handle(uint16_t u16PinIndex)
{
    for(uint8_t i = 0;i < (sizeof(g_BspButtonTab)/sizeof(g_BspButtonTab[0]));i++)
    {
        if(u16PinIndex == g_BspButtonTab[i].u16PinIndex)
        {
            if(bsp_button_level_get(u16PinIndex) == g_BspButtonTab[i].u8ActiveLevel)
            {//按键按下
                g_BspButtonTab[i].u8Flag = 1;//记录该按键被按下了
                g_BspButtonTab[i].u32Count = 0;
            }
            else
            {//按键松开
                #if (1 == BSP_BUTTON_LONG_ENBLE)
                if((g_BspButtonTab[i].u32Count > BSP_BUTTON_ELIMINATE_COUNT)
                    && (g_BspButtonTab[i].u32Count <= BSP_BUTTON_LONG_COUNT_MIN))
                {//传递短按事件
                    bsp_button_upload(g_BspButtonTab[i].eButtonName,BSP_BUTTON_TYPE_SHORT);
                }
                #endif

                g_BspButtonTab[i].u8Flag = 0;
                g_BspButtonTab[i].u32Count = 0;
            }
        }
    }
}

/**************************************************************
*函数功能描述 ：定时器中断处理，结合g_BspButtonTab实现“消抖、长按、超时”的时间处理
*函数参数 ：
*函数返回值 ：
*备注：定时器中断触发
**************************************************************/
void bsp_button_IrqCheck(void)
{
    for(uint8_t i = 0;i < (sizeof(g_BspButtonTab)/sizeof(g_BspButtonTab[0]));i++)
    {
        if(1 == g_BspButtonTab[i].u8Flag)
        {
            g_BspButtonTab[i].u32Count++;
            if(g_BspButtonTab[i].u32Count > BSP_BUTTON_ELIMINATE_COUNT)
            {//消抖通过
                #if (0 == BSP_BUTTON_LONG_ENBLE)
                //不支持长按功能，传递短按事件到app层
                g_BspButtonTab[i].u8Flag = 0;
                g_BspButtonTab[i].u32Count = 0;
                bsp_button_upload(g_BspButtonTab[i].eButtonName,BSP_BUTTON_TYPE_SHORT);
                #else
                if(g_BspButtonTab[i].u32Count == BSP_BUTTON_LONG_COUNT_MIN)
                {//长按，传递长按事件到app层
                    bsp_button_upload(g_BspButtonTab[i].eButtonName,BSP_BUTTON_TYPE_LONG);
                }
                else if(g_BspButtonTab[i].u32Count > BSP_BUTTON_LONG_COUNT_MAX)
                {//长按超时
                    g_BspButtonTab[i].u8Flag = 0;
                    g_BspButtonTab[i].u32Count = 0;
                    bsp_button_upload(g_BspButtonTab[i].eButtonName,BSP_BUTTON_TYPE_TIMEOUT);
                }
                #endif
            }
        }
    }
}

