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
/******************************************************************************/

#ifndef _LP55231_H_
#define _LP55231_H_

#include "i2c_api.h"

/*****************************************************************************
* DEFINES
*/

/* TIDA-00757 Board specific LEDs */
#define TIDA00757_D1	1
#define TIDA00757_D2	2
#define TIDA00757_D3	3
#define TIDA00757_D4	4
#define TIDA00757_D5	5
#define TIDA00757_D6	6
#define TIDA00757_ALL	7

/* Colors */
#define COLOR_OFF		0
#define COLOR_BLUE		1
#define COLOR_GREEN		2
#define COLOR_SKYBLUE	3
#define COLOR_RED		4
#define COLOR_PURPLE	5
#define COLOR_YELLOW	6
#define COLOR_WHITE		7

/* Address */
#define LP55231_DEVICE1_ADDR			0x32
#define LP55231_DEVICE2_ADDR			0x33

/* Registers */
#define LP55231_REG_ENABLE				0x00
#define LP55231_REG_OUTPUT_CTRL_MSB		0x04
#define LP55231_REG_OUTPUT_CTRL_LSB		0x05
#define LP55231_REG_D1_CTRL				0x06
#define LP55231_REG_D2_CTRL				0x07
#define LP55231_REG_D3_CTRL				0x08
#define LP55231_REG_D4_CTRL				0x09
#define LP55231_REG_D5_CTRL				0x0A
#define LP55231_REG_D6_CTRL				0x0B
#define LP55231_REG_D7_CTRL				0x0C
#define LP55231_REG_D8_CTRL				0x0D
#define LP55231_REG_D9_CTRL				0x0E
#define LP55231_REG_D1_PWM				0x16
#define LP55231_REG_D2_PWM				0x17
#define LP55231_REG_D3_PWM				0x18
#define LP55231_REG_D4_PWM				0x19
#define LP55231_REG_D5_PWM				0x1A
#define LP55231_REG_D6_PWM				0x1B
#define LP55231_REG_D7_PWM				0x1C
#define LP55231_REG_D8_PWM				0x1D
#define LP55231_REG_D9_PWM				0x1E
#define LP55231_REG_D1_CURRENT_CTRL		0x26
#define LP55231_REG_D2_CURRENT_CTRL		0x27
#define LP55231_REG_D3_CURRENT_CTRL		0x28
#define LP55231_REG_D4_CURRENT_CTRL		0x29
#define LP55231_REG_D5_CURRENT_CTRL		0x2A
#define LP55231_REG_D6_CURRENT_CTRL		0x2B
#define LP55231_REG_D7_CURRENT_CTRL		0x2C
#define LP55231_REG_D8_CURRENT_CTRL		0x2D
#define LP55231_REG_D9_CURRENT_CTRL		0x2E
#define LP55231_REG_MISC				0x36
#define LP55231_REG_STATUS				0x3A
#define LP55231_REG_RESET				0x3D
#define LP55231_REG_MASTER_FADER1		0x48
#define LP55231_REG_MASTER_FADER2		0x49
#define LP55231_REG_MASTER_FADER3		0x4A

/* Bit description in registers */
#define LP55231_ENABLE					0x40
#define LP55231_DISABLE					0x00
#define LP55231_PWR_SAVE				0x20
#define LP55231_PWM_PWR_SAVE			0x04
#define LP55231_CP_AUTO					0x18
#define LP55231_AUTO_CLK				0x02

#define LP55231_RESET					0xFF
#define LP55231_FADER_MAPPING_MASK		0xC0
#define LP55231_FADER_MAPPING_SHIFT		6


/*****************************************************************************
* GLOBAL FUNCTIONS
*/
void lp55231_enable_hw(void);
void lp55231_disable_hw(void);
void lp55231_enable(uint8_t device_addr);
void lp55231_disable(uint8_t device_addr);
void lp55231_led_control(uint8_t led_id, uint8_t color, uint8_t intensity);

void light_scheme_nFault();
void light_scheme_unlock();
void light_scheme_unlock_lite();
void light_scheme_lock();
void light_scheme_lock_lite();
void light_scheme_low_battery();
void light_scheme_light_show();
void light_scheme_power_on();

#endif /* TIDA00757_FUNCTEST_LP55231_H_ */
