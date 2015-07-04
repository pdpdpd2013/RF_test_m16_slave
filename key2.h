//��һ����������¼KEY()�ķ���ֵ
#define		KEY_DDR		DDRC
#define		KEY_PORT   PORTC
#define 	KEY_PIN		PINC

//˵���������߷�ת��4*4����   ����PA��
//��ֵ��  ��һ��A3 �ڶ���A2 ������A1 ������A0
//A7��һ�У�0x77   0x7b    0x7d    0x7e
//A6�ڶ��У�0xb7   0xbb    0xbd    0xbe
//A5�����У�0xd7   0xdb    0xdd    0xde
//A4�����У�0xe7   0xeb    0xed    0xee
//���ٶ�״̬������ʱ
unsigned char KEY(void)
{
	KEY_PORT=0X0F;
	KEY_DDR=0XF0;				//��4λ���ͣ���4λ����
	unsigned char key_data=0;	//���水��״̬�ı���	
	if(KEY_PIN!=0x0F)			//���а�������
	{
		_delay_ms(10);			//������
		key_data=KEY_PIN;		//�����ʱ����λ��״̬

		KEY_PORT=0XF0;
		KEY_DDR=0X0F;			//��4λ���ߣ���4λ����
		key_data|=KEY_PIN;		//�����ʱ����λ��״̬���������λ�ϲ�

		while(KEY_PIN!=0xF0);	//�ȴ������ͷ�
		_delay_ms(10);			//��ʱ����
		while(KEY_PIN!=0xF0);	//�ȴ������ͷ�
		switch(key_data)		//�԰�����ֵ����
		{
			case 0x77:key_data=1; break;	//��һ��
			case 0x7b:key_data=2; break;
			case 0x7d:key_data=3; break;
			case 0x7e:key_data=4; break;
			case 0xb7:key_data=5; break;	//�ڶ���
			case 0xbb:key_data=6; break;
			case 0xbd:key_data=7; break;
			case 0xbe:key_data=8; break;
			case 0xd7:key_data=9; break;	//������
			case 0xdb:key_data=10;break;
			case 0xdd:key_data=11;break;
			case 0xde:key_data=12;break;
			case 0xe7:key_data=13;break;	//������
			case 0xeb:key_data=14;break;
			case 0xed:key_data=15;break;
			case 0xee:key_data=16;break;
			default:key_data=0;break;		//����Ч����
		}
	}
	return (key_data);						//���ؼ�ֵ
}
