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

void spi_init()
{
	DDRB = (1<<MOSI)|(1<<SCK)|(1<<SS);  // set pin direction output
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void max7221_tx(unsigned char max7221_cmd, unsigned char dec_num)
{
	//cbi(PORTB,SS);
	PORTB &= ~(1<<SS);
	
	SPDR = max7221_cmd;			// decimal select for data write
	while(!(SPSR & (1<<SPIF)));	// wait till transfer finish
	
	SPDR = dec_num;			// write decimal data
	while(!(SPSR & (1<<SPIF)));	// wait till transfer finish
	
	//sbi(PORTB,SS);
	PORTB |= (1<<SS);
}

void spi_tx(unsigned char data)
{
	//cbi(PORTB,SS);
	PORTB &= ~(1<<SS);

	SPDR = data;			// write decimal data
	while(!(SPSR & (1<<SPIF)));	// wait till transfer finish
	
	//sbi(PORTB,SS);
	PORTB |= (1<<SS);
}

unsigned char spi_read(unsigned char data)
{	
	unsigned char rawread;
	//cbi(PORTB,SS);
	PORTB &= ~(1<<SS);
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));	// wait till transfer finish
	rawread = SPDR;
	//sbi(PORTB,SS);
	PORTB |= (1<<SS);
	
	 return rawread;
}

int main(void)
{
	unsigned char count=0,tc72_raw[1];
	
	
	/* Replace with your application code */
	spi_init();
	
	uart_init();
	
	spi_tx(0x00);
	
	spi_tx(0x80);
	
	spi_tx(0x02);
	
	while (1)
	{
		//spi_tx(0x02);
		
		tc72_raw[0]=spi_read(0x02);
		
		uart_tx('8');
		uart_tx('\r');
		
		uart_tx(tc72_raw[0]);
		
		_delay_ms(1500);
		
		
	}
}


