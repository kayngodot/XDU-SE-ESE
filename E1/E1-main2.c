/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>

/**
  * @brief  延时函数
  * @param  ms: 指定延时的毫秒数
  * @retval None
  */
void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 4000; i++) // 延时循环
        __NOP();
}

/**
  * @brief  配置GPIO引脚用于LED和按键
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 启用GPIOA、GPIOB和GPIOC的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);

    // 配置LED0 (PC13)和LED1 (PB2)为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 配置KEY0 (PA8)和KEY1 (PB10)为上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  将printf输出重定向到USART1
  * @param  ch: 要发送的字符
  * @param  f: 文件指针
  * @retval 发送的字符
  */
int fputc(int ch, FILE *f)
{    
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
    USART_SendData(USART1, (uint8_t)ch);
          
    return ch;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  主程序
  * @param  None
  * @retval None
  */
int main(void)
{
    printf("USART1 Init Completely\r\n");
    
    GPIO_Config();
  
    /* Infinite loop */
    while (1)
    {
        uint8_t key0_status = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8);  // 读取KEY0状态
        uint8_t key1_status = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10); // 读取KEY1状态

        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0) // 如果按下KEY0 (PA8)，LED同步闪烁
        {
            while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0) // 等待释放KEY0
            {
                GPIO_SetBits(GPIOC, GPIO_Pin_13);  // 打开LED0
                GPIO_SetBits(GPIOB, GPIO_Pin_2);   // 打开LED1
                delay_ms(500);

                GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // 关闭LED0
                GPIO_ResetBits(GPIOB, GPIO_Pin_2);   // 关闭LED1
                delay_ms(500);
            }
        }
        else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 0) // 如果按下KEY1 (PB10)，LED交替闪烁
        {
            while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 0) // 等待释放KEY1
            {
                GPIO_SetBits(GPIOC, GPIO_Pin_13);  // 打开LED0
                GPIO_ResetBits(GPIOB, GPIO_Pin_2); // 关闭LED1
                delay_ms(500);

                GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // 关闭LED0
                GPIO_SetBits(GPIOB, GPIO_Pin_2);     // 打开LED1
                delay_ms(500);
            }
        }
        else // 如果未按下任何按键，关闭LED
        {
            GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // 关闭LED0
            GPIO_ResetBits(GPIOB, GPIO_Pin_2);   // 关闭LED1
        }
    }
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
