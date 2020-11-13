
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
void set_rtc_time(void)
{
	I2CSendAddr(RTCADDR,WRITE);
	I2CSendByte(0x00);	 //address from which writing has to start
	I2CSendByte(0x00);	 //seconds
	I2CSendByte(0x23);   //minutes
	I2CSendByte(0x18);   //hr
	I2CSendByte(0x00);   //day
	I2CSendByte(0x23);   //date
	I2CSendByte(0x08);   //month
	I2CSendByte(0x20);   //year
	I2CSendByte(0x00);   //disable square wave output
	I2CSendStop(); 
}


void readrtc(unsigned int DISPLAY_TIME)
{
	I2CSendAddr(RTCADDR,WRITE); 
	I2CSendByte(0x00);
	I2CSendAddr(RTCADDR,READ); // command byte read temp
	Second_current 		= I2CGetByte(0x00); // will have seconds_ but not saved
	Minute_current 		= I2CGetByte(0x00); // will have minutes
	HR_current 			= I2CGetByte(0x00); // will have current hour
	Date_current 		= I2CGetByte(0x00); // will have day_ but not saved
	Date_current 		= I2CGetByte(0x00); // will have current date
	Month_current 		= I2CGetByte(0x00); // will have current month
	Year_current        = I2CGetByte(0x01); // will have current year
	//Year_current        = I2CGetByte(0x00);
	//Pumping_time[0]		= I2CGetByte(0x00);
	//Pumping_time[1]		= I2CGetByte(0x00);
	//Pumping_time[2]		= I2CGetByte(0x00);
	//Pumping_time[3]		= I2CGetByte(0x00);
	//Pumping_time[4]		= I2CGetByte(0x00);
	//Pumping_time[5]		= I2CGetByte(0x00);
	//Pumping_time[6]		= I2CGetByte(0x00);
	//Pumping_time[7]		= I2CGetByte(0x00);
	//Pumping_time[8]		= I2CGetByte(0x00);
	//Pumping_time[9]		= I2CGetByte(0x01);
	I2CSendStop(); //send stop	
		
	
	//LcdSend( 0x80, LCD_CMD );  // (x) = (0)
	//LcdSend( 0x44, LCD_CMD );  // (y) = (4)
	//Lcd_print_byte(count);
	
	if (DISPLAY_TIME == 1)
	{
	LcdSend( 0x80, LCD_CMD );  // (x) = (0)
	LcdSend( 0x45, LCD_CMD );  // (y) = (5)
	Lcd_print_byte(Date_current);
	Lcd_print_character('/');
	Lcd_print_byte(Month_current);
	Lcd_print_character('/');
	Lcd_print_byte(Year_current);
	Lcd_print_character(' ');
	Lcd_print_byte(HR_current);
	Lcd_print_character(':');
	Lcd_print_byte(Minute_current);
	//LcdSend( 0x80, LCD_CMD );  // (x) = (0)
	//LcdSend( 0x40, LCD_CMD );
	//Lcd_print_byte(Pumping_time[0]);
	//Lcd_print_byte(Pumping_time[1]);
	//Lcd_print_byte(Pumping_time[2]);
	//Lcd_print_byte(Pumping_time[3]);
	//Lcd_print_byte(Pumping_time[4]);
	//Lcd_print_byte(Pumping_time[5]);
	//Lcd_print_byte(Pumping_time[6]);
	//Lcd_print_byte(Pumping_time[7]);
	//Lcd_print_byte(Pumping_time[8]);
	//Lcd_print_byte(Pumping_time[9]);	
	}
	//count++;
	
}




