/**************************************************************************
   FILE          :    port.h
 
   PURPOSE       :    port header - define port and its pin assignment.
 
   AUTHOR        :     K.M. Arun Kumar alias Arunkumar Murugeswaran 
 
  KNOWN BUGS     :
	
  NOTE           :    port header - user interface to external device, such as LED, Switch,
 	                  connection to other microcontroller via RS232, USB, etc.  
	
  CHANGE LOGS     :  
	   
 **************************************************************************/
 /*To avoid this header file to be included more than once, conditional directive is used  */
 
#ifndef _PORT_H
#define _PORT_H

#define LCD_RS_TRIS                          TRISEbits.TRISE0
#define LCD_RW_TRIS                          TRISEbits.TRISE1 
#define LCD_EN_TRIS                          TRISEbits.TRISE2                    

#define LCD_RS_PIN                           RE0
#define LCD_RW_PIN                           RE1
#define LCD_EN_PIN                           RE2

#define LCD_DATA_4_TRIS                      TRISDbits.TRISD4  
#define LCD_DATA_5_TRIS                      TRISDbits.TRISD5  
#define LCD_DATA_6_TRIS                      TRISDbits.TRISD6  
#define LCD_DATA_7_TRIS                      TRISDbits.TRISD7  

#define LCD_DATA_4_PIN                      PORTDbits.PORTD4   
#define LCD_DATA_5_PIN                      PORTDbits.PORTD5
#define LCD_DATA_6_PIN                      PORTDbits.PORTD6
#define LCD_DATA_7_PIN                      PORTDbits.PORTD7

#define LCD_DATA_TRIS                      TRISD
#define LCD_DATA_PORT                      PORTD

#define R_RMS_MAX_LIMIT_VOLT_LED_TRIS     TRISBbits.TRISB1
#define R_RMS_MIN_LIMIT_VOLT_LED_TRIS     TRISBbits.TRISB2 
#define MOTOR_CTRL_TRIS                   TRISBbits.TRISB3 

#define R_RMS_MAX_LIMIT_VOLT_LED_PIN           RB1
#define R_RMS_MIN_LIMIT_VOLT_LED_PIN           RB2
#define MOTOR_CTRL_PIN                         RB3 

#endif 

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
