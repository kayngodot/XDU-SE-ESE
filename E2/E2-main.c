#include "stm32f10x.h"  // STM32 标准库头文件

void GPIO_Config(void);
void TIM2_Config(void);

int main(void) {
    GPIO_Config();   // 配置 GPIO 端口
    TIM2_Config();   // 配置 TIM2 定时器
    
    while (1) {
        // 等待 TIM2 更新标志位
        while (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == RESET);
        TIM_ClearFlag(TIM2, TIM_FLAG_Update);  // 清除更新标志位

        // 翻转 LED 状态
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)));  // 翻转 PC13 LED0
        GPIO_WriteBit(GPIOB, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_2)));   // 翻转 PB2 LED1
    }
}

void GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能 GPIO 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);

    // 配置 PC13 为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // 配置 PB2 为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void TIM2_Config(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    // 使能 TIM2 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 配置 TIM2
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;  // 自动重装载值设为 10000
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1; // 预分频器设置为 7200
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 启动 TIM2
    TIM_Cmd(TIM2, ENABLE);
}
