unsigned char ur_data=0;	//�洢���ڽ��յ�������
//���ڳ�ʼ��
void Usart_Init(unsigned int baud)
{
	DDRD|=0X02;								//TXD�����������ǿ������������
	DDRD&=~(1<<0);
	PORTD|=0X03;							//RXD�ڲ�������Ч
	UBRRH=(unsigned char)(baud>>8);			//���ò�����:fosc/(16*(UBRR+1))
	UBRRL=(unsigned char)baud;
	UCSRB|=(1<<RXEN)|(1<<TXEN);				//ʹ�ܽ��ա�����
	UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);//Z1Z0:00-11,5-8λ����
	//����֡��ʽ��8λ���ݣ�1λֹͣλ��USBS=1ʱ2λ������У��λ
	UCSRB|=(0<<TXCIE)|(1<<RXCIE)|(0<<UDRIE);//�ж�����
} 
//��������
void Usart_Transmit(unsigned char data)
{
	while(!(UCSRA&(1<<UDRE)));				//�ȴ����ͼĴ�����
	UDR=data;								//�����ݷ��뻺����������
}
//�����ж�
SIGNAL(SIG_UART_RECV)
{
	unsigned char data=0;
	data=UDR;
	Usart_Transmit(data);
	ur_data=data;
}
