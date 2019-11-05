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
#pragma config WDTE = OFF        // Watchdog Timer Enable (WDT not-enabled) NOTE: turned off for i2c testing
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
#pragma config PLLEN = DISABLED // PLL Enable Bit (3x or 4x PLL Disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#include <stdbool.h>
#include <string.h>
#include <pic16f1455.h>
#include "sparkfun_lte.h"
#include "states.h"
#include "usartserial.h"
#include "pic_i2c.h"
#include "app.h" // headers for json, 
// settings macros
#define LTEBAUD 9600
#define LTE_MAX_ATTEMPS 6 // number of attempts that the LTE board will be given to start

// global values
bool running;
STATE_T current_state;

// global values for i2c devices 
//I2C_DEVICE_T test_device; // genral test device 
//I2C_DEVICE_T temp1,temp2,temp3; // idk, maybe we'll need 3 ?

// prototypes for the setup functions
void setup(); 
//void pingpong(); // simple game to test the serial 

// interrupt service routine used for updateing the counter 
#define TMR0_RESET (0xFF - 250 + 1)
void __interrupt() tmr0isr()
{
    if(T0IF) // if interupt was triggerd by timer 0 overflow...
    {
        TMR0 = TMR0_RESET;
        T0IF = 0;
        mill_seconds++; // mill_seconds is defined in sparkfun_let.h
    }
}

int main()
{
    //setup(); // setup all the timers and general io
    //usart_setup(); // setup the serial comminication syste,
    running = true;
    current_state = ST_START;
    unsigned short adreading; // test value for ADC
    char bufferC = 0; // buffer for portC, used in some testing functions
    unsigned char LTE_START_attempts = 0; // number of attempts that the LTE has taken to start
    
    while(running)
    {
        STATE_T next_state;
        
        // using a switch case to implement a state mashine
        switch(current_state)
        {
            case ST_START: 
                setup(); // setup the general IO
                IREF_setup(); // setup the internal voltage refrence
                AD_setup(); // setup the analog to digital converter
                //next_state = ST_PINGPONG; 
                //next_state = ST_FUNCT_TEST; // goto the genral test state
                //next_state = ST_CHECK_POWERDRAW;
                next_state = ST_LTE_START;
                break;
                
            case ST_LTE_START: // run the startup for the LTE board
                               // I think that the LTE board needs to do some calibration among other things
                if(testAT()) // test if the system is already started 
                {
                    next_state = ST_CHECK_ENVIROMENT; // if the system is turned on, advance to check envrioment
                }
                else
                {
                    //powerOn(); // do the power on toggle thing
                    if(lte_start(LTEBAUD) == 0) // start the power on system
                    {
                        next_state = ST_HOLOGRAM_CONNECT; // if the LTE devices was succesfully started then move on to the next state
                    }
                    else
                    {
                        if(LTE_START_attempts < LTE_MAX_ATTEMPS)
                        {
                            LTE_START_attempts++;
                            next_state = ST_LTE_START;
                        }
                        else
                        {
                            // at this point just give up
                            next_state = ST_FUNCT_TEST;
                        }
                    }
                    
                }
                break;
                
            case ST_SEND_SMS: // send an SMS
                // send a series to text messages to the contact list
                asm("nop");
                next_state = ST_FAST_BLINK;
                break;
            case ST_PINGPONG: // test state for useart
                //pingpong(); // run the ping pong function
                next_state = ST_FUNCT_TEST; 
            
                break;
                
            case ST_HOLOGRAM_CONNECT:
                
                next_state = ST_CHECK_ENVIROMENT;
                break;
                
            case ST_HOLOGRAM_DISCONNECT:
                
                break;
                
            case ST_CHECK_ENVIROMENT:
                
                next_state = ST_CHECK_POWERDRAW;
                break;
                
            case ST_SEND_FAILSTATE:
                asm("nop");
                next_state = ST_SEND_FAILSTATE; // just loop...
                break;
            case ST_FIRMWARE_CRASH:
                if(mill_seconds >= 666) // freq on RC0 will be 1/666 0.0015015 evil...
                {
                    bufferC ^= 0x01; // blink RC0
                    PORTC = bufferC;
                    mill_seconds = 0;
                }
                next_state = ST_FIRMWARE_CRASH; // just run in a loop
                break;
                
            case  ST_SLEEP_MODE:
                
                break;
                
            case ST_CHECK_POWERDRAW:
                // make test calls to the power sensing stuff
                read_current();
                read_voltage();
                read_power();
                
                asm("nop");
                next_state = ST_SEND_SMS;
                break;
                
            case ST_SEND_UPDATE:
                
                next_state = ST_FUNCT_TEST;
                break;
                
            case ST_GET_FROM_HOLOGRAM:
                
                break;
            case ST_FUNCT_TEST:

                // print out a 1hz wave using the millisecond counter
                if(mill_seconds >= 500)
                {
                    bufferC ^= 0x01; // blink RC0
                    PORTC = bufferC;
                    mill_seconds = 0;
                }
                 
               next_state = ST_FUNCT_TEST; // go in a loop
               //next_state = ST_PINGPONG;
               break;
            case ST_FAST_BLINK: // pretty much the same as funct test, but blink twice as fast
                asm("nop");
                if(mill_seconds >= 250)
                {
                    bufferC ^= 0x01; // blink RC0
                    PORTC = bufferC;
                    mill_seconds = 0;
                }
                break;
        }
        
        asm("clrwdt"); // clear the watch dog timer
        current_state = next_state; // update to the next state
    }
    
    return 0;
}

void setup()
{
    // page 74 of data sheet
    //OSCCON = 0b00110100; // set the system clock to 4 Mhz
    OSCCON = 0b00111100; // set the system block to 16 Mhz
    /* Select pins 4&5 as the uart - page 102 */
    TRISCbits.TRISC0 = 0; // RC0 as an output
    //TRISCbits.TRISC4 = 0;   /* RC4 as output TX pin 6 */
    //TRISCbits.TRISC5 = 1;   /* RC5 as input RX  pin 5*/
    TRISAbits.TRISA5 = 0; // RA5 as an output
    
    // configure IO for analog to digital converter
    TRISCbits.TRISC3 = 1; // RC3 is an input
    
    // configure timer0 prescaler and interupts
    //OPTION_REG = 0b10000001; // option reg, 1:4 presclaler ,datasheet page 178
    OPTION_REG = 0b10000011; // 1:16 prescaler
    // configure global interupt bois
    T0IE = 1; // tmr0 interupt
    GIE = 1; // global interupt
}

/*
 *  ping pont
 *  you type ping it types pong
 *  you type pong it types ping
 */

//void pingpong()
//{
//    char strIn[32];
//    memset(strIn,'\0',32); // make sure that strIn is clear
//    
//    if(is_Avaible())
//    {
//        char bytesin = getln(strIn,32);
//        if(bytesin > 0)
//        {
//            if(strcmp(strIn,"ping") == 0)
//            {
//                putln("pong\n");
//            }
//            else if(strcmp(strIn,"pong") == 0)
//            {
//                putln("ping\n");
//            }
//            else if(strcmp(strIn,"self destruct") == 0)
//            {
//                putln("preparing to self destruct....\n");
//                putln("have a nice day :) \n");
//            }
//            else if(strcmp(strIn,"who's the man?") == 0)
//            {
//                putln("Dillon's the man!\n");
//            }
//            else
//            {
//                putln("unkown command, but thanks for playing\n");
//            }
//        }
//        flush(); // flush the serial ports
//    }
//}