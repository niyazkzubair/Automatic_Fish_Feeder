//#include <macros.h>
//#include <iom8v.h>
// 17-08-2020  	:	Initial fish feeder file (base file taken from APC)
// 19-08-2020	:	Added FEEDER_MOTOR_OUT_0/1/2/3/4 outputs with staggered sec outputs
#include <stdio.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <inttypes.h>
#include <avr/delay.h>
#include <avr/pgmspace.h>
#include "VARIABLES.h"
#include "DEFINES.h"
#include "BCD.h"
#include "LCD.h"
#include "I2C.h"
#include "ADC.h"
#include "RAM.h"
#include "SET.h"
#include "RTC.h"
//#include "PUMPING_CTRL.h"
//#include "PR_CHECK.h"
//#include "NS_PUMPING.h"



extern unsigned char Year_current,Minute_current,Minute_started,Second_current,HR_current,Date_current,Month_current,count,adc_channel_number;
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
extern unsigned char efg,HR_restricted_lower,HR_restricted_upper,system_error;
//extern unsigned char pumping_trig_sec_count,pumping_trig_en;

int main (void)
{
	unsigned char ijk,lmn = 0,sec,dead_zone_flag=0x0;
	unsigned int count;
	unsigned int program_rtc;
	

	//DDRA = 0xFF;
	//PORTA = 0x3B;
	
	//while(1)
	//{
	//	PORTA = 0xAC;
	//}
	
	DDRA = DDRA | OUTPUT_PIN;
	DDRA = DDRA | LED_OUT;
	DDRA = DDRA | FIVE_LEDS; //5 LED outputs for each pumping time
	DDRB = DDRB | PUMPING_START_TRIG | PUMPING_END_TRIG;
	DDRC = DDRC | SYSTEM_ALIVE;
	DDRD = DDRD | FEEDER_MOTOR_OUT_0;
	DDRD = DDRD | FEEDER_MOTOR_OUT_1;
	DDRD = DDRD | FEEDER_MOTOR_OUT_2;
	DDRD = DDRD | FEEDER_MOTOR_OUT_3;
	DDRD = DDRD | FEEDER_MOTOR_OUT_4;
	
	PORTA = PORTA & (~OUTPUT_PIN); // keep pump off at start
	PORTA = PORTA & (~LED_OUT);
	PORTA = PORTA & (~FIVE_LEDS);
	PORTB = PORTB & (~(PUMPING_START_TRIG |PUMPING_END_TRIG));
	PORTD = PORTD & (~FEEDER_MOTOR_OUT_0);
	PORTD = PORTD & (~FEEDER_MOTOR_OUT_1);
	PORTD = PORTD & (~FEEDER_MOTOR_OUT_2);
	PORTD = PORTD & (~FEEDER_MOTOR_OUT_3);
	PORTD = PORTD & (~FEEDER_MOTOR_OUT_4);
	
	/* Global Interrupt Enable*/
	SREG = SREG | (0x80);
	TWBR = 10;                    /* smallest TWBR value*/
	TWBR = 5;
	count = 0;		
	for ( ijk =1; ijk<10; ijk++)
	{
		Delay();
	}
	

	LcdInit();	
	Clear_LCD();
	Delay();

	
	//-----------USEFUL FOR DEBUG---------------------------------------------------
	// Setting pumping times from Software -- for easy debug
	//I2CSendAddr(RTCADDR,WRITE);
	//I2CSendByte(0x08);	 //address from which writing has to start
	//I2CSendByte(0x22);
	//I2CSendByte(0x50);
	//I2CSendByte(0x23);
	//I2CSendByte(0x30);
	//I2CSendByte(0x10);
	//I2CSendByte(0x00);
	//I2CSendByte(0x11);
	//I2CSendByte(0x35);
	//I2CSendByte(0x99);
	//I2CSendByte(0x99);
	//I2CSendStop(); 
	//------------------------------------------------------------------------------
	
	//while(1)
	//{
	//;
	//}
	
	readrtc(1);
	
	//get_pumping_times();
	//Lcd_x_y_set(0,1);
	//Lcd_print_byte(Pumping_time[0]);
	//Lcd_print_byte(Pumping_time[1]);
	//Lcd_print_byte(Pumping_time[2]);
	//Lcd_print_byte(Pumping_time[3]);
	//Lcd_print_byte(Pumping_time[4]);
	//Lcd_print_byte(Pumping_time[5]);
	//Lcd_print_byte(Pumping_time[6]);
	//Lcd_print_byte(Pumping_time[7]);
	//Lcd_print_byte(Pumping_time[8]);
	
	
	//-----------USEFUL FOR DEBUG-------------------------------
	// setting time
	//set_rtc_time();
	//write_all_pumping_duration();
	//write_pumping_flag(0,0x22);
	//write_pumping_flag(1,0x22);
	//write_pumping_flag(2,0x22);
	//write_pumping_flag(3,0x22);
	//write_pumping_flag(4,0x22);
	//----------------------------------------------------------
	
	//while(1)
	//{
	//;
	//}
	
	
	//Major controls added to remove LCD
	program_rtc = 0; //1 - to program RTC, 0-skip RTC programming
	if(program_rtc == 1)
	{
		//1
		set_rtc_time(); //function implemented in RTC.h
	
		//2
		write_all_pumping_times(); //function implemented in RAM.h
		entry_flag('N');
	
		//3
		write_all_pumping_durations();//function implemented in RAM.h
	
		//while(1) {;}
	}
	//Control ends here
	
	
	
	check_for_pending_runs();
	
	
	
	//Lcd_x_y_set(0,1);
	//Lcd_print_character('F');
	//Lcd_print_character(0x30+time_refr);
	//Lcd_print_character('N');
	
	//while(1)
	//{
	//;
	//}
	
	
	idle_window();	
	
	//ijk = get_next_scheduled_pumping_time();
	//Lcd_x_y_set(40,0);
	//Lcd_print_byte(Next_scheduled_pumping_time_HR);
	//Lcd_print_character(':');
	//Lcd_print_byte(Next_scheduled_pumping_time_Minute);
	//Lcd_print_character(':');
	//Lcd_print_character(0x31+ijk);

	//while(1)
	//{;
	//}
	PORTC = PORTC | SYSTEM_ALIVE;
	sec = Second_current;
	//pumping_trig_sec_count = 0;
	//pumping_trig_en = 0;
	
	while(1)
	{
		if(lmn > 200)
		{
			LcdInit();	
			Delay();
			Clear_LCD();
			lmn = 0;			
		}
		
		if(Second_current != sec)
		{
			sec = Second_current;
			PORTC = ~PORTC;
		}
		
		idle_window();	
		ijk = 0x0;
		key_pressed = 0;
		
		//26122018	-- commented to remove LCD
		//acquire_adc_channels();	
		
		ijk++;
		if(key_pressed == 1)
		{			
			key_pressed = 0;
			call_menu();
			ijk = get_next_scheduled_pumping_time();
			ijk = 0x7F;
		}	
		lmn++;
		readrtc(1);
		
//		if((HR_current < HR_restricted_lower) && (HR_current > HR_restricted_upper) && system_error == 0x0)
		if(system_error == 0x0)
		{
			pumping_control();
			dead_zone_flag = 0x0;
		}
		else
		{
			if(dead_zone_flag == 0x0)
			{
				dead_zone_flag == 0x1;
				for ( count =0; count<6; count++)
				{
					write_pumping_flag(count,0x22);
					write_pumping_count(count,0x00);
				}
			}	
			ijk = get_next_scheduled_pumping_time();
			ijk = 0x7F;
		}
	}
	return 0;
}


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


