/*
 * SPI_TC72_SENSOR.c
 *
 * Created: 30-Sep-22 8:26:19 PM
 * Author : admin
 */ 

#undef	F_CPU
#define F_CPU	16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <compat/deprecated.h>
#include "uartnaval.h"
#include "uartnaval.c"

#define MOSI		5
#define SCK			7
#define SS			4
#define DIGIT0		0X01
#define DIGIT1		0X02
#define DECODE		0X09
#define DECODE_R	0XFF

unsigned char str[20];

void spi_init()
{
	DDRB = (1<<MOSI)|(1<<SCK)|(1<<SS);  // set pin direction output
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<CPHA);
}

void spi_starttx(unsigned char tc72_write_address, unsigned char tc72_control)
{
	cbi(PORTB,SS);
	//PORTB &= ~(1<<SS);
	
	SPDR = tc72_write_address;			
	while(!(SPSR & (1<<SPIF)));	// wait till transfer finish
	
	SPDR = tc72_control;			
	while(!(SPSR & (1<<SPIF)));	// wait till transfer finish
	
	sbi(PORTB,SS);
	//PORTB |= (1<<SS);
}

void spi_tx(unsigned char data)
{
	//cbi(PORTB,SS);
	//PORTB &= ~(1<<SS);

	SPDR = data;			
	while(!(SPSR & (1<<SPIF)));	// wait till transfer finish
	
	//sbi(PORTB,SS);
	//PORTB |= (1<<SS);
}

char spi_read()
{	
	//cbi(PORTB,SS);
	//PORTB &= ~(1<<SS);
	//SPDR = tc72_read_address;
	while(!(SPSR & (1<<SPIF)));	// wait till transfer finish
	
	//sbi(PORTB,SS);
	//PORTB |= (1<<SS);
	
	 return SPDR;
}

void tc72Init(void){
	cbi(PORTB,SS);
	/*Select Control Register*/
	spi_tx(0x80);
	/*Select Continuous temperature Conversion*/
	spi_tx(0x04);
	sbi(PORTB,SS);
	_delay_ms(150);
}

int main(void)
{
	char count=0,tc72_raw[1];
	short int tc72_temperature;
	DDRA = 0xff;
	
	PORTA = 0x00;
	
	/* Replace with your application code */
	spi_init();
	
	tc72Init();
	
	uart_init();	
	
	while (1)
	{
		cbi(PORTB,SS);
		
		spi_tx(0x02);
		
		spi_tx(0x00);
		
		sbi(PORTB,SS);
		
		//_delay_ms(1);
		
		tc72_raw[1]=spi_read();
		
		tc72_temperature = tc72_raw[1]; 
		
		if(tc72_temperature >= 200)
		{
			tc72_temperature = (256-tc72_temperature)*(-1);
			
			sprintf(str,"temp is %d",tc72_temperature);
		}
		
		else ;
		
		sprintf(str,"temp is %d",tc72_temperature);
		
		uart_txstr(str);
		//uart_tx('8');
		uart_tx('\r');
		
		//uart_tx(tc72_raw[1]);
		
		PORTA = tc72_raw[1];
		_delay_ms(1500);
		
		
	}
}


