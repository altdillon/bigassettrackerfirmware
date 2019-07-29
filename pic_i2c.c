#include "pic_i2c.h"

void i2c_wait()
{
    while(R_nW || (SSPCON2 & 0x1F));
    return;
}

char i2c_sendbyte(I2C_DEVICE_T *dev,char *data,unsigned char datalen)
{
    
    return 0; // zero on success
}

char i2c_readbyte(I2C_DEVICE_T *dev,char *data,unsigned char datalen)
{
    
    return -1; // stub, when success return the number of bytes read from device
}

I2C_DEVICE_T new_device(unsigned char addr)
{
    I2C_DEVICE_T nd;
    nd.i2c_addr = addr;
    nd.hits = 0;
    nd.isConnected = false;
    return nd;
}