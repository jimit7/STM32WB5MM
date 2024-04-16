/*
 * LPS25hb.c
 *
 *  Created on: Mar 22, 2024
 *      Author: Jimitp
 */
#include "LPS25hb.h"
#include "string.h"
#include "i2c.h"
void LPS25HB_INIT(void)
{
	uint8_t cmd  =0x04;
	HAL_I2C_Mem_Write(&hi2c3,LPS25HB_I2C_ADD_L, LPS25HB_CTRL2 ,I2C_MEMADD_SIZE_8BIT, &cmd,1,1000);
	HAL_Delay(10);
	cmd = 0x84;
	HAL_I2C_Mem_Write(&hi2c3,LPS25HB_I2C_ADD_L, LPS25HB_CTRL1 ,I2C_MEMADD_SIZE_8BIT, &cmd,1,1000);
}

uint8_t LPS25HB_GET_DATA(float *data)
{
	uint8_t reg,cmd  =0x01;
	uint8_t Raw_data[3];
	int32_t temp;
	HAL_I2C_Mem_Write(&hi2c3,LPS25HB_I2C_ADD_L, LPS25HB_CTRL2 ,I2C_MEMADD_SIZE_8BIT, &cmd,1,1000);
	HAL_I2C_Mem_Read(&hi2c3, LPS25HB_I2C_ADD_H, LPS25HB_STATUS,I2C_MEMADD_SIZE_8BIT,&reg, 1,1000);
	if(reg==0x03||0x33)
	{
		HAL_I2C_Mem_Read(&hi2c3,LPS25HB_I2C_ADD_H, LPS25HB_PRESS_OUT_XL,I2C_MEMADD_SIZE_8BIT,&Raw_data[0],1,1000);
		HAL_I2C_Mem_Read(&hi2c3,LPS25HB_I2C_ADD_H, LPS25HB_PRESS_OUT_L,I2C_MEMADD_SIZE_8BIT,&Raw_data[1], 1,1000);
		HAL_I2C_Mem_Read(&hi2c3,LPS25HB_I2C_ADD_H, LPS25HB_PRESS_OUT_H,I2C_MEMADD_SIZE_8BIT,&Raw_data[2], 1,1000);
		if(Raw_data[2]>>7 == 0x01){
			return 0x01;
		}
		//printf("data %x\n\r",((Raw_data[2]<<16)|(Raw_data[1]<<8)|(Raw_data[0])));
		temp = ((Raw_data[2]<<16)|(Raw_data[1]<<8)|(Raw_data[0]));
		data[0]=(float)temp/(float)4096.0;
		//printf("pressure %f \n",data[0]);
		memset(&Raw_data,0x00,sizeof(int32_t));
		temp=0x00;
		HAL_I2C_Mem_Read(&hi2c3,LPS25HB_I2C_ADD_H, LPS25HB_TEMP_OUT_L,I2C_MEMADD_SIZE_8BIT,&Raw_data[0],1,1000);
		HAL_I2C_Mem_Read(&hi2c3,LPS25HB_I2C_ADD_H, LPS25HB_TEMP_OUT_H,I2C_MEMADD_SIZE_8BIT,&Raw_data[1], 1,1000);
	    temp= ((Raw_data[1]<<8)|(Raw_data[0]));
	    if(Raw_data[1]>>7 == 0x01)
	    {
	    	 //printf("temp %x \n",temp);
	    	temp = ((~temp)&0x0000ffff) + 1;
	    	// printf("temp %d \n",temp);
	    }
	    data[1]= (float)((float)temp/(float)480) + 42.5;
//	    printf("temperature %f \n",data[1]);
	}
	else
	{
		return 0x02;
	}
 return 0;
}
