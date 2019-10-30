#include "usartserial.h"

/*
 * defintions of serial functions
 */

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

// set desired baud rate
// this function always assumes a 4Mhz clock
void set_baud(unsigned long desired_baud)
{
    unsigned long Fosc = 4000000; // 6 zeros, I counted...
    unsigned long x = Fosc/desired_baud;
    x = x/32; // 16 bit baud gen
    x = x -1;
    SPBRGL = floor(x);
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

// non blocking function to read a char from serial
char getch()
{
    // if there is a character to read, then return it, else return a -1
    if(PIR1bits.RCIF) 
    {
        return RCREG;
    }
    else
    {
        return -1;
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

void putln_size(char *str,unsigned char length)
{
    unsigned char i;
    for(i=0;i<length;i++)
    {
        putch(str[i]);
    }
}

char getln(char *buffer,char length)
{
    char chbuffer = 0;
    unsigned char index;
    for(index=0;index<length;index++)
    {
        while(PIR1bits.RCIF == 0); // wait for character to become avalible
        // then copy the byte over and make see if its one of end line bits
        chbuffer = RCREG; 
        if(chbuffer == CARRAGERETURN || chbuffer == NEWLINE || chbuffer == '\0') // check to see if the message has been terminated 
        {
            break; 
        }
        else
        {
            buffer[index] = chbuffer;
        }
    }
    
    return index; // return the number of bytes read from the serial buffer
}

void flush()
{
    char thevoid;
    while(PIR1bits.RCIF == 1)
    {
        thevoid = RCREG;
    }
}

bool is_Avaible()
{
    // wrapper for one status bit, but this will make thinges easer to read
    return PIR1bits.RCIF; 
}

bool is_whitespace(char c)
{
    bool whitespace = false;
    
    if(c == 0x20 || c == NEWLINE || c == CARRAGERETURN)
    {
        whitespace = true;
    }
    
    return whitespace;
}