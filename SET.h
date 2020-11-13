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
extern unsigned char pumping_trig_sec_count,pumping_trig_en;

void set_rtc(void)
{
	Clear_LCD();
	x_point = 0;
	Lcd_x_y_set(x_point,0);
	Lcd_print_sentance(set_date_1);
	Lcd_x_y_set(x_point,1);
	Lcd_print_sentance(set_date_2);
	key_input = 'X';
	//Lcd_x_y_set(x_point,3);
	//Lcd_print_character(key_input);
		Lcd_x_y_set(6,2);
		time_array[0] = ((Date_current & 0xF0) >> 4 )+ 0x30;
		Lcd_print_character(time_array[0]);
		time_array[1] = (Date_current & 0x0F) + 0x30;
		Lcd_print_character(time_array[1]);
		Lcd_print_character(':');
		time_array[3] = ((Month_current >> 4) & 0x0F )+ 0x30;
		Lcd_print_character(time_array[3]);
		time_array[4] = (Month_current & 0x0F) + 0x30;
		Lcd_print_character(time_array[4]);
		Lcd_print_character(':');
		time_array[6] = ((Year_current >> 4) & 0x0F )+ 0x30;
		Lcd_print_character(time_array[6]);
		time_array[7] = (Year_current & 0x0F) + 0x30;
		Lcd_print_character(time_array[7]);
		
	y_point = 0x2;
	x_point = 0x6;
	//while(1)
	
	while(key_pressed == 1)
	{
			acquire_adc_channels();	
	}
	
	while (key_input != '#')
	{
			
		while(key_pressed != 1)
		{
			acquire_adc_channels();	
		}
		
		//if (key_pressed == 1)
		//{
		//	key_pressed = 0;
		//}
		
		if(key_input == '*')
		{
			if (x_point > 6)
			{
				if((x_point == 24) ||(x_point == 42))
				{
					x_point = x_point - 6;
				}
					x_point = x_point - 6;
					Lcd_x_y_set(x_point,y_point);
					Lcd_print_character('.');
			}
		}
		
		else if(key_input == '#')
		{
			break;
		}
		else
		{
		
			Lcd_x_y_set(x_point,y_point);
			time_array_index = x_point/6 - 1;
			time_array[time_array_index] = key_input;
			Lcd_print_character(time_array[time_array_index]);
			
			if(x_point < 48)
			{
				x_point = x_point+6;
				if(((x_point % 18) == 0))
				{	
					Lcd_x_y_set(x_point,y_point);
					Lcd_print_character(':');
					x_point = x_point+6;
				}
			}
			
		}
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		
		while(key_pressed != 0)
		{
			acquire_adc_channels();
		}
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		key_pressed = 0;
	}
	
			I2CSendAddr(RTCADDR,WRITE);
			I2CSendByte(0x00); // writing to CH bit
			I2CSendByte(0x00);
			I2CSendStop(); 
			
			I2CSendAddr(RTCADDR,WRITE);
			I2CSendByte(0x04);	 //address from which writing has to start
			I2CSendByte(((time_array[0] - 0x30) << 4 | (time_array[1] - 0x30)));   //date
			I2CSendByte(((time_array[3] - 0x30) << 4 | (time_array[4] - 0x30)));   //month
			I2CSendByte(((time_array[6] - 0x30) << 4 | (time_array[7] - 0x30)));   //year
			I2CSendStop(); 
	
	    readrtc(0); // read RTC without display
		
		Lcd_x_y_set(6,5);
		time_array[0] = ((HR_current >> 4) & 0x0F) + 0x30;
		Lcd_print_character(time_array[0]);
		time_array[1] = (HR_current & 0x0F) + 0x30;
		Lcd_print_character(time_array[1]);
		Lcd_print_character(':');
		time_array[3] = ((Minute_current >> 4) & 0x0F) + 0x30;
		Lcd_print_character(time_array[3]);
		time_array[4] = (Minute_current & 0x0F) + 0x30;
		Lcd_print_character(time_array[4]);
	key_input = 'n';
	x_point = 0;
	Lcd_x_y_set(x_point,3);
	Lcd_print_sentance(set_time_1);
	Lcd_x_y_set(x_point,4);
	Lcd_print_sentance(set_time_2);
	y_point = 0x5;
	x_point = 0x6;
	
	while(key_pressed == 1)
	{
			acquire_adc_channels();	
	}
	while (key_input != '#')
	{
		while(0)
		{
			key_pressed = 0;
			while(key_pressed == 1);
			key_pressed = 0;
		
			while(key_pressed != 0)
				;
	
			while(key_pressed != 1)
			{
				acquire_adc_channels();	
			}
		
			if (key_pressed == 1)
			{
				key_pressed = 0;
			}
		}
		
		while(key_pressed != 1)
		{
			acquire_adc_channels();	
		}
		
		
		
		if(key_input == '*')
		{
			if (x_point > 6)
			{
				if((x_point == 24))
				{
					x_point = x_point - 6;
				}
					x_point = x_point - 6;
					Lcd_x_y_set(x_point,y_point);
					Lcd_print_character('.');
			}
		}
		
		else if(key_input == '#')
		{
			break;
		}
		else
		{
		
			Lcd_x_y_set(x_point,y_point);
			time_array_index = x_point/6 - 1;
			time_array[time_array_index] = key_input;
			Lcd_print_character(time_array[time_array_index]);
			
			if(x_point < 30)
			{
				x_point = x_point+6;
				if(((x_point % 18) == 0))
				{	
					Lcd_x_y_set(x_point,y_point);
					Lcd_print_character(':');
					x_point = x_point+6;
				}
			}
			
		}
		
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		
		while(key_pressed != 0)
		{
			acquire_adc_channels();
		}
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		key_pressed = 0;
	}
	
			I2CSendAddr(RTCADDR,WRITE);
			I2CSendByte(0x01);	 //address from which writing has to start
			I2CSendByte(((time_array[3] - 0x30) << 4 | (time_array[4] - 0x30)));   //minutes
			I2CSendByte(((time_array[0] - 0x30) << 4 | (time_array[1] - 0x30)));   //hours
			I2CSendStop(); 
	
	key_pressed = 0;
	Delay();
	Delay();
	
	while(key_pressed != 1)
	{
		acquire_adc_channels();	
	}
	Clear_LCD();
}

