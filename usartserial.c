#include "usartserial.h"

// define the bodies of the serial functions
void usart_setup()
{
    /* Enabling transmitter 23.1.1.1 page 259 - TX/CK I/O pin */
    TXSTAbits.TXEN = 1;
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
    /* Enabling receiver 23.1.2.1 page 262 - RX/DT I/O pin */
    RCSTAbits.CREN = 1;

    /* Select pins 4&5 as the uart - page 102 */
    TRISCbits.TRISC4 = 0;   /* RC4 as output  */
    TRISCbits.TRISC5 = 1;   /* RC5 as input */

    /* assume 4MHz clock, 19k2 baud */
    TXSTAbits.BRGH = 0;
    BAUDCONbits.BRG16 = 1;
    SPBRGL = 12;

    /* Enable interrupts */
    //INTCONbits.GIE = 1;
    //INTCONbits.PEIE = 1;

    /* enable uart receiver interupt */
    //PIE1bits.RCIE = 1;
}

// blocking function to write 1 byte to serial
// assumes no asych interups or anything that fancy.
void putch(char ch)
{
    // check to see if the TXIF bit of the PIR1 register is set
    if(PIR1bits.TXIF == 1)
    {
        // if ready to transmit then load the character into the TXREG
        TXREG = ch;
        // wait two instructions then wait for the TXREG flag to clear
        unsigned char wait;
        for(wait=0x00;wait<0xff;wait++);
        while(PIR1bits.TXIF == 0); // wait for the bit to clear then return
    }
}

char putln(char *str)
{
    char bytes_sent = strlen(str);
    
    unsigned char i;
    for(i=0;i<strlen(str);i++)
    {
        putch(str[i]);
    }
    
    return bytes_sent;
}

char getch()
{
    return -1; // stub
}