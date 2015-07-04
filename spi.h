///SPI�궨��
#define DDR_SPI		DDRB
#define PORT_SPI	PORTB
#define CSN 		PB4	
#define MOSI 		PB5
#define MISO 		PB6
#define SCK 		PB7
#define CE			PB3
#define IRQ			PB2

//SPI��ʼ��
void SPI_Init(void)
{
	DDR_SPI|=((1<<MOSI)|(1<<SCK)|(1<<CSN)|(1<<CE));	//����MOSI��SCK��CSN�������������
	DDR_SPI&=~((1<<MISO)|(1<<IRQ));					//һֱΪ����ģʽ
	SPCR|=((1<<SPE)|(1<<MSTR));						//ʹ��SPI��������set clock rate fck/4
	PORTB|=(1<<0);						//VCC
	PORTB&=~(1<<1);						//GND
}

// SPI������д����
uint8_t SpiRW(uint8_t val)  	
{
	uint8_t temp; 
    SPDR=val; 
	while (!(SPSR&(1<<SPIF)))
		; 
	temp=SPDR; 
    return temp; 
} 
