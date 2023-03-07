/**************************************************************************
   FILE          :    adc.h
 
   PURPOSE       :    Analog to Digital library Header
 
   AUTHOR        :     K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 
 #ifndef _ADC_H
 #define _ADC_H
 
 /* ---------------------- macro defination ------------------------------------------------ */
 
#define ADC_MAX_ENCODED_VALUE    (1023UL) 
#define ADC_INVALID_VALUE        (1024UL)
#define ADC_INVALID_CH              (14U)
 

#define ADC_INVALID_SERVICE      (0x02)
#define ADC_INTP_SERVICE         (0x01)
#define ADC_POLLING_SERVICE      (0x00)

 //Programmable on-chip voltage reference(CVREF) module (% of VDD)
#define  ADC_CHANNEL_CVEF          (0)  
//Fixed Ref (0.6 volt fixed reference)
#define  ADC_CHANNEL_FIXED_VREF    (1)  
 
/* ---------------------- data type defination --------------------------------------------- */
typedef enum {
	ADC_CH_00 = 0, ADC_CH_01, ADC_CH_02, ADC_CH_03, ADC_CH_04, ADC_CH_05, ADC_CH_06, ADC_CH_07, ADC_CH_08, ADC_CH_09, ADC_CH_10, ADC_CH_11, ADC_CH_12, ADC_CH_13, ADC_INVALID_CH_14   
} adc_channels;

//A/D Conversion Clock, FOSC/2 
#define 	ADC_CLK_SRC_OSC_2      (0u) 
//A/D Conversion Clock, FOSC/8 
#define 	ADC_CLK_SRC_OSC_8      (1u) 
//A/D Conversion Clock, FOSC/32 
#define 	ADC_CLK_SRC_OSC_32     (2u) 
//A/D Conversion Clock, FRC (clock derived from a dedicated internal oscillator = 500 kHz max)
#define    ADC_CLK_SRC_OSC_INTR    (3u) 
 
//A/D Conversion Result Format is Right justified	  
#define ADC_RESULT_RIGHT_FMT       (1u) 
// A/D Conversion Result Format is left justified
#define	ADC_RESULT_LEFT_FMT        (0u) 

 //source for Voltage Reference- is VREF- pin(external)
#define  ADC_VREF_NEG_SRC_EXTR     (1u) 
//source for Voltage Reference- is VSS (internal)
#define	ADC_VREF_NEG_SRC_INTR      (0u) 

//source for Voltage Reference+ is VREF+ pin(external)	 
#define	ADC_VREF_PLUS_SRC_EXTR      (1u) 
//source for Voltage Reference+ is VDD (internal)
#define	ADC_VREF_PLUS_SRC_INTR      (0u)  

 
 typedef struct {
	unsigned long adc_value_channel;
	unsigned int adc_cur_service_type: 2;
	unsigned int adc_clk_src_type: 2;
	unsigned int volt_ref: 1;
	unsigned int adc_result_format_justfied_type: 1;
	unsigned int adc_vref_neg_src_type: 1;
	unsigned int adc_vref_plus_src_type: 1;		
 } adc_parameter_types;
 
 /* -------------------- public variable declaration --------------------------------------- */
 
extern unsigned long analog_val_in_digital_ch[1]; 
extern adc_parameter_types  adc_cur_parameters[];
extern adc_channels adc_cur_channel;
 /* -------------------- public prototype declaration --------------------------------------- */
void ADC_Start_Conv(adc_parameter_types *set_adc_conf);
void ADC_Disable();
char Is_ADC_Converting();
void Encoded_To_Actual_Analog_Val_Calc(unsigned long adc_value, unsigned long int full_scale_analog_val,unsigned long int min_analog_val, \
   unsigned long *const analog_val_in_digital_int, unsigned long *const analog_val_in_digital_frac );
void Encoded_To_Percent_Calc(unsigned long adc_value, unsigned int *const percent_int, unsigned int *const percent_frac );   
void ADC_Conf_Channel(const adc_channels set_adc_cur_channel);

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
