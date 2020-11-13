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
unsigned char BCD_DIFF(unsigned char num1, unsigned char num2)
{
	unsigned char t1,t2;
	t1 = ((num1 & 0xF0) >> 4)*10 + (num1 &0x0F);
	t2 = ((num2 & 0xF0) >> 4)*10 + (num2 &0x0F);
	if (t1>=t2)
	{
		t1 = t1-t2;
		t1 =  ((t1/10)<< 4)+ t1%10 ;
	}
	else
	{
		t1 = t2-t1;
		t1 =  ((t1/10)<< 4)+ t1%10 ;
		t1 = t1 |0x80;
	}
	return (t1);
}



unsigned char BCD_ADD(unsigned char num1, unsigned char num2)
{
	unsigned char t1,t2;
	t1 = ((num1 & 0xF0) >> 4)*10 + (num1 &0x0F);
	t2 = ((num2 & 0xF0) >> 4)*10 + (num2 &0x0F);
	t1 = t1+t2;
	t1 =  ((t1/10)<< 4)+ t1%10 ;
	return (t1);
}


unsigned int bcd_to_integer(unsigned char bcd)
{
	return (((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F));
}

