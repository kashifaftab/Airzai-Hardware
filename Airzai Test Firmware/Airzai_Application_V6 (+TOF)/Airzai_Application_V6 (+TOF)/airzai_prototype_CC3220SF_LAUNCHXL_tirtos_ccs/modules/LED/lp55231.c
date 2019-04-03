/*
 * Copyright (c) 2014-2015, Texas Instruments Incorporated - http://www.ti.com
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

/*****************************************************************************
* INCLUDES
*/
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

#include <ti/drivers/GPIO.h>

#include "board.h"
#include "lp55231.h"

/*****************************************************************************
* DEFINES
*/
#define TIDA00757_D1_DEVICE		LP55231_DEVICE1_ADDR
#define TIDA00757_D1_RED		LP55231_REG_D7_PWM
#define TIDA00757_D1_GREEN		LP55231_REG_D1_PWM
#define TIDA00757_D1_BLUE		LP55231_REG_D2_PWM

#define TIDA00757_D2_DEVICE		LP55231_DEVICE1_ADDR
#define TIDA00757_D2_RED		LP55231_REG_D8_PWM
#define TIDA00757_D2_GREEN		LP55231_REG_D3_PWM
#define TIDA00757_D2_BLUE		LP55231_REG_D4_PWM

#define TIDA00757_D3_DEVICE		LP55231_DEVICE1_ADDR
#define TIDA00757_D3_RED		LP55231_REG_D9_PWM
#define TIDA00757_D3_GREEN		LP55231_REG_D5_PWM
#define TIDA00757_D3_BLUE		LP55231_REG_D6_PWM

#define TIDA00757_D4_DEVICE		LP55231_DEVICE2_ADDR
#define TIDA00757_D4_RED		LP55231_REG_D7_PWM
#define TIDA00757_D4_GREEN		LP55231_REG_D1_PWM
#define TIDA00757_D4_BLUE		LP55231_REG_D2_PWM

#define TIDA00757_D5_DEVICE		LP55231_DEVICE2_ADDR
#define TIDA00757_D5_RED		LP55231_REG_D8_PWM
#define TIDA00757_D5_GREEN		LP55231_REG_D3_PWM
#define TIDA00757_D5_BLUE		LP55231_REG_D4_PWM

#define TIDA00757_D6_DEVICE		LP55231_DEVICE2_ADDR
#define TIDA00757_D6_RED		LP55231_REG_D9_PWM
#define TIDA00757_D6_GREEN		LP55231_REG_D5_PWM
#define TIDA00757_D6_BLUE		LP55231_REG_D6_PWM

/*****************************************************************************
* LOCAL FUNCTION PROTOTYPES
*/

/*****************************************************************************
* LOCAL VARIABLES
*/

/*****************************************************************************
* FUNCTIONS
*/

