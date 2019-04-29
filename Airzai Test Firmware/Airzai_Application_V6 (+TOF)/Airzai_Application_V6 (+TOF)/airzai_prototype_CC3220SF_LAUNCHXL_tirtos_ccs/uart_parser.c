#include <stdio.h>
#include <string.h>
#include "uart_parser.h"
#include "uart_term.h"
#include "lp55231.h"

extern char UARTCommands_cmdBuffer[UARTCOMMANDS_CMD_BUFFER_SIZE];
int getColorNumber(void * pColor);

/*
 UART commands for LED control
 on #led
 off #led
 dim #led #brightness
 color #led #color
 */

uint8_t g_drvState;
struct Led_state
{
    uint8_t state;
    uint8_t color;
    uint8_t brightness;
}ledState[6];

//------------- command parser table --------------//
 _cmdtabl const CMDTABL[]=
  {
      {"on"       ,_UNLIM_ , CmdSetLedOn 				},
      {"off"      ,_UNLIM_ , CmdSetLedOff 				},
      {"dim"      ,_UNLIM_ , CmdSetLedBrightness		},
      {"color"    ,_UNLIM_ , CmdSetLedColor             },
      {"?"        ,_NONE_  , CmdGetHelp                 },
      { NULL,0,0},
  };
//------------------------------------------------------//

//control command parser
int Parse_control_cmd (char * buf)
{
    uint32_t    len;
    char * command;
    char * params;
    uint32_t    i;
    uint8_t    ret;

    //Init LedStates structure only once, when do first call
    static int flag_ledStateInit = 0;
    if(flag_ledStateInit == 0)
    {
        g_drvState = 0;
        memset((uint8_t *)ledState, '\0', sizeof(ledState));
        flag_ledStateInit = 1;
    }

    command = strtok (buf, " ");
    if (command == NULL) return 0;
    if (*command == 0x0D || *command == 0x0A)
    {
        return 0;
    }
    params = strtok (NULL, "\n");
    i = 0;
    while(CMDTABL[i].cmd != NULL)
    {
        len = strlen(CMDTABL[i].cmd);
        if (strncmp (command,CMDTABL[i].cmd,len) == 0)
        {//find a command
            if (params == NULL && CMDTABL[i].param != 0 && CMDTABL[i].param != 0xff)
            {
                UART_PRINT (">>parameters missing!\r\n");
                break;
            }

            //call handler
            if ((ret = CMDTABL[i].func(params)) == 0)
            {
                UART_PRINT (">>wrong or missing parameters\r\n");
            }
            UART_PRINT ("\r\n");
            break;
        }
        if (++i>MAX_PARSER_CMD) break;
    }
    if (CMDTABL[i].cmd == NULL)
    {
        UART_PRINT (">>unknown cmd\r\n");

    }
    return 1;
}

// Callback function for UART console command on #led
// Change led state to on
int CmdSetLedOn (void * param)
{
    char * ptr;
    uint8_t    n, num;


    if (param == NULL)  return 0;
    else
    {
        //get led number
        ptr = strtok( param, "\n");
        if (ptr == NULL) return 0;
        n = sscanf (ptr,"%hhu",&num);
        if(n==0 || ptr == NULL || num == 0 || num > 6 ) return 0;
        if(g_drvState == 0)
        {
            lp55231_enable(LP55231_DEVICE1_ADDR);
            lp55231_enable(LP55231_DEVICE2_ADDR);
            g_drvState = 1;
        }
        ledState[num-1].state = 1;
        lp55231_led_control(num, ledState[num-1].color, ledState[num-1].brightness);
    }
    return 1;
}

// Callback function for UART console command off #led
// Change led state to off
int CmdSetLedOff (void * param)
{
    char * ptr;
    int    n, num;


    if (param == NULL)  return 0;
    else
    {
        //get led number
        ptr = strtok( param, "\n");
        if (ptr == NULL) return 0;
        n = sscanf ((const char *)ptr,"%u",&num);
        if(n==0 || ptr == NULL || num == 0 || num > 6 ) return 0;
        ledState[num-1].state = 0;
        lp55231_led_control(num, ledState[num-1].color, 0);
    }
    return 1;
}

