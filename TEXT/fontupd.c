#include "fontupd.h"
#include "w25qxx.h"   
#include "lcd.h"  
#include "string.h"
#include "delay.h"
#include "usart.h"

extern u16 Usart6_Rec_Len;
extern u32 GBK_OVER_Flag;
extern u8 GBK_BUF_Flag;
extern u8 Usart6_Rece_Buf0[Usart6_DMA_Len];
extern u8 Usart6_Rece_Buf1[Usart6_DMA_Len];

//�ֿ�����ռ�õ�����������С(3���ֿ�+unigbk��+�ֿ���Ϣ=3238700�ֽ�,Լռ791��W25QXX����)
#define FONTSECSIZE	 	791
//�ֿ�����ʼ��ַ 
#define FONTINFOADDR 	1024*1024*0 		//�ֿ����׵�ַ
//��������ֿ�Ĵ�С
#define UNIGBK         171*1024        //171KB
#define GBK12_FONSIZE  562*1024        //562KB
#define GBK16_FONSIZE  749*1024			   //749KB
#define GBK24_FONSIZE  1684*1024       //1684KB

//���������ֿ������Ϣ����ַ����С��
_font_info ftinfo;


//��ʼ������
//����ֵ:0,�ֿ����.
//		 ����,�ֿⶪʧ
u8 font_init(void)
{		
	u8 t=0;
	W25QXX_Init();  
	while(t<10)//������ȡ10��,���Ǵ���,˵��ȷʵ��������,�ø����ֿ���
	{
		t++;
		W25QXX_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//����ftinfo�ṹ������
		if(ftinfo.fontok==0XAA)break;
		delay_ms(20);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}

//��ʾ��ǰ������½���
//x,y:����
//size:�����С
//fsize:�����ļ���С
//pos:��ǰ�ļ�ָ��λ��
u32 fupd_prog(u16 x,u16 y,u8 size,u32 fsize,u32 pos)
{
	float prog;
	u8 t=0XFF;
	prog=(float)pos/fsize;
	prog*=100;
	if(t!=prog)
	{
		LCD_ShowString(x+3*size/2,y,240,320,size,"%");		
		t=prog;
		if(t>100)t=100;
		LCD_ShowNum(x,y,t,3,size);//��ʾ��ֵ
	}
	return 0;					    
} 

//����ĳһ��
//x,y:����
//size:�����С
//fx:���µ����� 0,ungbk;1,gbk12;2,gbk16;3,gbk24;
//����ֵ:0,�ɹ�;����,ʧ��.
u8 updata_fontx(u16 x,u16 y,u8 size,u8 fx)
{
	u32 flashaddr=0;								    
 	u8 res;	
	u32 offx=0;    
  u32 fsize=0;
		switch(fx)
		{
			case 0:												//����UNIGBK.BIN
				ftinfo.ugbkaddr=FONTINFOADDR+sizeof(ftinfo);	//��Ϣͷ֮�󣬽���UNIGBKת�����
				fsize=ftinfo.ugbksize=UNIGBK;					//UNIGBK��С
				flashaddr=ftinfo.ugbkaddr;
			  printf("Please send UNIGBK.bin\r\n");
				break;
			case 1:
				ftinfo.f12addr=ftinfo.ugbkaddr+ftinfo.ugbksize;	//UNIGBK֮�󣬽���GBK12�ֿ�
				fsize=ftinfo.gbk12size=GBK12_FONSIZE;					//GBK12�ֿ��С
				flashaddr=ftinfo.f12addr;						//GBK12����ʼ��ַ
			  printf("Please send GBK12.FON\r\n");
				break;
			case 2:
				ftinfo.f16addr=ftinfo.f12addr+ftinfo.gbk12size;	//GBK12֮�󣬽���GBK16�ֿ�
				fsize=ftinfo.gbk16size=GBK16_FONSIZE;					//GBK16�ֿ��С
				flashaddr=ftinfo.f16addr;						//GBK16����ʼ��ַ
			  printf("Please send GBK16.FON\r\n");
				break;
			case 3:
				ftinfo.f24addr=ftinfo.f16addr+ftinfo.gbk16size;	//GBK16֮�󣬽���GBK24�ֿ�
				fsize=ftinfo.gkb24size=GBK24_FONSIZE;					//GBK24�ֿ��С
				flashaddr=ftinfo.f24addr;						//GBK24����ʼ��ַ
			  printf("Please send GBK24.FON\r\n");
				break;
		} 
		
		fupd_prog(x,y,size,fsize,offx);	 			//������ʾ
		while(1)//��ѭ��ִ��
		{
	 		if(GBK_OVER_Flag)
 				GBK_OVER_Flag++;
			if(GBK_BUF_Flag!=2)
			{
				GBK_OVER_Flag=1;
				if(GBK_BUF_Flag==0)
					W25QXX_Write(Usart6_Rece_Buf0,offx+flashaddr,Usart6_DMA_Len);		//��ʼд��Usart6_DMA_Len������  
				else if(GBK_BUF_Flag==1)
					W25QXX_Write(Usart6_Rece_Buf1,offx+flashaddr,Usart6_DMA_Len);		//��ʼд��Usart6_DMA_Len������  
				offx+=Usart6_DMA_Len;
				GBK_BUF_Flag=2;
				fupd_prog(x,y,size,fsize,offx);	 			//������ʾ
			}
			delay_us(100);
			if(GBK_OVER_Flag>(WATE_TIME+10)*10)   //��������ʱ��10ms��˵�����ֿⷢ�����
				break;
	 	} 
		if(DMA_GetCurrentMemoryTarget(DMA2_Stream1)==1)
		   W25QXX_Write(Usart6_Rece_Buf1,offx+flashaddr,Usart6_DMA_Len-DMA_GetCurrDataCounter(DMA2_Stream1));//��DMA����һ֡����д��FLASH
		else
			 W25QXX_Write(Usart6_Rece_Buf0,offx+flashaddr,Usart6_DMA_Len-DMA_GetCurrDataCounter(DMA2_Stream1));//��DMA����һ֡����д��FLASH
		
		printf("This Font updated successfull!\r\n");
		uart_init(BAUD_RATE);		//���³�ʼ�����ڼ�DMA
		GBK_OVER_Flag=0;

	return res;
} 

//���������ļ�,UNIGBK,GBK12,GBK16,GBK24һ�����
//x,y:��ʾ��Ϣ����ʾ��ַ
//size:�����С
//��ʾ��Ϣ�����С										  
//����ֵ:0,���³ɹ�;
//		 ����,�������.	  
u8 update_font(u16 x,u16 y,u8 size)
{	
	 u16 i,j;
	
	 LCD_ShowString(x,y,240,320,size,(u8*)"Erasing sectors... ");//��ʾ���ڲ�������	
		for(i=0;i<FONTSECSIZE;i++)	//�Ȳ����ֿ�����,���д���ٶ�
		{
      fupd_prog(x+20*size/2,y,size,FONTSECSIZE,i);//������ʾ
			W25QXX_Read((u8*)Usart6_Rece_Buf1,((FONTINFOADDR/4096)+i)*4096,4096);//������������������(����һ��DMA������)
			for(j=0;j<4096;j++)//У������
			{
				if(Usart6_Rece_Buf1[j]!=0XFF)break;//��Ҫ����  	  
			}
			if(j!=4096)W25QXX_Erase_Sector((FONTINFOADDR/4096)+i);	//��Ҫ����������
		}
		delay_ms(100);
		
		LCD_ShowString(x,y,240,320,size,(u8*)"Updating UNIGBK.BIN  ");
		updata_fontx(x+20*size/2,y,size,0);	//����GBK12.FON
		LCD_ShowString(x,y,240,320,size,(u8*)"Updating GBK12.FON  ");
		updata_fontx(x+20*size/2,y,size,1);	//����GBK12.FON
		LCD_ShowString(x,y,240,320,size,(u8*)"Updating GBK16.FON  ");
		updata_fontx(x+20*size/2,y,size,2);	//����GBK16.FON
		LCD_ShowString(x,y,240,320,size,(u8*)"Updating GBK24.FON  ");
		updata_fontx(x+20*size/2,y,size,3);	//����GBK16.FON
		//ȫ�����º���
		ftinfo.fontok=0XAA;
		W25QXX_Write((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//�����ֿ���Ϣ
		printf("All Font file updated successfull!!!\r\n");
		LCD_Clear(WHITE);
		
		return 0;
}



