/*****************************************************************************
* @brief    Enable pin
*
* @param    None
*
* @return   None
******************************************************************************/
void lp55231_enable_hw()
{
    /* Configure the LED pin */
    GPIO_setConfig(Board_GPIO_LP55231_EN, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn off GPIO PIN */
    GPIO_write(Board_GPIO_LP55231_EN, 0);
    usleep(10000);

    GPIO_write(Board_GPIO_LP55231_EN, 1);
    usleep(10000);
}


/*****************************************************************************
* @brief    Disable lp55231 chip
*
* @param    None
*
* @return   None
******************************************************************************/
void lp55231_disable_hw()
{
    /* Configure the LED pin */
    GPIO_setConfig(Board_GPIO_LP55231_EN, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn off GPIO PIN */
    GPIO_write(Board_GPIO_LP55231_EN, 0);
    usleep(10000);

}


/*****************************************************************************
* @brief    Initialize the LP55231 device
*
* @param  	device_addr		I2C device address
*
* @return   None
******************************************************************************/
void lp55231_init(uint8_t device_addr)
{
	/* Configure LP55231 */
	i2c_write(device_addr, LP55231_REG_MISC, LP55231_CP_AUTO | LP55231_AUTO_CLK | LP55231_PWR_SAVE);

	/* Enable all output */
	i2c_write(device_addr, LP55231_REG_OUTPUT_CTRL_MSB, 0x01);
	i2c_write(device_addr, LP55231_REG_OUTPUT_CTRL_LSB, 0xFF);
}

/*****************************************************************************
* @brief    Enable LP55231 device
*
* @param  	device_addr		I2C device address
*
* @return   None
******************************************************************************/
void lp55231_enable(uint8_t device_addr)
{
	// Reset the device to set it in known good state
    i2c_write(device_addr, LP55231_REG_RESET, LP55231_RESET);

	// Initialize device
	lp55231_init(device_addr);

	// Set CHIP_EN to high to enable device
	i2c_write(device_addr, LP55231_REG_ENABLE, LP55231_ENABLE);

	// Wait 500uS for the device to start up
	usleep(500);
//	usleep( 500);
}

/*****************************************************************************
* @brief    Disable LP55231 device
*
* @param  	device_addr		I2C device address
*
* @return   None
******************************************************************************/
void lp55231_disable(uint8_t device_addr)
{
	// Set CHIP_EN to low to enter standby
    i2c_write(device_addr, LP55231_REG_ENABLE, LP55231_DISABLE);
}

/*****************************************************************************
* @brief    Change the color and intensity of the specified LED
*
* @param  	led_id			TIDA-00757 LED ID (D1-D6)
* 			color			Color of LED light.  Valid options are:
* 							COLOR_OFF, COLOR_BLUE, COLOR_GREEN, COLOR_SKYBLUE
* 							COLOR_RED, COLOR_PURPLE, COLOR_YELLOW, COLOR_WHITE
* 			intensity		Intensity of light (0-255)
*
* @return   None
******************************************************************************/
void lp55231_led_control(uint8_t led_id, uint8_t color, uint8_t intensity)
{
	uint8_t rgb_array[3];
	switch(color)
	{
		case COLOR_OFF:
			rgb_array[0] = 0;
			rgb_array[1] = 0;
			rgb_array[2] = 0;
			break;
		case COLOR_BLUE:
			rgb_array[0] = 0;
			rgb_array[1] = 0;
			rgb_array[2] = intensity;
			break;
		case COLOR_GREEN:
			rgb_array[0] = 0;
			rgb_array[1] = intensity;
			rgb_array[2] = 0;
			break;
		case COLOR_SKYBLUE:
			rgb_array[0] = 0;
			rgb_array[1] = intensity;
			rgb_array[2] = intensity;
			break;
		case COLOR_RED:
			rgb_array[0] = intensity;
			rgb_array[1] = 0;
			rgb_array[2] = 0;
			break;
		case COLOR_PURPLE:
			rgb_array[0] = intensity;
			rgb_array[1] = 0;
			rgb_array[2] = intensity;
			break;
		case COLOR_YELLOW:
			rgb_array[0] = intensity;
			rgb_array[1] = intensity;
			rgb_array[2] = 0;
			break;
		case COLOR_WHITE:
			rgb_array[0] = intensity;
			rgb_array[1] = intensity;
			rgb_array[2] = intensity;
			break;
	}

	switch(led_id)
	{
		case TIDA00757_ALL:
		case TIDA00757_D1:
			i2c_write(TIDA00757_D1_DEVICE, TIDA00757_D1_RED,   rgb_array[0]);
			i2c_write(TIDA00757_D1_DEVICE, TIDA00757_D1_GREEN, rgb_array[1]);
			i2c_write(TIDA00757_D1_DEVICE, TIDA00757_D1_BLUE,  rgb_array[2]);
			if(led_id != TIDA00757_ALL) break;
		case TIDA00757_D2:
			i2c_write(TIDA00757_D2_DEVICE, TIDA00757_D2_RED,   rgb_array[0]);
			i2c_write(TIDA00757_D2_DEVICE, TIDA00757_D2_GREEN, rgb_array[1]);
			i2c_write(TIDA00757_D2_DEVICE, TIDA00757_D2_BLUE,  rgb_array[2]);
			if(led_id != TIDA00757_ALL) break;
		case TIDA00757_D3:
			i2c_write(TIDA00757_D3_DEVICE, TIDA00757_D3_RED,   rgb_array[0]);
			i2c_write(TIDA00757_D3_DEVICE, TIDA00757_D3_GREEN, rgb_array[1]);
			i2c_write(TIDA00757_D3_DEVICE, TIDA00757_D3_BLUE,  rgb_array[2]);
			if(led_id != TIDA00757_ALL) break;
		case TIDA00757_D4:
			i2c_write(TIDA00757_D4_DEVICE, TIDA00757_D4_RED,   rgb_array[0]);
			i2c_write(TIDA00757_D4_DEVICE, TIDA00757_D4_GREEN, rgb_array[1]);
			i2c_write(TIDA00757_D4_DEVICE, TIDA00757_D4_BLUE,  rgb_array[2]);
			if(led_id != TIDA00757_ALL) break;
		case TIDA00757_D5:
			i2c_write(TIDA00757_D5_DEVICE, TIDA00757_D5_RED,   rgb_array[0]);
			i2c_write(TIDA00757_D5_DEVICE, TIDA00757_D5_GREEN, rgb_array[1]);
			i2c_write(TIDA00757_D5_DEVICE, TIDA00757_D5_BLUE,  rgb_array[2]);
			if(led_id != TIDA00757_ALL) break;
		case TIDA00757_D6:
			i2c_write(TIDA00757_D6_DEVICE, TIDA00757_D6_RED,   rgb_array[0]);
			i2c_write(TIDA00757_D6_DEVICE, TIDA00757_D6_GREEN, rgb_array[1]);
			i2c_write(TIDA00757_D6_DEVICE, TIDA00757_D6_BLUE,  rgb_array[2]);
			if(led_id != TIDA00757_ALL) break;
	}
}

/*****************************************************************************
* @brief    nFault light scheme
* 			Yellow pulsing on all LEDs
*
* @return   None
******************************************************************************/
void light_scheme_nFault()
{
	int i;

	lp55231_enable(LP55231_DEVICE1_ADDR);
	lp55231_enable(LP55231_DEVICE2_ADDR);

	for(i=0; i<3; i++)
	{
		lp55231_led_control(TIDA00757_ALL, COLOR_YELLOW, 31);
        usleep(100000);
		lp55231_led_control(TIDA00757_ALL, COLOR_YELLOW, 82);
		usleep( 100000);
		lp55231_led_control(TIDA00757_ALL, COLOR_YELLOW, 133);
		usleep( 100000);
		lp55231_led_control(TIDA00757_ALL, COLOR_YELLOW, 184);
		usleep( 100000);
		lp55231_led_control(TIDA00757_ALL, COLOR_YELLOW, 255);
		usleep( 100000);
		lp55231_led_control(TIDA00757_ALL, COLOR_YELLOW, 184);
		usleep( 100000);
		lp55231_led_control(TIDA00757_ALL, COLOR_YELLOW, 133);
		usleep( 100000);
		lp55231_led_control(TIDA00757_ALL, COLOR_YELLOW, 82);
		usleep( 100000);
		lp55231_led_control(TIDA00757_ALL, COLOR_YELLOW, 31);
		usleep( 100000);
		lp55231_led_control(TIDA00757_ALL, COLOR_YELLOW, 0);
		usleep( 100000);
	}

	lp55231_disable(LP55231_DEVICE1_ADDR);
	lp55231_disable(LP55231_DEVICE2_ADDR);
}

/*****************************************************************************
* @brief    Power on light scheme
* 			Flash LEDs once
*
* @return   None
******************************************************************************/
void light_scheme_power_on()
{
	lp55231_enable(LP55231_DEVICE1_ADDR);
	lp55231_enable(LP55231_DEVICE2_ADDR);

	lp55231_led_control(TIDA00757_ALL, COLOR_PURPLE, 255);
	usleep( 500000);
	lp55231_led_control(TIDA00757_ALL, COLOR_PURPLE, 0);

	lp55231_disable(LP55231_DEVICE1_ADDR);
	lp55231_disable(LP55231_DEVICE2_ADDR);
}


/*****************************************************************************
* @brief    Low battery light scheme
* 			Red blinking on all LEDs
*
* @return   None
******************************************************************************/
void light_scheme_low_battery()
{
	int i;

	lp55231_enable(LP55231_DEVICE1_ADDR);
	lp55231_enable(LP55231_DEVICE2_ADDR);

	for(i=0; i<6; i++)
	{
		lp55231_led_control(TIDA00757_ALL, COLOR_RED, 255);
		usleep( 250000);
		lp55231_led_control(TIDA00757_ALL, COLOR_RED, 0);
		usleep( 250000);
	}

	lp55231_disable(LP55231_DEVICE1_ADDR);
	lp55231_disable(LP55231_DEVICE2_ADDR);
}

/*****************************************************************************
* @brief    Unlock light scheme (lite)
* 			In clockwise pattern, fade in green one by one
*
* @return   None
******************************************************************************/
void light_scheme_unlock_lite()
{
	uint16_t timedelay = 50000;
	uint8_t  i;

	lp55231_enable(LP55231_DEVICE1_ADDR);
	lp55231_enable(LP55231_DEVICE2_ADDR);

	lp55231_led_control(TIDA00757_D1, COLOR_GREEN, 15);
	usleep( timedelay/2);
	lp55231_led_control(TIDA00757_D1, COLOR_GREEN, 35);
	usleep( timedelay/2);
	lp55231_led_control(TIDA00757_D1, COLOR_GREEN, 55);
	usleep( timedelay/2);
	lp55231_led_control(TIDA00757_D1, COLOR_GREEN, 75);
	usleep( timedelay/2);
	lp55231_led_control(TIDA00757_D1, COLOR_GREEN, 100);
	usleep( timedelay/2);

	for (i=TIDA00757_D1; i<=TIDA00757_D6; i++)
	{
		uint8_t led_next;
		if(i == TIDA00757_D6)
			led_next = TIDA00757_D1;
		else led_next = i+1;

		lp55231_led_control(i, COLOR_GREEN, 75);
		lp55231_led_control(led_next, COLOR_GREEN, 15);
		usleep( timedelay);
		lp55231_led_control(i, COLOR_GREEN, 55);
		lp55231_led_control(led_next, COLOR_GREEN, 35);
		usleep( timedelay);
		lp55231_led_control(i, COLOR_GREEN, 35);
		lp55231_led_control(led_next, COLOR_GREEN, 55);
		usleep( timedelay);
		lp55231_led_control(i, COLOR_GREEN, 15);
		lp55231_led_control(led_next, COLOR_GREEN, 75);
		usleep( timedelay);
		lp55231_led_control(i, COLOR_GREEN, 0);
		lp55231_led_control(led_next, COLOR_GREEN, 100);
		usleep( timedelay);
	}

	usleep( 250000);
	lp55231_led_control(TIDA00757_ALL, COLOR_OFF, 0);

	lp55231_disable(LP55231_DEVICE1_ADDR);
	lp55231_disable(LP55231_DEVICE2_ADDR);
}

/*****************************************************************************
* @brief    Lock light scheme (lite)
* 			In counter clockwise pattern, fade in blue one by one
*
* @return   None
******************************************************************************/
void light_scheme_lock_lite()
{
	uint16_t timedelay = 50000;
	uint8_t  i;

	lp55231_enable(LP55231_DEVICE1_ADDR);
	lp55231_enable(LP55231_DEVICE2_ADDR);

	//31, 82, 133, 184, 255

	lp55231_led_control(TIDA00757_D1, COLOR_BLUE, 25);
	usleep( timedelay/2);
	lp55231_led_control(TIDA00757_D1, COLOR_BLUE, 50);
	usleep( timedelay/2);
	lp55231_led_control(TIDA00757_D1, COLOR_BLUE, 75);
	usleep( timedelay/2);
	lp55231_led_control(TIDA00757_D1, COLOR_BLUE, 100);
	usleep( timedelay/2);
	lp55231_led_control(TIDA00757_D1, COLOR_BLUE, 200);
	usleep( timedelay/2);

	for (i=TIDA00757_D6; i>=TIDA00757_D1; i--)
	{
		uint8_t led_next;
		if(i == TIDA00757_D6)
			led_next = TIDA00757_D1;
		else led_next = i+1;

		lp55231_led_control(led_next, COLOR_BLUE, 100);
		lp55231_led_control(i, COLOR_BLUE, 25);
		usleep( timedelay);
		lp55231_led_control(led_next, COLOR_BLUE, 75);
		lp55231_led_control(i, COLOR_BLUE, 50);
		usleep( timedelay);
		lp55231_led_control(led_next, COLOR_BLUE, 50);
		lp55231_led_control(i, COLOR_BLUE, 75);
		usleep( timedelay);
		lp55231_led_control(led_next, COLOR_BLUE, 25);
		lp55231_led_control(i, COLOR_BLUE, 100);
		usleep( timedelay);
		lp55231_led_control(led_next, COLOR_BLUE, 0);
		lp55231_led_control(i, COLOR_BLUE, 200);
		usleep( timedelay);
	}

	usleep( 250000);
	lp55231_led_control(TIDA00757_ALL, COLOR_OFF, 0);

	lp55231_disable(LP55231_DEVICE1_ADDR);
	lp55231_disable(LP55231_DEVICE2_ADDR);
}


/*****************************************************************************
* @brief    Unlock light scheme
* 			In clockwise pattern, fade in green one by one
*
* @return   None
******************************************************************************/
void light_scheme_unlock()
{
	uint8_t i;

	lp55231_enable(LP55231_DEVICE1_ADDR);
	lp55231_enable(LP55231_DEVICE2_ADDR);

	for (i=TIDA00757_D1; i<=TIDA00757_D6; i++)
	{
		lp55231_led_control(i, COLOR_GREEN, 31);
		usleep( 100000);
		lp55231_led_control(i, COLOR_GREEN, 82);
		usleep( 100000);
		lp55231_led_control(i, COLOR_GREEN, 133);
		usleep( 100000);
		lp55231_led_control(i, COLOR_GREEN, 184);
		usleep( 100000);
		lp55231_led_control(i, COLOR_GREEN, 255);
		usleep( 100000);
	}

	usleep( 500000);
	lp55231_led_control(TIDA00757_ALL, COLOR_OFF, 0);

	lp55231_disable(LP55231_DEVICE1_ADDR);
	lp55231_disable(LP55231_DEVICE2_ADDR);
}

/*****************************************************************************
* @brief    Lock light scheme
* 			In counter clockwise pattern, fade in blue one by one
*
* @return   None
******************************************************************************/
void light_scheme_lock()
{
	uint8_t i;

	lp55231_enable(LP55231_DEVICE1_ADDR);
	lp55231_enable(LP55231_DEVICE2_ADDR);

	lp55231_led_control(TIDA00757_D1, COLOR_BLUE, 31);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D1, COLOR_BLUE, 82);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D1, COLOR_BLUE, 133);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D1, COLOR_BLUE, 184);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D1, COLOR_BLUE, 255);
	usleep( 100000);

	for (i=TIDA00757_D6; i>=TIDA00757_D2; i--)
	{
		lp55231_led_control(i, COLOR_BLUE, 31);
		usleep( 100000);
		lp55231_led_control(i, COLOR_BLUE, 82);
		usleep( 100000);
		lp55231_led_control(i, COLOR_BLUE, 133);
		usleep( 100000);
		lp55231_led_control(i, COLOR_BLUE, 184);
		usleep( 100000);
		lp55231_led_control(i, COLOR_BLUE, 255);
		usleep( 100000);
	}

	usleep( 500000);
	lp55231_led_control(TIDA00757_ALL, COLOR_OFF, 0);

	lp55231_disable(LP55231_DEVICE1_ADDR);
	lp55231_disable(LP55231_DEVICE2_ADDR);
}

