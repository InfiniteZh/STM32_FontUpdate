#include "sys.h"
#include "usart.h"	
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "spi.h"
#include "w25qxx.h"  
#include "fontupd.h"
#include "text.h"	
 

int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(BAUD_RATE);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED  
 	LCD_Init();					//LCD��ʼ��  
	W25QXX_Init();				//��ʼ��W25Q64

   
		  printf("Font no found...\r\n");
		  LCD_ShowString(20,50,200,24,24,(u8*)"Font no found...");
			LCD_ShowString(20,80,200,24,24,(u8*)"Font Updating...");
      update_font(20,110,24);//�����ֿ�		
		       
	printf("Font is readly ...\r\n");
	POINT_COLOR=RED;       
	Show_Str(30,50+24*0,320,24,"STM32F407��Ƭ��",24,0);				    	 
	Show_Str(30,50+24*1,320,24,"���ߵ�ʵ����",24,0);				    	 
	Show_Str(30,50+24*2,320,24,"2019��1��23��",24,0);
 	POINT_COLOR=BLUE;  


  while(1)
	{
		
		LED0=!LED0;
    delay_ms(1000);

	 }
	
}

