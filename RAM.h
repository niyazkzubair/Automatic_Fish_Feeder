#include "DEFINES.h"
#include <stdio.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <inttypes.h>
#include <avr/delay.h>
#include <avr/pgmspace.h>


//Outputs will be staggered in accumulated manner			
//value 2000 corresponds to ~2secs		
#define TIME_CONTROL_0  2000 //pin number 20
#define TIME_CONTROL_1  1000 //pin number 19
#define TIME_CONTROL_2  1000 //pin number 17
#define TIME_CONTROL_3  1000 //pin number 16
#define TIME_CONTROL_4  1000 //pin number 15

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
void get_pumping_flag(unsigned int ref)
{
	I2CSendAddr(RTCADDR,WRITE); 
	I2CSendByte(0x17 + ref*4);
	I2CSendAddr(RTCADDR,READ); 
	Pumping_flag 		= I2CGetByte(0x01); 
	I2CSendStop();
}

void get_pumping_count(unsigned int ref)
{
	I2CSendAddr(RTCADDR,WRITE); 
	I2CSendByte(0x16+ref*4);
	I2CSendAddr(RTCADDR,READ); 
	Pumping_count 		= I2CGetByte(0x01); 
	I2CSendStop();
}

void get_pumping_duration(void)
{
	I2CSendAddr(RTCADDR,WRITE); 
	I2CSendByte(0x12);
	I2CSendAddr(RTCADDR,READ); 
	Pumping_duration_hour	 		= I2CGetByte(0x00); 
	Pumping_duration_minute	 		= I2CGetByte(0x01); 
	I2CSendStop();
}


void get_pumping_times(void)
{
	I2CSendAddr(RTCADDR,WRITE); 
	I2CSendByte(0x08);
	I2CSendAddr(RTCADDR,READ); // command byte read temp
	Pumping_time[0]		 		= I2CGetByte(0x00); 
	Pumping_time[1]		 		= I2CGetByte(0x00); 
	Pumping_time[2]				= I2CGetByte(0x00); 
	Pumping_time[3]		 		= I2CGetByte(0x00);
	Pumping_time[4]		 		= I2CGetByte(0x00); 
	Pumping_time[5]		 		= I2CGetByte(0x00); 
	Pumping_time[6]				= I2CGetByte(0x00); 
	Pumping_time[7]		 		= I2CGetByte(0x00);
	Pumping_time[8]				= I2CGetByte(0x00); 
	Pumping_time[9]		 		= I2CGetByte(0x01); 
	
	I2CSendStop(); //send stop	
}

void get_start_date(unsigned int ref)
{
	
	I2CSendAddr(RTCADDR,WRITE); 
	I2CSendByte(0x14+ref*4);
	I2CSendAddr(RTCADDR,READ); 
	Pumping_date 		= I2CGetByte(0x00); 
	Pumping_month 		= I2CGetByte(0x01); 
	I2CSendStop();
}


void read_pumping_duration(void)
{
	I2CSendAddr(RTCADDR,WRITE); 
	I2CSendByte(0x28);
	I2CSendAddr(RTCADDR,READ); // command byte read temp
	Pumping_drtn[0]		 		= I2CGetByte(0x00); 
	Pumping_drtn[1]		 		= I2CGetByte(0x00); 
	Pumping_drtn[2]				= I2CGetByte(0x00); 
	Pumping_drtn[3]		 		= I2CGetByte(0x00);
	Pumping_drtn[4]		 		= I2CGetByte(0x00); 
	Pumping_drtn[5]		 		= I2CGetByte(0x00); 
	Pumping_drtn[6]				= I2CGetByte(0x00); 
	Pumping_drtn[7]		 		= I2CGetByte(0x00);
	Pumping_drtn[8]				= I2CGetByte(0x00); 
	Pumping_drtn[9]		 		= I2CGetByte(0x01); 
	
	I2CSendStop(); //send stop	
}


void write_pumping_flag(unsigned int ref, unsigned char flag)
{
	I2CSendAddr(RTCADDR,WRITE);
	I2CSendByte(0x17 + ref*4);	
	I2CSendByte(flag);
	I2CSendStop(); 
}
void write_pumping_count(unsigned int ref, unsigned char cnt)
{
	I2CSendAddr(RTCADDR,WRITE);
	I2CSendByte(0x16 + ref*4);	
	I2CSendByte(cnt);
	I2CSendStop();
}


void write_all_pumping_duration(void)
{

	I2CSendAddr(RTCADDR,WRITE);
	I2CSendByte(0x28);	 
	I2CSendByte(0x00);
	I2CSendByte(0x01);
	I2CSendByte(0x00);
	I2CSendByte(0x01);
	I2CSendByte(0x00);
	I2CSendByte(0x01);
	I2CSendByte(0x00);
	I2CSendByte(0x01);
	I2CSendByte(0x00);
	I2CSendByte(0x01);
	I2CSendStop(); 
}

void write_start_date(unsigned int ref,unsigned char date_input,unsigned char month_input)
{
	I2CSendAddr(RTCADDR,WRITE);
	I2CSendByte(0x14+ref*4);	
	I2CSendByte(date_input);
	I2CSendByte(month_input);
	I2CSendStop();
}

void entry_flag(unsigned char flag)
{
	I2CSendAddr(RTCADDR,WRITE);
	I2CSendByte(0x39);	
	I2CSendByte(flag);
	I2CSendStop(); 
}

void read_entry_status(void)
{
	I2CSendAddr(RTCADDR,WRITE); 
	I2CSendByte(0x39);
	I2CSendAddr(RTCADDR,READ); 
	entry_stat 	= I2CGetByte(0x01); 
	I2CSendStop();
}

void write_all_pumping_times(void)
{
	I2CSendAddr(RTCADDR,WRITE);
	I2CSendByte(0x08);	 //address from which writing has to start
	
	I2CSendByte(0x07);	//pumping time 0; absolute HR
	I2CSendByte(0x00);	//pumping time 0; absolute minute
	
	I2CSendByte(0x07);	//pumping time 1
	I2CSendByte(0x02);
	
	I2CSendByte(0x07); //pumping time 2
	I2CSendByte(0x04);
	
	I2CSendByte(0x07); //pumping time 3
	I2CSendByte(0x06);
	
	I2CSendByte(0x07); //pumping time 4
	I2CSendByte(0x08);
	I2CSendStop(); 
}

void write_all_pumping_durations(void)
{
	I2CSendAddr(RTCADDR,WRITE);
	I2CSendByte(0x28);	 //address from which writing has to start
	
	I2CSendByte(0x00);	//pumping duration 0; absolute HR
	I2CSendByte(0x01);	//pumping duration 0; absolute minute
	
	I2CSendByte(0x00);	//pumping duration 1
	I2CSendByte(0x01);
	
	I2CSendByte(0x00); //pumping duration 2
	I2CSendByte(0x01);
	
	I2CSendByte(0x00); //pumping duration 3
	I2CSendByte(0x01);
	
	I2CSendByte(0x00); //pumping duration 4
	I2CSendByte(0x01);
	I2CSendStop();
}