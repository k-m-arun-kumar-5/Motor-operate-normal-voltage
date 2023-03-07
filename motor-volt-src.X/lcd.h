/**************************************************************************
   FILE          :    lcd.h
 
   PURPOSE       :    LCD Header
 
   AUTHOR        :     K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :  for  LCD disp 
 
  CHANGE LOGS    :
	   
 **************************************************************************/
 /* to avoid this header file to be included more than once, conditional directive is used  */
#ifndef _LCD_H
#define _LCD_H
                            

#define LCD_WRITE_ENABLE_PULSE_IN_USEC       (1)
#define LCD_ADDR_SETUP_IN_USEC               (1)
#define LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC (1)
#define LCD_DATA_SETUP_IN_USEC               (1)
#define LCD_WRITE_ENABLE_PULSE_DELAY_IN_USEC (1)
#define LCD_CLEAR_EXEC_IN_USEC             (1650)
#define LCD_CURSOR_RETURN_EXEC_IN_USEC     (1650)
#define LCD_OTHER_INST_EXEC_IN_USEC          (40)

#define INVALID_DATA               (0)
#define ALL_LINES                  (0)
#define NUM_LINE1                  (1)
#define NUM_LINE2                  (2)
#define NUM_LINE3                  (3)
#define NUM_LINE4                  (4)

#define NUM_COL1                   (1)

#define CHECK_BUSY_FLAG             (1)
#define NO_CHECK_BUSY_FLAG          (0)

#define DATA_INTERFACE_8_BITS       (1) 
#define DATA_INTERFACE_4_BITS       (0)

#define CHAR_FONT_5_10_DOTS        (1)
#define CHAR_FONT_5_8_DOTS         (0)

#define MAX_DISP_NUM_LINES_2      (1)
#define MAX_DISP_NUM_LINES_1      (0)     



typedef struct {
		struct 	{
	    	unsigned int rs_pin: 1;
		    unsigned int rs_tris: 1;
		    unsigned int rw_pin: 1;
	    	unsigned int rw_tris: 1;
		    unsigned int en_pin: 1;
		    unsigned int en_tris: 1;
		    unsigned int interface: 1;
		    unsigned int num_lines: 2;
		    unsigned int font: 1;
		    unsigned int check_bf: 1;
	    } conf;
	    union {
	    	unsigned int tris;
	    	struct {
			   unsigned int tris_0: 1;
			   unsigned int tris_1: 1;
			   unsigned int tris_2: 1;
			   unsigned int tris_3: 1;
			   unsigned int tris_4: 1;
			   unsigned int tris_5: 1;
			   unsigned int tris_6: 1;
			   unsigned int tris_7: 1;
		   } data_tris;
	   } data_triss;	
	    union {
	    	unsigned int pins;
	    	struct {
			   unsigned int pin_0: 1;
			   unsigned int pin_1: 1;
			   unsigned int pin_2: 1;
			   unsigned int pin_3: 1;
			   unsigned int pin_4: 1;
			   unsigned int pin_5: 1;
			   unsigned int pin_6: 1;
			   unsigned int pin_7: 1;
		   } data_pin;
	   } data_pins;		
   } lcd;

extern char lcd_avail_loc_within_limit;
extern unsigned int cur_disp_lcd_loc , cur_input_lcd_loc ;
extern unsigned int lcd_read_command;
extern lcd lcd_unit; 

/* -------------------- public prototype declaration --------------------------------------- */
void Delay_Time_By_Count(unsigned long int delay_time_in_count);
void LCD_Write_Pulse();
void LCD_Read_Pulse();
void Write_LCD_Command_NO_BF(const unsigned int lcd_cmd);
void Write_LCD_Command(const unsigned int lcd_cmd);
void Write_LCD_Data(const char lcd_disp_ch);
void LCD_Disp_Str(const char *lcd_disp_str);
void LCD_Disp_Num(const disp_num_types lcd_datanum_disp_format, const value_types lcd_disp_data_int);
void Goto_XY_LCD_Disp(const unsigned int start_line_num, const unsigned int start_col_num);
void Goto_XY_LCD_Input(unsigned int start_line_num, unsigned int start_col_num);
void From_Loc_to_XY_LCD(const unsigned int loc_lcd, unsigned int * const loc_lcd_line_num, unsigned int * const loc_lcd_col_num );
void From_XY_To_Loc_LCD(const unsigned int start_line_num, const unsigned int start_col_num, unsigned int * const lcd_loc);
void LCD_Init();
void Check_LCD_Busy();
unsigned int Read_LCD_Command();
void LCD_Clear_Screen();
void Write_Bit_in_Data( unsigned int *data, const unsigned int bit_pos, unsigned int set_bit_val );

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
