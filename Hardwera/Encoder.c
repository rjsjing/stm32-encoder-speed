#include "stm32f10x.h"   // Device header

void Encoder_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;			        //上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);  
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //无需计数方式，编码器自动设置
	TIM_TimeBaseInitStruct.TIM_Period = 65536 - 1;				   //ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler = 1 - 1;			  //PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;				//选择配置定时器通道1
	TIM_ICInitStruct.TIM_ICFilter = 0xF ;						//输入滤波器参数，可以过滤信号抖动
	//TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising ;		//代表编码器A，B，输入时不反相
	/*TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;			//捕获预分频，选择不分频，每次信号都触发捕获
	TIM_ICInitStruct.TIM_ICSelection =  TIM_ICSelection_DirectTI;  //输入信号交叉，选择直通，不交叉  
	*/
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;				//选择配置定时器通道2
	TIM_ICInitStruct.TIM_ICFilter = 0xF ;						//输入滤波器参数，可以过滤信号抖动
	//TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising ;
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);   //覆盖了TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising ;代表编码器A，B，输入时不反相
	
	TIM_Cmd(TIM3,ENABLE);
}

int16_t Encoder_Get(void)
{
	int16_t temp;
	temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
	return temp;
}
