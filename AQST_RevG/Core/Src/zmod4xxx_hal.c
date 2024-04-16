/*******************************************************************************
 * Copyright (c) 2023 Renesas Electronics Corporation
 * All Rights Reserved.
 *
 * This code is proprietary to Renesas, and is license pursuant to the terms and
 * conditions that may be accessed at:
 * https://www.renesas.com/eu/en/document/msc/renesas-software-license-terms-gas-sensor-software
 *
 ******************************************************************************/

/**
 * @file    zmod4xxx_hal.c
 * @brief   zmod4xxx hardware abstraction layer function definitions
 * @version 2.6.0
 * @author Renesas Electronics Corporation
 */

#include <string.h>   // for memcpy
#include "i2c.h"

#include "zmod4xxx_hal.h"
#include "zmod4xxx_types.h"

void Delay(uint8_t ms)
{
	HAL_Delay(ms);
}

// wrapper function, mapping register read api to generic I2C API
 int8_t
_i2c_read_reg ( uint8_t  slaveAddr, uint8_t  addr, uint8_t*  data, uint8_t  len ) {

  if (HAL_I2C_Mem_Read(&hi2c3,slaveAddr, addr,I2C_MEMADD_SIZE_8BIT,data,len,HAL_MAX_DELAY))
    return ERROR_I2C;
  return ZMOD4XXX_OK;
}

// wrapper function, mapping register write api to generic I2C API
int8_t
_i2c_write_reg ( uint8_t  slaveAddr, uint8_t  addr, uint8_t*  data, uint8_t  len ) {

  if ( HAL_I2C_Mem_Write(&hi2c3, slaveAddr,addr,I2C_MEMADD_SIZE_8BIT,data,len,HAL_MAX_DELAY) )
    return ERROR_I2C;
  return ZMOD4XXX_OK;
}



int
init_hardware ( zmod4xxx_dev_t*  dev ) {
  // initialize the target hardware and populate functions in _hal object


  // populate function pointers in legacy ZMOD4xxx API
  dev -> write    = _i2c_write_reg;
  dev -> read     = _i2c_read_reg;
  dev -> delay_ms = Delay;

  dev -> delay_ms ( 200 );

  return 0;
}


int
deinit_hardware ( ) {
	return ZMOD4XXX_OK;
}
