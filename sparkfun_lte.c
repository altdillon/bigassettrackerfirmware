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

// super important function for actually starting the LTE system
char lte_start(unsigned long desired_baud)
{
    char err = -1;
    // inital values for the state mashine
    unsigned char current_state = LTE_AUTOBAUD;
    unsigned char next_state = 0;
    // inital value for the timeout system
    //unsigned int lte_timeout = 60000; // 60k ms = 1 minuite, which is the max amount of time I'm willing to wait for this thing to start
    unsigned int start_time = mill_seconds; // save the start time
    while((start_time + LTE_SHIELD_INIT_TIMEOUT) > mill_seconds) // while we havent timed out...
    {
        // run the state mashine...
        if(current_state == LTE_RESET)
        {
            
        }
        else if(current_state == LTE_AUTOBAUD)
        {
            if(autobaud(desired_baud) != 0) // run the autobaud function and determine if it worked
            {
                
            }
        }
        else if(current_state == LTE_CONFIGURE)
        {
            
        }
        
        current_state = next_state;
    }

    
    return err;
    //return 0; // stub to make sure that the rest of the system works
}

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

//void send_command(char *cmd,bool at)
//{
//    if(at)
//    {
//        putln("AT");
//    }
//    putln(cmd);
//    putch('\r');
//}

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
        //putln(cmd);
        //putln("\r");
    }
//    else
//    {
//        putln(cmd);
//        //putln("\r");
//    }
    
    putln(cmd);
    putln("\r");
    
    return read_responce(buffres,timeout);
}

// see auto baud in the LTE lib
/*
 * auto baud steps:
 * 1. pick a test baud from a list of supported baud rates
 * 2. set the local USART to that baud rate
 * 3. set the remote baud rate of the lte devices from a buad rate set by an lete device
 * 4. the local usart is then set to the local baud rate
 */

char autobaud(unsigned long baud)
{
    char err = -1; 
    unsigned long test_baud = 0;
    bool baud_found = false;
    // interate through possible baud rates
    char b;
    for(b=0;b<NUM_SUPPORTED_BAUD;b++)
    {
        test_baud = LTE_SHIELD_SUPPORTED_BAUD[b]; // pull a test baud form the test baud array
        set_baud(test_baud); // set the local USART to the test baud rate
        set_lte_baud(baud); // set the LTE baud to the desired baud rate
        sysDelay_ms(200); // now pause for 200 ms
        set_baud(baud); // restart the local baud rate to the desired baud rate
        if(testAT()) // ok, now see if it works
        {
            // omg! it worked!
            baud_found = true;
            break;
        }
    }
    
    if(baud_found)
    {
        set_baud(baud); // make sure that the local USART is running at the local baud
        err = 0; 
    }
    
    return err;
}

// send an AT command to set the baud rate
char set_lte_baud(unsigned long baud)
{
    // LTE_SHIELD_COMMAND_BAUD
    // make sure that the desired baud is supported
    char err = -1; // error code
    bool baud_supported = false;
//    char i;
//    for(i=0;i<NUM_SUPPORTED_BAUD;i++)
//    {
//        if(baud == LTE_SHIELD_SUPPORTED_BAUD[i])
//        {
//            baud_supported = true;
//            break;
//        }
//    }
    
    // if the baud rate is one of the supported baud rates, then send an updated baud rate
    //if(baud_supported)
    //{
      char cmdBuffer[20],backBuffer[20]; // buffers for outgoing and incoming data
      //sprintf(cmdBuffer,"%s=%lu",LTE_SHIELD_COMMAND_BAUD,baud); // build up the command for the send buffer
      // build up the command and send it
      char baud_str[7];
      long2str(baud,baud_str,6);
      strcpy(cmdBuffer,LTE_SHIELD_COMMAND_BAUD);
      strcat(cmdBuffer,"=");
      strcat(cmdBuffer,baud_str);
      asm("nop");
      char sent_bytes = sendATcmd(cmdBuffer,backBuffer,true,500);
      if(sent_bytes > 0)
      {
        err = 0; // if Zero then setting the baud rate works
      }
    //}
    
    return err; 
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

// enable echo command
char enable_echo(bool echon)
{
    char err = 0;
    // commands for turning echo on and off
    //const char *echoOn = "ATE1";
    //onst char *echoOff = "ATE0";
    char backSendBuffer[20],cmdBuffer[9];
    
    if(echon)
    {
        strcpy(cmdBuffer,"ATE1");
    }
    else
    {
        strcpy(cmdBuffer,"ATE0");
    }
    
    unsigned char bytes_back = sendATcmd(cmdBuffer,backSendBuffer,false,500);
    if(bytes_back == 0)
    {
        err = -1; // if no bytes are sent back then -1 as an error code
    }
    
    
    return err;
}

// power on function
// physical pin 8, pin RC2
void powerOn()
{
    // set pin 8 low impedence(z) state
    TRISCbits.TRISC2 = 0; // set pin 8 to an output
    PORTCbits.RC2 = 0; // output low
    sysDelay_ms(LTE_SHIELD_POWER_PULSE_PERIOD); // delay for 3200 ms
    TRISCbits.TRISC2 = 1; // set back to a high Z state
}


char set_gpio_mode(LTE_Shield_gpio_t gpio, LTE_Shield_gpio_mode_t mode)
{
    char err = -1;
    // buffers for commands and return output
    char cmdBuffer[20];
    char backBuffer[20];
    // LTE_SHIELD_COMMAND_GPIO --> +UGPIOC
    //sprintf(cmdBuffer, "%s=%d,%d",LTE_SHIELD_COMMAND_GPIO, gpio, mode); // build up the command in Sprintf
    // convert some constants to ints
    char str_gpio[3];
    char str_gpio_mode[4];
    int2str(gpio,str_gpio,3);
    int2str(mode,str_gpio_mode,4);
    
    // build up the command in strcat
    strcpy(cmdBuffer,LTE_SHIELD_COMMAND_GPIO);
    strcat(cmdBuffer,"=");
    strcat(cmdBuffer,str_gpio);
    strcat(cmdBuffer,",");
    strcat(cmdBuffer,str_gpio_mode);
    asm("asm");
    
    
    if(sendATcmd(cmdBuffer, backBuffer, true, LTE_SHIELD_SET_BAUD_TIMEOUT) > 0) // send the command and check for success
    {
        // if more then 0 bytes returned condtion is working
        err = 0;
    }
    
    return err;
}