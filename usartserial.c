#include "usartserial.h"

/*
 * defintions of serial functions
 */

void usart_setup()
{
    /* Enabling transmitter 23.1.1.1 page 259 - TX/CK I/O pin */
    TXSTAbits.TXEN = 1; // transmit enable
    TXSTAbits.SYNC = 0; // syc mode/ asycnh mode
    RCSTAbits.SPEN = 1; // receive enable
    /* Enabling receiver 23.1.2.1 page 262 - RX/DT I/O pin */
    RCSTAbits.CREN = 1; 

    /* Select pins 4&5 as the uart - page 102 */
    TRISCbits.TRISC4 = 0;   /* RC4 as output  */
    TRISCbits.TRISC5 = 1;   /* RC5 as input */

    ///* assume 4MHz clock, 19k2 baud */
    TXSTAbits.BRGH = 1; // BRG H made 1
    BAUDCONbits.BRG16 = 1;
    //SPBRGL = 12; // defult baud

    /* Enable interrupts */
    //INTCONbits.GIE = 1;
    //INTCONbits.PEIE = 1;

    /* enable uart receiver interupt */
    //PIE1bits.RCIE = 1;
}

// set desired baud rate
char set_baud(unsigned long desired_baud)
{
    char err = 0;
    
    if(TXSTAbits.TXEN && RCSTAbits.SPEN) // make sure that the serial device is already configured
    {
        // baud rates 230400 and 115200 reqire more percission than what the pic's floating point can provide
        if(desired_baud == 230400)
        {
            
            
            //return 0; // early terminate
        }
        else if(desired_baud == 115200)
        {
            asm("nop");
            //return 0; // early terminate
        }
        
        // maybe swap this out for a loop up table
        // compute the new baud rate
        float divValue = 4; // value found in datasheet for setting new baud rate
        double set_baud = (double)desired_baud;
        // see page 261 of the datasheet
        //unsigned long Fosc = 4000000; // 6 zeros, I counted...
        unsigned long Fosc = 16000000;
        double x = Fosc/desired_baud;
        x = x/divValue; // 16 bit baud gen
        x = x -1;
        //SPBRGL = floor(x);
        SPBRG = floor(x); // NOTE: this is a 16bit copy
        // compute the err between the set and desired baud rate
        float calculated_baud_rate = set_baud/(16*(x+1));
        float baud_err = (calculated_baud_rate - desired_baud)/desired_baud;
        // figure out the the error is excpetable
        if(abs(baud_err) > 0.1)
        {
            err = -1; // return -1 as an error code
        }
    }
    
    return err;
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