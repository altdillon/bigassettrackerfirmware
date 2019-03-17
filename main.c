/*
 * File:   main.c
 * Author: dmanhall@gmail.com
 *
 * Created on 3/8/2019 1:28:33 AM UTC
 * "Created in MPLAB Xpress"
 * 
 * really fantastic exsample project:
 * https://github.com/jgeisler0303/PIC16F1454_USB2Serial
 * 
 */


// PIC16F1455 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = ON        // Watchdog Timer Enable (WDT enabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = ON    // Clock Out Enable (CLKOUT function is enabled on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is disabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config CPUDIV = NOCLKDIV// CPU System Clock Selection Bit (NO CPU system divide)
#pragma config USBLSCLK = 48MHz // USB Low SPeed Clock Selection bit (System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.)
#pragma config PLLMULT = 3x     // PLL Multipler Selection Bit (3x Output Frequency Selected)
#pragma config PLLEN = ENABLED  // PLL Enable Bit (3x or 4x PLL Enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#include <stdbool.h>
#include <string.h>
#include "sparkfun_lte.h"


// prototypes for the setup functions
void setup(); 
void serialSetup();
void i2cSetup();

// prototypes for the serial functions
void putc(char ch);
char getc();
char getstr(char *buffer,char length);
void sendstr(char *str);

void main(void) 
{
    setup();
    bool sent=false;
    unsigned char callcount = 0x00;
    
    while(true)
    {
        
        asm("nop");
        if(PORTAbits.RA0 == true && sent == false)
        {
            char *ncount = atoi(callcount);
            char *msg = "Hello Serial!\t";
            sendstr(msg);
            //sendstr(ncount);
            sent = true;
            callcount++;
        }
        else if(PORTAbits.RA0 == false)
        {
            sent = false;
        }
        
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
    // configure the internal clock.  100 Khz
    OSCCON = 0b00100000;
    // configure the IO
    TRISA = 0xff;
    TRISC = 0x00;
    
    
    serialSetup(); // call the function for setting up the serial devices 
}

// sets usart device
void serialSetup()
{
    // these settings are pretty much lifted from the exsample github repo
    //TXSTA = 0x24;  // TX enable BRGH=1, so high speed baoud rate is on, register is on page 258
    TXSTAbits.TXEN = true;
    TXSTAbits.BRGH = false;
    TXSTAbits.SYNC = true;
    
    
    RCSTA = 0x90; // RX enable, single RX
    // setup the baoud rate.  For testing this will be around 9600;
    // calculating the baoud rate is given on page 261 of the datasheet
    // see SPBRGH:SPBRG registers, also line 291 of the example
    SPBRG = 0x4D;
    SPBRGH = 0x00;
    // configure baoudcon
    BAUDCONbits.SCKP = true; // data clocked on rising edge
    BAUDCONbits.BRG16 = false; // using 8bit counter for baoud generation
    
    
}

/*
 * functions for doing serial IO.  I think these will be moved to their own file at some point
 * all these files are based on the serial exsample project in github
 * putc, send a character to the usart 
 * getc, block until there's a chracter from the usart 
 * getstr, read chars until a new line or carry return is detected 
 * sendstr, send a string
 */

void putc(char ch)
{
    TXREG = ch;
}

char getc()
{
    return -1; // stub
}

// returns the length of the string returned
// returns -1 if there's been an error 
char getstr(char *buffer,char length)
{
    return -1; // stub for now
}

// sends a string, returns -1 if there's been an error
void sendstr(char *str)
{
    for(char *itr=str;*itr != '\0';itr++)
    {
        putc(*itr);
    }
}