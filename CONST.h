const prog_uint8_t TABLE5[] PROGMEM= {
  0x00,0x00,0x00,0x00,0x00,   // 20 space         
  0x00,0x00,0x5f,0x00,0x00,   // 21 ! 
  0x00,0x07,0x00,0x07,0x00,   // 22 " 
  0x14,0x7f,0x14,0x7f,0x14,   // 23 # 
  0x24,0x2a,0x7f,0x2a,0x12,   // 24 $ 
  0x23,0x13,0x08,0x64,0x62,   // 25 % 
  0x36,0x49,0x55,0x22,0x50,   // 26 & 
  0x00,0x05,0x03,0x00,0x00,   // 27 ' 
  0x00,0x1c,0x22,0x41,0x00,   // 28 ( 
  0x00,0x41,0x22,0x1c,0x00,   // 29 ) 
  0x14,0x08,0x3e,0x08,0x14,   // 2a * 
  0x08,0x08,0x3e,0x08,0x08,   // 2b + 
  0x00,0x50,0x30,0x00,0x00,   // 2c , 
  0x08,0x08,0x08,0x08,0x08,   // 2d - 
  0x00,0x60,0x60,0x00,0x00,   // 2e . 
  0x20,0x10,0x08,0x04,0x02,   // 2f / 
  0x3e,0x51,0x49,0x45,0x3e,   // 30 0 
  0x00,0x42,0x7f,0x40,0x00,   // 31 1 
  0x42,0x61,0x51,0x49,0x46,   // 32 2 
  0x21,0x41,0x45,0x4b,0x31,   // 33 3 
  0x18,0x14,0x12,0x7f,0x10,   // 34 4 
  0x27,0x45,0x45,0x45,0x39,   // 35 5 
  0x3c,0x4a,0x49,0x49,0x30,   // 36 6 
  0x01,0x71,0x09,0x05,0x03,   // 37 7 
  0x36,0x49,0x49,0x49,0x36,   // 38 8 
  0x06,0x49,0x49,0x29,0x1e,   // 39 9 
  0x00,0x36,0x36,0x00,0x00,   // 3a : 
  0x00,0x56,0x36,0x00,0x00,   // 3b ; 
  0x08,0x14,0x22,0x41,0x00,   // 3c < 
  0x14,0x14,0x14,0x14,0x14,   // 3d = 
  0x00,0x41,0x22,0x14,0x08,   // 3e > 
  0x02,0x01,0x51,0x09,0x06,   // 3f ? 
  0x32,0x49,0x79,0x41,0x3e,   // 40 @ 
  0x7e,0x11,0x11,0x11,0x7e,   // 41 A 
  0x7f,0x49,0x49,0x49,0x36,   // 42 B 
  0x3e,0x41,0x41,0x41,0x22,   // 43 C 
  0x7f,0x41,0x41,0x22,0x1c,   // 44 D 
  0x7f,0x49,0x49,0x49,0x41,   // 45 E 
  0x7f,0x09,0x09,0x09,0x01,   // 46 F 
  0x3e,0x41,0x49,0x49,0x7a,   // 47 G 
  0x7f,0x08,0x08,0x08,0x7f,   // 48 H 
  0x00,0x41,0x7f,0x41,0x00,   // 49 I 
  0x20,0x40,0x41,0x3f,0x01,   // 4a J 
  0x7f,0x08,0x14,0x22,0x41,   // 4b K 
  0x7f,0x40,0x40,0x40,0x40,   // 4c L 
  0x7f,0x02,0x0c,0x02,0x7f,   // 4d M 
  0x7f,0x04,0x08,0x10,0x7f,   // 4e N 
  0x3e,0x41,0x41,0x41,0x3e    // 4f O 
}; 
	
 const prog_uint8_t TABLE6[] PROGMEM= {
  0x7f,0x09,0x09,0x09,0x06,   // 50 P 
  0x3e,0x41,0x51,0x21,0x5e,   // 51 Q 
  0x7f,0x09,0x19,0x29,0x46,   // 52 R 
  0x46,0x49,0x49,0x49,0x31,   // 53 S 
  0x01,0x01,0x7f,0x01,0x01,   // 54 T 
  0x3f,0x40,0x40,0x40,0x3f,   // 55 U 
  0x1f,0x20,0x40,0x20,0x1f,   // 56 V 
  0x3f,0x40,0x38,0x40,0x3f,   // 57 W 
  0x63,0x14,0x08,0x14,0x63,   // 58 X 
  0x07,0x08,0x70,0x08,0x07,   // 59 Y 
  0x61,0x51,0x49,0x45,0x43,   // 5a Z 
};  

