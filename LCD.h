#include "DEFINES.h"
#include "CONST.h"
#include <stdio.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <inttypes.h>
#include <avr/delay.h>
#include <avr/pgmspace.h>
extern unsigned char Year_current,Minute_current,Second_current,HR_current,Date_current,Month_current,count,adc_channel_number;
extern unsigned char Pumping_time[10],adc_complete_flag,key_pressed,key_input,x_point,y_point;
extern unsigned char Pumping_drtn[10];
extern unsigned int  adc_value;
extern unsigned char time_array[25],time_array_read[20],Pumping_duration_hour,Pumping_duration_minute;
extern unsigned char stop_at_minute,stop_at_hour,start_pumping,minute_incr,minute_count;
extern unsigned char Pumping_flag,Pumping_count,pending_run,Pumping_date,Pumping_month;
extern unsigned int  time_array_index;
extern unsigned char Next_scheduled_pumping_time_HR,Next_scheduled_pumping_time_Minute;
extern unsigned int  global_var1, pending_minutes;
extern unsigned long int total_available_minutes,time_ref;
extern unsigned char pending_hours,day_has_changed;
extern unsigned int  pqr,time_refr,for_debug;
extern unsigned char temp1,temp2,temp3,remaining_minutes,dsp_counter;	
extern unsigned char earlier_completed,with_in_minute,entry_stat;
extern unsigned char efg;

void print_options(void)
{
	Clear_LCD();
	Lcd_x_y_set(0,0);
	Lcd_print_sentance(message_1);
	Lcd_x_y_set(0,1);
	Lcd_print_sentance(message_2);
	Lcd_x_y_set(0,2);
	Lcd_print_sentance(message_3);
	Lcd_x_y_set(0,3);
	Lcd_print_sentance(message_4);
	Lcd_x_y_set(0,4);
	Lcd_print_sentance(message_5);
	Lcd_x_y_set(0,5);
	Lcd_print_sentance(message_6);
}

void idle_window(void)
{	
	//Lcd_x_y_set(0,0);
	//Lcd_print_character(0x30+global_var1);
	//Lcd_x_y_set(0,1);
	//Lcd_print_byte(Next_scheduled_pumping_time_HR);
	//Lcd_print_character(':');
	//Lcd_print_byte(Next_scheduled_pumping_time_Minute);
	LcdInit();	
	Delay();
	Lcd_x_y_set(0,0);
	Lcd_print_sentance(wel_note_1);
	//Lcd_print_sentance(device_type_1);
	//Lcd_print_character('F');
	//Lcd_print_character(0x30+for_debug);
	//Lcd_print_character('N');
	Lcd_x_y_set(0,1);
	Lcd_print_sentance(wel_note_2);
	//Lcd_print_sentance(device_type_2);
	//Lcd_print_character('F');
	//Lcd_print_character(0x30+time_refr);
	//Lcd_print_character('N');
	Lcd_x_y_set(0,2);
	Lcd_print_sentance(wel_note_3);
	
	
	Lcd_x_y_set(0,3);
	if((Next_scheduled_pumping_time_HR == 0x0) && (Next_scheduled_pumping_time_Minute == 0x0))
	{
		Lcd_print_sentance(error_message_1);
		system_error = 0x1;
	}
	else
	{
		system_error = 0x0;
		Lcd_print_sentance(wel_note_4);
		Lcd_print_byte(Next_scheduled_pumping_time_HR);
		Lcd_print_character(':');
		Lcd_print_byte(Next_scheduled_pumping_time_Minute);
	}
}


void show_pumping_status(void)
{
		Lcd_x_y_set(0,0);
		Lcd_print_sentance(pumping_stat_1); //prints "PUMPING STATUS"	
		Lcd_x_y_set(0,1);
		Lcd_print_sentance(pumping_stat_2); //prints "Pump is on ..."
		Lcd_x_y_set(0,2);
		Lcd_print_sentance(pumping_stat_3); //prints started time
		Lcd_print_character(' ');
		Lcd_print_byte(HR_current);
		Lcd_print_character(':');
		Lcd_print_byte(Minute_current);
		Lcd_print_character(' ');
		//Lcd_print_byte(Pumping_duration_hour);
		//Lcd_print_character(':');
		//Lcd_print_byte(Pumping_duration_minute);
		Lcd_x_y_set(0,3);
		Lcd_print_sentance(pumping_stat_4); //prints expected stopping time
		Lcd_print_character(' ');
		Lcd_print_byte(stop_at_hour);
		Lcd_print_character(':');
		Lcd_print_byte(stop_at_minute);		
		Lcd_x_y_set(0,4);
		Lcd_print_sentance(pumping_stat_6);
}


