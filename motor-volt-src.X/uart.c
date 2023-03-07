/**************************************************************************
   FILE          :    uart.c
 
   PURPOSE       :   UART Library
 
   AUTHOR        :     K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 #include "main.h"
 #include "lcd.h"
 #include "uart.h"
 #include "io_conf.h"
 /*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : change SPBRG depending on  Osc freq  and baud rate 

Func ID        : 21   
-*------------------------------------------------------------*/

void UART_Init()
{
	//SPBRG = 25;  /* (_XTAL_FREQ /(16 * baud_rate)) - 1,  Osc freq = 4MHz, Baud Rate = 2400  */
	SPBRG = 32;  /* (_XTAL_FREQ /(16 * baud_rate)) - 1,  Osc freq = 20MHz, Baud Rate = 9600  */
	BRG16 = 0 ;  /* 8-bit Baud Rate Generator is used */
	BRGH  = 0;   /* Fast baud rate */
	SPEN  = 1;   /* enable serial port pins */
	SYNC  = 0;   /* Asynchrous mode */ 
	CREN  = 1;   /* enable continuous receive */	
	TXEN  = 0;   /* reset transmitter */
	TXEN  = 1;   /* enable transmitter */
	
	SREN  = 0;   /* single receive disabled */	
	TXIE  = 0;   /* disable transmit interrupt */
	RCIE  = 1;   /* enable receiver interrupt */
	TX9   = 0;   /* 8 bit transmission */
	RX9   = 0;   /* 8 bit reception */
	
}
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void UART_Transmit_Char(const char transmit_char)
{
	TXREG = transmit_char;	
	while(TRMT == 0); /* wait as TSR is full */	
}
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void UART_Transmit_Str(const char *transmit_str)
{
	while(*transmit_str)
	{
		 UART_Transmit_Char(*transmit_str); 		 	
		 ++transmit_str;
	}		
}

/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
char UART_Receive_Char()
{
	char receive_char = 0;
	const char framing_error_msg_disp[] = "FERR", overrun_error_msg_disp[] = "OERR";
	
	/* overrun error has occured */
	if(OERR == 1)
	{
		/* internal error : overrun error has occurred */
		
		//SHOULD_REMOVE
		Goto_XY_LCD_Disp(LCD_DISP_ERR_LINE_NUM, NUM_COL1);
	    LCD_Disp_Str(overrun_error_msg_disp);
		UART_Transmit_Str("\rERR: OERR\r ");	
		
		CREN = 0; /* clear (CREN) continuous reception flag to clear OERR flag */
		CREN = 1; /* enable (CREN) continuous reception flag to continue receive datas */
	}
	while(RCIF == 0);	
	
	/* received char has framing error */
 	if(FERR == 1)  
    {  
       receive_char = RCREG;
	   /* internal error: framing error has occurred */
	   
	   //SHOULD_REMOVE
	   Goto_XY_LCD_Disp(LCD_DISP_ERR_LINE_NUM, NUM_COL1);
	   LCD_Disp_Str(framing_error_msg_disp);
	   UART_Transmit_Str("\rERR: FERR\r ");	
	   
	   return 0;
 	}
	receive_char = RCREG;
	/* echo received char in virtual terminal */
	UART_Transmit_Char(receive_char);
	return receive_char;
}

