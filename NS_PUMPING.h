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
unsigned int get_next_scheduled_pumping_time(void)
{
		unsigned char got_one = 0, got_scheduled_time_in_this_hour = 0,HR_to_scheduled_times[5];
		unsigned char Minute_to_scheduled_times[5],finished_search = 0;
		char temp1,temp2,temp3;
		unsigned int ij,time_reference = 0;
		
		get_pumping_times();
		
		readrtc(0);
		x_point = 6;
		got_one = 6;
		temp1 = 0;
		//Lcd_x_y_set(6,0);
		for(ij = 0;ij<5;ij++)
		{
			temp1++;
			if(((Pumping_time[ij*2] & 0xF0) >> 4) == 0x9)
			{
				HR_to_scheduled_times[ij] = 0x99;
				Minute_to_scheduled_times[ij] = 0x99;
				continue;
			}
						
			if((Pumping_time[ij*2] == HR_current) &&((Pumping_time[ij*2+1]) > Minute_current))
			{
				HR_to_scheduled_times[ij] = 0x00;
				Minute_to_scheduled_times[ij] = BCD_DIFF(Pumping_time[ij*2+1],Minute_current);
			}
			else if(Pumping_time[ij*2] > HR_current)
			{
				HR_to_scheduled_times[ij] = BCD_DIFF(BCD_DIFF(Pumping_time[ij*2],HR_current),0x01);
				Minute_to_scheduled_times[ij] = BCD_ADD(BCD_DIFF(0x60,Minute_current),Pumping_time[ij*2+1]);
			}
			else
			{
				HR_to_scheduled_times[ij] = BCD_DIFF(BCD_ADD(BCD_DIFF(0x24,HR_current),Pumping_time[ij*2]),0x01);
				Minute_to_scheduled_times[ij] = BCD_ADD(BCD_DIFF(0x60,Minute_current),Pumping_time[ij*2+1]);
			}
			
			
			if(Minute_to_scheduled_times[ij] > 0x59)
			{
				HR_to_scheduled_times[ij] = HR_to_scheduled_times[ij] + 0x01;
				Minute_to_scheduled_times[ij] = Minute_to_scheduled_times[ij] - 0x60;
			}
			//Lcd_x_y_set(0,temp1 - 1);
			//Lcd_print_byte(HR_to_scheduled_times[ij]);
			//Lcd_print_byte(Minute_to_scheduled_times[ij]);
			
		}
		
		temp1 = 0x70;
		temp2 = 0x25;
		temp3 = 0;
		//Lcd_x_y_set(0,2);
		got_one = 0;
		for(ij = 0;ij < 5;ij++)
		{
			if(HR_to_scheduled_times[ij] == 0x99)
			{
				ij = 8;
				finished_search = 0x1;
			}
			
			if (finished_search == 0x0)
			{
				if(HR_to_scheduled_times[ij] == 0)
				{
					if(Minute_to_scheduled_times[ij] > 0)
					{
					
					if(Minute_to_scheduled_times[ij] < temp1)
						{
							temp1 = Minute_to_scheduled_times[ij];
							Next_scheduled_pumping_time_HR = Pumping_time[ij*2];
							Next_scheduled_pumping_time_Minute = Pumping_time[ij*2+1];
							got_scheduled_time_in_this_hour = 1;
							time_reference = ij;
						}
					}
				}
				else if((got_scheduled_time_in_this_hour == 0) && (HR_to_scheduled_times[ij] < 0x80))
				{
					
					if(HR_to_scheduled_times[ij] < temp2)
					{
						temp2 = HR_to_scheduled_times[ij];
						Next_scheduled_pumping_time_HR = Pumping_time[ij*2];
						Next_scheduled_pumping_time_Minute = Pumping_time[ij*2+1];
						got_one = 1;
						time_reference = ij;
					}
				}
				else if((got_scheduled_time_in_this_hour == 0) && (got_one == 0))
				{
				
				
					if(HR_to_scheduled_times[ij] > temp3)
					{
						temp3 = HR_to_scheduled_times[ij];
						Next_scheduled_pumping_time_HR = Pumping_time[ij*2];
						Next_scheduled_pumping_time_Minute = Pumping_time[ij*2+1];
						time_reference = ij;
					}
				}
			}
		}
		return (time_reference);
}