void set_times(void)
{
	unsigned char time_ref = 0,tmp1,tmp2,tmp3;
	unsigned int ref_for_array;
	
	get_pumping_times();
	Clear_LCD();
	x_point = 0;
	Lcd_x_y_set(x_point,0);
	Lcd_print_sentance(set_timings_1);
	
	read_pumping_duration();
	
	for (time_ref = 0; time_ref<5; time_ref++)
	{
		Lcd_x_y_set(0,(time_ref + 1));
		Lcd_print_character(0x30+time_ref+1);
		Lcd_print_character(':');
		Lcd_print_character(' ');
	
		Lcd_x_y_set(18,(time_ref+1));
		ref_for_array = time_ref * 2;
		
		time_array_read[ref_for_array * 2] = ((Pumping_time[ref_for_array] & 0xF0) >> 4 )+ 0x30;
		Lcd_print_character(time_array_read[ref_for_array*2]);
		time_array_read[ref_for_array*2 + 1] = (Pumping_time[ref_for_array] & 0x0F) + 0x30;
		Lcd_print_character(time_array_read[ref_for_array*2 + 1]);
		Lcd_print_character(':');
		
		ref_for_array++;
		
		time_array_read[ref_for_array*2] = ((Pumping_time[ref_for_array] >> 4) & 0x0F )+ 0x30;
		Lcd_print_character(time_array_read[ref_for_array*2]);
		time_array_read[ref_for_array *2 +1] = (Pumping_time[ref_for_array] & 0x0F) + 0x30;
		Lcd_print_character(time_array_read[ref_for_array*2+ 1]);
		
		
				stop_at_minute = Pumping_time[ref_for_array] + Pumping_drtn[time_ref*2+1];
				if((stop_at_minute & 0x0F) >= 0x0A)
				{
					tmp1 = ( stop_at_minute & 0x0F ) - 0x0A;
					tmp2 = (( stop_at_minute & 0xF0 ) >> 4) + 0x01;
					stop_at_minute = ((tmp2 & 0x0F) << 4 ) | (tmp1 & 0x0F);
				}
				tmp3 = Pumping_time[ref_for_array - 1];
				if(stop_at_minute >= 0x60)
				{
					tmp3 =  Pumping_time[ref_for_array - 1] + 1;
					stop_at_minute = stop_at_minute - 0x60;
				}
				stop_at_hour = tmp3 + Pumping_drtn[time_ref*2];
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
		Lcd_print_character(' ');
		Lcd_print_character(((stop_at_hour>> 4) & 0x0F )+ 0x30);
		Lcd_print_character((stop_at_hour & 0x0F )+ 0x30);
		Lcd_print_character(':');
		Lcd_print_character(((stop_at_minute>> 4) & 0x0F )+ 0x30);
		Lcd_print_character((stop_at_minute & 0x0F )+ 0x30);

		y_point = time_ref+1;
		x_point = 18;
		
		time_array[time_ref*5] = time_array_read[time_ref*4];
		time_array[time_ref*5+1]= time_array_read[time_ref*4+1];
		time_array[time_ref*5+3] = time_array_read[time_ref*4+2];
		time_array[time_ref*5+4] = time_array_read[time_ref*4+3];	
		
		while(key_pressed == 1)
		{
			acquire_adc_channels();	
		}
		
		while (key_input != '#')
		{
			while(key_pressed != 1)
			{
				acquire_adc_channels();	
			}
		
			if(key_input == '*')
			{
				if (x_point > 6)
				{
					if((x_point == 36))
					{
						x_point = x_point - 6;
					}
					x_point = x_point - 6;
					Lcd_x_y_set(x_point,y_point);
					Lcd_print_character('_');
					Lcd_x_y_set(x_point,y_point+1);
					Lcd_print_character(' ');
				}
			}
		
			else if(key_input == '#')
			{
				break;
			}
			else
			{
		
				Lcd_x_y_set(x_point,y_point);
				time_array_index = ((x_point- 12)/6 - 1) + time_ref *5;
				time_array[time_array_index] = key_input;
				Lcd_print_character(time_array[time_array_index]);
				Lcd_x_y_set(x_point,y_point+1);
				//Lcd_print_character(time_array_index+0x30);
				Lcd_print_character('*');
			
				if(x_point < 42)
				{
					x_point = x_point+6;
					if(((x_point % 30) == 0))
					{	
						Lcd_x_y_set(x_point,y_point);
						Lcd_print_character(':');
						x_point = x_point+6;
					}
				}
			
			}
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
		
			while(key_pressed != 0)
			{
				acquire_adc_channels();
			}
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			key_pressed = 0;
		}
		
		while(key_pressed == 1)
		{
			acquire_adc_channels();	
		}
		
		key_input = 'n';
	}
	
	I2CSendAddr(RTCADDR,WRITE);
	I2CSendByte(0x08);	 //address from which writing has to start
	I2CSendByte(((time_array[0] - 0x30) << 4 | (time_array[1] - 0x30)));   //date
	I2CSendByte(((time_array[3] - 0x30) << 4 | (time_array[4] - 0x30)));   //month
	I2CSendByte(((time_array[5] - 0x30) << 4 | (time_array[6] - 0x30)));   
	I2CSendByte(((time_array[8] - 0x30) << 4 | (time_array[9] - 0x30))); 
	I2CSendByte(((time_array[10] - 0x30) << 4 | (time_array[11] - 0x30)));   
	I2CSendByte(((time_array[13] - 0x30) << 4 | (time_array[14] - 0x30))); 	
	I2CSendByte(((time_array[15] - 0x30) << 4 | (time_array[16] - 0x30)));  
	I2CSendByte(((time_array[18] - 0x30) << 4 | (time_array[19] - 0x30))); 
	I2CSendByte(((time_array[20] - 0x30) << 4 | (time_array[21] - 0x30)));   
	I2CSendByte(((time_array[23] - 0x30) << 4 | (time_array[24] - 0x30))); 
	I2CSendStop(); 
	for(tmp1 = 0;tmp1<5;tmp1++)
	{
		write_pumping_flag(tmp1,0x22);
		write_start_date(tmp1,0x00,0x00);
		write_pumping_count(tmp1,0x00);	
	}
		entry_flag('N');
		Clear_LCD();
}