#ifdef WANT_REPLACE
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
char UART_Receive_Char()
{
	char receive_char = 0;
	const char framing_error_msg_disp[] = "FERR", overrun_error_msg_disp[] = "OERR";
	
	if(cur_data_conf_parameter.cur_data_input_dev_id == INPUT_DEV_ID_UART && uart_rcv_enable_flag == STATE_YES_IN_CHAR)
	{
	   /* overrun error has occured */
	   if(OERR == 1)
	   {
		  /* internal error : overrun error has occurred */
		
		  //SHOULD_REMOVE
		   #ifdef TRACE_ERROR
	          Goto_XY_LCD_Disp(LCD_DISP_ERR_LINE_NUM, NUM_COL1);
	          LCD_Disp_Str(overrun_error_msg_disp);
		      UART_Transmit_Str("\rERR: rcvd Uart overrun error \r ");	
		   #endif
		   CREN = 0; /* clear (CREN) continuous reception flag to clear OERR flag */
		   CREN = 1; /* enable (CREN) continuous reception flag to continue receive datas */
	    }
	    while(RCIF == 0);	
	    receive_char = RCREG;
	   /* received char has framing error */
 	    if(FERR == 1)  
        {        
	       /* internal error: framing error has occurred */
	       #ifdef TRACE_ERROR
	         //SHOULD_REMOVE
	         Goto_XY_LCD_Disp(LCD_DISP_ERR_LINE_NUM, NUM_COL1);
	         LCD_Disp_Str(framing_error_msg_disp);
	         UART_Transmit_Str("\rERR: rcvd Uart framing error \r ");	
	      #endif
	      return STATE_INVALID;
 	   }
	   /* echo received char in virtual terminal */
	   UART_Transmit_Char(receive_char);
	   return receive_char;
    }
	else
	{
		#ifdef TRACE_ERROR
		   UART_Transmit_Str("ERR: try to rcv uart but uart is disabled \r");
		#endif
	}
	return STATE_INVALID;
}
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
int UART_Receive_Str()
{
	char receive_char = NULL_CHAR;
	const char framing_error_msg_disp[] = "FERR", overrun_error_msg_disp[] = "OERR";
	
	if(cur_data_conf_parameter.cur_data_input_dev_id == INPUT_DEV_ID_UART && uart_rcv_enable_flag == STATE_YES_IN_CHAR)
	{
		switch(cur_data_conf_parameter.cur_data_rcv_mode)
		{
			//enter data till enter key is pressed
		    case CUR_DATA_RCV_MODE_TILL_TERMINATOR:
			    lcd_cur_disp_loc = LCD_BEGIN_LOC_ECHO_UART_RCVD_STR;
		        do
			    {
	                receive_char = UART_Receive_Char();
					if(receive_char == STATE_INVALID)
						return FAILURE;
		            switch(receive_char)
		            {
			             case ENTER_CHAR:
			              /* rcvd char is enter key, and act as data terminator char */
			                cur_data_status.cur_data_max_allocated_num_chars_rcvd_flag = STATE_NO_IN_CHAR;
                            cur_data_status.cur_data_valid_end_rcvd_flag = STATE_YES_IN_CHAR;
		                 break;
                         case BACKSPACE_CHAR:
			              /* rcvd char is backspace */
				            if(cur_data_status.cur_data_num_chars_rcvd > 0)
				            {
				                --lcd_cur_disp_loc;
				                Write_LCD_Command(lcd_cur_disp_loc);
				                Write_LCD_Data(' '); 
                                Write_LCD_Command(0x10); //shift cursor to left 
					            --cur_data_status.cur_data_num_chars_rcvd;				           
				            }
                            else
				            {
				    	         cur_data_status.cur_data_rcvd_str[0] = NULL_CHAR;
					             lcd_cur_disp_loc = LCD_BEGIN_LOC_ECHO_UART_RCVD_STR;
			                }
			             break;   
			             default:
			    	         cur_data_status.cur_data_rcvd_str[cur_data_status.cur_data_num_chars_rcvd] = receive_char;   
				   
		                     #ifdef TRACE
				                Write_LCD_Command(lcd_cur_disp_loc); 
		                        Write_LCD_Data(cur_data_status.cur_data_rcvd_str[cur_data_status.cur_data_num_chars_rcvd]);
		                     #endif	
				 
				              ++lcd_cur_disp_loc;
					          ++cur_data_status.cur_data_num_chars_rcvd;
				              if(cur_data_status.cur_data_num_chars_rcvd >= UART_CONF_MAX_NUM_CHARS_TO_RCV )
                              {
	                              cur_data_status.cur_data_max_allocated_num_chars_rcvd_flag = STATE_YES_IN_CHAR;
                                  cur_data_status.cur_data_valid_end_rcvd_flag = STATE_NO_IN_CHAR;					   
                              }
			         }				
	            }
	            while(cur_data_status.cur_data_max_allocated_num_chars_rcvd_flag == STATE_NO_IN_CHAR && cur_data_status.cur_data_valid_end_rcvd_flag == STATE_NO_IN_CHAR);
			break;
			//enter data till max nums of chars is reached
            case CUR_DATA_RCV_MODE_TILL_ALLOCATED:	
		       if(cur_data_conf_parameter.cur_data_allocated_max_num_chars_to_rcv > 0 )
		       {
			       if(cur_data_conf_parameter.cur_data_allocated_max_num_chars_to_rcv <= UART_CONF_MAX_NUM_CHARS_TO_RCV)
	               {
			            lcd_cur_disp_loc = LCD_BEGIN_LOC_ECHO_UART_RCVD_STR;
		                for(cur_data_status.cur_data_num_chars_rcvd = 0; cur_data_status.cur_data_num_chars_rcvd < cur_data_conf_parameter.cur_data_allocated_max_num_chars_to_rcv; ++cur_data_status.cur_data_num_chars_rcvd )
                        {
		        	        receive_char = UART_Receive_Char();
					        if(receive_char == STATE_INVALID)
					         	return FAILURE; 
				            switch(receive_char)	
                            {
					            case ENTER_CHAR:
					               /* rcvd char is enter key, and act as data terminator char */
			                        cur_data_status.cur_data_max_allocated_num_chars_rcvd_flag = STATE_NO_IN_CHAR;
                                    cur_data_status.cur_data_valid_end_rcvd_flag = STATE_YES_IN_CHAR;
					            break;   
					            case BACKSPACE_CHAR:
			     	               /* rcvd char is backspace */
				                   if(cur_data_status.cur_data_num_chars_rcvd > 0)
				                   {
				                 	   --lcd_cur_disp_loc;
				                       Write_LCD_Command(lcd_cur_disp_loc);
				                       Write_LCD_Data(' '); 
                                       Write_LCD_Command(0x10); //shift cursor to left 
					                  --cur_data_status.cur_data_num_chars_rcvd;				           
				                   }
                                   else
				                   {
					                   cur_data_status.cur_data_rcvd_str[0] = NULL_CHAR;
					                   lcd_cur_disp_loc = LCD_BEGIN_LOC_ECHO_UART_RCVD_STR;
			                       }
					            break;   
			                    default:
					                cur_data_status.cur_data_rcvd_str[cur_data_status.cur_data_num_chars_rcvd] = receive_char;   
	                  
		                            #ifdef TRACE
					                    Write_LCD_Command(lcd_cur_disp_loc); 
		                                Write_LCD_Data(cur_data_status.cur_data_rcvd_str[cur_data_status.cur_data_num_chars_rcvd]);
		                            #endif	
					  
				                   ++lcd_cur_disp_loc;
			                 }
					         if(cur_data_status.cur_data_valid_end_rcvd_flag == STATE_YES_IN_CHAR)
					         {
					        	break;
					         }
	                    }
				        if(cur_data_status.cur_data_num_chars_rcvd == cur_data_conf_parameter.cur_data_conf_max_num_chars_to_rcv)
				        {
	                        cur_data_status.cur_data_valid_end_rcvd_flag = STATE_NO_IN_CHAR;
                            cur_data_status.cur_data_max_allocated_num_chars_rcvd_flag = STATE_YES_IN_CHAR;
				        }
			        }
                    else
                    {
			         	#ifdef TRACE_ERROR
			                UART_Transmit_Str("ERR: uart set max chars to rcv > configured \r");
			 	        #endif
				        return FAILURE;
			        }  				
	            }	
            break;
		}	
		cur_data_status.cur_data_rcvd_str[cur_data_status.cur_data_num_chars_rcvd]= NULL_CHAR;
	    UART_Transmit_Char(ENTER_CHAR);
        return SUCCESS;
	}
	else
	{
		#ifdef TRACE_ERROR
		   UART_Transmit_Str("ERR: try to rcv uart but uart is disabled \r");
		#endif
	}
	return FAILURE;
}
#endif

