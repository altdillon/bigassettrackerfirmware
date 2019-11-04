/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/*
 *  This header file defines all the states that the microcontroller will use
 *  it also defines the typedef for the state type, which is  STATE_T 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef STATES_HEADER
#define	STATES_HEADER

#include <xc.h> // include processor files - each processor file is guarded.  

// define macros for the states
// ideally numbers should be less than 255
#define ST_START 1 // start up the micro
#define ST_LTE_START 2
#define ST_HOLOGRAM_CONNECT 3 // connect to hologram
#define ST_HOLOGRAM_DISCONNECT 4 // disconnect from hologram
#define ST_CHECK_ENVIROMENT 5 // take envrimental readings
#define ST_SEND_FAILSTATE 6 // tell hologram that we've failed
#define ST_SLEEP_MODE 7 // put the micro to sleep
#define ST_CHECK_POWERDRAW 8 // check the battery
#define ST_SEND_UPDATE 9 // send normal update to hologram
#define ST_GET_FROM_HOLOGRAM 10 // pull data down from hologram
#define ST_SEND_SMS 11 // send an SMS to a list of phone numbers
#define ST_FIRMWARE_CRASH 12 // special state if the firmware has crashed, blink out a spcial error
#define ST_PINGPONG 100 // run the ping pong program, for testing serial
#define ST_FUNCT_TEST 101 // special testing state used to tesk functions while under devlopment

// define state type 
typedef unsigned char STATE_T;


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

