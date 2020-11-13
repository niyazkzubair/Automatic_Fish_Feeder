#include "DEFINES.h"
//#include "NS_PUMPING.h"
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
void check_for_pending_runs(void)
{
	read_entry_status();
	time_refr = get_next_scheduled_pumping_time();	
	if (entry_stat != 'N')
	{
		//time_refr = get_next_scheduled_pumping_time();	
		//Lcd_x_y_set(6,4);
		//Lcd_print_character(' ');
		//Lcd_print_character(0x30+time_refr);
	
		if(time_refr == 0)
		{
			pqr = 4;
			while(Pumping_time[pqr*2] == 0x99)
			{
				pqr--;
			}
		//Lcd_x_y_set(0,2);
		//Lcd_print_character('&');
		
			if((Pumping_date == (Date_current - 1)) || ((Pumping_month != Month_current) && (Date_current == 0x01)))
			{
				day_has_changed = 1;
				//Lcd_x_y_set(0,2);
				//Lcd_print_character('*');
			}
		}
		else
		{
			pqr = time_refr - 1;
		//Lcd_x_y_set(0,2);
		//Lcd_print_character('(');
			get_start_date(pqr);
		}
	
		get_start_date(pqr);
		if((day_has_changed == 0) && (Pumping_date != Date_current))
		{
			write_pumping_count(pqr,0);
			write_pumping_flag(pqr,0x11);
		//Lcd_x_y_set(6,2);
		//Lcd_print_byte(Pumping_date);
		//Lcd_print_character(')');
		}
		else
		{
		//Lcd_x_y_set(6,2);
		//Lcd_print_character('_');
		;
		}
	//Lcd_x_y_set(6,2);
	//Lcd_print_character(' ');
	//Lcd_print_character(0x30+pqr);
		get_pumping_flag(pqr);
	//Lcd_print_character(' ');
	//Lcd_print_byte(Pumping_flag);
	
		if(Pumping_flag == 0x11)
		{
		
			//if((Next_scheduled_pumping_time_HR - HR_current) == 0x01)
			//{
			//Lcd_x_y_set(0,4);
			//Lcd_print_byte(BCD_DIFF(0x60,Minute_current));
			//	total_available_minutes =   bcd_to_integer(BCD_DIFF(0x60,Minute_current)) + bcd_to_integer(Next_scheduled_pumping_time_Minute);
			//total_available_minutes = 537;
			//	global_var1 = 1;
			//while(1)
			//{;}
			//}
			
			if((Next_scheduled_pumping_time_HR - HR_current) >= 0x01)
			{
				//total_available_minutes =   bcd_to_integer(BCD_DIFF(BCD_DIFF(Next_scheduled_pumping_time_HR,HR_current),0x1)) * 60 + bcd_to_integer(BCD_DIFF(0x60,Minute_current)) + bcd_to_integer(Next_scheduled_pumping_time_Minute);
				total_available_minutes =   bcd_to_integer(BCD_DIFF(Next_scheduled_pumping_time_HR,HR_current)) * 60 - bcd_to_integer(Minute_current) + bcd_to_integer(Next_scheduled_pumping_time_Minute);						
				global_var1 = 3;
			}	
			else if((Next_scheduled_pumping_time_HR < HR_current) || (Next_scheduled_pumping_time_Minute < Minute_current) )
			{
				total_available_minutes =   (bcd_to_integer(BCD_DIFF(0x24,HR_current))) * 60 + (bcd_to_integer(Next_scheduled_pumping_time_HR))* 60 - bcd_to_integer(Minute_current) + bcd_to_integer(Next_scheduled_pumping_time_Minute);
				global_var1 = 2;
			}
			//else if(Next_scheduled_pumping_time_HR == HR_current)	
			else
			{
				total_available_minutes = bcd_to_integer(BCD_DIFF(Next_scheduled_pumping_time_Minute,Minute_current));
				global_var1 = 4;
			}
		
		
	
			read_pumping_duration();
			get_pumping_count(pqr);
			pending_minutes =   bcd_to_integer(Pumping_drtn[pqr*2])* 60 + bcd_to_integer(Pumping_drtn[pqr*2+1]) - Pumping_count;
			
		
			if(total_available_minutes > pending_minutes)
			{
								
				pending_hours = pending_minutes / 60;	
				remaining_minutes = pending_minutes % 60;
				remaining_minutes = ((remaining_minutes / 10) & 0x0F) << 4 | remaining_minutes % 10;
				
				stop_at_minute = Minute_current + remaining_minutes;
				if((stop_at_minute & 0x0F) >= 0x0A)
				{
					temp1 = ( stop_at_minute & 0x0F ) - 0x0A;
					temp2 = (( stop_at_minute & 0xF0 ) >> 4) + 0x01;
					stop_at_minute = ((temp2 & 0x0F) << 4 ) | (temp1 & 0x0F);
				}
				temp3 = HR_current;
				if(stop_at_minute >= 0x60)
				{
					temp3 =  HR_current + 1;
					stop_at_minute = stop_at_minute - 0x60;
				}
				stop_at_hour = temp3 + pending_hours;
				if((stop_at_hour & 0x0F) >= 0x0A)
				{
					temp1 = ( stop_at_hour & 0x0F ) - 0x0A;
					temp2 = (( stop_at_hour & 0xF0 ) >> 4) + 0x01;
					stop_at_hour = ((temp2 & 0x0F) << 4 ) | (temp1 & 0x0F);
				}
				if(stop_at_hour >= 0x24)
				{
					stop_at_hour = stop_at_hour - 0x24;
				}			
				
			}
			else
			{
				stop_at_hour = Next_scheduled_pumping_time_HR;
				stop_at_minute = Next_scheduled_pumping_time_Minute;
			}
		
			pending_run = 0x0C;
			time_ref = pqr;
		//Lcd_print_character(' ');
		//Lcd_print_character(0x30 + pending_run);
			start_pumping = 0x01;
		
		}
	}
}

