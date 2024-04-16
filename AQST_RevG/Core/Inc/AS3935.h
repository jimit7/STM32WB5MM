/*
 * AS3935.h
 *
 *  Created on: May 11, 2023
 *      Author: Jimitp
 */

#ifndef INC_AS3935_H_
#define INC_AS3935_H_
#include "i2c.h"


#define AS3935_IC_ADDRESS 0x00
#define AS3935_REG_GAIN  0x00
#define AS3935_REG_NOISE 0x01
#define AS3935_REG_STAT  0x02
#define AS3935_REG_INT   0x03
#define AS3935_REG_ELSB  0x04
#define AS3935_REG_EMSB  0x05
#define AS3935_REG_EMMSB 0x06
#define AS3935_REG_DIST  0x07
#define AS3935_REG_TUNE  0x08
#define LIGHTNING_INT   0x08
#define Device_address 0x00
#define DIV_mask 0x3F
#define OSC_mask 0x1F
#define CAP_mask 0xF0
#define Calib_mask 0x40
#define Indoor 0x12
#define Outdoor 0xE
#define Thrsld_mask 0xF0
#define Gain_mask 0xC1
#define AFE_Gain 0x00
#define Wdog_thrsld 0x01
#define Lightning_reg 0x02
#define Noise_mask 0x8F
#define Srej_mask 0xF0
#define Disturb_mask 0xDF
#define INT_mask 0xF
#define RESET_LIGHT 0x3C
#define Light_mask  0xCF
#define Distance_mask 0x3F
#define Wip_all 0x00
#define CALIB_TRCO = 0x3A;
#define CALIB_SRCO = 0x3B;
#define CALIB_RCO  0x3D
#define  DIRECT_COMMAND 0x96
#define PWD_mask 0xFE
void write_i2c_rw(uint8_t Wreg,uint8_t mask,uint8_t wbit,uint8_t position);
void AS3935_Init(void);
uint8_t AS3935_reading_distance(void);
#endif /* INC_AS3935_H_ */
