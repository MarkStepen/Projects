/**
************************************************************
* @file         main.c
* @brief        MCU版 入口文件
* @author       Gizwits
* @date         2016-09-05
* @version      V03010101
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/

#include <string.h>
#include "Hal_key/hal_key.h"
#include "Hal_Usart/hal_uart.h"
#include "Hal_Watchdog/hal_watchdog.h"
#include "gizwits_protocol.h"

/** 用户区当前设备状态结构体*/
dataPoint_t currentDataPoint;

/**@name 按键相关定义 
* @{
*/
#define GPIO_KEY_NUM 2                                  ///< 定义按键成员总数
keyTypedef_t singleKey[GPIO_KEY_NUM];                   ///< 定义单个按键成员数组指针
keysTypedef_t keys;                                     ///< 定义总的按键模块结构体指针
/**@} */

/**
* 用户初始化函数

* 在该函数中完成了外设驱动初始化以及用户相关数据的初始
* @param none
* @return none
* @note 开发者可在此函数内添加新的驱动初始及状态初始化
*/
void userInit(void)
{
    uartxInit();        //printf打印串口初始化
    watchdogInit(2);    //看门狗复位时间2s
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
}

/**
* 用户数据获取

* 此处需要用户实现除可写数据点之外所有传感器数据的采集,可自行定义采集频率和设计数据过滤算法
* @param none
* @return none
*/

void userHandle(void)
{
	    /*
    currentDataPoint.valueData = ;//Add Sensor Data Collection

    */
}

/**
* key1按键短按处理
* @param none
* @return none
*/
void key1ShortPress(void)
{
    printf("KEY1 PRESS ,Production Mode\n");
    gizwitsSetMode(WIFI_PRODUCTION_TEST);
}

/**
* key1按键长按处理
* @param none
* @return none
*/
void key1LongPress(void)
{
    printf("KEY1 PRESS LONG ,Wifi Reset\n");
    gizwitsSetMode(WIFI_RESET_MODE);

}

/**
* key2按键短按处理
* @param none
* @return none
*/
void key2ShortPress(void)
{
    printf("KEY2 PRESS ,Soft AP mode\n");
    gizwitsSetMode(WIFI_SOFTAP_MODE);
}

/**
* key2按键长按处理
* @param none
* @return none
*/
void key2LongPress(void)
{
    //AirLink mode
    printf("KEY2 PRESS LONG ,AirLink mode\n");
    gizwitsSetMode(WIFI_AIRLINK_MODE);
}

/**
* 按键初始化
* @param none
* @return none
*/
void keyInit(void)
{
    singleKey[0] = keyInitOne(RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_10, key1ShortPress, key1LongPress);
    singleKey[1] = keyInitOne(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_8, key2ShortPress, key2LongPress);
    keys.singleKey = (keyTypedef_t *)&singleKey;
    keyParaInit(&keys); 
}

/**
* @brief程序入口函数

* 在该函数中完成用户相关的初始化,及主逻辑循环
* @param none
* @return none
*/
int main(void)
{
    SystemInit();
    
    userInit();
    keyInit();

    timerInit();
    uartInit();

    gizwitsInit();
    
    printf("MCU Init Success \n");
    while(1)
    {
        watchdogFeed();
        
        userHandle();
        
        gizwitsHandle((dataPoint_t *)&currentDataPoint);
    }
}
