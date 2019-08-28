#include "app.h"

unsigned short pearson_hash(char *str,unsigned char len)
{
    unsigned short hash = len;

    unsigned short i;
    for(i=0;i<len;i++)
    {
        hash = lookup[hash^str[i]];
    }

    return hash;
}

// functions for seting up and reading the Analog to digital converter

void AD_setup()
{
    ADCON1=0b11100000;
    ADCON0=0b00011101;
    ANSELCbits.ANSC3 = 1;
    char i;
    for(i=0x00;i<0xff;i++); //delay to set up the a/d 
}

short read_AD()
{
    // start the ADREADing
    ADCON0bits.ADGO = 1;
    while(ADCON0bits.ADGO); // wait for the read to finish
    return ADRES;
}

// functions for reading power and current and voltage...
// special thanks to Dana
float read_voltage()
{
    float adread = (float)read_AD(); // this is dirty...
    float volts = 5*(adread/1024);
    return volts;
}

float read_current()
{
    float I = read_voltage()/SENSR; 
    return I;
}

float read_power()
{
    float P = read_voltage() * read_current();
    return P;
}
