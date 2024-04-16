/*
 * LPS25hb.h
 *
 *  Created on: Mar 22, 2024
 *      Author: Jimitp
 */

#ifndef INC_LPS25HB_H_
#define INC_LPS25HB_H_

#include "i2c.h"
#define LPS25HB_I2C_ADD_L    0xB8U
#define LPS25HB_I2C_ADD_H    0xB9U

/** Device Identification (Who am I) **/
#define LPS25HB_ID             0xBDU

/**
  * @}
  *
  */

#define LPS25HB_REF_P_XL        0x08U
#define LPS25HB_REF_P_L         0x09U
#define LPS25HB_REF_P_H         0x0AU
#define LPS25HB_WHO_AM_I        0x0FU
#define LPS25HB_RES_CONF        0x10U
#define LPS25HB_CTRL1           0X20U
#define LPS25HB_CTRL2           0x21U
#define LPS25HB_STATUS          0x27U
#define LPS25HB_PRESS_OUT_XL    0x28U
#define LPS25HB_PRESS_OUT_L     0x29U
#define LPS25HB_PRESS_OUT_H     0x2AU
#define LPS25HB_TEMP_OUT_L      0x2BU
#define LPS25HB_TEMP_OUT_H      0x2CU

void LPS25HB_INIT(void);
uint8_t LPS25HB_GET_DATA(float *data);
#endif /* INC_LPS25HB_H_ */
