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

void main(void) 
{
    setup();
    
    while(true)
    {
        
        asm("clrwdt"); // clear the watch dog timer
    }
    
    return;
}

void setup()
{
    OSCCON=0b01101100;    
    OPTION_REG=0b10001111;
    INTCON=0b10100000; //we may not need this
}