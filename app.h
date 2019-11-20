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
 * 
 * 
 *  This is for app code spesfic for this project
 *   
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef APP_H
#define	APP_H
#define SENSR 10 // value in ohms of the voltage sensing resistor
#define VREF 2.048

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>

    // 255 value update list
    // this is used by the pearson hash to generate values
    // shouldn't have any reapeating numbers
unsigned char lookup[255] = {
         21,  63,  17,  31,  99,  92,  84,  89,  33,  30,  16,  81,  63,
         89,  56,  84,  30,  10,  93,  32,  84,  14,  47,  82,  19,  14,
         71,  73,  86,  55,  71,  75,  57,  22,  18,  54,  99,  16,  13,
         94,  28,  60,  67,  53,  92,  45,  21,   1,  44,  27,  30,  13,
         74,  56,  77,   4,  71,   1,  66,  83,  59,  14,  77,  67,  78,
         66,  89,  61,  41,  15,  31,  68,  84,  88,  95,  26,  83,  13,
         22,  28,  32,  44,  92,  40,  60,  68,  54,  27,   2,  14,  27,
         90,  88,  26,  84,  62,  93,  80,   9,  35,  45,  95,   4, 100,
         16,  81, 100,  11,  55,  47,  38,  89,  75,  52,  23,   9,  45,
         46,  83,  32,  75,  52,  93,  98,  15,  11,  18,  75,  95,  90,
          2,  92,  78,  31,  89,  65,  86,  83,  66,  46,  53,  22,   5,
         56,  11,   2, 100,  25,  74,  35,  57,  32,  80,  50,  80,  31,
         56,  99,  95,  77,  29,  39,  22,  63,  91,  66,  90,  23,  34,
         33,  95,  29,  52,  55,  41,  30,  96,  38,  84,  10,  27,  54,
          8,  75,  24,  65,  78,  54,  88,  67,  86,  17,  23,  68,  39,
         42,  62,  79,  39,   1,  97,  39,  99,  64,  58,  25,  50,  28,
         99,  67,  87,  42,   1,  98,  87,   2,  33,  29,  89,  88,  37,
         21,  75,  71,  94,  74,  35,  33,  34,  35,   9, 100,  59,  47,
         33,  85,  52,   5,  25,  90,  34,  60,  68,  93,   5,  72,  37,
         52,   9,  41,  63,  51,  21,  23,  85};
    
         // wikipeida link that tells what this dose:
         // https://en.wikipedia.org/wiki/Pearson_hashing
         unsigned short pearson_hash(char *str,unsigned char len);
         void AD_setup();
         void IREF_setup(); // function to setup the internal voltage refrence
         short read_AD(); // do a single blocking read from the analog to digital converter
         float read_voltage(); // voltage
         float read_current(); // current
         float read_power(); // power = voltage * current
         void sysDelay_ms(unsigned int delayms); // delay for milliseconds, note: this is only good for ~7 weeks
         // functions for making numbers into strings 
         void long2str(unsigned long num,char *str,unsigned char len); // take a long and make it a string
         void int2str(unsigned int num,char *str,unsigned char len);
         char count_digits(unsigned long num); // count the number of digits 
         // define the millisecond count variable in the LTE lib since it'll be used in here
         // this variable is incrimented in the isr function in main.c
         // right now I guess this makes since 
         unsigned int mill_seconds;
         
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

