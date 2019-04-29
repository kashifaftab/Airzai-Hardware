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
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/Timer.h>

#include <ti/drivers/gpio/GPIOCC32XX.h>

#include <pthread.h>

#include "conf.h"
#include "pwm_api.h"
#include "touch_api.h"
#include "lp55231.h"
#include "uart_term.h"
#include "SHT31.h"

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_api_hl.h"

//*****************************************************************************
// defines
//*****************************************************************************


//****************************************************************************
// globals
//****************************************************************************
extern pthread_t UARTCommands_Thread;
extern void * UARTCommands_Task(void *pvParameters);
float g_sht31_temperature;
float g_sht31_humidity;


//****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//****************************************************************************


//*****************************************************************************
//
//! mainThread
//!
//!  \param  pvParameters
//!
//!  \return none
//!
//!  \brief Task handler
//
//*****************************************************************************
void mainThread(void *pvParameters)
{
    pthread_t thread;
    pthread_attr_t pAttrs;
    struct sched_param priParam;
    int retc;
    int detachState;
    int button_state;


    /* Call driver init functions */
    GPIO_init();

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


    retc |= pthread_attr_setstacksize(&pAttrs, UART_THREAD_STACKSIZE);
    if(retc != 0)
    {
        /* pthread_attr_setstacksize() failed */
        while(1)
        {
            ;
        }
    }


    retc = pthread_create(&thread, &pAttrs, UARTCommands_Task, NULL);

    if(retc != 0)
    {
        /* pthread_create() failed */
        while(1)
        {
            ;
        }
    }

    retc |= pthread_attr_setstacksize(&pAttrs, PROX_THREAD_STACKSIZE);
    if(retc != 0)
    {
        /* pthread_attr_setstacksize() failed */
        while(1)
        {
            ;
        }
    }

    retc = pthread_create(&thread, &pAttrs, VL53L0X_Task, NULL);

    if(retc != 0)
    {
        /* pthread_create() failed */
        while(1)
        {
            ;
        }
    }

    // Init peripheral and sensors
    api_pwm_init();
    i2c_init();
    lp55231_enable_hw();
    touch_init();
    SHT31_init(SHT31_DEFAULT_ADDR);

    sleep(1);
    //**********************************************************************
    //Checking LED Driver's
    light_scheme_nFault();


#if SHT31_DEMO_ENABLE
    while(1)
    {
        sht31_status = SHT31_readStatus();
        if(SHT31_readTempHum() == true)
        {
            sht31_temperature = SHT31_readTemperature();
            sht31_humidity = SHT31_readHumidity();
        }
        usleep(100000);
    }
#endif

#if LED_DEMO_ENABLE
    uint8_t Led_Color = 0;
    uint8_t i;
#endif

    //Infinity loop
    while (1)
    {
#if LED_DEMO_ENABLE
        for(i = 0; i < 255; i++)
            lp55231_led_control(TIDA00757_ALL, Led_Color&0x7, i);
        Led_Color++;
        light_scheme_nFault();
        usleep(100000);
        light_scheme_power_on();
        usleep(100000);
        light_scheme_low_battery();
        usleep(100000);
        light_scheme_unlock_lite();
        usleep(100000);
        light_scheme_lock_lite();
        usleep(100000);
        light_scheme_unlock();
        usleep(100000);
        light_scheme_lock();
        usleep(100000);
        light_scheme_light_show();
        usleep(100000);
#endif

        // Task blocked here until button will not pressed
        button_state = touch_button_wait_blocked();
        if(button_state == BUT_PRESSED)
            UART_PRINT("Touch Button is pressed!\n\r");
        else if(button_state == BUT_RELEASED)
            UART_PRINT("Touch Button is released!\n\r");


        if(button_state == BUT_PRESSED)
            api_pwm1_enable();
        else
            api_pwm1_disable();


        // Read data from SHT31 Sensor
        // If true, then we can get Humidity and Temperature
        if(SHT31_readTempHum() == true)
        {
            g_sht31_temperature = SHT31_readTemperature();
            g_sht31_humidity = SHT31_readHumidity();

            UART_PRINT("Temperature is: %.1f Humidity is: %.1f\n\r", g_sht31_temperature, g_sht31_humidity);


        }

    }
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
