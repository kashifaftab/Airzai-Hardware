/*!
 * @file Adafruit_VL53L0X.cpp
 *
 * @mainpage Adafruit VL53L0X time-of-flight sensor
 *
 * @section intro_sec Introduction
 *
 * This is the documentation for Adafruit's VL53L0X driver for the
 * Arduino platform.  It is designed specifically to work with the
 * Adafruit VL53L0X breakout: https://www.adafruit.com/product/3317
 *
 * These sensors use I2C to communicate, 2 pins (SCL+SDA) are required
 * to interface with the breakout.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section dependencies Dependencies
 *
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>

#include <semaphore.h>

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "VL53L0X_api_hl.h"

#include "conf.h"

sem_t sem_vl53l0x_interrupt;

void gpioProximitryInterruptCallback(uint_least8_t index);

VL53L0X_Dev_t MyDevice;
VL53L0X_Dev_t *pMyDevice;

void vl53l0x_api_hl_init(void)
{
    pMyDevice = &MyDevice;

    // Initialize Comms
    pMyDevice->I2cDevAddr      =  VL53L0X_I2C_ADDR;  // default
    pMyDevice->comms_type      =  1;
    pMyDevice->comms_speed_khz =  400;

#if TOF_ENABLE_CONTUNIOUS_MEAS_INT
    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    /* For use interrupt in this pin (P60) you need install R82 resistor 0 Ohm */
    /* and delete R84 resistor  FROM  LAUNCHCC3220MODASF Board                 */
    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

    /* install Button callback */
    GPIO_setCallback(CC3220SF_LAUNCHXL_VL53L0X_INT, gpioProximitryInterruptCallback);

    /* Enable interrupts */
    GPIO_enableInt(Board_VL53L0X_INT);

    sem_init(&sem_vl53l0x_interrupt, 0, 0);
#endif

    //Enable VL53L0X chip. Set High level on SHDN Pin
    GPIO_write(Board_VL53L0X_SHDN, 1);


    usleep(10000);
}


void print_pal_error(VL53L0X_Error Status){
    char buf[VL53L0X_MAX_STRING_LENGTH];
    VL53L0X_GetPalErrorString(Status, buf);
    printf("API Status: %i : %s\n", Status, buf);
}

void print_range_status(VL53L0X_RangingMeasurementData_t* pRangingMeasurementData){
    char buf[VL53L0X_MAX_STRING_LENGTH];
    uint8_t RangeStatus;

    /*
     * New Range Status: data is valid when pRangingMeasurementData->RangeStatus = 0
     */

    RangeStatus = pRangingMeasurementData->RangeStatus;

    VL53L0X_GetRangeStatusString(RangeStatus, buf);
    printf("Range Status: %i : %s\n", RangeStatus, buf);

}



VL53L0X_Error WaitMeasurementDataReady(VL53L0X_DEV Dev) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint8_t NewDatReady=0;
    uint32_t LoopNb;

    // Wait until it finished
    // use timeout to avoid deadlock
    if (Status == VL53L0X_ERROR_NONE) {
        LoopNb = 0;
        do {
            Status = VL53L0X_GetMeasurementDataReady(Dev, &NewDatReady);
            if ((NewDatReady == 0x01) || Status != VL53L0X_ERROR_NONE) {
                break;
            }
            LoopNb = LoopNb + 1;
            VL53L0X_PollingDelay(Dev);
        } while (LoopNb < VL53L0X_DEFAULT_MAX_LOOP);

        if (LoopNb >= VL53L0X_DEFAULT_MAX_LOOP) {
            Status = VL53L0X_ERROR_TIME_OUT;
        }
    }

    return Status;
}

VL53L0X_Error WaitStopCompleted(VL53L0X_DEV Dev) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint32_t StopCompleted=0;
    uint32_t LoopNb;

    // Wait until it finished
    // use timeout to avoid deadlock
    if (Status == VL53L0X_ERROR_NONE) {
        LoopNb = 0;
        do {
            Status = VL53L0X_GetStopCompletedStatus(Dev, &StopCompleted);
            if ((StopCompleted == 0x00) || Status != VL53L0X_ERROR_NONE) {
                break;
            }
            LoopNb = LoopNb + 1;
            VL53L0X_PollingDelay(Dev);
        } while (LoopNb < VL53L0X_DEFAULT_MAX_LOOP);

        if (LoopNb >= VL53L0X_DEFAULT_MAX_LOOP) {
            Status = VL53L0X_ERROR_TIME_OUT;
        }

    }

    return Status;
}


VL53L0X_Error vl53l0x_api_hl_mode_init(VL53L0X_Dev_t *pMyDevice)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;

     if(Status == VL53L0X_ERROR_NONE)
     {
         Status = VL53L0X_StaticInit(pMyDevice); // Device Initialization
     }

     if(Status == VL53L0X_ERROR_NONE)
     {
         Status = VL53L0X_PerformRefCalibration(pMyDevice,
                 &VhvSettings, &PhaseCal); // Device Initialization
     }

     if(Status == VL53L0X_ERROR_NONE)
     {
         Status = VL53L0X_PerformRefSpadManagement(pMyDevice,
                 &refSpadCount, &isApertureSpads); // Device Initialization
     }

#if TOF_ENABLE_CONTUNIOUS_MEAS_INT == 1
     if(Status == VL53L0X_ERROR_NONE)
     {
         // no need to do this when we use VL53L0X_PerformSingleRangingMeasurement
         Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING); // Setup in continious ranging mode
     }

     if(Status == VL53L0X_ERROR_NONE)
     {
         Status = VL53L0X_StartMeasurement(pMyDevice);
     }

#else

     // Enable/Disable Sigma and Signal check
     if (Status == VL53L0X_ERROR_NONE) {
         Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                 VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
     }
     if (Status == VL53L0X_ERROR_NONE) {
         Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                 VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
     }

     if (Status == VL53L0X_ERROR_NONE) {
         Status = VL53L0X_SetLimitCheckValue(pMyDevice,
                 VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
                 (FixPoint1616_t)(0.25*65536));
     }
     if (Status == VL53L0X_ERROR_NONE) {
         Status = VL53L0X_SetLimitCheckValue(pMyDevice,
                 VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,
                 (FixPoint1616_t)(18*65536));
     }
     if (Status == VL53L0X_ERROR_NONE) {
         Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice,
                 200000);
     }
#endif

     return Status;
}



void gpioProximitryInterruptCallback(uint_least8_t index)
{
    GPIO_clearInt(Board_VL53L0X_INT);
    sem_post(&sem_vl53l0x_interrupt);
}
