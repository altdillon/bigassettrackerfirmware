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
    // ADCON bits 
    //ADCON1=0b11100000;  // right justified FOSC/64 VREF is VDD(for now)
    ADCON1 = 0b11100011; // right justified FOSC/64 VREF is internal VREF
    ADCON0=0b00011101;  // connected to AN7, which is RC3 for port C. Or physical pin 7
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

void IREF_setup()
{
    // definitions for FVR con on page 145
    // FVREN enabled
    // ADC fixed voltage refrence is 2.048
    FVRCON = 0b10010010;
    while(!FVRCONbits.FVRRDY); // wait for the fixed voltage ref to be stable and ready
}

// functions for reading power and current and voltage...
// special thanks to Dana
float read_voltage()
{
    float adread = (float)read_AD(); // this is dirty...
    float volts = VREF*(adread/1024);
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

