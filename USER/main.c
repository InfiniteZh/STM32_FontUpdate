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

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(BAUD_RATE);		//初始化串口波特率为115200
	LED_Init();					//初始化LED  
 	LCD_Init();					//LCD初始化  
	W25QXX_Init();				//初始化W25Q64

   
		  printf("Font no found...\r\n");
		  LCD_ShowString(20,50,200,24,24,(u8*)"Font no found...");
			LCD_ShowString(20,80,200,24,24,(u8*)"Font Updating...");
      update_font(20,110,24);//更新字库		
		       
	printf("Font is readly ...\r\n");
	POINT_COLOR=RED;       
	Show_Str(30,50+24*0,320,24,"STM32F407单片机",24,0);				    	 
	Show_Str(30,50+24*1,320,24,"无线电实验室",24,0);				    	 
	Show_Str(30,50+24*2,320,24,"2019年1月23日",24,0);
 	POINT_COLOR=BLUE;  


  while(1)
	{
		
		LED0=!LED0;
    delay_ms(1000);

	 }
	
}

