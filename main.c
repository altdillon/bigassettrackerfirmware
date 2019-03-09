/*
 * File:   main.c
 * Author: dmanhall@gmail.com
 *
 * Created on 3/8/2019 1:28:33 AM UTC
 * "Created in MPLAB Xpress"
 */


#include <xc.h>
#include <stdbool.h>
#include <string.h>
#include "sparkfun_lte.h"



void setup(); 
void serialSetup();
void i2cSetup();

void main(void) 
{
    setup();
    
    while(true)
    {
        
        asm("clrwdt"); // clear the watch dog timer
    }
    
    return;
}

/*
 * setup is the genral boot up functions for the pic micro
 * this sets up the clock, interupts and the digial io stuff  
 * Every thing else is handled by their respective functions
 * 
 * rember boys, making a tris bit true is an input, false is an output
 * 
 */

void setup()
{
    OSCCON=0b01101100; // configure the internal clock
    OPTION_REG=0b10001111;
    INTCON=0b10100000; //we may not need this
    // for the time being make port A an input and port c and output 
    TRISC = 0x00;
    TRISA = 0xFF;
    
    serialSetup(); // call the function for setting up the serial devices 
}

// sets up the stuff 
void serialSetup()
{
    
}