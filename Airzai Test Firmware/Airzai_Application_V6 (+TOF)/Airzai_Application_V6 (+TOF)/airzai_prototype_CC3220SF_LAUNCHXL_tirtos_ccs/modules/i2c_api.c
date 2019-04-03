#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

/* POSIX Header files */
#include <pthread.h>

#include <ti/drivers/I2C.h>
#include "board.h"

#include "i2c_api.h"


/*****************************************************************************
* LOCAL FUNCTION PROTOTYPES
*/

/*****************************************************************************
* LOCAL VARIABLES
*/

static I2C_Handle      i2c;
static I2C_Params      i2cParams;
static I2C_Transaction i2cTransaction;

static pthread_mutex_t i2cMutex;


/*****************************************************************************
* @brief    Init i2c module
*
*
* @return   1 - Initialization failed
*           0 - Initialization success
******************************************************************************/
int i2c_init(void)
{
    int retc;
    retc = pthread_mutex_init(&i2cMutex, NULL);
    if (retc != 0) {
        /* pthread_mutex_init() failed */
        while (1) {}
    }

	// Init here I2C
    I2C_init();
    /* Create I2C for usage */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2c = I2C_open(Board_I2C0, &i2cParams);
    if (i2c == NULL) {
        //Display_printf(display, 0, 0, "Error Initializing I2C\n");
        return -1;
    }
    else {
        //Display_printf(display, 0, 0, "I2C Initialized!\n");
        return 0;
    }
}

/*****************************************************************************
* @brief    Write data to i2c Slave
*
*
*
* @return   1 - Initialization failed
*           0 - Initialization success
******************************************************************************/
void i2c_write(uint8_t device_addr, uint8_t reg, uint8_t value)
{
    uint8_t     txBuffer[2] = {reg, value};
    i2c_write_buf(device_addr, txBuffer, 2);

}





// Read data from
uint8_t i2c_read(uint8_t device_addr, uint8_t reg)
{
    pthread_mutex_lock(&i2cMutex);

    uint8_t     txBuffer[1];
    uint8_t     rxBuffer[1];

    txBuffer[0] = reg;

    i2cTransaction.slaveAddress = device_addr;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 1;
    if (I2C_transfer(i2c, &i2cTransaction)) {
        pthread_mutex_unlock(&i2cMutex);
        return rxBuffer[0];
    }
    pthread_mutex_unlock(&i2cMutex);
    return 0xFF;
}



// This function write 16 bit command to Slave
void i2c_write_u16_cmd(uint8_t device_addr, uint16_t reg)
{
    pthread_mutex_lock(&i2cMutex);
    uint8_t     txBuffer[2];

    txBuffer[0] = reg>>8;
    txBuffer[1] = reg&0xFF;

    i2cTransaction.slaveAddress = device_addr;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;
    if (I2C_transfer(i2c, &i2cTransaction)) {
        //temperature = (rxBuffer[0] << 6) | (rxBuffer[1] >> 2);
        pthread_mutex_unlock(&i2cMutex);
        return ;
    }
    else
    {
        pthread_mutex_unlock(&i2cMutex);
        return ;
    }
}

// This function read data buffer from Slave
uint8_t i2c_write_buf(uint8_t device_addr, uint8_t *txBuffer, uint32_t writeBufferLen)
{
    pthread_mutex_lock(&i2cMutex);

    i2cTransaction.slaveAddress = device_addr;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = writeBufferLen;
    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;
    if (I2C_transfer(i2c, &i2cTransaction)) {
        pthread_mutex_unlock(&i2cMutex);
        //Success
        return 0;
    }
    pthread_mutex_unlock(&i2cMutex);
    return 0xFF;
}

// This function read data buffer from Slave
uint8_t i2c_read_buf(uint8_t device_addr, uint8_t *rxBuffer, uint32_t readBufferLen)
{
    pthread_mutex_lock(&i2cMutex);

    i2cTransaction.slaveAddress = device_addr;
    i2cTransaction.writeBuf = NULL;
    i2cTransaction.writeCount = 0;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = readBufferLen;
    if (I2C_transfer(i2c, &i2cTransaction)) {
        pthread_mutex_unlock(&i2cMutex);
        //Success
        return 0;
    }
    pthread_mutex_unlock(&i2cMutex);
    return 0xFF;
}



