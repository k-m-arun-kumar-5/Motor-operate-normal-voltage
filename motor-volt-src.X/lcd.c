/* ********************************************************************
   FILE                   : lcd.c

   PROGRAM DESCRIPTION    : LCD library 
                      									 
	 
   AUTHOR                :  K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
   KNOWN BUGS            : 

   NOTE                  :  										
                                    
   CHANGE LOGS           : 

*****************************************************************************/
#include "main.h"
#include "port.h"
#include "lcd.h"
#include "io_conf.h"
#include "uart.h"

lcd lcd_unit; 
char lcd_avail_loc_within_limit = STATE_YES_IN_CHAR;
unsigned int cur_disp_lcd_loc = BEGIN_LOC_LINE1, cur_input_lcd_loc = BEGIN_LOC_LINE2;
unsigned int lcd_read_command;

/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void LCD_Init()
{
	unsigned int lcd_func_command = 0x20;
	
	lcd_unit.conf.rs_tris = LCD_RS_TRIS;
	lcd_unit.conf.rw_tris = LCD_RW_TRIS;
	lcd_unit.conf.en_tris = LCD_EN_TRIS;
	
	LCD_RS_TRIS = 0;
	LCD_RW_TRIS = 0;
	LCD_EN_TRIS = 0;
	
	#if (LCD_DATA_INTERFACE == DATA_INTERFACE_4_BITS)
		lcd_unit.conf.interface = DATA_INTERFACE_4_BITS;
	#else
		lcd_unit.conf.interface = DATA_INTERFACE_8_BITS;
	#endif
	
	if(lcd_unit.conf.interface == DATA_INTERFACE_4_BITS)
	{
		// higher port nibble of LCD PORT is connected to LCD(DB4 - DB7)	
    	lcd_unit.data_triss.data_tris.tris_4  =   LCD_DATA_4_TRIS ;         
        lcd_unit.data_triss.data_tris.tris_5  =   LCD_DATA_5_TRIS ;
        lcd_unit.data_triss.data_tris.tris_6  =   LCD_DATA_6_TRIS ;
        lcd_unit.data_triss.data_tris.tris_7  =   LCD_DATA_7_TRIS ;		
	}
	else
	{
		//data interface is 8 bit
		lcd_unit.data_triss.tris = LCD_DATA_TRIS;		
	}
	#if (LCD_BUSY_FLAG == CHECK_BUSY_FLAG)
		lcd_unit.conf.check_bf = CHECK_BUSY_FLAG;
	    #ifdef TRACE
		   UART_Transmit_Str("LCD check BUSY Flag \r");
		#endif 
	#else
		lcd_unit.conf.check_bf = NO_CHECK_BUSY_FLAG;
	    #ifdef TRACE
		   UART_Transmit_Str("LCD dont check BUSY Flag \r");
		#endif
	#endif	
	
	/* wait for more than 15ms after LCD VSS rises to 4.5V, Busy Flag in LCD (BF) cannot be checked */
	__delay_ms(1500);
    Write_LCD_Command_NO_BF(0x30);
	/* wait for more than 4.1 ms, Busy Flag in LCD (BF) cannot be checked */
	__delay_ms(10);
    Write_LCD_Command_NO_BF(0x30);
	/* wait for more than 100 us, Busy Flag in LCD (BF) cannot be checked */
	__delay_ms(1);
    Write_LCD_Command_NO_BF(0x30);
	__delay_ms(1);
	if(lcd_unit.conf.interface == DATA_INTERFACE_4_BITS)
	{		
	   Write_LCD_Command_NO_BF(0x20); 
	}
	
    Write_Bit_in_Data(&lcd_func_command , 4, lcd_unit.conf.interface);
	Write_Bit_in_Data(&lcd_func_command , 3, LCD_MAX_DISP_NUM_LINES);
	Write_Bit_in_Data(&lcd_func_command , 2, LCD_CHAR_FONT);
	Write_LCD_Command(lcd_func_command);  
   // Write_LCD_Command(0x28);
	
    #ifdef TRACE
	   to_disp.unsigned_val.value_long = lcd_func_command;
       UART_Transmit_Str("LCD Func Set: 0x");
	   UART_Transmit_Num(DISP_HEX_DIGIT2, to_disp);
	   UART_Transmit_Char('\r');
	#endif 
	   
   
	Write_LCD_Command(0x08);
	LCD_Clear_Screen();
	Write_LCD_Command(0x06);  
	Write_LCD_Command(0x0E);
} 
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void LCD_Clear_Screen()
{
	Write_LCD_Command(0x01);
	__delay_us(LCD_CLEAR_EXEC_IN_USEC);
}
 
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void Write_LCD_Command_NO_BF(const unsigned int lcd_cmd)
{
   unsigned int write_lcd_cmd = lcd_cmd;  
   
   LCD_RW_PIN = 0;
   LCD_RS_PIN = 0;
   if(lcd_unit.conf.interface == DATA_INTERFACE_4_BITS)
   {
	   // higher port nibble of LCD PORT is connected to LCD(DB4 - DB7)	
       LCD_DATA_4_TRIS = 0;
	   LCD_DATA_5_TRIS = 0;
	   LCD_DATA_6_TRIS = 0;
	   LCD_DATA_7_TRIS = 0;
   }
   else
   {
	   LCD_DATA_TRIS = 0x00;
   }
   __delay_us(LCD_ADDR_SETUP_IN_USEC);   
    LCD_EN_PIN = 1;
    __delay_us(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);

	// higher port nibble of LCD PORT is connected to LCD(DB4 - DB7)	
    LCD_DATA_PORT = (lcd_cmd & 0xF0u) ;
  
   // lower port nibble of LCD PORT is connected to LCD(DB4 - DB7)	
   // LCD_DATA_PORT = (lcd_cmd & 0xf0u) >> 4; 
	__delay_us(LCD_DATA_SETUP_IN_USEC);
    LCD_EN_PIN = 0;
    //SHOULD_REMOVE
    //UART_Transmit_Char(LCD_DATA_PORT);
    __delay_us(LCD_OTHER_INST_EXEC_IN_USEC);  
     
}
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void Write_LCD_Command(const unsigned int lcd_cmd)
{
   unsigned int write_lcd_cmd = lcd_cmd;
   
	if(lcd_unit.conf.check_bf == CHECK_BUSY_FLAG)
	{
       Check_LCD_Busy(); 
	}
		
   LCD_RW_PIN = 0;
   LCD_RS_PIN = 0;
   
   if(lcd_unit.conf.interface == DATA_INTERFACE_4_BITS)
   {
	   // higher port nibble of LCD PORT is connected to LCD(DB4 - DB7)	
       LCD_DATA_4_TRIS = 0;
	   LCD_DATA_5_TRIS = 0;
	   LCD_DATA_6_TRIS = 0;
	   LCD_DATA_7_TRIS = 0;
   }
   else
   {
	   LCD_DATA_TRIS = 0x00;
   } 
   
   __delay_us(LCD_ADDR_SETUP_IN_USEC);   
    LCD_EN_PIN = 1;
    __delay_us(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
	if(lcd_unit.conf.interface == DATA_INTERFACE_4_BITS)
	{
	    // send higher nibble first and higher port nibble of LCD PORT is connected to LCD(DB4 - DB7)	 
        LCD_DATA_PORT = (lcd_cmd & 0xF0u); 
	    __delay_us(LCD_DATA_SETUP_IN_USEC);
        LCD_EN_PIN = 0;
        //SHOULD_REMOVE
        //UART_Transmit_Char(LCD_DATA_PORT);
        __delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 	 
   
        __delay_us(LCD_ADDR_SETUP_IN_USEC);   
        LCD_EN_PIN = 1;
        __delay_us(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
	    // send lower nibble and higher port nibble of LCD PORT is connected to LCD(DB4 - DB7)	 
        LCD_DATA_PORT = (lcd_cmd & 0x0Fu) << 4; 
	}
    else
	{	
        LCD_DATA_PORT = (lcd_cmd ); 
	}
	 __delay_us(LCD_DATA_SETUP_IN_USEC);
     LCD_EN_PIN = 0;
       //SHOULD_REMOVE
       //UART_Transmit_Char(LCD_DATA_PORT);
	
    __delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 
   
}
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
 void Write_LCD_Data(const char lcd_data)
{
	unsigned int write_lcd_data = lcd_data;
	
	if(lcd_unit.conf.check_bf == CHECK_BUSY_FLAG)
	{
       Check_LCD_Busy(); 
	} 
	
	 LCD_RW_PIN = 0;
     LCD_RS_PIN = 1;
	
	if(lcd_unit.conf.interface == DATA_INTERFACE_4_BITS)
   {
	   // higher port nibble of LCD PORT is connected to LCD(DB4 - DB7)	
       LCD_DATA_4_TRIS = 0;
	   LCD_DATA_5_TRIS = 0;
	   LCD_DATA_6_TRIS = 0;
	   LCD_DATA_7_TRIS = 0;
   }
   else
   {
	   LCD_DATA_TRIS = 0x00;
   } 
	 
	 __delay_us(LCD_ADDR_SETUP_IN_USEC);   
    LCD_EN_PIN = 1;
    __delay_us(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
	if(lcd_unit.conf.interface == DATA_INTERFACE_4_BITS)
	{
	    // send higher nibble first and higher port nibble of LCD PORT must be connected to LCD(DB4 - DB7)	 	 
        LCD_DATA_PORT = (lcd_data & 0xF0u); 
	    __delay_us(LCD_DATA_SETUP_IN_USEC);
        LCD_EN_PIN = 0;
        //SHOULD_REMOVE
        //UART_Transmit_Char(LCD_DATA_PORT);
       __delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 	 
   
        __delay_us(LCD_ADDR_SETUP_IN_USEC);   
        LCD_EN_PIN = 1;
       __delay_us(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
	   // send lower nibble and higher port nibble of LCD PORT must be connected to LCD (DB4 - DB7)		 
       LCD_DATA_PORT = (lcd_data & 0x0Fu) << 4;  
	}
	else
	{
		LCD_DATA_PORT = (lcd_data ); 
	}
	  __delay_us(LCD_DATA_SETUP_IN_USEC);
    LCD_EN_PIN = 0;
    //SHOULD_REMOVE
    //UART_Transmit_Char(LCD_DATA_PORT);
    __delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 
}

/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void Check_LCD_Busy()
{
	// send higher nibble first and higher data nibble of LCD PORT must be connected to LCD 	 
     /* busy flag = Bit 7 in LCD PORT, if busy flag == 1, wait till busy flag = 0, then any operation on LCD can be done */
    while(((lcd_read_command = Read_LCD_Command()) & 0x80));
   	   
}
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void Delay_Time_By_Count(unsigned long int time_delay)
{
     while(time_delay--);
}
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
unsigned int Read_LCD_Command()
{
	 unsigned int read_lcd_command; 
	 LCD_RW_PIN = 1;
     LCD_RS_PIN = 0;
	 
	if(lcd_unit.conf.interface == DATA_INTERFACE_4_BITS)
   {
	   // higher port nibble of LCD PORT is connected to LCD(DB4 - DB7)	
       LCD_DATA_4_TRIS = 1;
	   LCD_DATA_5_TRIS = 1;
	   LCD_DATA_6_TRIS = 1;
	   LCD_DATA_7_TRIS = 1;
   }
   else
   {
	   LCD_DATA_TRIS = 0xFF;
   } 
	 
	 __delay_us(LCD_ADDR_SETUP_IN_USEC);   
     LCD_EN_PIN = 1;
	if(lcd_unit.conf.interface == DATA_INTERFACE_4_BITS)
	{ 
	    // read higher nibble first and higher port nibble of LCD PORT is connected to LCD(DB4 - DB7)	
        read_lcd_command  = LCD_DATA_PORT;
	    LCD_EN_PIN = 0;
	     lcd_read_command = (read_lcd_command & 0xF0u);	 
	     __delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 
	     __delay_us(LCD_ADDR_SETUP_IN_USEC);   
         LCD_EN_PIN = 1;
	     // read lower nibble and higher port nibble of LCD PORT is connected to LCD(DB4 - DB7)	
         read_lcd_command  = LCD_DATA_PORT;
	     LCD_EN_PIN = 0;
	    __delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 
	    lcd_read_command = lcd_read_command | (read_lcd_command & 0x0Fu);
	 }
	 else
	 {
		  lcd_read_command  = LCD_DATA_PORT;
	      LCD_EN_PIN = 0;
		  __delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 
	 }
	 	 	 
	 return lcd_read_command;
}


/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 25   
-*------------------------------------------------------------*/

void LCD_Disp_Str(const char *char_ptr)
{ 
       char cur_char;
	   
       while(*char_ptr)
       {
		    cur_char = *(char_ptr++);
            Write_LCD_Data(cur_char);
			UART_Transmit_Char(cur_char);
       }
	   UART_Transmit_Char('\r');     
}

/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 26  
-*------------------------------------------------------------*/
 void LCD_Disp_Num(const disp_num_types lcd_datanum_disp_format, const value_types lcd_disp_data_int)
{
    unsigned int tens_thousand_digit,thousands_digit, hundreds_digit,tens_digit, unit_digit;
	unsigned long disp_num, num ;
	char num_data[] ={'0','1','2','3','4','5','6','7','8','9'};  
	char hex_data[] ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'}; 
		
	Write_LCD_Command(cur_disp_lcd_loc);
	if(lcd_datanum_disp_format & 0x10 ) //signed lcd_datanum_disp_format
	{
		if(lcd_disp_data_int.signed_val.value_long < 0 )
		{
			Write_LCD_Data('-');
			disp_num = -lcd_disp_data_int.signed_val.value_long;
		}
        else
		{
			Write_LCD_Data('+');
			disp_num = lcd_disp_data_int.signed_val.value_long;
		}
        
	}
	else
	{
    	disp_num = lcd_disp_data_int.unsigned_val.value_long; 
	}
	num =  disp_num ;
	
    switch(lcd_datanum_disp_format)
	{
	  case DISP_UNSIGN_NUM_DIGIT5:
	  case DISP_SIGN_NUM_DIGIT5:  
		  num =  disp_num ;
		  tens_thousand_digit = (unsigned int)(num / (unsigned long)(10000UL));
          Write_LCD_Data(num_data[tens_thousand_digit]);
	  case DISP_UNSIGN_NUM_DIGIT4:
	  case DISP_SIGN_NUM_DIGIT4: 
	      num = disp_num % 10000UL;
	      thousands_digit = (unsigned int)(num / (unsigned long)(1000UL));
	      Write_LCD_Data(num_data[thousands_digit]); 
	  case DISP_UNSIGN_NUM_DIGIT3:
	  case DISP_SIGN_NUM_DIGIT3:
		  num = disp_num % 1000UL;
	      hundreds_digit = (unsigned int) (num / (unsigned long) (100));
	      Write_LCD_Data(num_data[hundreds_digit]);
	  case DISP_UNSIGN_NUM_DIGIT2:
	  case DISP_SIGN_NUM_DIGIT2:
          num = disp_num % 100;
          tens_digit = (unsigned int) (num / 10);
          Write_LCD_Data(num_data[tens_digit]); 		  
	  case DISP_UNSIGN_NUM_DIGIT1:
	  case DISP_SIGN_NUM_DIGIT1:
	     unit_digit = (unsigned int) (disp_num % 10);
         Write_LCD_Data(num_data[unit_digit]); 
	  break;
	  case DISP_HEX_DIGIT4:
	      //  ( 16 * 16 * 16 *16 )  = 0 as divide by zero warning 
	      //num = disp_num % ( 16 * 16 * 16 *16 );
          thousands_digit = (num / (unsigned long) (16 * 16 * 16));
	      Write_LCD_Data(hex_data[thousands_digit]);
	  case DISP_HEX_DIGIT3:
	      num = disp_num %(unsigned long)(16 * 16 * 16);
	      hundreds_digit = (unsigned int) (num / (unsigned long) (16 * 16));
	      Write_LCD_Data(hex_data[hundreds_digit]);
	  case DISP_HEX_DIGIT2:
	      num = disp_num %(unsigned long)(16 * 16);
          tens_digit = (unsigned int) (num / 16);
          Write_LCD_Data(hex_data[tens_digit]);
	  case DISP_HEX_DIGIT1: 
	      unit_digit = (unsigned int) (disp_num % 16);
          Write_LCD_Data(hex_data[unit_digit]);    
	  break;
	  
	}   	
}


/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 28  
-*------------------------------------------------------------*/
/* make sure that loc are within avail lcd loc limit */
void Set_Cur_Loc_LCD(const char set_input_loc_flag,const unsigned int set_input_loc, const char set_disp_loc_flag, const unsigned int set_disp_loc)
{
   if(set_input_loc_flag == STATE_YES_IN_CHAR)  
     cur_input_lcd_loc = set_input_loc;
   if(set_disp_loc_flag == STATE_YES_IN_CHAR)
     cur_disp_lcd_loc = set_disp_loc; 
}
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 29   
-*------------------------------------------------------------*/
void Goto_XY_LCD_Disp(const unsigned int start_line_num, const unsigned int start_col_num)
{
	/* max 4 lines and 20 columns */
	/* for us, lcd line starts from 1, but for uC, line starts from 0 */
	/* for us, lcd col starts from 1, but for uC, col starts from 0 */
    unsigned int start_line_lcd = start_line_num - 1, start_col_lcd = start_col_num - 1, error_disp_start_loc; 
	lcd_avail_loc_within_limit = STATE_YES_IN_CHAR;
	
   if(start_line_num <= CONFIGURE_MAX_NUM_LINES && start_col_num <= CONFIGURE_MAX_NUM_COLS )
   {
     switch(start_line_num)
	 {
		 case NUM_LINE1:
		   cur_disp_lcd_loc = BEGIN_LOC_LINE1;
		   break;
		 case NUM_LINE2:
		   cur_disp_lcd_loc = BEGIN_LOC_LINE2;
		   break;
		 case NUM_LINE3:
		   cur_disp_lcd_loc = BEGIN_LOC_LINE3;
		   break;
		  case NUM_LINE4:
		   cur_disp_lcd_loc = BEGIN_LOC_LINE4;
		   break;		 
	 }	
      cur_disp_lcd_loc = cur_disp_lcd_loc + start_col_lcd;
      Write_LCD_Command(cur_disp_lcd_loc);       
   }
   else
   {
	   /* error due to invalid lcd DISP loc  */
	     lcd_avail_loc_within_limit = STATE_NO_IN_CHAR;	
   }	   
} 
 /*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 30   
-*------------------------------------------------------------*/ 
void Goto_XY_LCD_Input(unsigned int start_line_num,unsigned int start_col_num)
{
	/* for us, lcd line starts from 1, but for uC, line starts from 0 */
	/* for us, lcd col starts from 1, but for uC, col starts from 0 */
    unsigned int start_line_lcd = start_line_num - 1, start_col_lcd = start_col_num - 1; 
	
	/* max 4 lines and 20 columns */
	lcd_avail_loc_within_limit = STATE_YES_IN_CHAR;
	if( start_line_num <= CONFIGURE_MAX_NUM_LINES &&  start_col_num <= CONFIGURE_MAX_NUM_COLS )
   {
     switch(start_line_num)
	 {
		 case NUM_LINE1:
		   cur_input_lcd_loc = BEGIN_LOC_LINE1;
		   break;
		 case NUM_LINE2:
		   cur_input_lcd_loc = BEGIN_LOC_LINE2;
		   break;
		 case NUM_LINE3:
		   cur_input_lcd_loc = BEGIN_LOC_LINE3;
		   break;
		  case NUM_LINE4:
		   cur_input_lcd_loc = BEGIN_LOC_LINE4;
		   break; 		  
	 }
	 cur_input_lcd_loc = cur_input_lcd_loc + start_col_lcd;
     Write_LCD_Command(cur_input_lcd_loc); 	 	   
  }
  else
  {
	   /* error due to invalid Lcd loc  */	
	   lcd_avail_loc_within_limit = STATE_NO_IN_CHAR;
  }	  
} 

 /*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 31   
-*------------------------------------------------------------*/
void From_XY_To_Loc_LCD(const unsigned int start_line_num, const unsigned int start_col_num, unsigned int * const lcd_loc)
{
	/* max 4 lines and 20 columns */
	   
	lcd_avail_loc_within_limit = STATE_YES_IN_CHAR;
	
   if(start_line_num <= CONFIGURE_MAX_NUM_LINES && start_col_num <= CONFIGURE_MAX_NUM_COLS )
   {
      switch(start_line_num)
	 {
		 case NUM_LINE1:
		   *lcd_loc = BEGIN_LOC_LINE1;
		   break;
		 case NUM_LINE2:
		   *lcd_loc = BEGIN_LOC_LINE2;
		   break;
		 case NUM_LINE3:
		   *lcd_loc= BEGIN_LOC_LINE3;
		   break;
		  case NUM_LINE4:
		  *lcd_loc = BEGIN_LOC_LINE4;
		   break;		 
	 }	
      *lcd_loc = *lcd_loc + start_col_num - 1;           
   }
   else
   {
	    lcd_avail_loc_within_limit = STATE_NO_IN_CHAR;
		/* error: due to loc_lcd's line num > max configured line nums */	
		 
	   		
   }	   
} 
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 32   
-*------------------------------------------------------------*/
void From_Loc_to_XY_LCD(const unsigned int loc_lcd, unsigned int * const loc_lcd_line_num, unsigned int * const loc_lcd_col_num  )
{
	/* loc_lcd's corrosponding line num and col num */
	
	lcd_avail_loc_within_limit = STATE_YES_IN_CHAR;
	
	if(CONFIGURE_MAX_NUM_LINES <= MAX_AVAIL_NUM_LINES)
	{	
    	if(loc_lcd >= BEGIN_LOC_LINE1 && loc_lcd <= END_LOC_LINE1)
	    {
		     *loc_lcd_line_num = NUM_LINE1;
	         *loc_lcd_col_num = loc_lcd + 1 - BEGIN_LOC_LINE1;		    
	    }
       if(loc_lcd >= BEGIN_LOC_LINE2 && loc_lcd <= END_LOC_LINE2)
	   {
	     	*loc_lcd_line_num = NUM_LINE2;
		    *loc_lcd_col_num = loc_lcd + 1 - BEGIN_LOC_LINE2; 		
	   }     
	   if(loc_lcd >= BEGIN_LOC_LINE3 && loc_lcd <= END_LOC_LINE3)
	   {
		   	*loc_lcd_line_num = NUM_LINE3;
		  	*loc_lcd_col_num = loc_lcd + 1 - BEGIN_LOC_LINE3;			
	   }
       if(loc_lcd >= BEGIN_LOC_LINE4 && loc_lcd <= END_LOC_LINE4)
	   {
		  	*loc_lcd_line_num = NUM_LINE4;
		   	*loc_lcd_col_num = loc_lcd + 1 - BEGIN_LOC_LINE4;				
	   }	  
   } 
   else
   {
	   /* error: configured max lines > 4 */
	   
   }      
}

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
