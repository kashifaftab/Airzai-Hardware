//*****************************************************************************
// includes
//*****************************************************************************
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>

/* TI-DRIVERS Header files */
#include <ti/drivers/GPIO.h>

#include <pthread.h>
#include <semaphore.h>

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_api_hl.h"

#include "proximitry_thread.h"

#include "conf.h"

//*****************************************************************************
// defines
//*****************************************************************************


//****************************************************************************
// globals
//****************************************************************************
extern VL53L0X_Dev_t  MyDevice;
extern VL53L0X_Dev_t *pMyDevice;
extern sem_t sem_vl53l0x_interrupt;

//****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//****************************************************************************
void * VL53L0X_Interrupts_Handler_Task(void *pvParameters);

void run_VL53L0X_Interrupts_Handler_Task(void)
{
    pthread_t thread;
    pthread_attr_t pAttrs;
    struct sched_param priParam;
    int retc;
    int detachState;

    // Create new thread for handle uart data from console
    /* Set priority and stack size attributes */
    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 1;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&pAttrs, detachState);
    if(retc != 0)
    {
        /* pthread_attr_setdetachstate() failed */
        while(1)
        {
            ;
        }
    }

    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc |= pthread_attr_setstacksize(&pAttrs, 2048);
    if(retc != 0)
    {
        /* pthread_attr_setstacksize() failed */
        while(1)
        {
            ;
        }
    }

    retc = pthread_create(&thread, &pAttrs, VL53L0X_Interrupts_Handler_Task, NULL);

    if(retc != 0)
    {
        /* pthread_create() failed */
        while(1)
        {
            ;
        }
    }
}

void * VL53L0X_Task(void *pvParameters)
{
    VL53L0X_RangingMeasurementData_t    RangingMeasurementData;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    sleep(3);
    vl53l0x_api_hl_init();

#if TOF_ENABLE_CONTUNIOUS_MEAS_INT
    run_VL53L0X_Interrupts_Handler_Task();
#endif

    // End of implementation specific
    if(Status == VL53L0X_ERROR_NONE)
    {
        Status = VL53L0X_DataInit(&MyDevice); // Data initialization
        print_pal_error(Status);
    }

#if VL53L0X_DEBUG
    if(Status == VL53L0X_ERROR_NONE)
    {
        Status = VL53L0X_GetDeviceInfo(&MyDevice, &DeviceInfo);
    }
#endif


    if(Status == VL53L0X_ERROR_NONE)
    {
        Status = vl53l0x_api_hl_mode_init(pMyDevice);
    }
#if VL53L0X_DEBUG
    print_pal_error(Status);
#endif
    while(1){
#if TOF_ENABLE_CONTUNIOUS_MEAS_INT
        sleep(10);
#else

        Status = VL53L0X_PerformSingleRangingMeasurement(pMyDevice,
                &RangingMeasurementData);

        if(RangingMeasurementData.RangeStatus == 0)
            UART_PRINT("Distance is: %d \n\r", RangingMeasurementData.RangeMilliMeter);
        else
            UART_PRINT("Over range or Error! \n\r");
        usleep(200000);
#endif
    }
}


void * VL53L0X_Interrupts_Handler_Task(void *pvParameters)
{
    VL53L0X_RangingMeasurementData_t    RangingMeasurementData;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint32_t InterruptMask;

    sleep(2);

    Status = VL53L0X_SetInterruptThresholds(pMyDevice, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING, (VL53L0X_THRESHOLD_LOW<<16), (VL53L0X_THRESHOLD_HIGH<<16));

    /* Set interrupt config */
    if (Status == VL53L0X_ERROR_NONE) {
        Status = VL53L0X_SetGpioConfig(pMyDevice, 0, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING,
                                       VL53L0X_THRESHOLD_MODE,
                                       VL53L0X_INTERRUPTPOLARITY_LOW);
    }

    while(1){
        sem_wait(&sem_vl53l0x_interrupt);

        Status = VL53L0X_GetInterruptMaskStatus(pMyDevice, &InterruptMask);
        if(InterruptMask == 0)
            continue;
        UART_PRINT("|----------------------------------------------------|\n\r");
        UART_PRINT(" VL53L0X Interrupt. Int. mask 0x0%x. Status %d \n\r", InterruptMask, Status);

        //Status = WaitMeasurementDataReady(pMyDevice);
        Status = VL53L0X_GetRangingMeasurementData(pMyDevice, &RangingMeasurementData);

        if(Status == VL53L0X_ERROR_NONE)
        {
            switch(InterruptMask)
            {
            case VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_LEVEL_LOW:


                UART_PRINT(" Low range\n\r");
                UART_PRINT(" Distance is %d \n\r", RangingMeasurementData.RangeMilliMeter);
                // Clear the interrupt
                VL53L0X_ClearInterruptMask(pMyDevice, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_LEVEL_LOW);
                break;

            case VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_LEVEL_HIGH:


                UART_PRINT(" High range\n\r");
                UART_PRINT(" Distance is %d \n\r", RangingMeasurementData.RangeMilliMeter);
                // Clear the interrupt
                VL53L0X_ClearInterruptMask(pMyDevice, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_LEVEL_HIGH);
                break;

            case VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_OUT_OF_WINDOW:


                UART_PRINT(" Window out\n\r");
                UART_PRINT(" Distance is %d \n\r", RangingMeasurementData.RangeMilliMeter);
                // Clear the interrupt
                VL53L0X_ClearInterruptMask(pMyDevice, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_OUT_OF_WINDOW);
                break;

            case VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY:


                UART_PRINT(" Sample ready\n\r");
                UART_PRINT(" Distance is %d \n\r", RangingMeasurementData.RangeMilliMeter);
                // Clear the interrupt
                VL53L0X_ClearInterruptMask(pMyDevice, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
                break;
            }

        }
        else if(Status == VL53L0X_ERROR_RANGE_ERROR)
            UART_PRINT(" Over Range error! \n\r");
        else
            UART_PRINT(" VL53L0x status error! \n\r");
        UART_PRINT("|----------------------------------------------------|\n\r \n\r \n\r");
    }
}



