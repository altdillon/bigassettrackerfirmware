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
#pragma config WDTE = OFF        // Watchdog Timer Enable (WDT is off !!!!!!!!!!)
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
    asm("nop");
    setup();
   
    while(true)
    {
        
        asm("nop");
        if(PORTAbits.RA0 == true)
        {
            PORTCbits.RC0 = true; // set status lamp
            putc('a');
            putc('b');
            putc('\n');
        }
        else
        {
            PORTCbits.RC0 = false;
        }
        
        for(char i=0;i<0xFF;i++);
        
        asm("clrwdt"); // clear the watch dog timer, this is an assembly command
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
    // configure the internal clock.  125 Khz
    OSCCON = 0b00100000; // see page 73 of the datasheet
    // configure the IO
    TRISA = 0xff; // input 
    //TRISC = 0x00; // output, right now it's turned off to test the serial stuff
    TRISCbits.TRISC0 = 0; // set the first bit of portc to output
    serialSetup(); // call the function for setting up the serial devices 
}

// sets usart device
//void serialSetup() // old function, dosn't work :(
//{
//    // these settings are pretty much lifted from the exsample github repo
//    //TXSTA = 0x24;  // TX enable BRGH=1, so high speed baoud rate is on, register is on page 258
//    TXSTAbits.TXEN = true;
//    TXSTAbits.BRGH = false;
//    TXSTAbits.SYNC = true;
//    
//    
//    RCSTA = 0x90; // RX enable, single character RX
//    // setup the baoud rate.  For testing this will be around 9600;
//    // calculating the baoud rate is given on page 261 of the datasheet
//    // see SPBRGH:SPBRG registers, also line 291 of the example
//    SPBRG = 0x4D;
//    SPBRGH = 0x00;
//    // configure baoudcon
//    BAUDCONbits.SCKP = true; // data clocked on rising edge
//    BAUDCONbits.BRG16 = false; // using 8bit counter for baoud generation
//    
//    
//}

// new function for configuring the serial device 
void serialSetup()
{
    // configure the IO ports for the serial ports
    TRISCbits.TRISC4 = false; // configure RC4 as output since it's TX
    TRISCbits.TRISC5 = true; // configure RC5 as input since it's RX
    for(char i=0;i<0xFF;i++); // delay for good measure 
    
    // TXSTA register is outlined on page 258 of the datasheet
    TXSTAbits.TXEN = true; // enable transmitting
    TXSTAbits.SYNC = false; // use async mode
    TXSTAbits.SENDB = false; // send break character as soon as the transmission is completed
    TXSTAbits.TX9 = false; // select 8 bit word size.  As aposed to 9bit
    
    // RCSTA register is outlined on page 259 of the datasheet
    RCSTAbits.SREN = true; // enable the serial port
    RCSTAbits.RX9 = false; // accept 8bit values
    RCSTAbits.CREN = true; // enable continous recive.
    
    // configure the baud generator
    // baoudcon register is on 290 of the datasheet
    // used formula on page 261 to get a value of brg = 202.  
    SPBRG = 0xCA;
    TXREG = 0x00; // clear out the send register
    unsigned char c = RCREG;
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
    while(TXSTAbits.TRMT != true); // wait until last byte is finished sending
    TXREG = ch;
    asm("nop");
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