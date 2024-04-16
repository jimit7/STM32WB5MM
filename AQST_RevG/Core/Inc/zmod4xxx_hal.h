/*
 * zmod4xxx_hal.h
 *
 *  Created on: Dec 18, 2023
 *      Author: Jimitp
 */

#ifndef INC_ZMOD4XXX_HAL_H_
#define INC_ZMOD4XXX_HAL_H_

#include "zmod4xxx_types.h"
int init_hardware (zmod4xxx_dev_t*  dev );
void Delay(uint8_t ms);
 int8_t _i2c_read_reg ( uint8_t  slaveAddr, uint8_t  addr, uint8_t*  data, uint8_t  len );
 int8_t _i2c_write_reg ( uint8_t  slaveAddr, uint8_t  addr, uint8_t*  data, uint8_t  len );

int deinit_hardware ( );


#endif /* INC_ZMOD4XXX_HAL_H_ */
