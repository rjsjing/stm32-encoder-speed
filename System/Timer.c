#include "Timer.h"


// 全局标志位（可选：用于主循环判断中断触发）
//uint16_t Timer2_IRQ_Flag = 0;

/**
 * @brief  TIM2定时器中断初始化
 * @param  arr: 自动重装载值（ARR）
 * @param  psc: 预分频器值（PSC）
 * @note   定时周期公式：T = (arr+1)*(psc+1)/72000  (ms)
 *         例：arr=999, psc=71 → T=(1000)*(72)/72000=1ms
 */
void Timer2_Init(uint16_t arr, uint16_t psc)
{
    // 1. 开启定时器时钟（TIM2挂载在APB1总线）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 2. 配置定时器时基结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 10000-1;                // 自动重装载值arr
    TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;             // 预分频器psc
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 时钟分频因子（无分频）
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
    // 3. 使能定时器更新中断（溢出中断）
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // 4. 配置NVIC中断优先级
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;               // TIM2中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 抢占优先级2（可调整）
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               // 使能中断通道
    NVIC_Init(&NVIC_InitStructure);

    // 5. 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief  TIM2中断服务函数（固定命名，不可修改）
 */
/**void TIM2_IRQHandler(void) 
{
    // 检查更新中断标志位
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        Timer2_IRQ_Flag = 1;          // 设置中断触发标志
        Timer2_IRQ_Callback();        // 执行用户回调逻辑
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位（必须）
    }
}
*/

/**
 * @brief  定时器中断回调函数（用户自定义）
 * @note   示例：空函数，你可根据需求修改（如编码器计数防抖、OLED刷新）
 */
/**void Timer2_IRQ_Callback(void)
{
    // 示例1：编码器防抖（替换你之前的软件延时）
    // Encoder_Debounce();
    
    // 示例2：OLED定时刷新（降低刷新频率）
    // OLED_Refresh();
    
    // 示例3：简单测试——翻转GPIO电平
    // GPIO_WriteBit(GPIOA, GPIO_Pin_0, !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
}
*/
