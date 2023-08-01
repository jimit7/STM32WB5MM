/*
 * UV.c
 *
 *  Created on: Mar. 23, 2023
 *      Author: Jimitp
 */

#include "UV.h"
#include "i2c.h"




void UV_init(void)
{
 uint8_t cmd[1]={0};
 cmd[0]=0x0A;

 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,LTR390_MAIN_CTRL,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
 cmd[0]=0x42;
 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,LTR390_MEAS_RATE,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
 cmd[0]=0x01;
 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,LTR390_GAIN,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
 cmd[0]=0x0A;
 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,LTR390_INT_PST,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
 cmd[0]=0x64;
 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,UVS_ALS_thr_low_l,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
 cmd[0]=0x00;
 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,UVS_ALS_thr_low_m,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
 //bus_write(LTR390_WR,UVS_ALS_thr_low_h,0x00);
 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,UVS_ALS_thr_low_h,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
 cmd[0]=0x3E;
 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,UVS_ALS_thr_up_l,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
 cmd[0]=0x08;
 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,UVS_ALS_thr_up_m,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
 cmd[0]=0x00;
 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,UVS_ALS_thr_up_h,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
 cmd[0]=0x34;
 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,LTR390_INT_CFG,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
}






int8_t *UV(void)
{
  static int8_t data_little_Endian[3];
  uint8_t data_Big_Endian[3];
  HAL_I2C_Mem_Read(&hi2c3,LTR390_RD,LTR390_UVSDATA,I2C_MEMADD_SIZE_8BIT,data_Big_Endian,3,HAL_MAX_DELAY);
  printf("uv_raw %d %d %d \n",data_Big_Endian[0],data_Big_Endian[1],data_Big_Endian[2]);
  data_little_Endian[0]=data_Big_Endian[2];
  data_little_Endian[1]=data_Big_Endian[1];
  data_little_Endian[2]=data_Big_Endian[0];
  printf("uv_raw %d  \n",((data_Big_Endian[2]*65536) + (data_Big_Endian[1]*256) +data_Big_Endian[0]));
  return data_little_Endian;

}
uint8_t *ALS(void)
{
  static int data[3]={0,0,0};
//  bus_read(LTR390_RD,LTR390_ALSDATA,data,3);
  return data;

}
