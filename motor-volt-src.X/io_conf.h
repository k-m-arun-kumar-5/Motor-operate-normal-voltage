/**************************************************************************
   FILE          :    io_conf.h
 
   PURPOSE       :   main peripherial configuration Header
 
   AUTHOR        :    K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 
 #ifndef _IO_CONF_H
 #define _IO_CONF_H
 
 /* -------------------------------- debug conf -----------------------------------------*/
//#define TRACE                                   (1U)
#define TRACE_ERROR                             (2U)

 /* -------------------------------- OSC Freq conf -------------------------------------*/
 
 /* set crystal Oscillator(stable operation) frequency (in Hz) here we set 3.6864MHz to get precise baud rate */
#define _XTAL_FREQ                             (20000000)

/* ------------------------------- LCD oper conf ---------------------------------------*/
 
 #define LCD_DISP_ERR_LINE_NUM            (NUM_LINE4) 
 
/* for 16 * 2 LCD disp */                             
#define BEGIN_LOC_LINE1                      (0X80)
#define BEGIN_LOC_LINE2                      (0xC0)
#define BEGIN_LOC_LINE3                      (0x94) 
#define BEGIN_LOC_LINE4                      (0xD4)
#define END_LOC_LINE1                        (0x8F)
#define END_LOC_LINE2                        (0xCF)
#define END_LOC_LINE3                        (0xA7) 
#define END_LOC_LINE4                        (0xE7)

/* num cols = num of chars in a line */
#define MAX_AVAIL_NUM_COLS                    (16)
#define CONFIGURE_MAX_NUM_LINES               (2)
#define MAX_AVAIL_NUM_LINES                   (2) 
#define MAX_AVAIL_NUM_CHARS_IN_LCD        (MAX_AVAIL_NUM_COLS * MAX_AVAIL_NUM_LINES) 
#define CONFIGURE_MAX_NUM_COLS             (MAX_AVAIL_NUM_COLS)
#define CONFIGURE_MAX_NUM_CHARS_IN_LCD    (CONFIGURE_MAX_NUM_LINES * CONFIGURE_MAX_NUM_COLS ) 
#define MAX_NUM_CHARS_INPUT_DATA          (MAX_AVAIL_NUM_COLS) 

#define LCD_DATA_INTERFACE                DATA_INTERFACE_8_BITS  
#define LCD_BUSY_FLAG                     NO_CHECK_BUSY_FLAG
#define LCD_CHAR_FONT                     CHAR_FONT_5_8_DOTS  
#define LCD_MAX_DISP_NUM_LINES            MAX_DISP_NUM_LINES_2

/* -------------------------------- timer oper conf ------------------------------------*/

#define TIMER1_PRESET_TIME_DELAY_IN_MULTIPLE                    (1U)
#define TIMER1_POSTSET_TIME_DELAY_IN_MULTIPLE                   (2U) 
#define TIMER1_SET_TIME_DELAY_IN_MULTIPLE                       TIMER1_PRESET_TIME_DELAY_IN_MULTIPLE

#define TIMER1_TICK_IN_MILLI_SEC                                            (50U)  
#define TIMER1_REQ_TIME_1_UPDATE_REACHED_IN_MILLI_SEC              TIME_UNIT_1_SEC_IN_MILLI_SEC
 //in Timer1_Load_Init_Val_Calc() in timer.c, change variable inc_timer1, which (directly loads initial TMR1 values ), depends on Osc freq
 
#define TIMER2_TICK_IN_MILLI_SEC                                            (50U)  
#define TIMER2_REQ_TIME_1_UPDATE_REACHED_IN_MILLI_SEC              TIME_UNIT_1_SEC_IN_MILLI_SEC 
#define TIMER2_SET_TIME_DELAY_IN_MULTIPLE                          TIMER2_POSTSET_TIME_DELAY_IN_MULTIPLE
  //in Timer2_Load_Init_Val_Calc() in timer.c, change variable inc_timer2, which (directly loads initial TMR2 values ), depends on Osc freq
  
/* ------------------------------- uart oper conf -------------------------------------- */

//in UART_Init() in uart.c, change SPBRG, which dependc on Osc freq  and baud rate

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
