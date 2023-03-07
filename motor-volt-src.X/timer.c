/**************************************************************************
   FILE          :    timer.c
 
   PURPOSE       :   Timer Library
 
   AUTHOR        :   K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 #include "main.h"
 #include "io_conf.h"
 #include "uart.h"
 #include "timer.h"

unsigned int timer1_prescale = 1, timer1_prescale_shift= 0, timer1_elapsed_num_update = 0 ;
unsigned long int  timer1_init_val = 0, timer1_elapsed_num_overflow_1_update = 0, timer1_1_update = 0, timer1_max_num_overflow =0, timer1_req_time_max_update = 0;
timer1_run_states  timer1_cur_run_state= TMR1_STOP_STATE, timer1_last_run_state_before_stop = TMR1_STOP_STATE;
unsigned long int timer1_req_time_delay_in_milli_sec;
timer1_service_types timer1_cur_service_type;
volatile unsigned long measure_pulse_lower_count, measure_pulse_upper_count;

unsigned int timer2_prescale = 1, timer2_prescale_shift= 0, timer2_elapsed_num_update = 0 ;
unsigned long int  timer2_init_val = 0, timer2_elapsed_num_overflow_1_update = 0, timer2_1_update = 0, timer2_max_num_overflow =0, timer2_req_time_max_update = 0;
timer2_run_states  timer2_cur_run_state= TMR2_STOP_STATE, timer2_last_run_state_before_stop = TMR2_STOP_STATE;
unsigned long int timer2_req_time_delay_in_milli_sec;
timer2_service_types timer2_cur_service_type;

 /*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void Timer1_Run(const timer1_run_states set_timer1_run_mode, const unsigned long int set_req_time_delay_in_milli_sec, const timer1_service_types set_timer1_service, const timer1_gate_control_types set_gate_control_type, \
  const timer1_input_clock_prescaler_types set_timer1_input_clock_prescaler_type, const timer1_LP_osc_control_types set_timer1_LP_osc_control_type, const timer1_clock_sources set_timer1_clock_source  )
{
	  Timer1_Stop();
	  
      if(set_timer1_service & 0x04)
	  {
		  //set_timer1_service  = TMR1_INVALID_SERVICE
		  return;
	  }		  
	  timer1_cur_run_state = timer1_last_run_state_before_stop = set_timer1_run_mode;	  
	  T1CON =  set_gate_control_type | set_timer1_clock_source |  set_timer1_LP_osc_control_type | set_timer1_input_clock_prescaler_type; 
	  Timer1_Prescale(); 
	  
	  #ifdef TRACE
	     to_disp.unsigned_val.value_long = T1CON;
         UART_Transmit_Str("T1CON config : 0x");
         UART_Transmit_Num(DISP_HEX_DIGIT2, to_disp);
	     UART_Transmit_Char('\r');                	  
      #endif
	  
	   timer1_cur_service_type = set_timer1_service;
	   
	   if(timer1_cur_service_type & 0x02)
	   {
		   //timer1 is in timer mode
		   if((set_timer1_clock_source & 0x02) | (T1CON & 0x02) == 0x00)
		   {
	            //Timer1 Clock Source is internal clock
				
				#ifdef TRACE
	              UART_Transmit_Str("tmr1 in timer mode \r"); 
	            #endif
				
				Timer1_Load_Init_Val_Calc(set_req_time_delay_in_milli_sec); 	
	       }
           else
		   {
			  #ifdef TRACE_ERROR
				   UART_Transmit_Str("ERR: tmr1 is timer but clk source is not internal \r");
			  #endif
			  
			  timer1_cur_service_type = TMR1_INVALID_SERVICE;
			  return;
		   }	
	   }
	   else
	   {
		   //timer1 is in counter mode
		   if((set_timer1_clock_source & 0x02) & (T1CON & 0x02)) 
		   {
				//Timer1 Clock Source is external clock  
				if(set_timer1_LP_osc_control_type & 0x08)
				{	
			         TMR1 = measure_pulse_lower_count;
					 
                   	//LP oscillator is enabled for Timer1 clock	
			    	#ifdef TRACE
					   to_disp.unsigned_val.value_long = TMR1;
	                   UART_Transmit_Str("tmr1 in counter mode, TMR1 = 0x"); 
					   UART_Transmit_Num(DISP_HEX_DIGIT4, to_disp );
	                   UART_Transmit_Char('\r');  
	                #endif
				}
				else
				{
					#ifdef TRACE_ERROR
				      UART_Transmit_Str("ERR: tmr1 is counter but LP osc is disabled \r");
				    #endif
					
					timer1_cur_service_type = TMR1_INVALID_SERVICE;
				    return;
				}
		   }
		   else
		   {
				#ifdef TRACE_ERROR
				   UART_Transmit_Str("ERR: tmr1 is counter but clk source is not external \r");
				#endif
				
				timer1_cur_service_type = TMR1_INVALID_SERVICE;
				return;
		   }
	   }
	   
	      if(timer1_cur_service_type & 0x01)
	      {
		      //timer service is interrupt
		       TMR1IF = 0;   		 
               PIE1bits.TMR1IE = 1; //Enables the Timer1 overflow interrupt 
		       INTCONbits.PEIE = 1;  //Enables all unmasked peripherals interrupts   
               INTCONbits.GIE = 1;  //Enables all unmasked interrupts 
	  
               #ifdef TRACE
	              UART_Transmit_Str("TMR1IE, PEIE & GIE are enabled, interrupt service \r"); 
	           #endif		   
	      }
	      else
	      {
		     //timer1 service is polling and disable the Timer1 overflow interrupt 
		     PIE1bits.TMR1IE = 0; 
			
		     #ifdef TRACE
	            UART_Transmit_Str("TMR1IE is disabled, polling service \r"); 
	         #endif
	      }       
	  
	  TMR1IF = 0;
      T1CONbits.TMR1ON = 1 ; //Enables Timer1 and timer1 runs	  
}   
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void Timer1_Stop()
{
	//if(timer1_cur_run_state != TMR1_STOP_STATE)
	{
	   T1CONbits.TMR1ON = 0 ; //stops Timer1.
	   PIE1bits.TMR1IE = 0; //disable the Timer1 overflow interrupt 
	   TMR1IF = 0;
	   timer1_cur_run_state = TMR1_STOP_STATE;
	   timer1_cur_service_type = TMR1_INVALID_SERVICE;
	   
	   //Commited due to stack overflow
	  /* #ifdef TRACE 
	     UART_Transmit_Str("Timer1 is stopped \r");
	   #endif */	 
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
void Timer1_Prescale()
{
	unsigned int i;
	
   timer1_prescale = 1;
   timer1_prescale_shift= 0;
   if(T1CKPS0 == 1)
   {
      timer1_prescale_shift |= 0x01;           
   }
   if(T1CKPS1 == 1)
   {
     timer1_prescale_shift |= 0x02;
   }
   for(i = 1; i <= timer1_prescale_shift; ++i)
   {
      timer1_prescale *= 2;
   }  
}

