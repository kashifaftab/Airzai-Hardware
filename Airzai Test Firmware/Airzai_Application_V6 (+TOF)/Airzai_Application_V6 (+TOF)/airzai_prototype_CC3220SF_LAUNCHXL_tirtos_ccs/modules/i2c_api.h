#ifndef _I2C_API_H_
#define _I2C_API_H_

int i2c_init(void);

// I2C operations with 1 byte write and read
void i2c_write(uint8_t device_addr, uint8_t reg, uint8_t value);
uint8_t i2c_read(uint8_t device_addr, uint8_t reg);

// I2C operations with 2 byte write and read
void i2c_write_u16_cmd(uint8_t device_addr, uint16_t reg);
uint8_t i2c_write_buf(uint8_t device_addr, uint8_t *txBuffer, uint32_t writeBufferLen);
uint8_t i2c_read_buf(uint8_t device_addr, uint8_t *rxBuffer, uint32_t readBufferLen);

#endif
