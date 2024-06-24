/*
 * mlx90632_depends.c
 *
 *  Created on: Apr. 10, 2023
 *      Author: Jimitp
 */
#include "mlx90632_depends.h"
#include "i2c.h"




int32_t mlx90632_i2c_read(int16_t register_address, uint16_t *value)
{
  uint8_t data[2];
  int32_t ret;
  ret =  HAL_I2C_Mem_Read(&hi2c3,Mlx90632_address,register_address,I2C_MEMADD_SIZE_16BIT,data,2,HAL_MAX_DELAY); //i2c_read(register_address,data,2);
    //Endianness
  *value = data[1]|(data[0]<<8);
//  //printf("%dread: %x %x=%x \n", ret,data[0],data[1],*value);
  return ret;

}

/* Implementation of I2C read for 32-bit values */
int32_t mlx90632_i2c_read32(int16_t register_address,uint32_t *value)
{
  uint8_t data[4];
  int32_t ret;
  ret =  HAL_I2C_Mem_Read(&hi2c3,Mlx90632_address,register_address,I2C_MEMADD_SIZE_16BIT,data,4,HAL_MAX_DELAY); //i2c_read(register_address,data,2);
  //Endianness
  *value = data[2]<<24|data[3]<<16|data[0]<<8|data[1];
//  *value= data[0]<<24|data[1]<<16|data[2]<<8|data[3];
  //printf("val32 %x %x %x %x %x\n",data[0],data[1],data[2],data[3],*value);
  return ret;
}

int32_t mlx90632_i2c_write(int16_t register_address, uint16_t value) {
  int32_t ret;
  uint8_t data[2];
  data[0] = value >> 8;
  data[1] = value;
  return ret =  HAL_I2C_Mem_Write(&hi2c3,Mlx90632_address,register_address,I2C_MEMADD_SIZE_16BIT,data,2,HAL_MAX_DELAY); //i2c_read(register_address,data,2);
}

void usleep(int min_range, int max_range)
{
 int a= min_range/1000;
	HAL_Delay(a);
//	 __HAL_TIM_SET_COUNTER(&htim, 0);
//	    while (__HAL_TIM_GET_COUNTER(&htim2) < min_range);
}

void msleep(int msecs)
{
	HAL_Delay(msecs);
}

