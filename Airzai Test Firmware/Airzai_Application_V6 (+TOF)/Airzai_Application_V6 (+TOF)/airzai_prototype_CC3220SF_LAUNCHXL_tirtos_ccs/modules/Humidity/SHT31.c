/***************************************************
  This is a library for the SHT31 Digital Humidity & Temp Sensor

  Designed specifically to work with the SHT31 Digital sensor from Adafruit
  ----> https://www.adafruit.com/products/2857

  These sensors use I2C to communicate, 2 pins are required to interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <pthread.h>

#include "SHT31.h"
#include "i2c_api.h"

#define delay(a) usleep(a*1000)

/**
 * Placeholder to track the I2C address.
 */
static uint8_t  _i2caddr;
/**
 * Placeholder to track humidity internally.
 */
static float    humidity;
/**
 * Placeholder to track temperature internally.
 */
static float    temp;


bool SHT31_init(uint8_t i2caddr) {
  _i2caddr = i2caddr;
  SHT31_reset();
  // return (readStatus() == 0x40);
  return true;
}

uint16_t SHT31_readStatus(void) {
  uint8_t rxBuffer[2];
  uint16_t stat;
  i2c_write_u16_cmd(_i2caddr, SHT31_READSTATUS);
  if(i2c_read_buf(_i2caddr, rxBuffer, 2) != 0)
  {
      //Error I2C read;
  }
  stat  = (rxBuffer[0]<<8);
  stat |= (rxBuffer[1]);

  return stat;
}

void SHT31_reset(void) {
    i2c_write_u16_cmd(_i2caddr, SHT31_SOFTRESET);
    delay(10);
}

void SHT31_heater(bool h) {
  if (h)
      i2c_write_u16_cmd(_i2caddr, SHT31_HEATEREN);
  else
      i2c_write_u16_cmd(_i2caddr, SHT31_HEATERDIS);
}


float SHT31_readTemperature(void) {
  return temp;
}

float SHT31_readHumidity(void) {
  return humidity;
}


bool SHT31_readTempHum(void) {
  uint8_t readbuffer[6];

  i2c_write_u16_cmd(_i2caddr, SHT31_MEAS_HIGHREP);

  delay(20);


  if(i2c_read_buf(_i2caddr, readbuffer, 6) != 0)
  {
      //Error I2C read;
      return false;
  }

  uint16_t ST, SRH;
  ST = readbuffer[0];
  ST <<= 8;
  ST |= readbuffer[1];

  if (readbuffer[2] != SHT31_crc8(readbuffer, 2))
    return false;

  SRH = readbuffer[3];
  SRH <<= 8;
  SRH |= readbuffer[4];

  if (readbuffer[5] != SHT31_crc8(readbuffer + 3, 2))
    return false;

  double stemp = ST;
  stemp *= 175;
  stemp /= 0xffff;
  stemp = -45 + stemp;
  temp = stemp;

  //  Serial.print("SRH = "); Serial.println(SRH);
  double shum = SRH;
  shum *= 100;
  shum /= 0xFFFF;

  humidity = shum;

  return true;
}


uint8_t SHT31_crc8(const uint8_t *data, int len) {
  /*
   *
   * CRC-8 formula from page 14 of SHT spec pdf
   *
   * Test data 0xBE, 0xEF should yield 0x92
   *
   * Initialization data 0xFF
   * Polynomial 0x31 (x8 + x5 +x4 +1)
   * Final XOR 0x00
   */

  const uint8_t POLYNOMIAL = 0x31;
  uint8_t crc = 0xFF;
  int j;
  for (j = len; j; --j) {
    crc ^= *data++;

    int i;
    for (i = 8; i; --i) {
      crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
    }
  }
  return crc;
}
