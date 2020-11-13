/*--------------------------------------------------------------------------------------------------

  Name         :  DEFINES.h

--------------------------------------------------------------------------------------------------*/
#ifndef _NOKIALCD_H_

#define _NOKIALCD_H_

/*--------------------------------------------------------------------------------------------------
                                  General purpose constants
--------------------------------------------------------------------------------------------------*/
#define NULL                       0
#define FALSE                      0
#define TRUE                       1

#define LCD_X_RES                  84
#define LCD_Y_RES                  48

//  ATMega8515 port B&D pinout for LCD.

#define PUMPING_START_TRIG		   0x01  //  PB0 - pin 2
#define PUMPING_END_TRIG           0x02  //  PB1 - pin 1
#define LCD_CE_PIN                 0x10  //  PB4 - pin 5
#define SPI_MOSI_PIN               0x20  //  PB5 - pin 6
#define LCD_RST_PIN                0x40  //  PB6 - pin 7
#define SPI_CLK_PIN                0x80  //  PB7 - pin 8
#define LCD_DC_PIN                 0x10  //  PD4 - pin 18
#define OUTPUT_PIN				   0x20  //  PA5 - pin 35 -- LED connected
#define LED_OUT					   0x40	 //  PA6 - pin 34
#define SYSTEM_ALIVE			   0x80	 //	 PC7 - pin 29 -- LED blinks
#define FIVE_LEDS				   0x1F  //  PA4:PA0

#define FEEDER_MOTOR_OUT_0		   0x40 //  PD6 - pin 20
#define FEEDER_MOTOR_OUT_1		   0x20 //  PD5 - pin 19
#define FEEDER_MOTOR_OUT_2		   0x08 //  PD3 - pin 17
#define FEEDER_MOTOR_OUT_3		   0x04 //  PD2 - pin 16
#define FEEDER_MOTOR_OUT_4		   0x02 //  PD1 - pin 15

#define DEBUG                      0
#define WRITE 					   0
#define READ 					   1
#define RTCADDR  				   0xD0  //  RTC
#define START 					   0x08
#define	REP_START		           0x10
#define MT_ADDR_ACK 	           0x18 //SLA + W transmitted and ACK got back
#define MT_DATA_ACK 	           0x28 //Data has been written and ACK got back
#define MR_ADDR_ACK 	           0x40 //SLA + R transmitted and ACK got back
#define MR_DATA_ACK 	           0x50 //Read the data and ACK sent to slave
#define MR_DATA_NACK	           0x58

#define LCD_CACHE_SIZE             ((LCD_X_RES * LCD_Y_RES) / 8)

/*--------------------------------------------------------------------------------------------------
                                       Type definitions
--------------------------------------------------------------------------------------------------*/
typedef char                       bool;
typedef unsigned char              byte;
typedef unsigned int               word;

typedef enum
{
    LCD_CMD  = 0,
    LCD_DATA = 1

} LcdCmdData;

typedef enum
{
    PIXEL_OFF =  0,
    PIXEL_ON  =  1,
    PIXEL_XOR =  2

} LcdPixelMode;

typedef enum
{
    FONT_1X = 1,
    FONT_2X = 2

} LcdFontSize;

/*--------------------------------------------------------------------------------------------------
                                 Public function prototypes
--------------------------------------------------------------------------------------------------*/
void LcdInit       ( void );
void LcdSend   	   (  byte data, LcdCmdData cd );
void Delay         ( void );
void Lcd_print_character (char ch);

void GenStart(void);
void I2CSendAddr(unsigned char addr, unsigned char rd);
void I2CSendByte(unsigned char data);
unsigned char I2CGetByte(unsigned char nack);
void I2CSendStop(void);
void readrtc(unsigned int DISPLAY_TIME);
void set_rtc_time(void);
void Lcd_print_byte (char ch);
void Clear_LCD(void);
void adc_capture_routine(unsigned char channel);
void Lcd_x_y_set(unsigned char x,unsigned char y);
void Accept_keypad_input(void);
void Lcd_print_sentance(unsigned char array[]);
void acquire_adc_channels(void);
void call_menu(void);
void print_options(void);
void set_rtc(void);
void set_times(void);
void set_duration(void);
void idle_window(void);
void get_pumping_times(void);
void get_pumping_duration(void);
void pumping_control(void);
void get_pumping_flag(unsigned int ref);
void get_pumping_count(unsigned int ref);
void get_start_date(unsigned int ref);
void write_pumping_flag(unsigned int ref,unsigned char flag);
void write_start_date(unsigned int ref,unsigned char date_input,unsigned char month_input);
void write_pumping_count(unsigned int ref,unsigned char cnt);
void write_all_pumping_duration(void);
unsigned char BCD_DIFF(unsigned char num1, unsigned char num2);
unsigned char BCD_ADD(unsigned char num1, unsigned char num2);
unsigned int get_next_scheduled_pumping_time(void);
unsigned int bcd_to_integer(unsigned char bcd);
void check_for_pending_runs(void);
void show_pumping_status(void);
void write_pumping_duration(unsigned int ref,unsigned char drtn);
void read_pumping_duration();

void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);
void entry_flag(unsigned char flag);
void read_entry_status(void);




#endif  //  _NOKIALCD_H_


/* RTC RAM Content Details
----------------------------------------------------
Address 	Content	
----------------------------------------------------
0x12	Duration of pumping - HR
0x13	Duration of pumping - Minute
----------------------------------------------------
0x14	Starting time of RUN_0 - Date
0x15	Starting time of RUN_0 - Month
0x16	Count
0x17	Flag of RUN_0
-----------------------------------------------------
0x18	Starting time of RUN_1 - HR
0x19	Starting time of RUN_1 - Minute
0x1A	Count of RUN_1
0x1B	Flag of RUN_1
-----------------------------------------------------
0x1C	Starting time of RUN_2 - HR
0x1D	Starting time of RUN_2 - Minute
0x1E	Count of RUN_2
0x1F	Flag of RUN_2
-----------------------------------------------------
0x20	Starting time of RUN_3 - HR
0x21	Starting time of RUN_3 - Minute
0x22	Count of RUN_3
0x23	Flag of RUN_3
-----------------------------------------------------
0x24	Starting time of RUN_4 - HR
0x25	Starting time of RUN_4 - Minute
0x26	Count of RUN_4
0x27	Flag of RUN_4
-----------------------------------------------------
0x28	Pumping_duration_hour of RUN_0
0x29	Pumoing_duration_minute of RUN_0
0x30	Pumping_duration_hour of RUN_1
0x31	Pumoing_duration_minute of RUN_1
0x32	Pumping_duration_hour of RUN_2
0x33	Pumoing_duration_minute of RUN_2
0x34	Pumping_duration_hour of RUN_3
0x36	Pumping_duration_minute of RUN_3
0x37	Pumping_duration_hour of RUN_4
0x38	Pumoing_duration_minute of RUN_4
-----------------------------------------------------
0x39	Flag for new entry
--------------------------------------------------------------------------------------------------
                                         End of file.
--------------------------------------------------------------------------------------------------*/