// Callback function for UART console command dim
// Set brightness for led
int CmdSetLedBrightness (void * param)
{
    char * ptr;
    uint8_t    n, num;
    uint8_t    dim;


    if (param == NULL)  return 0;
    else
    {
        //get led number
        ptr = strtok(param, " ");
        if (ptr == NULL) return 0;
        n = sscanf ((const char *)ptr,"%hhu",&num);
        if(n==0 || ptr == NULL || num == 0 || num > 6 ) return 0;

        ptr = strtok (NULL, " ");
        if (ptr == NULL) return 0;
        n = sscanf (ptr,"%hhu",&dim);
        if(n==0 || ptr == NULL) return 0;
        ledState[num-1].brightness = dim;
        if(ledState[num-1].state == 0)
            lp55231_led_control(num, ledState[num-1].color, 0);
        else
            lp55231_led_control(num, ledState[num-1].color, ledState[num-1].brightness);
    }
    return 1;
}

// Callback function for UART console command color
// Set color for led
int CmdSetLedColor (void * param)
{
    char * ptr;
    uint8_t    n, num;
    uint8_t    color;


    if (param == NULL)  return 0;
    else
    {
        //get led number
        ptr = strtok(param, " ");
        if (ptr == NULL) return 0;
        n = sscanf ((const char *)ptr,"%hhu",&num);
        if(n==0 || ptr == NULL || num == 0 || num > 6 )

        ptr = strtok (NULL, " ");
        ptr = strtok (NULL, " ");
        if (ptr == NULL) return 0;
        // Color
        if((color = (uint8_t)getColorNumber(ptr)) == 0) return 0;
        ledState[num-1].color = color;
        if(ledState[num-1].state == 0)
            lp55231_led_control(num, ledState[num-1].color, 0);
        else
            lp55231_led_control(num, ledState[num-1].color, ledState[num-1].brightness);
    }
    return 1;
}

// This function return commands description
int CmdGetHelp (void * param)
{
    UART_PRINT  ("|----------------------------------------------------|  \n\r");
    UART_PRINT  ("| All commands should be ended with \\n\\r               \n\r");
    UART_PRINT  ("|----------------------------------------------------|  \n\r");
    UART_PRINT  ("| Enable or disable led by number[1-6]:\n\r");
    UART_PRINT  ("| on #led\n\r");
    UART_PRINT  ("| off #led\n\r");
    UART_PRINT  ("|----------------------------------------------------|  \n\r");
    UART_PRINT  ("| Change led color by name:\n\r");
    UART_PRINT  ("| color #led color_name\n\r");
    UART_PRINT  ("| Available colors: blue green skyblue red purple yellow white\n\r");
    UART_PRINT  ("|----------------------------------------------------|  \n\r");
    UART_PRINT  ("| Brightness adjustment [0-254]:\n\r");
    UART_PRINT  ("| dim #led brightness_level\n\r");
    UART_PRINT  ("|----------------------------------------------------|  \n\r");

    return 1;
}

// This function return color number by name
int getColorNumber(void * pColor)
{
    if(strncmp(pColor,"blue",4) == 0)
    {
        return COLOR_BLUE;
    }
    else if(strncmp(pColor,"green",5) == 0)
    {
        return COLOR_GREEN;
    }
    else if(strncmp(pColor,"skyblue",7) == 0)
    {
        return COLOR_SKYBLUE;
    }
    else if(strncmp(pColor,"red",3) == 0)
    {
        return COLOR_RED;
    }
    else if(strncmp(pColor,"purple",6) == 0)
    {
        return COLOR_PURPLE;
    }
    else if(strncmp(pColor,"yellow",6) == 0)
    {
        return COLOR_YELLOW;
    }
    else if(strncmp(pColor,"white",5) == 0)
    {
        return COLOR_WHITE;
    }
    return 0;
}
