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