void set_duration(void)
{	
	unsigned char time_ref = 0,tmp1,tmp2,tmp3;
	unsigned int ref_for_array;
	
	read_pumping_duration();
	Clear_LCD();
	x_point = 0;
	Lcd_x_y_set(x_point,0);
	Lcd_print_sentance(set_duration_1);
	
	for (time_ref = 0; time_ref<5; time_ref++)
	{
		Lcd_x_y_set(0,(time_ref + 1));
		Lcd_print_character(0x30+time_ref+1);
		Lcd_print_character(':');
		Lcd_print_character(' ');
	
		Lcd_x_y_set(18,(time_ref+1));
		ref_for_array = time_ref * 2;
		
		time_array_read[ref_for_array * 2] = ((Pumping_drtn[ref_for_array] & 0xF0) >> 4 )+ 0x30;
		Lcd_print_character(time_array_read[ref_for_array*2]);
		time_array_read[ref_for_array*2 + 1] = (Pumping_drtn[ref_for_array] & 0x0F) + 0x30;
		Lcd_print_character(time_array_read[ref_for_array*2 + 1]);
		Lcd_print_character(':');
		
		ref_for_array++;
		
		time_array_read[ref_for_array*2] = (( Pumping_drtn[ref_for_array]>> 4) & 0x0F )+ 0x30;
		Lcd_print_character(time_array_read[ref_for_array*2]);
		time_array_read[ref_for_array *2 +1] = (Pumping_drtn[ref_for_array] & 0x0F) + 0x30;
		Lcd_print_character(time_array_read[ref_for_array*2+ 1]);

		y_point = time_ref+1;
		x_point = 18;
		
		time_array[time_ref*5] = time_array_read[time_ref*4];
		time_array[time_ref*5+1]= time_array_read[time_ref*4+1];
		time_array[time_ref*5+3] = time_array_read[time_ref*4+2];
		time_array[time_ref*5+4] = time_array_read[time_ref*4+3];	
		
		while(key_pressed == 1)
		{
			acquire_adc_channels();	
		}
		while (key_input != '#')
		{
			while(key_pressed != 1)
			{
			acquire_adc_channels();	
			}
		
			if(key_input == '*')
			{
				if (x_point > 6)
				{
					if((x_point == 36))
					{
						x_point = x_point - 6;
					}
					x_point = x_point - 6;
					Lcd_x_y_set(x_point,y_point);
					Lcd_print_character('_');
					Lcd_x_y_set(x_point,y_point+1);
					Lcd_print_character(' ');
				}
			}
		
			else if(key_input == '#')
			{
				break;
			}
			else
			{
		
				Lcd_x_y_set(x_point,y_point);
				time_array_index = ((x_point- 12)/6 - 1) + time_ref *5;
				time_array[time_array_index] = key_input;
				Lcd_print_character(time_array[time_array_index]);
				Lcd_x_y_set(x_point,y_point+1);
				//Lcd_print_character(time_array_index+0x30);
				Lcd_print_character('*');
			
				if(x_point < 42)
				{
					x_point = x_point+6;
					if(((x_point % 30) == 0))
					{	
						Lcd_x_y_set(x_point,y_point);
						Lcd_print_character(':');
						x_point = x_point+6;
					}
				}
			
			}
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
		
			while(key_pressed != 0)
			{
				acquire_adc_channels();
			}
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			Delay();
			key_pressed = 0;
			Delay();
			Delay();
			
			
		}
		while(key_pressed == 1)
		{
			acquire_adc_channels();	
		}
		key_input = 'n';
	}
	
	I2CSendAddr(RTCADDR,WRITE);
	I2CSendByte(0x28);	 //address from which writing has to start
	I2CSendByte(((time_array[0] - 0x30) << 4 | (time_array[1] - 0x30)));   //hour
	I2CSendByte(((time_array[3] - 0x30) << 4 | (time_array[4] - 0x30)));   //minute
	I2CSendByte(((time_array[5] - 0x30) << 4 | (time_array[6] - 0x30)));   
	I2CSendByte(((time_array[8] - 0x30) << 4 | (time_array[9] - 0x30))); 
	I2CSendByte(((time_array[10] - 0x30) << 4 | (time_array[11] - 0x30)));   
	I2CSendByte(((time_array[13] - 0x30) << 4 | (time_array[14] - 0x30))); 	
	I2CSendByte(((time_array[15] - 0x30) << 4 | (time_array[16] - 0x30)));  
	I2CSendByte(((time_array[18] - 0x30) << 4 | (time_array[19] - 0x30))); 
	I2CSendByte(((time_array[20] - 0x30) << 4 | (time_array[21] - 0x30)));   
	I2CSendByte(((time_array[23] - 0x30) << 4 | (time_array[24] - 0x30))); 
	I2CSendStop(); 
	Clear_LCD();
}

void pumping_trig_en1(void)
{
	if(pumping_trig_en == 1) 
	{
		PORTB = PORTB | (PUMPING_START_TRIG);
		PORTB = PORTB & (~PUMPING_END_TRIG);
		pumping_trig_sec_count++;
	}
}

void pumping_trig_en0(void)
{
	if(pumping_trig_en == 0)
	{
		pumping_trig_en = 1;
		pumping_trig_sec_count = 0;
	}
}

