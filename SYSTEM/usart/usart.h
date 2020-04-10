#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define Usart6_DMA_Len 4096   //����DMA����������
#define BAUD_RATE    128000
#define WATE_TIME    Usart6_DMA_Len*10000/BAUD_RATE    //���ڷ���Usart6_DMA_Len�����ֽ�����Ҫ��ʱ�䣬��λ��ms
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void send(u8 ch);
#endif


