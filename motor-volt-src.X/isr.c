/**************************************************************************
   FILE          :    isr.c
 
   PURPOSE       :   Interrupt Service Routine Library
 
   AUTHOR        :     K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 #include "main.h"
 #include "port.h"
 #include "io_conf.h"
 #include "appl_conf.h"
 #include "timer.h"
 #include "adc.h"
 #include "uart.h" 
 #include "intp_event_handle.h"
 
 /*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void interrupt Interrupt_ISR() 
{
	unsigned int adc_value_least_byte, adc_value_most_byte;
		
	if(INTE == 1 && INTF == 1)           // external interrupt ISR - higher priority
	{	
	    INTF = 0;
	    INTE = 0; //disable the INT pin external interrupt
		/*if INTF is not cleared in software, and when EXTR_INTP_SW is pressed on and after execution of External Interrupt ISR,   
		 and even when EXTR_INTP_SW is released, External Interrupt ISR keeps on been executed, until INTF, which was set, 
		 when EXTR_INTP_SW was pressed on, is cleared in software */		 
		
        			
	    #ifdef TRACE
	       UART_Transmit_Str("External Interrupt is occurred \r");
		   UART_Transmit_Str("INTE is disabled \r"); 
	    #endif 	
		
		External_Interrupt_Occured_Appl_Proc();			
		
	}
	if(ADIF == 1) //A/D interrupt ADC conversion complete (ADIF must be cleared in software). ADC ISR
	{	
	   PIE1bits.ADIE = 0;   // disable A/D Converter (ADC) Interrupt 
	  	
       adc_cur_parameters[adc_cur_channel].adc_cur_service_type = ADC_INVALID_SERVICE;   
	  
	   if(ADFM == 1) //right justfied ADC result  
	   {						   
           adc_value_least_byte = ADRESL;
	       adc_value_most_byte = ADRESH & 0x03u;
	       adc_cur_parameters[adc_cur_channel].adc_value_channel = (adc_value_most_byte << 8) + adc_value_least_byte;
		}
		else //left justfied ADC result
		{
		    adc_value_least_byte = ADRESL & 0xC0;
	        adc_value_most_byte = ADRESH ; 
			adc_cur_parameters[adc_cur_channel].adc_value_channel = (adc_value_most_byte << 8) + adc_value_least_byte;
			adc_cur_parameters[adc_cur_channel].adc_value_channel = adc_cur_parameters[adc_cur_channel].adc_value_channel >> 6;
	    }
		
	   #ifdef TRACE
	      to_disp.unsigned_val.val_in_bytes.value_byte[0] = adc_cur_channel;	
	      UART_Transmit_Str("ADC converted for channel : ");
		  UART_Transmit_Num(DISP_UNSIGN_NUM_DIGIT2, to_disp );
		  to_disp.unsigned_val.value_long = adc_cur_parameters[adc_cur_channel].adc_value_channel;
		  UART_Transmit_Str(" , val : ");
		  UART_Transmit_Num(DISP_UNSIGN_NUM_DIGIT4, to_disp );
		  UART_Transmit_Char(ENTER_CHAR);
		  UART_Transmit_Str("ADIE is disabled \r");     
	   #endif
	   
	   ADC_Conv_Over_Appl_Proc(adc_cur_channel);
	   ADIF = 0;
    } 
	if(TMR1IF == 1)     // timer1 overflow interrupt ISR - lower prority
	{
		  TMR1IF = 0;
		  TMR1H = timer1_init_val / 256UL;
          TMR1L = timer1_init_val % 256UL; 
		  //timer1 overflow for every TIMER1_TICK_IN_MILLI_SEC ie timer1_elapsed_num_overflow_1_update var is incremented for every TIMER1_TICK_IN_MILLI_SEC elapsed
          if(++timer1_elapsed_num_overflow_1_update >= timer1_1_update) 
          {
			  if(++timer1_elapsed_num_update >= timer1_req_time_max_update)
			  {
				  Timer1_Stop();
				  #ifdef TRACE
	                 UART_Transmit_Str("Timer1 is expired \r");			 
                  #endif 
				  
				  timer1_elapsed_num_update = 0;
                  timer1_elapsed_num_overflow_1_update = 0; 				  
                  Timer1_Req_Time_Expiry_Appl_Proc();  
			  }
              timer1_elapsed_num_overflow_1_update = 0;  			  
          }          		  
	 } 
}

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
