/**************************************************************************
   FILE          :    uart.h
 
   PURPOSE       :    UART Header
 
   AUTHOR        :     K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 
 #ifndef _UART_H
 #define _UART_H
 
/* ---------------------- macro defination ------------------------------------------------ */


 /* -------------------- public prototype declaration --------------------------------------- */
void UART_Init();
void UART_Transmit_Char(const char transmit_char);
void UART_Transmit_Str(const char *transmit_str);
char UART_Receive_Char();
int UART_Receive_Str();
void UART_Transmit_Num(const disp_num_types uart_datanum_disp_format, const value_types uart_disp_data_int);

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
