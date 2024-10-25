#include "stm32f10x.h"
#include <stdio.h>

#define LEDPORT0 GPIOC  // LED0 接口 PC13
#define LED0 GPIO_Pin_13
#define LEDPORT1 GPIOB  // LED1 接口 PB2
#define LED1 GPIO_Pin_2

#define KEYPORT0 GPIOA  // KEY0 接口 PA8
#define KEY0 GPIO_Pin_8
#define KEYPORT1 GPIOB  // KEY1 接口 PB10
#define KEY1 GPIO_Pin_10

/**
  * @brief  初始化LED引脚
  * @param  None
  * @retval None
  */
void Init_Led(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 启用GPIOC时钟 (用于LED0)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LEDPORT0, &GPIO_InitStructure);
    
    // 启用GPIOB时钟 (用于LED1)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED1;
    GPIO_Init(LEDPORT1, &GPIO_InitStructure);
}

/**
  * @brief  初始化按键引脚
  * @param  None
  * @retval None
  */
void Init_Key(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 启用GPIOA时钟 (用于KEY0)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = KEY0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入模式
    GPIO_Init(KEYPORT0, &GPIO_InitStructure);

    // 启用GPIOB时钟 (用于KEY1)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = KEY1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入模式
    GPIO_Init(KEYPORT1, &GPIO_InitStructure);
}

/**
  * @brief  重定向printf输出到USART1
  * @param  ch: 要发送的字符
  * @param  f: 文件指针
  * @retval 发送的字符
  */
int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, (uint8_t)ch);
    return ch;
}

/**
  * @brief  主程序
  * @param  None
  * @retval None
  */
int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // 初始化LED和按键
    Init_Led();
    Init_Key();

    // 启用GPIOA时钟, 用于USART1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 启用USART1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // 配置USART1_TX GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置USART1
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    // 启用USART1
    USART_Cmd(USART1, ENABLE);

    printf("USART1 Init Completely\r\n");

    while (1)
    {
        int key0_state = GPIO_ReadInputDataBit(GPIOA, KEY0); // 读取KEY0状态
        int key1_state = GPIO_ReadInputDataBit(GPIOB, KEY1); // 读取KEY1状态

        if (key0_state == 0) {
            GPIO_SetBits(LEDPORT0, LED0);   // 按下KEY0, 点亮LED0
        } else {
            GPIO_ResetBits(LEDPORT0, LED0); // 松开KEY0, 熄灭LED0
        }

        if (key1_state == 0) {
            GPIO_SetBits(LEDPORT1, LED1);   // 按下KEY1, 点亮LED1
        } else {
            GPIO_ResetBits(LEDPORT1, LED1); // 松开KEY1, 熄灭LED1
        }
    }
}
