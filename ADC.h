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
void acquire_adc_channels(void)
{
		unsigned char i;	
		unsigned int adc_chnl;
		
		key_pressed = 0;
		adc_chnl = 0;
		for(i=0;i<3;i++)
		{
			adc_channel_number = i;
			adc_capture_routine(i);
			
			while(adc_complete_flag != 1)
			{
				Lcd_print_character (' ');	
				
			}
			adc_complete_flag = 0;
			ADCSRA = ADCSRA | (1<<ADIF);
			
			if (adc_value > 500)
			{
				key_pressed = 1;
				
				if(adc_value > 900)
				{
					//Lcd_print_character(' ');
					key_input = pgm_read_byte(&KEYPAD1[(adc_chnl*4) + 3]);
					//Lcd_print_character (key_input);
					//Lcd_print_character (KEYPAD[(adc_chnl*4) + 3]);
				}
				else if (adc_value > 700)
				{
					//Lcd_print_character(' ');
					key_input = pgm_read_byte(&KEYPAD1[(adc_chnl*4) + 2]);
					//Lcd_print_character (key_input);
					//Lcd_print_character (KEYPAD[(adc_chnl*4) + 2]);
				}
				else if (adc_value > 600)
				{
					//Lcd_print_character(' ');
					key_input = pgm_read_byte(&KEYPAD1[(adc_chnl*4) + 1]);
					//Lcd_print_character (key_input);
					//Lcd_print_character (KEYPAD[(adc_chnl*4) + 1]);
				}
				else 
				{
					//Lcd_print_character(' ');
					key_input = pgm_read_byte(&KEYPAD1[(adc_chnl*4)]);
					//Lcd_print_character (key_input);
					//Lcd_print_character (KEYPAD[(adc_chnl*4)]);
				}
			}	
			adc_chnl++;
		}
}	


void adc_capture_routine(unsigned char channel)
{
    ADCSRA 	= 0x00; //disable adc
    ADMUX 	= 0x40 | channel ; //select adc input 0
    ACSR  	= 0x80; //Disable analog comparator
    ADCSRA 	= 0xC9; //enable ADC, start conversion, No auto trigger, ADC interrupt enabled
				    //division factor between clock frequency and input clock to ADC is 2
}

SIGNAL (SIG_ADC)
{
	unsigned char data1,data2;
	data1 = ADCL; //read lower byte first..important
	data2 = ADCH;
	adc_value = 256 * data2 + data1;
	LcdInit();
	// will show the captured ADC volatge levels
	while(0)
	{
	LcdSend( 0x40 + adc_channel_number + 0x01, LCD_CMD );  // (y) = (adc_channel_number)
	Delay();
	LcdSend( 0x80, LCD_CMD );  // (x) = (0)
	Lcd_print_byte(adc_channel_number);
	Lcd_print_character (' ');
	Lcd_print_character (0x30 + adc_value/100);
	adc_value = adc_value % 100;
	Lcd_print_character (0x30 + adc_value/10);
	adc_value = adc_value % 10;
	Lcd_print_character (0x30 + adc_value);
	adc_value = 256 * data2 + data1;
	}
	adc_complete_flag = 1;
}


void call_menu(void)
{
	//unsigned int ik;
	print_options();	
	LcdSend( 0x45 , LCD_CMD );  // (y) = 5
	LcdSend( 0x87 , LCD_CMD );  // (y) = 7
	while(key_pressed != 1)
	{
		acquire_adc_channels();	
		//Lcd_x_y_set(30,5);
		//Lcd_print_character(key_input);
	}
		key_pressed = 0;
		Clear_LCD();
		if(key_input == '1')
		{
			while(key_input == '1')
			{
			acquire_adc_channels();
			press_any_other_key();
			}
			set_rtc();
			
		}
		else if (key_input == '2')
		{
			while(key_input == '2')
			{
			acquire_adc_channels();
			press_any_other_key();
			}
			set_times();
		}
		else if (key_input == '3')
		{
			while(key_input == '3')
			{
			acquire_adc_channels();
			press_any_other_key();
			}
			set_duration();
		}
		else if (key_input == '4')
		{
			while(key_input == '4')
			{
			acquire_adc_channels();
			press_any_other_key();
			}
			show_log();
		}
		check_for_pending_runs();
		Clear_LCD();
}

