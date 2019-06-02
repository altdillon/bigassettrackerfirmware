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
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
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
#include "sparkfun_lte.h"
#include "states.h"
#include "usartserial.h"

// global values
bool running;
STATE_T current_state;
char strdat[16];

// prototypes for the setup functions
void setup(); 
void pingpong(); // simple game to test the serial 

int main()
{
    setup(); // setup all the timers and general io
    usart_setup(); // setup the serial comminication syste,
    running = true;
    
    while(running)
    {
        STATE_T next_state;
        
        pingpong();
        //char bytercd = getln(strdat,16);
        //asm("nop");
        //strcat(strdat,"\n");
        //putln(strdat);
        //flush();
        
        //char a;
        //for(a=0;a<0xff;a++);
        current_state = next_state; // update to the next state
    }
    
    return 0;
}

void setup()
{
    OSCCON = 0b00110100; // set the system clock to 4 Mhz
    /* Select pins 4&5 as the uart - page 102 */
    TRISCbits.TRISC4 = 0;   /* RC4 as output  */
    TRISCbits.TRISC5 = 1;   /* RC5 as input */
    TRISAbits.TRISA5 = 0; // RA5 as an output
}

/*
 *  ping pont
 *  you type ping it types pong
 *  you type pong it types ping
 */

void pingpong()
{
    char strIn[16];
    memset(strIn,'\0',16); // make sure that strIn is clear
    
    char bytesin = getln(strIn,16);
    if(bytesin > 0)
    {
        if(strcmp(strIn,"ping") == 0)
        {
            putln("pong\n");
        }
        else if(strcmp(strIn,"pong") == 0)
        {
            putln("ping\n");
        }
    }
    flush(); // flush the serial ports
}