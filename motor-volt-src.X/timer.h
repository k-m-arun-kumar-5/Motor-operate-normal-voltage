/**************************************************************************
   FILE          :    timer.h
 
   PURPOSE       :    timer library Header
 
   AUTHOR        :    K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 
 #ifndef _TIMER_H
 #define _TIMER_H
 
 #include "appl_conf.h"
 
 /* ---------------------- macro defination ------------------------------------------------ */
 
  
  
/* ---------------------- data type defination --------------------------------------------- */

typedef enum {
	TMR1_INVALID_SERVICE = 0x04, TMR1_TIMER_INTP_SERVICE = 0x03, TMR1_TIMER_POLLING_SERVICE = 0x02, TMR1_COUNTER_INTP_SERVICE = 0x01, TMR1_COUNTER_POLLING_SERVICE = 0x00
} timer1_service_types;
 
 typedef enum {
	 TMR1_GATE_CTRL_DISABLE = 0b00000000,         //Timer1 Gate control disable
	 TMR1_GATE_CTRL_ACTIVE_HIGH = 0b11000000,     //Timer1 Gate control enabled, and Timer1 gate is active-high (Timer1 counts when gate is high and timer 1 pauses when gate is low)
	 TMR1_GATE_CTRL_ACTIVE_LOW = 0b01000000     //Timer1 Gate control enabled, and Timer1 gate is active-low (Timer1 counts when gate is low and timer 1 pauses when gate is high)
 } timer1_gate_control_types;
 
 typedef enum {
   TMR1_CLK_SRC_INTR_OSC = 0b00000000,         // Timer1 Clock Source as Internal clock (FOSC/4)
   TMR1_CLK_SRC_EXTR_SYNC = 0b00000010,        // Timer1 Clock Source as External clock from T1CKI pin (on the rising edge) and Synchronize external clock input 
   TMR1_CLK_SRC_EXTR_NO_SYNC = 0b00000110      // Timer1 Clock Source as External clock from T1CKI pin (on the rising edge) and dont Synchronize external clock input     
 } timer1_clock_sources;
 
typedef enum { 
   TMR1_LP_OSC_ENABLE = 0b00001000,     // LP(low power) oscillator is enabled for Timer1 clock
   TMR1_LP_OSC_DISABLE = 0b00000000     // LP(low power) oscillator is off for Timer1 clock
 } timer1_LP_osc_control_types; 
 
 typedef enum { 
   TMR1_INPUT_CLK_PRESCALE_8 = 0b00110000, //Timer1 Input Clock 1:8 Prescale Value from Timer1 Clock Source
   TMR1_INPUT_CLK_PRESCALE_4 = 0b00100000, //Timer1 Input Clock 1:4 Prescale Value from Timer1 Clock Source
   TMR1_INPUT_CLK_PRESCALE_2 = 0b00010000, //Timer1 Input Clock 1:2 Prescale Value from Timer1 Clock Source
   TMR1_INPUT_CLK_PRESCALE_1 = 0b00000000  //Timer1 Input Clock 1:1 Prescale Value from Timer1 Clock Source
 } timer1_input_clock_prescaler_types; 
 
 typedef enum {
	TMR2_INVALID_SERVICE = 0x04, TMR2_PWM_INTP_SERVICE = 0x03, TMR2_PWM_POLLING_SERVICE = 0x02
} timer2_service_types;

typedef enum { 
   TMR2_PRESCALE_16 = 0b00000011, //Timer2 Input Clock 1:16 Prescale Value from Timer1 internal Clock  
   TMR2_PRESCALE_4 = 0b00000001,  //Timer2 Input Clock 1:4 Prescale Value from Timer1 internal Clock 
   TMR2_PRESCALE_1 = 0b00000000   //Timer2 Input Clock 1:1 Prescale Value from Timer1 internal Clock 
 } timer2_prescaler_types; 
 
 /* -------------------- public variable declaration --------------------------------------- */
 
extern unsigned int timer1_prescale, timer1_prescale_shift, timer1_elapsed_num_update;
extern unsigned long int  timer1_init_val, timer1_elapsed_num_overflow_1_update, timer1_1_update, timer1_max_num_overflow, timer1_req_time_max_update;
extern unsigned long int timer1_req_time_delay_in_milli_sec;
extern timer1_run_states timer1_cur_run_state, timer1_last_run_state_before_stop;
extern timer1_service_types timer1_cur_service_type;
extern volatile unsigned long measure_pulse_lower_count, measure_pulse_upper_count;
 
extern unsigned int timer2_prescale, timer2_prescale_shift, timer2_elapsed_num_update;
extern unsigned long int  timer2_init_val, timer2_elapsed_num_overflow_1_update, timer2_1_update, timer2_max_num_overflow, timer2_req_time_max_update;
extern unsigned long int timer2_req_time_delay_in_milli_sec;
extern timer2_run_states timer2_cur_run_state, timer2_last_run_state_before_stop;
extern timer2_service_types timer2_cur_service_type;

 /* -------------------- public prototype declaration --------------------------------------- */
 
void Timer1_Run(const timer1_run_states set_timer1_run_mode, const unsigned long int set_req_time_delay_in_milli_sec, const timer1_service_types set_timer1_service, const timer1_gate_control_types set_gate_control_type, \
  const timer1_input_clock_prescaler_types set_timer1_input_clock_prescaler_type, const timer1_LP_osc_control_types set_timer1_LP_osc_control_type, const timer1_clock_sources set_timer1_clock_source  );
void Timer1_Stop();
void Timer1_Prescale();
void Timer1_Load_Init_Val_Calc(const unsigned long int set_timer1_tick_in_milli_sec);

void Timer2_Run(const timer2_run_states set_timer2_run_mode, const unsigned long int set_req_time_delay_in_milli_sec, const timer2_service_types set_timer2_service, \
  const timer2_prescaler_types set_timer2_input_clock_prescaler_type, const unsigned int set_timer2_postscaler_val  );
void Timer2_Stop();
void Timer2_Prescale();
void Timer2_Load_Init_Val_Calc(const unsigned long int set_timer2_tick_in_milli_sec);

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
