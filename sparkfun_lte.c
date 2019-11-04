#include "sparkfun_lte.h"
#include "app.h"

#define LTE_SHIELD_STANDARD_RESPONSE_TIMEOUT 1000
#define LTE_SHIELD_SET_BAUD_TIMEOUT 500
#define LTE_SHIELD_POWER_PULSE_PERIOD 3200
#define LTE_RESET_PULSE_PERIOD 10000
#define LTE_SHIELD_IP_CONNECT_TIMEOUT 60000
#define LTE_SHIELD_POLL_DELAY 1
#define LTE_SHIELD_SOCKET_WRITE_TIMEOUT 10000

// ## Suported AT Commands
// ### General
const char LTE_SHIELD_COMMAND_AT[] = "AT";         // AT "Test"
const char LTE_SHIELD_COMMAND_ECHO[] = "E";        // Local Echo
const char LTE_SHIELD_COMMAND_IMEI[] = "+CGSN";    // IMEI identification
const char LTE_SHIELD_COMMAND_IMSI[] = "+CIMI";    // IMSI identification
const char LTE_SHIELD_COMMAND_CCID[] = "+CCID";    // SIM CCID
// ### Control and status
const char LTE_SHIELD_COMMAND_FUNC[] = "+CFUN";    // Functionality (reset, etc.)
const char LTE_SHIELD_COMMAND_CLOCK[] = "+CCLK";   // Clock
const char LTE_SHIELD_COMMAND_AUTO_TZ[] = "+CTZU"; // Automatic time zone update
// ### Network service
const char LTE_SHIELD_COMMAND_MNO[] = "+UMNOPROF"; // MNO (mobile network operator) Profile
const char LTE_SHIELD_SIGNAL_QUALITY[] = "+CSQ";
const char LTE_SHIELD_REGISTRATION_STATUS[] = "+CREG";
const char LTE_SHIELD_MESSAGE_PDP_DEF[] = "+CGDCONT";
const char LTE_SHIELD_MESSAGE_ENTER_PPP[] = "D";
const char LTE_SHIELD_OPERATOR_SELECTION[] = "+COPS";
// V24 control and V25ter (UART interface)
const char LTE_SHIELD_COMMAND_BAUD[] = "+IPR";     // Baud rate
// ### GPIO
const char LTE_SHIELD_COMMAND_GPIO[] = "+UGPIOC";  // GPIO Configuration
// ### IP
const char LTE_SHIELD_CREATE_SOCKET[] = "+USOCR";  // Create a new socket
const char LTE_SHIELD_CLOSE_SOCKET[] = "+USOCL";   // Close a socket
const char LTE_SHIELD_CONNECT_SOCKET[] = "+USOCO"; // Connect to server on socket
const char LTE_SHIELD_WRITE_SOCKET[] = "+USOWR";   // Write data to a socket
const char LTE_SHIELD_READ_SOCKET[] = "+USORD";    // Read from a socket
const char LTE_SHIELD_LISTEN_SOCKET[] = "+USOLI";  // Listen for connection on socket
// ### SMS
const char LTE_SHIELD_MESSAGE_FORMAT[] = "+CMGF";  // Set SMS message format
const char LTE_SHIELD_SEND_TEXT[] = "+CMGS";       // Send SMS message
// ### GPS
const char LTE_SHIELD_GPS_POWER[] = "+UGPS";
const char LTE_SHIELD_GPS_REQUEST_LOCATION[] = "+ULOC";
const char LTE_SHIELD_GPS_GPRMC[] = "+UGRMC";

const char LTE_SHIELD_RESPONSE_OK[] = "OK\r\n";

// CTRL+Z and ESC ASCII codes for SMS message sends
const char ASCII_CTRL_Z = 0x1A;
const char ASCII_ESC = 0x1B;



char read_responce(char *data,unsigned int timeout)
{
    unsigned char len;
    unsigned int time_in;
    bool rcvd = false;
    time_in = mill_seconds;
    while(time_in + timeout > mill_seconds)
    {
        if(is_Avaible()) // if there are bytes to read...
        {
            rcvd = true;
            char c = getch(); // read a single char
            if(data != NULL)
            {
                data[len++] = c;
            }
        }
    }
    
    // terminate the string
    if(data != NULL)
    {
        data[len] = '\0';
    }
    // if found is true, then return the length of bytes received.  Othersize return a -1
    if(rcvd == true)
    {
        return len;
    }
    else
    {
        return 0; // zero bytes where found, so zero where returned
    }
}

void send_command(char *cmd,bool at)
{
    if(at)
    {
        putln("AT");
    }
    putln(cmd);
    putch('\r');
}

LTE_Shield_error_t sendCommandWithResponse(const char * command, const char * expectedResponse, char * responseDest, unsigned int commandTimeout, bool at)
{
    // setup some instance variables
    LTE_Shield_error_t success;
    success = LTE_SHIELD_ERROR_NO_RESPONSE;
    return success;
}

char sendATcmd(char *cmd, char *buffres, bool AT, unsigned int timeout)
{
    char err = 0;
    
    if(AT)
    {
        putln("AT");
        putln(cmd);
        putln("\r");
    }
    else
    {
        putln(cmd);
        putln("\r");
    }
    
    return read_responce(buffres,timeout);
}

char autobaud(unsigned long baud)
{
    return -1; // stub
}

// send an AT command to set the baud rate
char set_lte_baud(unsigned long baud)
{
    // LTE_SHIELD_COMMAND_BAUD
    // make sure that the desired baud is supported
    char err = 0; // error code
    bool baud_supported = false;
    char i;
    for(i=0;i<NUM_SUPPORTED_BAUD;i++)
    {
        if(baud == LTE_SHIELD_SUPPORTED_BAUD[i])
        {
            baud_supported = true;
            break;
        }
    }
    
    // if the baud rate is one of the supported baud rates, then send an updated baud rate
    if(baud_supported)
    {
        char cmdBuffer[20],backBuffer[20]; // buffers for outgoing and incoming data
        sprintf(cmdBuffer,"%s=%lu",LTE_SHIELD_COMMAND_BAUD,baud); // build up the command for the send buffer
        char sent_bytes = sendATcmd(cmdBuffer,backBuffer,true,500);
    }
    
    return err; // stub
}

bool testAT()
{
    bool worked = false;
    
    putln("AT\r"); // send an AT command with a termination character
    char buffBack[20];
    char len = read_responce(buffBack,500);
    if(len > 0)
    {
        worked = true;
    }
    
    return worked;
}

char lte_start(unsigned long desired_baud)
{
    char err = -1;
    
    return err;
}

// power on function
// physical pin 8, pin RC2
void powerOn()
{
    // set pin 8 low impedence(z) state
    TRISCbits.TRISC2 = 0; // set pin 8 to an output
    PORTCbits.RC2 = 0; // output low
    sysDelay_ms(200); // delay for 200 ms
    TRISCbits.TRISC2 = 1; // set back to a high Z state
}