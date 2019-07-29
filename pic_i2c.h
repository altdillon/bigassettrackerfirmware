/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 * 
 *  https://www.teachmemicro.com/pic16-i2c/
 * 
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef I2C_HEADER_H
#define	I2C_HEADER_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdbool.h>
#define MAX_I2C_DEVICES 10

// struct for holding i2c address data
typedef struct 
{
    unsigned char hits; // number of times the device has been written to
    unsigned char i2c_addr; // i2c address of the device
    bool isConnected; // boolean to make sure that the device is connected; may not be used
}I2C_DEVICE_T;

I2C_DEVICE_T i2c_devices[MAX_I2C_DEVICES];

// functions for sending and recinving data from the i2c devices
char i2c_sendbyte(I2C_DEVICE_T *dev,char *data,unsigned char datalen);
char i2c_readbyte(I2C_DEVICE_T *dev,char *data,unsigned char datalen);
char i2c_check_connections(I2C_DEVICE_T *device_arr,unsigned char device_count);
// helper functions for i2c functions
void i2c_wait();
I2C_DEVICE_T new_device(unsigned char addr); // not really needed, but makes progrmaing easier make code is more simple


#endif	/* XC_HEADER_TEMPLATE_H */

