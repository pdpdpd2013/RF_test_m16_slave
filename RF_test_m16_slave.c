/*
Ŀ    �ģ�2401����
���뻷����avr studio+winavr
������ʾ��PC2���LED��LED��˸����ʾ�շ�������ģ��������ledû�仯��ģ��������
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#include "spi.h"			//2401�ܽ�����
#include "2401.h"		

unsigned char NUMTOSEG7(unsigned char DATA);
void serial_to_parral(unsigned char disp1,unsigned char disp2);

int main()
{
	SPI_Init();
	sei();

//***********************************************************
	RF2401_Init();			//��ʼ��2401
 	Rx_Mode();				//����ģʽ

	PORT_SPI&=~(1<<CSN);
	SpiRW(1|W_REGSITER);	//д �Ĵ���1
	SpiRW(0x0);				//��ֹ�Զ�Ӧ��  
	PORT_SPI|=(1<<CSN);

	PORT_SPI&=~(1<<CSN);   
	SpiRW(0x31);			
	SpiRW(0X20);			//ͨ��0��Ч���ݿ��32�����ڽ���ģʽ//�������FIFO
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
				for(i=0;i<32;i++)			//���ͽ��յ�������
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
  { case 0: AA=0xc0;break;  // ��0��
    case 1: AA=0xf9;break;  // ��1��
    case 2: AA=0xa4;break;  // ��2��
    case 3: AA=0xb0;break;  // ��3��
    case 4: AA=0x99;break;  // ��4��
    case 5: AA=0x92;break;  // ��5��
    case 6: AA=0x82;break;  // ��6��
    case 7: AA=0xf8;break;  // ��7��
    case 8: AA=0x80;break;  // ��8��
    case 9: AA=0x90;break;  // ��9��
    case 10: AA=0x88;break; // ��A��
    case 11: AA=0x83;break; // ��B��
    case 12: AA=0xc6;break; // ��C��
    case 13: AA=0xa1;break; // ��D��
    case 14: AA=0x86;break; // ��E��
    case 15: AA=0x8e;break; // ��F��
    case '-':AA=0xdf;break; // ���ۺ�
    case '_':AA=0xf7;break; // �»���
    case ' ':AA=0xff;break; // ����
	default: AA=0xff;
  }
  return(AA);
}

void serial_to_parral(unsigned char disp1,unsigned char disp2)    //���������ڽ�����8λ���ź�ͨ������ת�������
{                                                                 //���У���һ���������մ������U3,���ڶ��������������U2
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