const prog_uint8_t KEYPAD1[] PROGMEM= {0x31,0x34,0x37,'*',0x32,0x35,0x38,0x30,0x33,0x36,0x39,'#'};

const prog_uint8_t wel_note_1[] PROGMEM= {' ','S','M','A','R','T',' ','P','U','M','P','I','N','G','#'};
const prog_uint8_t wel_note_2[] PROGMEM= {' ','M','A','N','A','G','E','R',' ','V','0','.','3','#'};
const prog_uint8_t wel_note_3[] PROGMEM= {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'};
const prog_uint8_t wel_note_4[] PROGMEM= {' ','N','E','X','T',' ','@',' ','#'};

const prog_uint8_t message_1[] PROGMEM= {' ',' ',' ',' ',' ','M','E','N','U','#'};
const prog_uint8_t message_2[] PROGMEM= {'1',' ','S','E','T',' ','T','I','M','E',' ','A','N','D','#'};
const prog_uint8_t message_3[] PROGMEM= {' ',' ','D','A','T','E','#'};
const prog_uint8_t message_4[] PROGMEM= {'2',' ','S','E','T',' ','T','I','M','E','S','#'};
const prog_uint8_t message_5[] PROGMEM= {'3',' ','S','E','T',' ','D','U','R','A','T','I','O','N','#'};
const prog_uint8_t message_6[] PROGMEM= {'4',' ','L','O','G','#'};

const prog_uint8_t log_1[] PROGMEM=     {'T','H','S','T',':','#'};
const prog_uint8_t log_2[] PROGMEM=     {'T','H','R','T',':','#'};
const prog_uint8_t log_3[] PROGMEM=     {'T','H','R','W',':','#'};

const prog_uint8_t intermdt_screen_1[] PROGMEM= {' ','P','R','E','S','S',' ','A','N','O','T','H','E','R','#'};
const prog_uint8_t intermdt_screen_2[] PROGMEM= {' ',' ',' ',' ',' ',' ','K','E','Y','#'};

const prog_uint8_t error_message_1[] PROGMEM= {'C','H','E','C','K',' ','E','N','T','R','I','E','S','#'};

const prog_uint8_t set_date_1[] PROGMEM = {' ','E','N','T','E','R',' ','D','A','T','E','#'};
const prog_uint8_t set_date_2[] PROGMEM = {' ','D','D',':','M','M',':','Y','Y','#'};
const prog_uint8_t set_time_1[] PROGMEM = {' ','E','N','T','E','R',' ','T','I','M','E','#'};
const prog_uint8_t set_time_2[] PROGMEM = {' ','H','H',':','M','M','#'};
const prog_uint8_t set_timings_1[] PROGMEM = {' ','F','R','O','M',' ',' ',' ',' ',' ','T','O',' ',' ','#'};
const prog_uint8_t set_duration_1[] PROGMEM = {' ','S','E','T',' ','D','U','R','A','T','I','O','N','#'};

const prog_uint8_t pumping_stat_1[] PROGMEM = {' ',' ',' ',' ','S','T','A','T','U','S',' ',' ',' ',' ','#'};
const prog_uint8_t pumping_stat_2[] PROGMEM = {'P','U','M','P','I','N','G','.','.','.',' ',' ',' ',' ','#'};
const prog_uint8_t pumping_stat_3[] PROGMEM = {'S','T','R','T','D',' ','@','#'};
const prog_uint8_t pumping_stat_4[] PROGMEM = {'S','T','O','P','S',' ','@','#'};
const prog_uint8_t pumping_stat_5[] PROGMEM = {'P','R','#'};
const prog_uint8_t pumping_stat_6[] PROGMEM = {'C','O','U','N','T',':','#'};
const prog_uint8_t pumping_stat_7[] PROGMEM = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'};

const prog_uint8_t device_type_1[] PROGMEM =  {' ',' ','N','O',' ','P','U','M','P','I','N','G','#'};
const prog_uint8_t device_type_2[] PROGMEM =  {'F','R','M',' ','6','P','M',' ','T','O',' ','6','A','M','#'};