/*****************************************************************************
* @brief    Light show light scheme
* 			Change the LEDs to various colors
*
* @return   None
******************************************************************************/
void light_scheme_light_show()
{
	lp55231_enable(LP55231_DEVICE1_ADDR);
	lp55231_enable(LP55231_DEVICE2_ADDR);

	lp55231_led_control(TIDA00757_D1, COLOR_BLUE, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D2, COLOR_BLUE, 255);
	lp55231_led_control(TIDA00757_D6, COLOR_BLUE, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D3, COLOR_BLUE, 255);
	lp55231_led_control(TIDA00757_D5, COLOR_BLUE, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D4, COLOR_BLUE, 255);
	usleep( 150000);

	lp55231_led_control(TIDA00757_D4, COLOR_GREEN, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D3, COLOR_GREEN, 255);
	lp55231_led_control(TIDA00757_D5, COLOR_GREEN, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D2, COLOR_GREEN, 255);
	lp55231_led_control(TIDA00757_D6, COLOR_GREEN, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D1, COLOR_GREEN, 255);
	usleep( 150000);

	lp55231_led_control(TIDA00757_D1, COLOR_PURPLE, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D2, COLOR_PURPLE, 255);
	lp55231_led_control(TIDA00757_D6, COLOR_PURPLE, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D3, COLOR_PURPLE, 255);
	lp55231_led_control(TIDA00757_D5, COLOR_PURPLE, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D4, COLOR_PURPLE, 255);
	usleep( 150000);

	lp55231_led_control(TIDA00757_D4, COLOR_YELLOW, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D3, COLOR_YELLOW, 255);
	lp55231_led_control(TIDA00757_D5, COLOR_YELLOW, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D2, COLOR_YELLOW, 255);
	lp55231_led_control(TIDA00757_D6, COLOR_YELLOW, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D1, COLOR_YELLOW, 255);
	usleep( 150000);

	lp55231_led_control(TIDA00757_D1, COLOR_SKYBLUE, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D2, COLOR_SKYBLUE, 255);
	lp55231_led_control(TIDA00757_D6, COLOR_SKYBLUE, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D3, COLOR_SKYBLUE, 255);
	lp55231_led_control(TIDA00757_D5, COLOR_SKYBLUE, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D4, COLOR_SKYBLUE, 255);
	usleep( 150000);

	lp55231_led_control(TIDA00757_D4, COLOR_RED, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D3, COLOR_RED, 255);
	lp55231_led_control(TIDA00757_D5, COLOR_RED, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D2, COLOR_RED, 255);
	lp55231_led_control(TIDA00757_D6, COLOR_RED, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D1, COLOR_RED, 255);
	usleep( 150000);

	lp55231_led_control(TIDA00757_D1, COLOR_WHITE, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D2, COLOR_WHITE, 255);
	lp55231_led_control(TIDA00757_D6, COLOR_WHITE, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D3, COLOR_WHITE, 255);
	lp55231_led_control(TIDA00757_D5, COLOR_WHITE, 255);
	usleep( 100000);
	lp55231_led_control(TIDA00757_D4, COLOR_WHITE, 255);

	usleep( 500000);
	lp55231_led_control(TIDA00757_ALL, COLOR_OFF, 255);

	lp55231_disable(LP55231_DEVICE1_ADDR);
	lp55231_disable(LP55231_DEVICE2_ADDR);
}
