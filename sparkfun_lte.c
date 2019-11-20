#include "sparkfun_lte.h"
#include "app.h"



// super important function for actually starting the LTE system
char lte_start(unsigned long desired_baud)
{
    char err = -1;
    // inital values for the state mashine
    unsigned char current_state = LTE_AUTOBAUD; // starting state...
    unsigned char next_state = 0;
    // inital value for the timeout system
    //unsigned int lte_timeout = 60000; // 60k ms = 1 minuite, which is the max amount of time I'm willing to wait for this thing to start
    unsigned int start_time = mill_seconds; // save the start time
    while((start_time + LTE_SHIELD_INIT_TIMEOUT) > mill_seconds) // while we havent timed out...
    {
        // run the state mashine...
        if(current_state == LTE_RESET)
        {
            // reset state, try to turn the LTE device on
            // send a power on command, then wait for the devices to start
            powerOn(); 
            if(!testAT()) // send an AT command to see if the send command was successfull.
            {
                next_state = LTE_AUTOBAUD; // next, configure
            }
            else
            {
                next_state = LTE_CONFIGURE; // if sucessful, then move on to figure state 
            }
        }
        else if(current_state == LTE_AUTOBAUD)
        {
            if(autobaud(desired_baud) != 0) // run the autobaud function and determine if it worked
            {
                next_state = LTE_RESET; // reset the LTE device
            }
            else
            {
                next_state = LTE_CONFIGURE;
            }
        }
        else if(current_state == LTE_CONFIGURE)
        {
            // run basic configuring stuff for the micro
            if(enable_echo(false)) // run a quick test by disabling serial echo
            {
                // set some gpio modes
                set_gpio_mode(GPIO1,NETWORK_STATUS);
                set_gpio_mode(GPIO2,GNSS_SUPPLY_ENABLE);
                // set the error codes and then run
                err = 0; // return zero if the system worked
                break;
            }
            else
            {
                next_state = LTE_AUTOBAUD;
            }
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

//LTE_Shield_error_t sendCommandWithResponse(const char * command, const char * expectedResponse, char * responseDest, unsigned int commandTimeout, bool at)
//{
//    // setup some instance variables
//    LTE_Shield_error_t success;
//    success = LTE_SHIELD_ERROR_NO_RESPONSE;
//    return success;
//}

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
      memset(cmdBuffer,0,20); // variables relating 
      long2str(baud,baud_str,count_digits(baud));
      strcpy(cmdBuffer,"+IPR");
      strcat(cmdBuffer,"=");
      strcat(cmdBuffer,baud_str);
      //asm("nop");
      char sent_bytes = sendATcmd(cmdBuffer,backBuffer,true,500);
      if(sent_bytes > 0)
      {
        err = 0; // if Zero then setting the baud rate works
      }
      asm("nop");
    
    return err; 
}

bool testAT()
{
    bool worked = false;
    
    putln("AT\r"); // send an AT command with a termination character
    char buffBack[20];
    memset(buffBack,0,20);
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