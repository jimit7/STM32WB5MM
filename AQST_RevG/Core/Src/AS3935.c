/*
 * AS3935.c
 *
 *  Created on: May 11, 2023
 *      Author: Jimitp
 */
#include "AS3935.h"

void write_i2c_rw(uint8_t Wreg,uint8_t mask,uint8_t wbit,uint8_t position)
{
	uint8_t reg_data_ptr;
//	HAL_I2C_Mem_Write(&hi2c1,AS3935_IC_ADDRESS,Wreg,I2C_MEMADD_SIZE_8BIT,reg_data_ptr,1,HAL_MAX_DELAY);
	HAL_I2C_Mem_Read(&hi2c3,AS3935_IC_ADDRESS,Wreg,I2C_MEMADD_SIZE_8BIT,&reg_data_ptr,1,HAL_MAX_DELAY);
	reg_data_ptr &=mask;
	reg_data_ptr |=(wbit<<position);
	HAL_I2C_Mem_Write(&hi2c3,AS3935_IC_ADDRESS,Wreg,I2C_MEMADD_SIZE_8BIT,&reg_data_ptr,1,HAL_MAX_DELAY);
}
void AS3935_Init(void)
{
	write_i2c_rw(RESET_LIGHT,Wip_all, DIRECT_COMMAND, 0);  //preset to default setting
	  write_i2c_rw(AS3935_REG_TUNE, OSC_mask,0,7);   //LCO route to intr pin 0-no/1-yes
	  write_i2c_rw(AS3935_REG_TUNE, CAP_mask,0,0);   // internal cap value =7 (0-15) value vary board to board
	  write_i2c_rw(CALIB_RCO, Wip_all, DIRECT_COMMAND, 0); //calibrate RCO
	  write_i2c_rw(AS3935_REG_TUNE, OSC_mask,1,6);
	  HAL_Delay(2);             // calibration
	  write_i2c_rw(AS3935_REG_TUNE, OSC_mask,0, 6);
	  write_i2c_rw(AFE_Gain,Gain_mask,Outdoor,1);//  Indoor/Outdoor

	  write_i2c_rw(Wdog_thrsld,Thrsld_mask,1,0);//watch_dog_thersold =4
	  write_i2c_rw(Wdog_thrsld,Noise_mask,1,4);// Noise=2
	  write_i2c_rw(Lightning_reg,Srej_mask,1,0);//spike_rejection= 2
	  write_i2c_rw(Lightning_reg,Light_mask,0,4); // limit no of lightning strike to issue a interrupt =1
	  write_i2c_rw(AS3935_REG_INT,Disturb_mask,1,5);    //enable 1/ disable 0




  write_i2c_rw(RESET_LIGHT,Wip_all, DIRECT_COMMAND, 0);  //preset to default setting
  write_i2c_rw(AFE_Gain,PWD_mask,1,0);
  HAL_Delay(1000);
  write_i2c_rw(AFE_Gain,PWD_mask,0,0);
  write_i2c_rw(AS3935_REG_TUNE, OSC_mask,0,7);   //LCO route to intr pin 0-no/1-yes
  write_i2c_rw(AS3935_REG_TUNE, CAP_mask,7,0);
  write_i2c_rw(CALIB_RCO, Wip_all, DIRECT_COMMAND, 0); //calibrate RCO
  write_i2c_rw(AS3935_REG_TUNE, OSC_mask,1,6);
  HAL_Delay(2);             // calibration
  write_i2c_rw(AS3935_REG_TUNE, OSC_mask,0, 6);
  write_i2c_rw(AFE_Gain,Gain_mask,Outdoor,1);//  Indoor/Outdoor
//  write_i2c_rw(AS3935_REG_TUNE, OSC_mask,0,7);   //LCO route to intr pin 0-no/1-yes
//  write_i2c_rw(AS3935_REG_TUNE, CAP_mask,7,0);   // internal cap value =7 (0-15) value vary board to board
  write_i2c_rw(Wdog_thrsld,Thrsld_mask,2,0);//watch_dog_thersold =4
  write_i2c_rw(Wdog_thrsld,Noise_mask,1,4);// Noise=2
  write_i2c_rw(Lightning_reg,Srej_mask,1,0);//spike_rejection= 2
  write_i2c_rw(Lightning_reg,Light_mask,1,4); // limit no of lightning strike to issue a interrupt =1
  write_i2c_rw(AS3935_REG_INT,Disturb_mask,1,5);    //enable 1/ disable 0

}

uint8_t AS3935_reading_distance(void)
{
	uint8_t Intr_lightning, lightning=0xff;

	HAL_I2C_Mem_Read(&hi2c3,AS3935_IC_ADDRESS,AS3935_REG_INT,I2C_MEMADD_SIZE_8BIT,&Intr_lightning,1,HAL_MAX_DELAY);
	 printf("lightining_intr: %d \n",Intr_lightning);
	 if((Intr_lightning & 0x08)==0x08)
	    {
		 HAL_I2C_Mem_Read(&hi2c3,AS3935_IC_ADDRESS,AS3935_REG_DIST,I2C_MEMADD_SIZE_8BIT,&lightning,1,HAL_MAX_DELAY);
		 printf("lightining_data: %d \n",lightning);
		 lightning &=Distance_mask;
	    }
	 else if((Intr_lightning & 0x04)==0x04)
	           {
	             printf("Disturber Detected"); //debug
	           }
	         else if((Intr_lightning&0x01)==0x01)
	           {
	             printf("\nNoise level to high\n");//debug
	           }
	 return lightning;

}