void Lcd_print_sentance(unsigned char array[])
{
	unsigned int j=0;
	
	while(pgm_read_byte(&array[j]) != '#')
	{
		Lcd_print_character(pgm_read_byte(&array[j]));
		j++;
	}
}

void Lcd_print_character (char ch)
{
  
	char chardata;
	int charpos;
	int char_row;
	
	
	if (ch < 0x20)return; 
	if (ch > 0x7f)return; 

	for (char_row=0;char_row<5;char_row++) 
	{      // 5 bytes 

		if (ch < 0x50)
		{
			charpos=(((ch&0xff)-0x20)*5);
			//charpos = 0;
			//chardata=TABLE5[(charpos+char_row)];
			chardata=pgm_read_byte(&TABLE5[(charpos+char_row)]);
			//key_input = pgm_read_byte(&KEYPAD1[(adc_chnl*4) + 3]);
		}            // use TABLE5 
    else if (ch > 0x4f)
		{
			charpos=(((ch&0xff)-0x50)*5);
			//chardata=TABLE6[(charpos+char_row)];
			chardata=pgm_read_byte(&TABLE6[(charpos+char_row)]);
		}            // use TABLE6 

		LcdSend( chardata,LCD_DATA );      // send data to nokia 
		//Delay();
	} 
	//Delay();
	LcdSend( 0x00,LCD_DATA ); 
	//Delay();
}


void Lcd_print_byte (char ch)
{

	unsigned char temp1;
	temp1 = ch >> 4;
	Lcd_print_character(temp1+0x30);
	temp1 = ch & 0X0F;
	Lcd_print_character(temp1+0x30);
}

void LcdInit ( void )
{
 	
	//  Set output bits on port B.
    DDRB |= LCD_RST_PIN | LCD_CE_PIN | SPI_MOSI_PIN | SPI_CLK_PIN;
	DDRD |= LCD_DC_PIN;
	
	//  Pull-up on reset pin.
    PORTB |= LCD_RST_PIN;


    Delay();

    //  Toggle display reset pin.
    PORTB &= ~LCD_RST_PIN;
    Delay();
    PORTB |= LCD_RST_PIN;

    //  Enable SPI port: No interrupt, MSBit first, Master mode, CPOL->0, CPHA->0, Clk/4
    SPCR = 0x53;

    //  Disable LCD controller
    PORTB |= LCD_CE_PIN;

    //PORTA = 0xAA;	
	
	LcdSend( 0x21, LCD_CMD );  // LCD Extended Commands.
	//Delay();
	//PORTA = 0x01;
    LcdSend( 0xC8, LCD_CMD );  // Set LCD Vop (Contrast).
	//Delay();
	//PORTA = 0x02;
    LcdSend( 0x06, LCD_CMD );  // Set Temp coefficent.
	//Delay();
	//PORTA = 0x03;	
    LcdSend( 0x13, LCD_CMD );  // LCD bias mode 1:48.
	//Delay();
	//PORTA = 0x04;
    LcdSend( 0x20, LCD_CMD );  // LCD Standard Commands, Horizontal addressing mode.
	//Delay();
	//PORTA = 0x05;
	LcdSend( 0x40, LCD_CMD );  // (y) = (0)
	//Delay();
	//PORTA = 0x06;
	LcdSend( 0x80, LCD_CMD );  // (x) = (0)
	//Delay();
	//PORTA = 0x07;
    LcdSend( 0x0C, LCD_CMD );  // LCD in normal mode.
	//Delay();
	//PORTA = 0x08;
}

void Clear_LCD(void)
{

	unsigned int i;
	for (i=0;i< 600;i++)
	{
		LcdSend( 0x00, LCD_DATA);
	}
}

void LcdSend ( byte data, LcdCmdData cd )
{
    //  Enable display controller (active low).
    PORTB &= ~LCD_CE_PIN;

    if ( cd == LCD_DATA )
    {
        PORTD |= LCD_DC_PIN;
    }
    else
    {
        PORTD &= ~LCD_DC_PIN;
    }

    //  Send data to display controller.
    SPDR = data;

    //  Wait until Tx register empty.
    while ( (SPSR & 0x80) != 0x80 );

    //  Disable display controller.
    PORTB |= LCD_CE_PIN;
}

void Lcd_x_y_set(unsigned char x,unsigned char y)
{
	LcdSend( 0x80 + x, LCD_CMD );
	LcdSend( 0x40 + y, LCD_CMD ); 
}

