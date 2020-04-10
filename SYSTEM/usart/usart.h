#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define Usart6_DMA_Len 4096   //定义DMA缓冲区长度
#define BAUD_RATE    128000
#define WATE_TIME    Usart6_DMA_Len*10000/BAUD_RATE    //串口发送Usart6_DMA_Len长度字节所需要的时间，单位：ms
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void send(u8 ch);
#endif


