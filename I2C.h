#include "DEFINES.h"
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
extern unsigned int  pqr,time_refr;
extern unsigned char temp1,temp2,temp3,remaining_minutes,dsp_counter;	
extern unsigned char earlier_completed,with_in_minute,entry_stat;
extern unsigned char efg;
void I2CSendAddr(unsigned char addr, unsigned char rd)
{	
	unsigned char temp1;
	GenStart();
	TWDR = addr + rd;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	if ((rd == 0x00) && ((TWSR & 0xF8) != MT_ADDR_ACK))
	{
		I2CSendStop();
		if(DEBUG)
		{
			Lcd_print_character (' ');
			Lcd_print_character ('M');	
			Lcd_print_character ('T');
			Lcd_print_character (' ');
			temp1= TWSR & 0xF8;
			temp1=temp1 >> 4;
			Lcd_print_character (temp1+0x30);
			temp1= TWSR & 0xF8;
			temp1 = temp1 &0x0F;
			Lcd_print_character (temp1+0x30);
			Lcd_print_character ('E');
		}
		
		while(1)
		{
			;
		}
	}

	if ((rd == 0x01) && ((TWSR & 0xF8) != MR_ADDR_ACK))
	{
		I2CSendStop();
		if(DEBUG)
		{
			Lcd_print_character (' ');
			Lcd_print_character ('M');
			Lcd_print_character ('R');
		}
		while(1)
		{
			;
		}
	}
	if(DEBUG)
	{
		Lcd_print_character ('S');
		Lcd_print_character ('A');
	}		
}

void GenStart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)))
	{
		if(DEBUG)
		{
			Lcd_print_character (' ');
			Lcd_print_character ('S');
			Lcd_print_character ('t');
		}
	}
}

void I2CSendByte(unsigned char data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) != MT_DATA_ACK)
	{
		I2CSendStop();
		if(DEBUG)
		{
			Lcd_print_character (' ');
			Lcd_print_character ('M');
			Lcd_print_character ('T');
		}
		
		while(1)
		{
			;
		}
	}	
	if(DEBUG)
	{
		Lcd_print_character ('S');
		Lcd_print_character ('B');
	}
}

unsigned char I2CGetByte(unsigned char nack)
{
	unsigned char read_data;
	if (nack == 0x00)
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	}
	else
	{
		TWCR = (1<<TWINT)|(1<<TWEN);
	}
		
	while (!(TWCR & (1<<TWINT)));
	
	while((TWSR & 0xF8) == 0xF8);
		
	if ((nack == 0x00) && ((TWSR & 0xF8) != MR_DATA_ACK))
	{
		I2CSendStop();
		if(DEBUG)
		{
			Lcd_print_character (' ');
			Lcd_print_character ('C');
		}
		while(1)
		{
			;
		}
	}
	
	if ((nack == 0x01) && ((TWSR & 0xF8) != MR_DATA_NACK))
	{
		I2CSendStop();
		if(DEBUG)
		{
			Lcd_print_character (' ');
			Lcd_print_character ('c');
		}
		
		while(1)
		{
			;
		}
	}
	read_data = TWDR;
	if(DEBUG)
	{
		Lcd_print_character ('G');
		Lcd_print_character ('B');
	}
	return (read_data);
}
	
void I2CSendStop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	if(DEBUG)
	{
		Lcd_print_character ('S');
		Lcd_print_character ('P');
	}
}

