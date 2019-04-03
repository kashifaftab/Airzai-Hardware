#ifndef __UART_PARSER_H__
#define __UART_PARSER_H__

#include <stdint.h>
#include "conf.h"

//#define parser_printf(...)		xprintf(__VA_ARGS__)

#define MAX_PARSER_CMD_BUF UARTCOMMANDS_CMD_BUFFER_SIZE
#define MAX_PARSER_CMD 10


#define _UNLIM_ 0xFF
#define _NONE_  0x00

//struct for parser
typedef struct
{
  char    CBuf[MAX_PARSER_CMD_BUF];     //parser rx buffer
  uint32_t     idx;                     //byte index
}_parser;


//command parser table struct
typedef struct
  {
    char  *cmd;             //command
    uint8_t   param;        //param cnt, 0xff - variable, 0 - without param
    int   (*func)(void*);   //pounter to handler
  }_cmdtabl;


int Parse_control_cmd (char * buf);
int CmdSetLedOn (void * param);
int CmdSetLedOff (void * param);
int CmdSetLedBrightness (void * param);
int CmdSetLedColor (void * param);
int CmdGetHelp (void * param);

#endif