void check_for_pending_runs(void)
{
	read_entry_status();
	time_refr = get_next_scheduled_pumping_time();	
	for_debug = time_refr;
	if (entry_stat != 'N') //Check only if some run has happen already
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

		}
		else
		{
			pqr = time_refr - 1;
		//Lcd_x_y_set(0,2);
		//Lcd_print_character('(');
		}
		
		get_start_date(pqr);
		
		if((Pumping_date == (Date_current - 1)) || ((Pumping_month != Month_current) && (Date_current == 0x01)))
		{
			day_has_changed = 1;
			//Lcd_x_y_set(0,2);
			//Lcd_print_character('*');
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

void pumping_control(void)
{
	unsigned int j,ijk,pqr;
	unsigned char tmp1,tmp2;//pumping_trig_en;
	
	get_pumping_times();
	//pumping_trig_en = 0;
	
	if((start_pumping == 0x00) || (earlier_completed == 0x01))
	{
		earlier_completed = 0x00;
		for(j=0;j<5;j++)
		{
			if(HR_current == Pumping_time[j*2])
			{
				if(Minute_current == Pumping_time[(j*2)+1])
				{
					Minute_started = Minute_current;
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
			Lcd_x_y_set(78,4);
			Lcd_print_character('P');
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
		Lcd_print_character(0x30 + (Pumping_count/100));
		Lcd_print_character(0x30 + ((Pumping_count%100)/10));
		Lcd_print_character(0x30 + ((Pumping_count%100)%10));
		
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
			
		while(start_pumping == 0x01 && (Pumping_count <= 120))
		{
			entry_flag('O'); /// entry has become old now.
			readrtc(1);
////////////// INCLUDED ONLY FOR DEBUG /////////////////

			key_pressed = 0;
			//26122018	-- commented to remove LCD
			//acquire_adc_channels();		
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
			PORTA = PORTA | (1 << (time_ref & FIVE_LEDS));
			
			if((Minute_current == Minute_started) && (Second_current < 0x05))
			{
				PORTB = PORTB | (PUMPING_START_TRIG);
				PORTB = PORTB & (~PUMPING_END_TRIG);
			} else {
				PORTB = PORTB & (~PUMPING_START_TRIG);
				PORTB = PORTB & (~PUMPING_END_TRIG);
			}
			
			if(minute_incr != Minute_current)
			{
				minute_incr = Minute_current;
				minute_count++;
				write_pumping_count(time_ref,minute_count);
				get_pumping_count(time_ref);
				Lcd_x_y_set(36,4);
				Lcd_print_character(0x30 + (Pumping_count/100));
				Lcd_print_character(0x30 + ((Pumping_count%100)/10));
				Lcd_print_character(0x30 + ((Pumping_count%100)%10));
				Lcd_print_character(' ');
				Lcd_print_character(0x31 + time_ref);
				//Lcd_x_y_set(48,2);
				//Lcd_print_character('$');
				//Lcd_print_character(0x30+time_ref);
				
				earlier_completed = 0x01;
//				if(((HR_current == stop_at_hour) && (Minute_current == stop_at_minute)) || (HR_current >= HR_restricted_lower) || (HR_current < HR_restricted_upper))
				if((HR_current == stop_at_hour) && (Minute_current == stop_at_minute) && (start_pumping == 1))
				{
						
					PORTB = PORTB & (~PUMPING_START_TRIG);
					PORTB = PORTB | PUMPING_END_TRIG;
					
					//Feeder motor ON
					PORTD = PORTD | FEEDER_MOTOR_OUT_0;
					PORTD = PORTD | FEEDER_MOTOR_OUT_1;
					PORTD = PORTD | FEEDER_MOTOR_OUT_2;
					PORTD = PORTD | FEEDER_MOTOR_OUT_3;
					PORTD = PORTD | FEEDER_MOTOR_OUT_4;
					
							
					for ( ijk =0; ijk<50; ijk++)
					{
						for ( pqr =0; pqr<TIME_CONTROL_0; pqr++)
						{
							Delay();
						}
					}
					
					PORTD = PORTD & (~FEEDER_MOTOR_OUT_0);
							
					for ( ijk =0; ijk<50; ijk++)
					{
						for ( pqr =0; pqr<TIME_CONTROL_1; pqr++)
						{
							Delay();
						}
					}
					PORTD = PORTD & (~FEEDER_MOTOR_OUT_1);
							
					for ( ijk =0; ijk<50; ijk++)
					{
						for ( pqr =0; pqr<TIME_CONTROL_2; pqr++)
						{
							Delay();
						}
					}
					PORTD = PORTD & (~FEEDER_MOTOR_OUT_2);
							
					for ( ijk =0; ijk<50; ijk++)
					{
						for ( pqr =0; pqr<TIME_CONTROL_3; pqr++)
						{
							Delay();
						}
					}
					PORTD = PORTD & (~FEEDER_MOTOR_OUT_3);
							
					for ( ijk =0; ijk<50; ijk++)
					{
						for ( pqr =0; pqr<TIME_CONTROL_4; pqr++)
						{
							Delay();
						}
					}
					PORTD = PORTD & (~FEEDER_MOTOR_OUT_4);
	
					PORTA = PORTA & (~OUTPUT_PIN);
					PORTA = PORTA & (~LED_OUT);
					PORTA = PORTA & (~FIVE_LEDS);
					
					PORTB = PORTB & (~PUMPING_START_TRIG);
					PORTB = PORTB & (~PUMPING_END_TRIG);
					
					start_pumping = 0x00;
					
					write_pumping_flag(time_ref,0x22);
					write_pumping_count(time_ref,0x00);
				}
				if(with_in_minute == 0x01)
				{
					
					PORTB = PORTB & (~PUMPING_START_TRIG);
					PORTB = PORTB & (PUMPING_END_TRIG);
					
					for ( ijk =0; ijk<500; ijk++)//5000
					{
						Delay();
					}	
					
					PORTB = PORTB & (~PUMPING_START_TRIG);
					PORTB = PORTB & (~PUMPING_END_TRIG);
					
					PORTA = PORTA & (~OUTPUT_PIN);
					PORTA = PORTA & (~LED_OUT);
					//PORTA = PORTA & (~FIVE_LEDS);
						
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
	
	//return(pumping_trig_en);
	
}

void show_log(void)
{
	LcdInit();	
	Delay();
	Lcd_x_y_set(0,0);
	Lcd_print_sentance(log_1);
	Lcd_x_y_set(0,1);
	Lcd_print_sentance(log_2);
	Lcd_x_y_set(0,2);
	Lcd_print_sentance(log_3);
}

void press_any_other_key(void)
{
	LcdInit();	
	Delay();
	Lcd_x_y_set(0,0);
	Lcd_print_sentance(intermdt_screen_1);
	Lcd_x_y_set(0,1);
	Lcd_print_sentance(intermdt_screen_2);
}

void Delay ( void )
{
    long int i;
	int ij;

    for ( ij =0; ij<100; ij++)
	{
		for ( i = -64000; i < 64000; i++ );
	}
}
