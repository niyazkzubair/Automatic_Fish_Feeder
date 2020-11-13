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
void pumping_control(void)
{
	unsigned int j;
	unsigned char tmp1,tmp2;
	get_pumping_times();
	
	if((start_pumping == 0x00) || (earlier_completed == 0x01))
	{
		earlier_completed = 0x00;
		for(j=0;j<5;j++)
		{
			if(HR_current == Pumping_time[j*2])
			{
				if(Minute_current == Pumping_time[(j*2)+1])
				{
					read_pumping_duration();
					stop_at_minute = Minute_current + Pumping_drtn[j*2+1];
					if((stop_at_minute & 0x0F) >= 0x0A)
					{
						tmp1 = ( stop_at_minute & 0x0F ) - 0x0A;
						tmp2 = (( stop_at_minute & 0xF0 ) >> 4) + 0x01;
						stop_at_minute = ((tmp2 & 0x0F) << 4 ) | (tmp1 & 0x0F);
					}
					if(stop_at_minute >= 0x60)
					{
						HR_current = HR_current + 1;
						stop_at_minute = stop_at_minute - 0x60;
					}
					stop_at_hour = HR_current + Pumping_drtn[j*2];
					if((stop_at_hour & 0x0F) >= 0x0A)
					{
						tmp1 = ( stop_at_hour & 0x0F ) - 0x0A;
						tmp2 = (( stop_at_hour & 0xF0 ) >> 4) + 0x01;
						stop_at_hour = ((tmp2 & 0x0F) << 4 ) | (tmp1 & 0x0F);
					}
					if(stop_at_hour >= 0x24)
					{
						stop_at_hour = stop_at_hour - 0x24;
					}
					start_pumping = 0x01;
				
					write_start_date(j,Date_current,Month_current);
					write_pumping_count(j,0); //scheduled start
					write_pumping_flag(j,0x11);				
					time_ref = j;
					break;
				}	
			}
		}
		
	}
	
	/////else if(((start_pumping == 0x01) ||(pending_run != 0x00)) && ((HR_current > 0x17) || (HR_current < 0x06)))
	//////{
	///////	start_pumping = 0;
	//////	pending_run = 0x00;
	//////	write_pumping_flag(time_ref,0x22);
	//////	write_pumping_count(time_ref,0x00);
	//////	entry_flag('O');
	//////	efg = get_next_scheduled_pumping_time();
	///////}
	
	else if((start_pumping == 0x01) || (pending_run != 0x00))
	{
		Lcd_x_y_set(60,4);
		Lcd_print_character('#');
		//Lcd_print_character(0x30+pending_run);
		
		if(pending_run == 0x0C)
		{
			Lcd_x_y_set(72,4);
			Lcd_print_sentance(pumping_stat_5);
			pending_run = 0x00;
			tmp1 = 1;
		}
				
		write_start_date(time_ref,Date_current,Month_current);
		//Clear_LCD();
		x_point = 0;
				
		//if(DEBUG)
		//{
			//Lcd_x_y_set(0,4);
			//Lcd_print_character(total_available_minutes/100 + 0x30);
			//total_available_minutes = total_available_minutes % 100;
			//Lcd_print_character(total_available_minutes/10 + 0x30);
			//total_available_minutes = total_available_minutes % 10;
			//Lcd_print_character(total_available_minutes + 0x30);
			//Lcd_print_character(' ');
			//Lcd_print_character(pending_minutes/100 + 0x30);
			//pending_minutes = pending_minutes % 100;
			//Lcd_print_character(pending_minutes/10 + 0x30);
			//pending_minutes = pending_minutes % 10;
			//Lcd_print_character(pending_minutes + 0x30);
			//Lcd_print_character(' ');
			//Lcd_print_character(global_var1 + 0x30);
			//Lcd_print_character(' ');
		//}
		
		show_pumping_status();

		
		minute_incr = Minute_current;
		get_pumping_count(time_ref);
		Lcd_x_y_set(36,4);
		Lcd_print_character(0x30 + (Pumping_count/10));
		Lcd_print_character(0x30 + (Pumping_count%10));
		Lcd_print_character(' ');
		Lcd_print_character(0x31 + time_ref);
		if(time_ref == 0x00)
		{
			Lcd_print_character('S');
			Lcd_print_character('T');
		}
		else if (time_ref == 0x02)
		{
			Lcd_print_character('R');
			Lcd_print_character('D');
		}
		else if (time_ref == 0x01)
		{
			Lcd_print_character('N');
			Lcd_print_character('D');
		}
		else 
		{
			Lcd_print_character('T');
			Lcd_print_character('H');
		}
		minute_count= Pumping_count;
		
		    if(HR_current == stop_at_hour)
			{
				if(Minute_current == stop_at_minute)
				{
					with_in_minute = 0x01;
				}
			}
			
		while(start_pumping == 0x01 && (Pumping_count < 120))
		{
			entry_flag('O'); /// entry has become old now.
			readrtc(1);
////////////// INCLUDED ONLY FOR DEBUG /////////////////

			key_pressed = 0;
			acquire_adc_channels();		
			if(key_pressed == 1)
			{			
				key_pressed = 0;
				call_menu();
				goto end_pumping_routine;
				//show_pumping_status();
				//if(tmp1 == 1)
				//{
				//	Lcd_x_y_set(66,4);
				//	Lcd_print_sentance(pumping_stat_5);
				//}
			}
///////////////////////////////////////////////////////////



						Lcd_x_y_set(0,1);
						if((dsp_counter % 2) == 0)
						{
							Lcd_print_sentance(pumping_stat_2); //prints "Pump is on ..."
						}
						Lcd_x_y_set(0,1);
						if((dsp_counter % 4) == 0)
						{
							Lcd_print_sentance(pumping_stat_7); //print blank 
							PORTC = ~PORTC;
						}
		
						dsp_counter++;


			PORTA = PORTA | (OUTPUT_PIN);	
			PORTA = PORTA | (LED_OUT);
			
			
			if(minute_incr != Minute_current)
			{
				minute_incr = Minute_current;
				minute_count++;
				write_pumping_count(time_ref,minute_count);
				get_pumping_count(time_ref);
				Lcd_x_y_set(36,4);
				Lcd_print_character(0x30 + (Pumping_count/10));
				Lcd_print_character(0x30 + (Pumping_count%10));
				Lcd_print_character(' ');
				Lcd_print_character(0x31 + time_ref);
				//Lcd_x_y_set(48,2);
				//Lcd_print_character('$');
				//Lcd_print_character(0x30+time_ref);
				
				earlier_completed = 0x01;
				if(HR_current == stop_at_hour)
				{
					if(Minute_current == stop_at_minute)
					{
						PORTA = PORTA & (~OUTPUT_PIN);
						PORTA = PORTA & (~LED_OUT);
						start_pumping = 0x00;
						write_pumping_flag(time_ref,0x22);
						write_pumping_count(time_ref,0x00);
					}
				}
				if(with_in_minute == 0x01)
				{
						PORTA = PORTA & (~OUTPUT_PIN);
						PORTA = PORTA & (~LED_OUT);
						start_pumping = 0x00;
						write_pumping_flag(time_ref,0x22);
						with_in_minute = 0x00;
						write_pumping_count(time_ref,0x00);
				}
				//Lcd_x_y_set(x_point,1);
				//Lcd_print_character(0x30 + time_ref);//1
				//Lcd_print_character(' ');
				//get_pumping_flag(time_ref);
				//Lcd_print_byte(Pumping_flag);//2
				//Lcd_print_character(' ');
				//get_pumping_count(time_ref);
				//Lcd_print_byte(Pumping_count);//3
				
			}
		}
end_pumping_routine:
		Clear_LCD();
		Clear_LCD();
		start_pumping = 0x00;
		with_in_minute = 0x00;
		j = get_next_scheduled_pumping_time();
	}
	
}

