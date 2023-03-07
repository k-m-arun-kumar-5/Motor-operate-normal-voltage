/**************************************************************************
   FILE          :    adc.c
 
   PURPOSE       :   Analog to Digital Library
 
   AUTHOR        :   K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 #include "main.h"
 #include "io_conf.h"
 #include "appl_conf.h"
 #include "adc.h"
 #include "uart.h"
 
unsigned long analog_val_in_digital_ch[1]; 
adc_parameter_types  adc_cur_parameters[1];
adc_channels adc_cur_channel;
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void ADC_Conf_Channel(const adc_channels set_adc_cur_channel)
{	
   if(set_adc_cur_channel <= ADC_CH_13)
   {
      adc_cur_channel = set_adc_cur_channel; 
	  adc_cur_parameters[adc_cur_channel].adc_cur_service_type = ADC_POLLING_SERVICE;
	  adc_cur_parameters[adc_cur_channel].adc_clk_src_type = ADC_CLK_SRC_OSC_32;
	  adc_cur_parameters[adc_cur_channel].adc_result_format_justfied_type = ADC_RESULT_RIGHT_FMT;
	  adc_cur_parameters[adc_cur_channel].adc_vref_neg_src_type = ADC_VREF_NEG_SRC_INTR;
	  adc_cur_parameters[adc_cur_channel].adc_vref_plus_src_type = ADC_VREF_PLUS_SRC_INTR;
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
 void ADC_Start_Conv(adc_parameter_types *set_adc_parameters )
{
	char rcvd_status;
	unsigned adc_value_least_byte, adc_value_most_byte;
		
	rcvd_status = Is_ADC_Converting();	
	switch(rcvd_status)
	{
		//A/D conversion completed/not in progress
        case STATE_NO_IN_CHAR:
		
		   if(adc_cur_channel != ADC_INVALID_CH_14  )
	       {
              if(adc_cur_channel < 14) //max of 14 ADC channels in pic16f887
	          {
	              ADCON0 = (set_adc_parameters->adc_clk_src_type <<6) | (adc_cur_channel << 2) ;
	              ADCON1 = (set_adc_parameters->adc_result_format_justfied_type<<7) | (set_adc_parameters->adc_vref_neg_src_type << 5) | (set_adc_parameters->adc_vref_plus_src_type<<4);	
				   /*  ADC is Enabled and not yet start ADC conversion */	
                  ADCON0bits.ADON = 1; 
				  ADIF = 0;
				  switch(set_adc_parameters->adc_cur_service_type)
				  {
					  case ADC_INTP_SERVICE:
					     PIE1bits.ADIE = 1;   // Enables A/D Converter (ADC) Interrupt 
				         INTCONbits.PEIE = 1;  //Enables all unmasked peripherals interrupts   
                         INTCONbits.GIE = 1;  //Enables all unmasked interrupts 
				  
				         #ifdef TRACE
                            UART_Transmit_Str("ADIE, PEIE & GIE are enabled, interrupt service \r");
					     #endif
						 
					  break;
                      case ADC_POLLING_SERVICE:
                          PIE1bits.ADIE = 0;   // disable A/D Converter (ADC) Interrupt 
						  
      					  #ifdef TRACE
                            UART_Transmit_Str("ADIE, is disabled, polling service \r");
					      #endif 
                       break;
				  }
				  
                  #ifdef TRACE		
                     to_disp.unsigned_val.val_in_bytes.value_byte[0] = ADCON0;				  
                     UART_Transmit_Str("ADCON0 config : 0x");	
					 UART_Transmit_Num(DISP_HEX_DIGIT2,to_disp );
					 UART_Transmit_Char(ENTER_CHAR);
					  to_disp.unsigned_val.val_in_bytes.value_byte[0] = ADCON1;
					 UART_Transmit_Str("ADCON1 config : 0x");	
					 UART_Transmit_Num(DISP_HEX_DIGIT2, to_disp);
					 UART_Transmit_Char(ENTER_CHAR);
					 to_disp.unsigned_val.val_in_bytes.value_byte[0] = adc_cur_channel;
                     UART_Transmit_Str("Start ADC conv for channel : ");
	                 UART_Transmit_Num(DISP_UNSIGN_NUM_DIGIT2, to_disp);
	                 UART_Transmit_Char(ENTER_CHAR);
                   #endif 
				   
                  /* After the analog input channel is selected (or changed), an A/D acquisition must be done before the conversion
		             can be started, minimum A/D acquisition time = 20us. For safe reason,  A/D acquisition time = 500 milli sec is used */
					 
			      __delay_us(20);
		          /* Start ADC conversion cycle for set_adc_channel.  
		            GO bit will automatically cleared by hardware when the A/D conversion has completed */
		            ADCON0bits.GO = 1; 
					switch(set_adc_parameters->adc_cur_service_type)
					{
						case ADC_POLLING_SERVICE:
						   while(GO == 1);
	                        /* ADC conversion has completed*/
						   ADIF = 0;
                           
                           if(ADFM == 1) //right justfied ADC result  
						   {						   
                                 adc_value_least_byte = ADRESL;
	                             adc_value_most_byte = ADRESH & 0x03u;
								 adc_value_most_byte <<= 8;
						         set_adc_parameters->adc_value_channel = (adc_value_most_byte) | adc_value_least_byte;
						   }
						   else //left justfied ADC result
						   {
							   adc_value_least_byte = ADRESL & 0xC0;
	                           adc_value_most_byte = ADRESH ; 
							   adc_value_most_byte <<= 8;
							   set_adc_parameters->adc_value_channel =  adc_value_most_byte | adc_value_least_byte;
							   set_adc_parameters->adc_value_channel >>= 6;
						   }
	                      // ADC_Conv_Over_Appl_Proc(adc_cur_channel);
						  /*  ADC is disabled */	
                          // ADC_Disable();
						break;
					 } 
	              }
	              else
	              {
		              #ifdef TRACE_ERROR
					    to_disp.signed_val.val_in_bytes.value_byte[0] = adc_cur_channel;
		                UART_Transmit_Str("ERR: invalid ADC channel select : ");
		                UART_Transmit_Num(DISP_SIGN_NUM_DIGIT2, to_disp );
		                UART_Transmit_Char(ENTER_CHAR);
		              #endif	 
	              }
	       }
           else
	       {
		       ADCON0 = (set_adc_parameters->adc_clk_src_type << 6)| (7 << 3) | (set_adc_parameters->volt_ref << 2) ;
	       } 
		break;
        default: 
           //error: ADC conversion in progress
	    	#ifdef TRACE_ERROR
		       UART_Transmit_Str("ERR: try to start ADC while ADC Converting \r");
		    #endif  		
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
 void ADC_Disable()
{
	char rcvd_status;
	
	rcvd_status = Is_ADC_Converting();	
	switch(rcvd_status)
	{
		//A/D conversion completed/not in progress
        case STATE_NO_IN_CHAR:
		   //ADC is disabled and consumes no operating current
           ADCON0bits.ADON = 0; 
		   PIE1bits.ADIE = 0;   // disable A/D Converter (ADC) Interrupt 
		   adc_cur_parameters[adc_cur_channel].adc_cur_service_type = ADC_INVALID_SERVICE;
		   
		   #ifdef TRACE
             UART_Transmit_Str("ADC is disabled \r");
             UART_Transmit_Str("ADIE is disabled \r");
           #endif  
		   
		break;
        default: 
           //error: ADC conversion in progress
	    	#ifdef TRACE_ERROR
		       UART_Transmit_Str("ERR: try to disable ADC while ADC Converting \r");
		    #endif  		
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
 char Is_ADC_Converting()
{
	//A/D conversion completed/not in progress
	if(ADCON0bits.GO == 0)
	   return STATE_NO_IN_CHAR;
    //A/D conversion cycle is in progress.
	return STATE_YES_IN_CHAR;	
}

/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void Encoded_To_Actual_Analog_Val_Calc(unsigned long adc_value, unsigned long int full_scale_analog_val,unsigned long int min_analog_val, \
unsigned long *const analog_val_in_digital_int, unsigned long *const analog_val_in_digital_frac )
{
	unsigned long temp_val_1, temp_val_10, temp_val_100, temp_val_1000, remainder_val, temp_remainder_val, remainder_val_1, remainder_val_10;
	
	
	//65535(max 16 bit unsigned integer)/1023(ADC_MAX_ENCODED_VALUE) approx = 64.06, so next integer value = 65 
	if(full_scale_analog_val < 65  )
	{		
    	temp_val_1 = ((full_scale_analog_val * adc_value) / ADC_MAX_ENCODED_VALUE) ;
	   *analog_val_in_digital_int = temp_val_1 + min_analog_val;
	    remainder_val = (full_scale_analog_val * adc_value) % ADC_MAX_ENCODED_VALUE;	
	    remainder_val_1 = ((remainder_val * 10) / ADC_MAX_ENCODED_VALUE);
		temp_remainder_val = ((remainder_val * 10) % ADC_MAX_ENCODED_VALUE);
		remainder_val_10 = ((temp_remainder_val * 10) / ADC_MAX_ENCODED_VALUE);
		*analog_val_in_digital_frac = (remainder_val_1 * 100 )+ remainder_val_10 * 10 ;
		return;
	}
	// full_scale_analog_val  >= 65, cannot compute, to support full_scale_analog_val  >= 65 , uC should support floating point operation or 32bit integer.	
	*analog_val_in_digital_int = 0;
	*analog_val_in_digital_frac = 0;
}
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void Encoded_To_Percent_Calc(unsigned long adc_value, unsigned int *const percent_int, unsigned int *const percent_frac )
{
	unsigned int remainder_val_1;
	unsigned long temp_percent_int;
	
	
	temp_percent_int =  (100 * adc_value); 
	*percent_int = (100 * adc_value) / ADC_MAX_ENCODED_VALUE;
	remainder_val_1 = temp_percent_int % ADC_MAX_ENCODED_VALUE;
	*percent_frac = (remainder_val_1 * 10) / ADC_MAX_ENCODED_VALUE;
}

/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void Actual_Analog_Val_To_Encoded_Calc(const unsigned long actual_analog_val, unsigned long int full_scale_analog_val, \
   unsigned long int min_analog_val, unsigned long *const encoded_val)
{
	unsigned long remainder_val_1;
	unsigned long temp_encoded_int;
	//65535(max 16 bit unsigned integer)/1023(ADC_MAX_ENCODED_VALUE) approx = 64.06, so next integer value = 65 
	if(actual_analog_val < 65)
	{
	   *encoded_val = ((actual_analog_val * ADC_MAX_ENCODED_VALUE) / full_scale_analog_val) - min_analog_val;
	   return;
	} 
	//actual_analog_val >= 65, cannot compute, to support actual_analog_val  >= 65 , uC should support floating point operation or 32bit integer.	 
    *encoded_val = 0;	
}

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
