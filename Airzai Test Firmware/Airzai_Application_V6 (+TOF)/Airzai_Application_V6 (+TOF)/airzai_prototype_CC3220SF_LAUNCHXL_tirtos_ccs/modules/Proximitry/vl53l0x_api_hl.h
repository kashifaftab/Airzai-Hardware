/*!
 * @file Adafruit_VL53L0X.h

  This is a library for the Adafruit VL53L0X Sensor Breakout

  Designed specifically to work with the VL53L0X sensor from Adafruit
  ----> https://www.adafruit.com/products/3317

  These sensors use I2C to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef VL53L0X_API_HL_H
#define VL53L0X_API_HL_H

#include "vl53l0x_api.h"

#define VL53L0X_I2C_ADDR  0x29 ///< Default sensor I2C address

#define VERSION_REQUIRED_MAJOR  1 ///< Required sensor major version
#define VERSION_REQUIRED_MINOR  0 ///< Required sensor minor version
#define VERSION_REQUIRED_BUILD  2 ///< Required sensor build

void * VL53L0X_Task(void *pvParameters);

void vl53l0x_api_hl_init(void); // Init interrupt and callback fro VL53L0X
VL53L0X_Error WaitMeasurementDataReady(VL53L0X_DEV Dev);
VL53L0X_Error vl53l0x_api_hl_mode_init(VL53L0X_Dev_t *pMyDevice); // Mode Init for VL53L0X

void print_pal_error(VL53L0X_Error Status);



#endif
