#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"

int16_t Speed;

int main(void)
{
	OLED_Init();
	Encoder_Init();
	Timer2_Init(65535,0);
	
	OLED_ShowString(1,1,"Speed:");
	
	while(1)
	{
		OLED_ShowSignedNum(1,7,Speed,5);
	}
	
}

void TIM2_IRQHandler(void) 
{
    // 检查更新中断标志位
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
		Speed = Encoder_Get();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位（必须）
    }
}
