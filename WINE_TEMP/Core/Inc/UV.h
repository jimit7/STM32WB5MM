/*
 * UV.h
 *
 *  Created on: Mar. 23, 2023
 *      Author: Jimitp
 */

#ifndef UV_H_
#define UV_H_

//#include <stddef.h>
//#include "sl_i2cspm_instances.h"
#include <stdint.h>

#define LTR390_I2CADDR_DEFAULT 0x53 ///< I2C address
#define LTR390_MAIN_CTRL 0x00       ///< Main control register
#define LTR390_MEAS_RATE 0x04       ///< Resolution and data rate
#define LTR390_GAIN 0x05            ///< ALS and UVS gain range
#define LTR390_PART_ID 0x06         ///< Part id/revision register
#define LTR390_MAIN_STATUS 0x07     ///< Main status register
#define LTR390_ALSDATA 0x0D         ///< ALS data lowest byte
#define LTR390_UVSDATA 0x10         ///< UVS data lowest byte
#define LTR390_INT_CFG 0x19         ///< Interrupt configuration
#define LTR390_INT_PST 0x1A         ///< Interrupt persistance config
#define LTR390_THRESH_UP 0x21       ///< Upper threshold, low byte
#define LTR390_THRESH_LOW 0x24      ///< Lower threshold, low byte
#define LTR390_RD 0xA7
#define LTR390_WR 0xA6
#define UVS_ALS_thr_up_l 0x21
#define UVS_ALS_thr_up_m 0x22
#define UVS_ALS_thr_up_h 0x23
#define UVS_ALS_thr_low_l 0x24
#define UVS_ALS_thr_low_m 0x25
#define UVS_ALS_thr_low_h 0x26

typedef union Two_byte
{
  uint8_t data[2];
  int16_t word;
};
extern float f_UVI;
void Set_thresold(uint32_t low,uint32_t high);
void config_interrupt(void);
void UV_init(void);
uint8_t *ALS(void);
uint8_t UV(void);
void f_UV(void);

#endif /* UV_H_ */
