/**************************************************************************
   FILE          :    intp_event_handle.h
 
   PURPOSE       :    interrupt Event Handler Header
 
   AUTHOR        :     K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 
 #ifndef _INTP_EVENT_HANDLE_H
 #define _INTP_EVENT_HANDLE_H
 
/* ---------------------- macro defination ------------------------------------------------ */


 /* -------------------- public prototype declaration --------------------------------------- */
 
void External_Interrupt_Occured_Appl_Proc();
void Timer1_Req_Time_Expiry_Appl_Proc();
void ADC_Conv_Over_Appl_Proc(adc_channels adc_cur_channel);

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
