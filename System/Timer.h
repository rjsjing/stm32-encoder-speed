#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>
#include "stm32f10x.h"

// 定时器中断初始化函数（定时周期：单位ms，范围1~32767ms @72MHz主频）
void Timer2_Init(uint16_t arr, uint16_t psc);

// 定时器中断回调函数（用户自定义逻辑写在这里）
void Timer2_IRQ_Callback(void);

#endif
