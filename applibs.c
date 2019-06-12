#include "app.h"

unsigned char pearson_hash(char *str,unsigned char len)
{
    unsigned char hash = len;

    unsigned char i;
    for(i=0;i<len;i++)
    {
        hash = lookup[hash^str[i]];
    }

    return hash;
}
