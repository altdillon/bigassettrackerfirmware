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

void AD_setup()
{
    ADCON1=0b11100000;
    ADCON0=0b00001101;
    ANSELAbits.ANSA4=true;
    char i;
    for(i=0x00;i<0xff;i++); //delay to set up the a/d 
}