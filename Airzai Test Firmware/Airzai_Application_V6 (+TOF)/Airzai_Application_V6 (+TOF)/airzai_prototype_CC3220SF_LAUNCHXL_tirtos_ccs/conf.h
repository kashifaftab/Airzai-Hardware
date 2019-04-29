#include "uart_term.h"

//*****************************************************************************
// defines
//*****************************************************************************

//Set stack size for threads
#define MAIN_THREAD_STACKSIZE          (4096*2)
#define UART_THREAD_STACKSIZE          (4096)
#define PROX_THREAD_STACKSIZE          (4096)

#define UARTCOMMANDS_STOP_TIMEOUT      (200)
#define UARTCOMMANDS_CMD_BUFFER_SIZE   (64)

#define printf UART_PRINT

//*****************************************************************************
// Defines for enable demo examples in main_thread
// For enable need to set 1, for disable 0
// This mode using only for debug
//*****************************************************************************
// Demo for LP55331 LED driver
#define LED_DEMO_ENABLE                (0)
// Demo for SHT31D LED driver
#define SHT31_DEMO_ENABLE              (0)


//*****************************************************************************
// VL53L0X Settings
//*****************************************************************************
#include "vl53l0x_device.h"
// Enable debug for VL53L0X
#define VL53L0X_DEBUG                  (0)
// if TOF_ENABLE_CONTUNIOUS_MEAS_INT == 1 VL53L0X set to Continious measurement with
// range interrupts
// if TOF_ENABLE_CONTUNIOUS_MEAS_INT == 0 VL53L0X set to Single measurement without
// range interrupts
#define TOF_ENABLE_CONTUNIOUS_MEAS_INT (0)


#define VL53L0X_THRESHOLD_MODE         VL53L0X_GPIOFUNCTIONALITY_THRESHOLD_CROSSED_OUT
#define VL53L0X_THRESHOLD_LOW          (50)
#define VL53L0X_THRESHOLD_HIGH         (200)

//*****************************************************************************


