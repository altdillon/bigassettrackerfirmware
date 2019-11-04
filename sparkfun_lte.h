/* 
 * File:   sparkfun_lte.h
 * Author: dmanhall@gmail.com
 *
 * Created on 3/8/2019 1:30:10 AM UTC
 * "Created in MPLAB Xpress"
 *
 *  Header file for the sparkfun LTE board
 *  Anytime line numbers referd to are in the git retpo
 *  See: https://github.com/sparkfun/SparkFun_LTE_Shield_Arduino_Library/blob/master/src/SparkFun_LTE_Shield_Arduino_Library.h
 *
 */

#ifndef SPARKFUN_LTE_H
#define	SPARKFUN_LTE_H

#include <string.h>
#include <stdbool.h>
#include <stdint.h> // some of the structs need this.  Don't remove
#include <xc.h> // special functions for the pic16f
#include <stdio.h> // for sprintf
#include "usartserial.h"
//#include "app.h"

#define LTE_SHIELD_POWER_PIN 5
#define LTE_SHIELD_RESET_PIN 6

// moble network operators from line 57
// pretty sure these are used to set things
#define MNO_INVALID -1
#define MNO_SW_DEFAULT 0
#define MNO_SIM_ICCD 1
#define MNO_ATT 2
#define MNO_VERIZON 3
#define MNO_TELSTRA 4
#define MNO_TMO 5
#define MNO_CT 6

// LTE error codes, see line 68 
typedef char LTE_Shield_error_t;
#define LTE_SHIELD_ERROR_INVALID -1
#define LTE_SHIELD_ERROR_SUCCESS 0
#define LTE_SHIELD_ERROR_OUT_OF_MEMORY 1
#define LTE_SHIELD_ERROR_TIMEOUT 2
#define LTE_SHIELD_ERROR_UNEXPECTED_PARAM 3
#define LTE_SHIELD_ERROR_UNEXPECTED_RESPONSE 4
#define LTE_SHIELD_ERROR_NO_RESPONSE 5
#define LTE_SHIELD_ERROR_DEREGISTERED 6

// LTE sheild registration status, see line 92
typedef char LTE_Shield_registration_status_t;
#define LTE_SHIELD_REGISTRATION_INVALID -1
#define LTE_SHIELD_REGISTRATION_NOT_REGISTERED 0
#define LTE_SHIELD_REGISTRATION_HOME 1
#define LTE_SHIELD_REGISTRATION_SEARCHING 2
#define LTE_SHIELD_REGISTRATION_DENIED 3
#define LTE_SHIELD_REGISTRATION_UNKNOWN 4
#define LTE_SHIELD_REGISTRATION_ROAMING 5
#define LTE_SHIELD_REGISTRATION_HOME_SMS_ONLY 6
#define LTE_SHIELD_REGISTRATION_ROAMING_SMS_ONLY 7
#define LTE_SHIELD_REGISTRATION_HOME_CSFB_NOT_PREFERRED 8
#define LTE_SHIELD_REGISTRATION_ROAMING_CSFB_NOT_PREFERRED 9

// LTE sheild codes 
//typedef char LTE_Shield_error_t;
#define LTE_SHIELD_STANDARD_RESPONSE_TIMEOUT 1000
#define LTE_SHIELD_SET_BAUD_TIMEOUT 500
#define LTE_SHIELD_POWER_PULSE_PERIOD 3200
#define LTE_RESET_PULSE_PERIOD 10000
#define LTE_SHIELD_IP_CONNECT_TIMEOUT 60000
#define LTE_SHIELD_POLL_DELAY 1
#define LTE_SHIELD_SOCKET_WRITE_TIMEOUT 10000

// macros for selecting the TCP/UDP network protocol
typedef char lte_shield_socket_protocol_t;
#define LTE_SHIELD_TCP 6
#define LTE_SHEILD_UDP 17

// LTE sheild message format
typedef char lte_shield_message_format_t;
#define LTE_SHIELD_MESSAGE_FORMAT_PDU 0
#define LTE_SHIELD_MESSAGE_FORMAT_TEXT 1

// array of supported baud rates for the LTE system
#define NUM_SUPPORTED_BAUD 6
const unsigned long LTE_SHIELD_SUPPORTED_BAUD[NUM_SUPPORTED_BAUD] =
{
    115200,
    9600,
    19200,
    38400,
    57600,
    230400
};



//void bDelay_ms(unsigned int delayms); // moved...

struct DateData {
    uint8_t day;
    uint8_t month;
    unsigned int year;
};

struct TimeData {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    unsigned int ms;
    uint8_t tzh;
    uint8_t tzm;
};

struct ClockData {
    struct DateData date;
    struct TimeData time;
};

struct PositionData {
    float utc;
    float lat;
    char latDir;
    float lon;
    char lonDir;
    float alt;
    char mode;
    char status;
};

struct SpeedData {
    float speed;
    float track;
    float magVar;
    char magVarDir;
};

struct operator_stats {
    uint8_t stat;
    char *shortOp;
    char *longOp;
    unsigned long numOp;
    uint8_t act;
};

// headers for LTE functions 

typedef struct
{
    unsigned short baud; // baud rate
    unsigned char powerPin; //...
    unsigned char resetPin;
} LTE_sheild;

// sets the LTE sheild to reseive commands
// returns true if worked, false if it dosnt
bool begin(unsigned short baud_rate); 

// functions defined in line 160 of the gitrepo
// loop polling and polling setup
bool poll();
LTE_Shield_error_t AT(); // send an AT test command 
LTE_Shield_error_t sendCommandWithResponse(const char * command, const char * expectedResponse, char * responseDest, unsigned int commandTimeout, bool at);
//LTE_Shield_error_t set_lte_baud(unsigned int baud); // set the desired baud rate on the lte board
void send_command(char *cmd,bool at); // send a command, maybe with an AT
char read_responce(char *data,unsigned int timeout); // read a responce and return the number of bytes returned
// functions for starting up the LTE board
void powerOn(); // run the power function, power on button is RC2, which is physical pin 8
char sendATcmd(char *cmd, char *buffres, bool AT, unsigned int timeout); // option to send an AT command to the LTE board
char set_lte_baud(unsigned long baud);
char lte_start(unsigned long desired_baud); // not implmented yet
char autobaud(unsigned long baud); // not implmented yet
bool testAT(); // send an AT command to set the baud rate

#endif	/* SPARKFUN_LTE_H */
