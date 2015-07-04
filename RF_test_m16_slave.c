/*
目    的：2401测试
编译环境：avr studio+winavr
功能演示：PC2外接LED，LED闪烁，表示收发正常，模块正常；led没变化，模块有问题
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#include "spi.h"			//2401管脚连接
#include "2401.h"		

unsigned char NUMTOSEG7(unsigned char DATA);
void serial_to_parral(unsigned char disp1,unsigned char disp2);

int main()
{
	SPI_Init();
	sei();

//***********************************************************
	RF2401_Init();			//初始化2401
 	Rx_Mode();				//接收模式

	PORT_SPI&=~(1<<CSN);
	SpiRW(1|W_REGSITER);	//写 寄存器1
	SpiRW(0x0);				//禁止自动应答  
	PORT_SPI|=(1<<CSN);

	PORT_SPI&=~(1<<CSN);   
	SpiRW(0x31);			
	SpiRW(0X20);			//通道0有效数据宽度32，用于接收模式//用于清除FIFO
	PORT_SPI|=(1<<CSN);
	_delay_ms(1);

	while(1) 
	{
		serial_to_parral(0X0F,NUMTOSEG7(0));
		if(!(PINB&(1<<IRQ)))
		{
			serial_to_parral(0X0F,NUMTOSEG7(1));
			unsigned char irq_sta;
			irq_sta=Read_IRQ();
			if(irq_sta&(1<<RX_DR))
			{
				serial_to_parral(0X0F,NUMTOSEG7(2));
				Clr_IRQ(1<<RX_DR);
				Read_Rx(32);

				unsigned char i;
				for(i=0;i<32;i++)			//发送接收到的数据
					TxData[i]=RxData[i];
				W_Send_Data(32);
				Tx_Mode();
			}
			if(irq_sta&(1<<TX_DS))
			{
				serial_to_parral(0X0F,NUMTOSEG7(3));
				Clr_IRQ(1<<TX_DS);
				Rx_Mode();
			}
			
		}
	}
}


unsigned char NUMTOSEG7(unsigned char DATA)
{ unsigned char AA;
  switch (DATA)
  { case 0: AA=0xc0;break;  // ‘0’
    case 1: AA=0xf9;break;  // ‘1’
    case 2: AA=0xa4;break;  // ‘2’
    case 3: AA=0xb0;break;  // ‘3’
    case 4: AA=0x99;break;  // ‘4’
    case 5: AA=0x92;break;  // ‘5’
    case 6: AA=0x82;break;  // ‘6’
    case 7: AA=0xf8;break;  // ‘7’
    case 8: AA=0x80;break;  // ‘8’
    case 9: AA=0x90;break;  // ‘9’
    case 10: AA=0x88;break; // ‘A’
    case 11: AA=0x83;break; // ‘B’
    case 12: AA=0xc6;break; // ‘C’
    case 13: AA=0xa1;break; // ‘D’
    case 14: AA=0x86;break; // ‘E’
    case 15: AA=0x8e;break; // ‘F’
    case '-':AA=0xdf;break; // 破折号
    case '_':AA=0xf7;break; // 下划线
    case ' ':AA=0xff;break; // 消隐
	default: AA=0xff;
  }
  return(AA);
}

void serial_to_parral(unsigned char disp1,unsigned char disp2)    //本函数用于将两个8位的信号通过串口转并口输出
{                                                                 //其中，第一个参数最终传输给了U3,而第二个参数传输给了U2
	DDRA=0XFF;
	PORTA|=0X80;
	int q,w;
	for(q=0;q<8;q++)
	{
		if((disp1&0x80)==0) PORTA&=~0X20;
		else PORTA|=0X20;
		PORTA&=~0X40;
		PORTA|=0X40;
		disp1<<=1;
	}
	for(w=0;w<8;w++)
	{
		if((disp2&0x80)==0) PORTA&=~0X20;
		else PORTA|=0X20;
		PORTA&=~0X40;
		PORTA|=0X40;
		disp2<<=1;
	}
	PORTA&=~0X80;
	PORTA|=0X80;
}
