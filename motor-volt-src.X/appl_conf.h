/**************************************************************************
   FILE          :    appl_conf.h
 
   PURPOSE       :   main application configuration Header
 
   AUTHOR        :     K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 
 #ifndef _APPL_CONF_H
 #define _APPL_CONF_H 
 
/*------------------------------- LCD disp conf ------------------------------------------*/

/* -------------------------------Timer state conf ---------------------------------------*/
typedef enum {
	TMR1_STOP_STATE
}	timer1_run_states; 

typedef enum {
	TMR2_STOP_STATE
}	timer2_run_states; 
/* ---------------------------------- ADC channel && input signal val conf -------------- */

#define R_RMS_VOLTAGE_CH                  ADC_CH_00
#define Y_RMS_VOLTAGE_CH                  ADC_CH_01
#define B_RMS_VOLTAGE_CH                  ADC_CH_02

#define R_RMS_CURRENT_CH                  ADC_CH_03
#define Y_RMS_CURRENT_CH                  ADC_CH_04
#define B_RMS_CURRENT_CH                  ADC_CH_05

#define TEMP_IN_CELCIUS_CH               ADC_CH_06
#define FLOW_SWITCH_CH                   ADC_CH_07     

#define MAX_ANALOG_VALUE_CH0           (5U)
#define MIN_ANALOG_VALUE_CH0           (0U)
#define FULL_SCALE_ANALOG_VAL_CH0     (MAX_ANALOG_VALUE_CH0 - MIN_ANALOG_VALUE_CH0)

/* ------------------------------- application conf --------------------------------------*/

#define MAX_R_RMS_VOLTAGE            (240UL)
#define MIN_R_RMS_VOLTAGE            (200UL)

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
