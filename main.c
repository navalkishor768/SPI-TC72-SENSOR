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
#include <avr/interrupt.h>
#include "uartnaval.h"
#include "uartnaval.c"
#include "lcdgdheerajat8.h"
#include "lcdgdheerajat8.c"

#define MOSI		5
#define SCK			7
#define SS			4

unsigned char str[20],str2[10];
volatile int flag_1000ms=0;
volatile int flag_500ms=0;


void init_t2()
{
	TCCR2 = 0x07;			//	PRESCALAR AS 1024
	TCNT2 = 240;			//  initialize counter for 1 ms
	TIMSK |= (1 << TOIE2);	//	Unmask Timer 2 overflow interrupt.
}

ISR(TIMER2_OVF_vect)
{
	static unsigned int count_1000ms=0;
	static unsigned int count_500ms=0;
	
	if(count_500ms == 500)
	{
		flag_500ms = 1;
		count_500ms = 0;
	}
	
	if(count_1000ms == 1000)
	{
		flag_1000ms = 1;
		count_1000ms = 0;
	}
	

	count_500ms++;
	count_1000ms++;
	TCNT2 = 240;
}



void spi_init(void)
{
	DDRB = (1<<MOSI)|(1<<SCK)|(1<<SS);  // set pin direction output
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<CPHA);  // clock polarity set to 1, presaclar 128
}

void spi_tx(unsigned char data)
{
	//cbi(PORTB,SS);

	SPDR = data;			    // load data to transfer
	while(!(SPSR & (1<<SPIF)));	// wait till transfer finish
	
	//sbi(PORTB,SS);
}

char spi_read()
{	
	//cbi(PORTB,SS);
	//SPDR = tc72_read_address;
	while(!(SPSR & (1<<SPIF)));	// wait till transfer finish
	
	//sbi(PORTB,SS);
	
	 return SPDR;               // return received data
}

void tc72Init(void){
	
	cbi(PORTB,SS);             // chip enable, TC72 is CS pin is active high
	
	spi_tx(0x80);
	/*Select Continuous temperature Conversion*/
	spi_tx(0x04);
	sbi(PORTB,SS);				// chip enable, TC72 is CS pin is active high
	_delay_ms(150);
}

float get_tc72_temperature()
{	
	unsigned char tc72_raw[1];
	
	float tc72_measured;
	
	cbi(PORTB,SS);						// chip enable, TC72 is CS pin is active high
	
	spi_tx(0x01);						// address of MSB
	
	spi_tx(0x00);						// dummy flush to read with SCK cycle
	
	sbi(PORTB,SS);						// chip enable, TC72 is CS pin is active high
	
	tc72_raw[0]= spi_read();           // reading received data from spi buffer to char variable
	
	_delay_ms(10);
	
	cbi(PORTB,SS);						// chip enable, TC72 is CS pin is active high
	
	spi_tx(0x02);						// address of MSB
	
	spi_tx(0x00);						// dummy flush to read with SCK cycle
	
	sbi(PORTB,SS);						// chip enable, TC72 is CS pin is active high
	
	tc72_raw[1]=spi_read();             // reading received data from spi buffer to char variable
	
	tc72_measured = tc72_raw[1] + (tc72_raw[0]>>6)*0.25 ;     // converting received 8 bit to int
	
	return tc72_measured;
}

int main(void)
{
	float tc72_temperature;
	
	/* Replace with your application code */
	
	lcd_init(LCD_DISP_ON);
	_delay_ms(10);
	
	lcd_clrscr();
	lcd_putsxy(0,0,"Initializing");
	_delay_ms(100);
	
	spi_init();
	
	lcd_clrscr();
	lcd_putsxy(0,0,"Initializing");
	lcd_putsxy(0,1,"SPI Init");
	_delay_ms(100);
	
	tc72Init();
	
	lcd_clrscr();
	lcd_putsxy(0,0,"Initializing");
	lcd_putsxy(0,1,"TC72 Init");
	_delay_ms(100);
	
	uart_init(9600);
	
	lcd_clrscr();
	lcd_putsxy(0,0,"Initializing");
	lcd_putsxy(0,1,"UART Init");
	_delay_ms(100);
	
	init_t2();
	
	lcd_clrscr();
	lcd_putsxy(0,0,"Initializing");
	lcd_putsxy(0,1,"Timer2 Init");
	_delay_ms(100);
	
	sei();
	
	lcd_clrscr();
	
	
	while (1)
	{
		
		if(flag_500ms == 1)
		{
			tc72_temperature = get_tc72_temperature();
			flag_500ms = 0;
		}
		
		else if(flag_1000ms == 1)
		{
			 if(tc72_temperature >= 200)			// condition to print negative temperature values
			{
				tc72_temperature = (256.0-tc72_temperature)*(-1.0);
			
				dtostrf(tc72_temperature,3,2,str2);
			
				sprintf(str,"Temperature is %s",str2);
			}
		
			else 
			{	/* if temperature is positive, same will be printed,
				converting temperature to string format
				*/
		
				dtostrf(tc72_temperature,3,2,str2);
			
				sprintf(str,"Temperature is %s",str2);
			}
		
			uart_txstr(str);
			
			lcd_clrscr();
		 
			if(tc72_temperature < 0.0)
			{
				tc72_temperature = tc72_temperature*(-1);
			
				dtostrf(tc72_temperature,3,2,str2);
		
				lcd_putsxy(0,0,"Temperature");
				lcd_putsxy(0,1,"-");
				lcd_putsxy(1,1,str2);
				lcd_putsxy(8,1,"Celsius");
		
			}	
		
			else
			{
				dtostrf(tc72_temperature,3,2,str2);
				lcd_putsxy(0,0,"Temperature");
				lcd_putsxy(0,1,str2);
				lcd_putsxy(8,1,"Celsius");					
			}
			uart_tx('\r');
			
		
		}
		
		flag_1000ms = 0;
	}
		
		
}



