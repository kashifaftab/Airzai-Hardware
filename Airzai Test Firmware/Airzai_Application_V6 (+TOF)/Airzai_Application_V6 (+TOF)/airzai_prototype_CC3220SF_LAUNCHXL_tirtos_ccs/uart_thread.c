//*****************************************************************************
// includes
//*****************************************************************************
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

/* TI-DRIVERS Header files */
#include <ti/drivers/UART.h>
#include "pthread.h"
#include <semaphore.h>
#include "conf.h"
#include "uart_term.h"

#include "uart_parser.h"
//*****************************************************************************
// defines
//*****************************************************************************


//****************************************************************************
// globals
//****************************************************************************
char UARTCommands_cmdBuffer[UARTCOMMANDS_CMD_BUFFER_SIZE];


//*****************************************************************************
//
//! UARTCommands_Task
//!
//!  \param  pvParameters
//!
//!  \return none
//!
//!  \brief   UART event Task handler function to receive inputs
//
//*****************************************************************************
void * UARTCommands_Task(void *pvParameters)
{
    int lRetVal;
    InitTerm();
    UART_PRINT("Airzai system started!\n\r");

    while(1)
    {
        UART_PRINT("Enter Command:\n\r");
        lRetVal = GetRawCmd(UARTCommands_cmdBuffer , UARTCOMMANDS_CMD_BUFFER_SIZE);
        UARTCommands_cmdBuffer[lRetVal-1] = '\0';
        UART_PRINT("Get command '%s' \n\r\n\r", UARTCommands_cmdBuffer);

        Parse_control_cmd(UARTCommands_cmdBuffer);

    }

    // Not reachable
    return (NULL);
}


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
