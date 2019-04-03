/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 /*
 *  Terminal
 */

/* POSIX Header files */
#include <pthread.h>

#include "uart_term.h"


#define UART_NONPOLLING

extern int vsnprintf(char * s,
                     size_t n,
                     const char * format,
                     va_list arg);

//*****************************************************************************
//                          LOCAL DEFINES
//*****************************************************************************
#define IS_SPACE(x)       (x == 32 ? 1 : 0)

//*****************************************************************************
//                 GLOBAL VARIABLES
//*****************************************************************************
UART_Handle uartHandle;
pthread_mutex_t uartMutex;
//*****************************************************************************
//
//! Initialization
//!
//! This function
//!        1. Configures the UART to be used.
//!
//! \param  none
//!
//! \return UART_Handle
//
//*****************************************************************************
UART_Handle InitTerm(void)
{
    int retc;

    UART_Params uartParams;

    UART_init();
    UART_Params_init(&uartParams);

    /* Create a UART with data processing off. */
    uartParams.writeDataMode = UART_DATA_TEXT;
    uartParams.readDataMode = UART_DATA_TEXT;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.readReturnMode = UART_RETURN_NEWLINE;
    uartParams.readEcho = UART_ECHO_ON;
    uartParams.baudRate = 115200;

    uartHandle = UART_open(Board_UART0, &uartParams);

    retc = pthread_mutex_init(&uartMutex, NULL);
    if (retc != 0) {
        /* pthread_mutex_init() failed */
        while (1) {}
    }

    return(uartHandle);

}






//*****************************************************************************
//
//! prints the formatted string on to the console
//!
//! \param[in]  format  - is a pointer to the character string specifying the
//!                       format in the following arguments need to be
//!                       interpreted.
//! \param[in]  [variable number of] arguments according to the format in the
//!             first parameters
//!
//! \return count of characters printed
//
//*****************************************************************************

int Report(const char *pcFormat,
           ...)
{
    int iRet = 0;
    char        *pcBuff;
    char        *pcTemp;
    int iSize = 256;
    va_list list;



    pthread_mutex_lock(&uartMutex);

    pcBuff = (char*)malloc(iSize);
    if(pcBuff == NULL)
    {
        pthread_mutex_unlock(&uartMutex);
        return(-1);
    }
    while(1)
    {
        va_start(list,pcFormat);
        iRet = vsnprintf(pcBuff, iSize, pcFormat, list);
        va_end(list);
        if((iRet > -1) && (iRet < iSize))
        {
            break;
        }
        else
        {
            iSize *= 2;
            if((pcTemp = realloc(pcBuff, iSize)) == NULL)
            {
                Message("Could not reallocate memory\n\r");
                iRet = -1;
                break;
            }
            else
            {
                pcBuff = pcTemp;
            }
        }
    }
    Message(pcBuff);
    free(pcBuff);
    pthread_mutex_unlock(&uartMutex);
    return(iRet);
}


//*****************************************************************************
//
//! Outputs a character string to the console
//!
//! This function
//!        1. prints the input string character by character on to the console.
//!
//! \param[in]  str - is the pointer to the string to be printed
//!
//! \return none
//!
//! \note If UART_NONPOLLING defined in than Message or UART write should be
//!       called in task/thread context only.
//
//*****************************************************************************
void Message(const char *str)
{
#ifdef UART_NONPOLLING
    UART_write(uartHandle, str, strlen(str));
#else
    UART_writePolling(uartHandle, str, strlen(str));
#endif
}

//*****************************************************************************
//
//! Get the raw Command string from UART - no processing of special characters
//!
//! \param[in]  pucBuffer   - is the command store to which command will be
//!                           populated
//! \param[in]  ucBufLen    - is the length of buffer store available
//!
//! \return Length of the bytes received. -1 if buffer length exceeded.
//!
//*****************************************************************************
int GetRawCmd(char *pcBuffer, unsigned int uiBufLen)
{
    int     iLen = 0;

    iLen = UART_read(uartHandle, pcBuffer , uiBufLen);

    return iLen;
}

//*****************************************************************************
//
//! Clear the console window
//!
//! This function
//!        1. clears the console window.
//!
//! \param  none
//!
//! \return none
//
//*****************************************************************************
void ClearTerm()
{
    Message("\33[2J\r");
}

//*****************************************************************************
//
//! Read a character from the console
//!
//! \param none
//!
//! \return Character
//
//*****************************************************************************
char getch(void)
{
    char ch;

    UART_readPolling(uartHandle, &ch, 1);
    return(ch);
}

//*****************************************************************************
//
//! Outputs a character to the console
//!
//! \param[in]  char    - A character to be printed
//!
//! \return none
//
//*****************************************************************************
void putch(char ch)
{
    UART_writePolling(uartHandle, &ch, 1);
}
