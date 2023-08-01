/*
 * Acclx.c
 *
 *  Created on: Apr. 14, 2023
 *      Author: Jimitp
 */
#include "Acclx.h"
#include "i2c.h"

typedef union U1
{
  uint8_t data[2];
  int16_t word;
} MY_CONV;
MY_CONV dataconver;

AccelData output;


void AccelInit(void)
{
	uint8_t cmd[1];
	cmd[0]=0x00;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_TEMP_CFG_REG,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x77;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_CTRL_REG1,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x00;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_CTRL_REG2,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x18;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_CTRL_REG3,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x00;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_CTRL_REG4,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x0A;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_CTRL_REG5,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x20;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_CTRL_REG6,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x00;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_REFERENCE_DATACAPTURE,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_FIFO_CTRL_REG,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_INT1_CFG,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_INT2_CFG,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
}

void AccelRead(void)
{

  uint8_t data[1];
  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_X_L,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
  dataconver.data[0]=data[0];
  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_X_H,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
  dataconver.data[1]=data[0];
  output.RawAccelX=dataconver.word;
  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_Y_L,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
  dataconver.data[0]=data[0];
  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_Y_H,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
  dataconver.data[1]=data[0];
  output.RawAccelY=dataconver.word;
  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_Z_L,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
  dataconver.data[0]=data[0];
  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_Z_H,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
  dataconver.data[1]=data[0];
  output.RawAccelZ=dataconver.word;
  output.AccelXScratch = ((float)(output.RawAccelX/64))*0.004f *100;
  output.AccelYScratch = ((float)(output.RawAccelY/64))*0.004f *100;
  output.AccelZScratch = ((float)(output.RawAccelZ/64))*0.004f *100;
  output.AccelX = (int16_t)output.AccelXScratch;
  output.AccelY = (int16_t)output.AccelYScratch;
  output.AccelZ = (int16_t)output.AccelZScratch;
  printf("Accelerometer X:%d Y:%d Z:%d \n",output.AccelX,output.AccelY,output.AccelZ);

}

