/**************************************************************************
   FILE          :    intp_event_Handle.c
 
   PURPOSE       :  interrupt Event Handler Library
 
   AUTHOR        :     K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 #include "main.h"
 #include "port.h"
 #include "io_conf.h"
 #include "appl_conf.h"
 #include "lcd.h"
 #include "uart.h"
 #include "adc.h"
 #include "timer.h" 
 
 
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 32   
-*------------------------------------------------------------*/
void External_Interrupt_Occured_Appl_Proc()
{	
  	
}

/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 32   
-*------------------------------------------------------------*/
void Timer1_Req_Time_Expiry_Appl_Proc()
{	
            
}

/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 32   
-*------------------------------------------------------------*/
void ADC_Conv_Over_Appl_Proc(adc_channels adc_cur_channel)
{	
    
}
/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