/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : http://picguides.com/beginner/timers.php
                 yet to calculate timer1_adjust_init_val to include timer1 overflow instructions executed delay 
				 from TMR1IF == 1 till instruction to reload TMR1 register to get almost precise time delay

Func ID        : 21   
-*------------------------------------------------------------*/
void Timer1_Load_Init_Val_Calc(const unsigned long int set_req_time_delay_in_milli_sec)
{
	unsigned long int inc_timer1;
	unsigned int timer1_adjust_init_val;
	unsigned long int set_timer1_tick_in_milli_sec = TIMER1_TICK_IN_MILLI_SEC;
	unsigned long int set_timer1_req_time_1_update_in_milli_sec = TIMER1_REQ_TIME_1_UPDATE_REACHED_IN_MILLI_SEC;
	unsigned long int timer1_remainder; 
		
   /*	#ifdef TRACE
        to_disp.unsigned_val.value_long = timer1_prescale;
       UART_Transmit_Str("Timer1 Prescaler : ");
       UART_Transmit_Num(DISP_UNSIGN_NUM_DIGIT2, to_disp );
	   UART_Transmit_Char('\r');		
    #endif */
	
      timer1_elapsed_num_overflow_1_update = 0;
	  timer1_1_update = set_timer1_req_time_1_update_in_milli_sec /set_timer1_tick_in_milli_sec;
      timer1_remainder = set_req_time_delay_in_milli_sec % set_timer1_req_time_1_update_in_milli_sec;	  
	  if( timer1_remainder == 0)
	  {
		  //set_req_time_delay_in_milli_sec is in multiples of  set_timer1_req_time_1_update_in_milli_sec
		  timer1_req_time_delay_in_milli_sec = set_req_time_delay_in_milli_sec;		  	 
	  }
	  else
	  {
		  //set_req_time_delay_in_milli_sec is not in multiples of  set_timer1_req_time_1_update_in_milli_sec
		  #if TIMER1_SET_TIME_DELAY_IN_MULTIPLE == TIMER1_PRESET_TIME_DELAY_IN_MULTIPLE
		    //timer1_req_time_delay_in_milli_sec is previous valid req_time_delay_in_milli_sec, which is multiple of set_timer1_req_time_1_update_in_milli_sec
		     timer1_req_time_delay_in_milli_sec = set_req_time_delay_in_milli_sec - timer1_remainder;
		  #else //TIMER1_SET_TIME_DELAY_IN_MULTIPLE == TIMER1_POSTSET_TIME_DELAY_IN_MULTIPLE
		     //timer1_req_time_delay_in_milli_sec is next valid req_time_delay_in_milli_sec, which is multiple of set_timer1_req_time_1_update_in_milli_sec
             timer1_req_time_delay_in_milli_sec = set_req_time_delay_in_milli_sec - timer1_reminder + set_timer1_req_time_1_update_in_milli_sec ;	
          #endif	
	  }
	  timer1_req_time_max_update = timer1_req_time_delay_in_milli_sec / set_timer1_req_time_1_update_in_milli_sec;
	  
	  #ifdef TRACE
	     to_disp.unsigned_val.value_long = timer1_req_time_delay_in_milli_sec;
	     UART_Transmit_Str("Timer1 start running set for expiry : ");
         UART_Transmit_Num(DISP_UNSIGN_NUM_DIGIT5, to_disp); 
	     UART_Transmit_Str(" millisecs \r"); 
      #endif	
	  
	switch(timer1_prescale)
    {
	   case 1:
	      timer1_adjust_init_val = 2;
       break;
	   case 2:
          timer1_adjust_init_val = 1;
       break;
       default:  // for timer1_prescale > 2 , timer1_adjust_init_val = 0;
          timer1_adjust_init_val = 0;	   
    } 
    /* for timer0, TMR0 = (256 - ((timerPeriod_in_sec * Fosc_in_Hertz)/(4*prescaler)) + timer0_adjust_init_val), 
	where timer0_adjust_init_val = 2 for prescaler = 1, timer0_adjust_init_val =  1 for prescaler = 2,  timer0_adjust_init_val = 0 for prescaler > 2 */
    // inc_timer1  =      (unsigned long)((unsigned long)(_XTAL_FREQ * set_timer1_tick_in_milli_sec ) / (unsigned long)(OSC_PER_INST * TIME_UNIT_1_SEC_IN_MILLI_SEC  )) 
	inc_timer1 = 1000ul * set_timer1_tick_in_milli_sec; // for 1000ul = 4MHz / (4 * 1000), when _XTAL_FREQ  = 4MHz  
    timer1_init_val = ((65535UL) - (inc_timer1/timer1_prescale)) + 1 + timer1_adjust_init_val; 
    TMR1H = timer1_init_val / 256UL;
    TMR1L = timer1_init_val % 256UL; 
}

 /*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void Timer2_Run(const timer2_run_states set_timer2_run_mode, const unsigned long int set_req_time_delay_in_milli_sec, const timer2_service_types set_timer2_service, \
  const timer2_prescaler_types set_timer2_prescaler_type, const unsigned int set_timer2_postscaler_val  )
{
	  	  
	  Timer2_Stop(); 
	  if(set_timer2_service & 0x04)
	  {
		  //set_timer2_service  = TMR1_INVALID_SERVICE
		  return;
	  }
	   
	  timer2_cur_service_type = set_timer2_service;
	  timer2_cur_run_state = timer2_last_run_state_before_stop = set_timer2_run_mode;
	  if(timer2_cur_service_type & 0x02)
	  {
		   //timer2 for PWM 
		    TMR2 = 0x0000;
	        TMR2IF = 0; 
	  }
	  if(timer2_cur_service_type & 0x01)
	  {
		   //timer2 service is interrupt
		   TMR2IF = 0;   		 
           PIE1bits.TMR2IE = 1; //Enables the Timer2 overflow interrupt 
		   INTCONbits.PEIE = 1;  //Enables all unmasked peripherals interrupts   
           INTCONbits.GIE = 1;  //Enables all unmasked interrupts 
	  
           #ifdef TRACE
	           UART_Transmit_Str("TMR2IE, PEIE & GIE are enabled, interrupt service \r"); 
	      #endif		   
	  }
	  else
	  {
		   //timer2 service is polling and disable the Timer2 overflow interrupt 
		   PIE1bits.TMR2IE = 0; 
			
		    #ifdef TRACE
	           UART_Transmit_Str("TMR2IE is disabled, polling service \r"); 
	        #endif
	  }       
	 	  
	  T2CON =  set_timer2_prescaler_type | ((set_timer2_postscaler_val - 1) << 4) ; 
	  Timer2_Prescale();
      
	  #ifdef TRACE
	      to_disp.unsigned_val.value_long = T2CON;
          UART_Transmit_Str("T2CON config : 0x");
          UART_Transmit_Num(DISP_HEX_DIGIT2, to_disp );
	      UART_Transmit_Char('\r');
		  
         /*  to_disp.unsigned_val.value_long = timer2_req_time_delay_in_milli_sec; 
		 UART_Transmit_Str("Timer2 start running set for expiry : ");
         UART_Transmit_Num(DISP_UNSIGN_NUM_DIGIT5, to_disp ); 
	     UART_Transmit_Str(" millisecs \r"); */         	  
      #endif
	  
      T2CONbits.TMR2ON = 1 ; //Enables Timer2 and timer2 runs
}   
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void Timer2_Stop()
{
	//if(timer2_cur_run_state != TMR2_STOP_STATE)
	{
	   T2CONbits.TMR2ON = 0 ; //stops Timer1.
	   PIE1bits.TMR2IE = 0; //disable the Timer2 overflow interrupt 
	   TMR2IF = 0;
	   timer2_cur_run_state = TMR2_STOP_STATE;
	   timer2_cur_service_type = TMR2_INVALID_SERVICE;
	   
	   //Commited due to stack overflow
	  /* #ifdef TRACE 
	     UART_Transmit_Str("Timer2 is stopped \r");
	   #endif */	 
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
void Timer2_Prescale()
{
	unsigned int i;
	
   timer2_prescale = 1;
   timer2_prescale_shift= 0;
   if(T2CKPS1 == 1)
   {
     timer2_prescale_shift = 0x02; 
   }
   else
   {
      if(T2CKPS0 == 1)
      {
          timer2_prescale_shift = 0x01;           
      }
   }
   for(i = 1; i <= timer2_prescale_shift; ++i)
   {
      timer2_prescale *= 4;
   }  
}

/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : http://picguides.com/beginner/timers.php
                 yet to calculate timer2_adjust_init_val to include timer1 overflow instructions executed delay 
				 from TMR2IF == 1 till instruction to reload TMR2 register to get almost precise time delay

Func ID        : 21   
-*------------------------------------------------------------*/
void Timer2_Load_Init_Val_Calc(const unsigned long int set_req_time_delay_in_milli_sec)
{
	unsigned long int inc_timer2;
	unsigned int timer2_adjust_init_val;
	unsigned long int set_timer2_tick_in_milli_sec = TIMER2_TICK_IN_MILLI_SEC;
	unsigned long int set_timer2_req_time_1_update_in_milli_sec = TIMER2_REQ_TIME_1_UPDATE_REACHED_IN_MILLI_SEC;
	unsigned long int timer2_remainder; 
	
	
/*	#ifdef TRACE
        to_disp.unsigned_val.value_long = timer2_prescale;
       UART_Transmit_Str("Timer2 Prescaler : ");
       UART_Transmit_Num(DISP_UNSIGN_NUM_DIGIT2, to_disp );
	   UART_Transmit_Char('\r');		
    #endif */
   
	  timer2_elapsed_num_overflow_1_update = 0;
	  timer2_1_update = set_timer2_req_time_1_update_in_milli_sec /set_timer2_tick_in_milli_sec;
      timer2_remainder = set_req_time_delay_in_milli_sec % set_timer2_req_time_1_update_in_milli_sec;	  
	  if( timer2_remainder == 0)
	  {
		  //set_req_time_delay_in_milli_sec is in multiples of  set_timer1_req_time_1_update_in_milli_sec
		  timer2_req_time_delay_in_milli_sec = set_req_time_delay_in_milli_sec;		  	 
	  }
	  else
	  {
		  //set_req_time_delay_in_milli_sec is not in multiples of  set_timer2_req_time_1_update_in_milli_sec
		  #if TIMER2_SET_TIME_DELAY_IN_MULTIPLE == TIMER2_PRESET_TIME_DELAY_IN_MULTIPLE
		    //timer2_req_time_delay_in_milli_sec is previous valid req_time_delay_in_milli_sec, which is multiple of set_timer2_req_time_1_update_in_milli_sec
		     timer2_req_time_delay_in_milli_sec = set_req_time_delay_in_milli_sec - timer2_remainder;
		  #else //TIMER2_SET_TIME_DELAY_IN_MULTIPLE == TIMER2_POSTSET_TIME_DELAY_IN_MULTIPLE
		     //timer2_req_time_delay_in_milli_sec is next valid req_time_delay_in_milli_sec, which is multiple of set_timer2_req_time_1_update_in_milli_sec
             timer2_req_time_delay_in_milli_sec = set_req_time_delay_in_milli_sec - timer2_reminder + set_timer2_req_time_1_update_in_milli_sec ;	
          #endif	
	  }
	  timer2_req_time_max_update = timer2_req_time_delay_in_milli_sec / set_timer2_req_time_1_update_in_milli_sec;
	  
	  #ifdef TRACE
	     to_disp.unsigned_val.value_long = timer2_req_time_delay_in_milli_sec;
	     UART_Transmit_Str("Timer2 start running set for expiry : ");
         UART_Transmit_Num(DISP_UNSIGN_NUM_DIGIT5, to_disp); 
	     UART_Transmit_Str(" millisecs \r"); 
      #endif	
	  
	switch(timer2_prescale)
    {
	   case 1:
	      timer2_adjust_init_val = 2;
       break;
	   case 2:
          timer2_adjust_init_val = 1;
       break;
       default:  // for timer1_prescale > 2 , timer1_adjust_init_val = 0;
          timer2_adjust_init_val = 0;	   
    } 
    /* for timer0, TMR0 = (256 - ((timerPeriod_in_sec * Fosc_in_Hertz)/(4*prescaler)) + timer0_adjust_init_val), 
	where timer0_adjust_init_val = 2 for prescaler = 1, timer0_adjust_init_val =  1 for prescaler = 2,  timer0_adjust_init_val = 0 for prescaler > 2 */
    // inc_timer2  =      (unsigned long)((unsigned long)(_XTAL_FREQ * set_timer2_tick_in_milli_sec ) / (unsigned long)(OSC_PER_INST * TIME_UNIT_1_SEC_IN_MILLI_SEC  )) 
	inc_timer2 = 1000ul * set_timer2_tick_in_milli_sec; // for 1000ul = 4MHz / (4 * 1000), when _XTAL_FREQ  = 4MHz  
    timer2_init_val = ((65535UL) - (inc_timer2/timer1_prescale)) + 1 + timer2_adjust_init_val; 
    TMR2 = timer2_init_val;    
}
/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