/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void UART_Transmit_Num(const disp_num_types uart_datanum_disp_format, const value_types uart_disp_data_int)
{
    unsigned int tens_thousand_digit,thousands_digit, hundreds_digit,tens_digit, unit_digit;
    unsigned long disp_num, num ;	
    char num_data[] ={'0','1','2','3','4','5','6','7','8','9'};  
	char hex_data[] ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'}; 
	
	
    if(uart_datanum_disp_format & 0x10 ) //signed uart_datanum_disp_format
	{
		if(uart_disp_data_int.signed_val.value_long < 0 )
		{
			UART_Transmit_Char('-');
			disp_num = -uart_disp_data_int.signed_val.value_long;
		}
        else
		{
			UART_Transmit_Char('+');
			disp_num = uart_disp_data_int.signed_val.value_long;
		}
        
	}
	else
	{
    	disp_num = uart_disp_data_int.unsigned_val.value_long; 
	}
	num =  disp_num ;
	
    switch(uart_datanum_disp_format)
	{
	  case DISP_UNSIGN_NUM_DIGIT5:
	  case DISP_SIGN_NUM_DIGIT5:  
		  num =  disp_num ;
		  tens_thousand_digit = (unsigned int)(num / (unsigned long)(10000UL));
          UART_Transmit_Char(num_data[tens_thousand_digit]);
	  case DISP_UNSIGN_NUM_DIGIT4:
	  case DISP_SIGN_NUM_DIGIT4: 
	      num = disp_num % 10000UL;
	      thousands_digit = (unsigned int)(num / (unsigned long)(1000UL));
	      UART_Transmit_Char(num_data[thousands_digit]); 
	  case DISP_UNSIGN_NUM_DIGIT3:
	  case DISP_SIGN_NUM_DIGIT3:
		  num = disp_num % 1000UL;
	      hundreds_digit = (unsigned int) (num / (unsigned long) (100));
	      UART_Transmit_Char(num_data[hundreds_digit]);
	  case DISP_UNSIGN_NUM_DIGIT2:
	  case DISP_SIGN_NUM_DIGIT2:
          num = disp_num % 100;
          tens_digit = (unsigned int) (num / 10);
          UART_Transmit_Char(num_data[tens_digit]); 		  
	  case DISP_UNSIGN_NUM_DIGIT1:
	  case DISP_SIGN_NUM_DIGIT1:
	     unit_digit = (unsigned int) (disp_num % 10);
         UART_Transmit_Char(num_data[unit_digit]); 
	  break;
	  case DISP_HEX_DIGIT4:
	      //  ( 16 * 16 * 16 *16 )  = 0 as divide by zero warning 
	      //num = disp_num % ( 16 * 16 * 16 *16 );
          thousands_digit = (num / (unsigned long) (16 * 16 * 16));
	      UART_Transmit_Char(hex_data[thousands_digit]);
	  case DISP_HEX_DIGIT3:
	      num = disp_num %(unsigned long)(16 * 16 * 16);
	      hundreds_digit = (unsigned int) (num / (unsigned long) (16 * 16));
	      UART_Transmit_Char(hex_data[hundreds_digit]);
	  case DISP_HEX_DIGIT2:
	      num = disp_num %(unsigned long)(16 * 16);
          tens_digit = (unsigned int) (num / 16);
          UART_Transmit_Char(hex_data[tens_digit]);
	  case DISP_HEX_DIGIT1: 
	      unit_digit = (unsigned int) (disp_num % 16);
          UART_Transmit_Char(hex_data[unit_digit]);    
	  break;
	  
	}   	
}
/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
