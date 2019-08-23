#include "pic_i2c.h"

I2C_DEVICE_T new_device(unsigned char addr)
{
    I2C_DEVICE_T nd;
    nd.i2c_addr = addr;
    nd.hits = 0;
    nd.isConnected = false;
    return nd;
}

void i2c_wait()
{
    while(R_nW || (SSPCON2 & 0x1F));
    return;
}

char i2c_sendbyte(I2C_DEVICE_T *dev,char *data,unsigned char datalen)
{
    unsigned char writeaddr = dev->i2c_addr & ~(1 << 0); // bit zero should be clear for writting 
    while(R_nW || (SSPCON2 & 0x1F));
    SEN = 1; // start condition
    while(R_nW || (SSPCON2 & 0x1F));
    SSPBUF = writeaddr; // after the start condition is out send out the write addr
    while(R_nW || (SSPCON2 & 0x1F));
    SSPBUF = *data; // data to write out
    while(R_nW || (SSPCON2 & 0x1F));
    PEN = 1; // stop condition
    
    return 0; 
}

char i2c_readbyte(I2C_DEVICE_T *dev,char *data,unsigned char datalen)
{
    
    return -1; // stub, when success return the number of bytes read from device
}

