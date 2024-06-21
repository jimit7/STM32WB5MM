/*
 * UV.c
 *
 *  Created on: Mar. 23, 2023
 *      Author: Jimitp
 */

#include "UV.h"
#include "i2c.h"
 uint8_t data_Big_Endian[3];
 float f_UVI =0;
void Set_thresold(uint32_t low,uint32_t high)
	{
	uint8_t Data_byte[3];
	Data_byte[0] = (low >> 16) & 0x0F;
	Data_byte[1] = (low >> 8 ) & 0xFF;
	Data_byte[2] = (low & 0xFF);
	HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,UVS_ALS_thr_low_l,I2C_MEMADD_SIZE_8BIT,&Data_byte[2],1,HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,UVS_ALS_thr_low_m,I2C_MEMADD_SIZE_8BIT,&Data_byte[1],1,HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,UVS_ALS_thr_low_h,I2C_MEMADD_SIZE_8BIT,&Data_byte[0],1,HAL_MAX_DELAY);
	Data_byte[0] = (high >> 16) & 0x0F;
	Data_byte[1] = (high >> 8 ) & 0xFF;
	Data_byte[2] = (high & 0xFF);
	HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,UVS_ALS_thr_up_l,I2C_MEMADD_SIZE_8BIT,&Data_byte[2],1,HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,UVS_ALS_thr_up_m,I2C_MEMADD_SIZE_8BIT,&Data_byte[1],1,HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,UVS_ALS_thr_up_h,I2C_MEMADD_SIZE_8BIT,&Data_byte[0],1,HAL_MAX_DELAY);

	}

void config_interrupt(void)
{
	//uint8_t cmd[2]={0x14,0x40};
	uint8_t cmd[2]={0x30,0x40};
	HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,LTR390_INT_CFG,I2C_MEMADD_SIZE_8BIT,&cmd[0],1,HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,LTR390_INT_PST,I2C_MEMADD_SIZE_8BIT,&cmd[1],1,HAL_MAX_DELAY);
}
// low throsold =100 //0x64 low byte:0x64,mid byte:0x00,high byte:00
// High_throsold=1000//0x3E8 low_byte:0x08,mid_byte=0x3E,High_byte:00
void UV_init(void)
{
	uint8_t cmd[1]={0};
	 //cmd[0]=0x10;//0x0A;//0x02;
	 //HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,LTR390_MAIN_CTRL,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	 cmd[0]=0x0A;//0x02;
	 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,LTR390_MAIN_CTRL,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	 cmd[0]=0x22;
	 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,LTR390_MEAS_RATE,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	 cmd[0]=0x04;//0x01;
	 HAL_I2C_Mem_Write(&hi2c3,LTR390_WR,LTR390_GAIN,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	 HAL_Delay(10);
 //Set_thresold(100,1000);
 //config_interrupt();

}


void f_UV(void)
{

  uint8_t Status=0;
  while(Status!=0x08){
  HAL_I2C_Mem_Read(&hi2c3,LTR390_RD,LTR390_MAIN_STATUS,I2C_MEMADD_SIZE_8BIT,&Status,1,HAL_MAX_DELAY);
  printf("Inside status %d:\n",Status);
  }
//  uint8_t data_Big_Endian[3];//  data= bus_read(LTR390_RD, LTR390_UVSDATA ,3);
  HAL_I2C_Mem_Read(&hi2c3,LTR390_RD,LTR390_UVSDATA,I2C_MEMADD_SIZE_8BIT,data_Big_Endian,3,HAL_MAX_DELAY);
  //HAL_I2C_Mem_Read(&hi2c3,LTR390_RD,LTR390_UVSDATA,I2C_MEMADD_SIZE_8BIT,data_Big_Endian,3,HAL_MAX_DELAY);
  //HAL_I2C_Mem_Read(&hi2c3,LTR390_RD,LTR390_UVSDATA,I2C_MEMADD_SIZE_8BIT,data_Big_Endian,3,HAL_MAX_DELAY);
// I2cread(LTR390_RD,LTR390_UVSDATA,data_Big_Endian,3);
  //printf("uv_raw %d %d %d \n",data_Big_Endian[0],data_Big_Endian[1],data_Big_Endian[2]);
//  data_little_Endian[0]=data_Big_Endian[2];
//  data_little_Endian[1]=data_Big_Endian[1];
//  data_little_Endian[2]=data_Big_Endian[0];
  uint32_t UV_count = ((data_Big_Endian[2]*65536) + (data_Big_Endian[1]*256) +data_Big_Endian[0]);
  f_UVI=((float)UV_count/2300)*1;
  printf("uv_raw :[%d] UVI: [%f] \n",UV_count,f_UVI);
  //return (float)f_UVI;

}



uint8_t UV(void)
{
  uint8_t UVI;
//  uint8_t data_Big_Endian[3];//  data= bus_read(LTR390_RD, LTR390_UVSDATA ,3);
  HAL_I2C_Mem_Read(&hi2c3,LTR390_RD,LTR390_UVSDATA,I2C_MEMADD_SIZE_8BIT,data_Big_Endian,3,HAL_MAX_DELAY);
  //HAL_I2C_Mem_Read(&hi2c3,LTR390_RD,LTR390_UVSDATA,I2C_MEMADD_SIZE_8BIT,data_Big_Endian,3,HAL_MAX_DELAY);
  //HAL_I2C_Mem_Read(&hi2c3,LTR390_RD,LTR390_UVSDATA,I2C_MEMADD_SIZE_8BIT,data_Big_Endian,3,HAL_MAX_DELAY);
// I2cread(LTR390_RD,LTR390_UVSDATA,data_Big_Endian,3);
  //printf("uv_raw %d %d %d \n",data_Big_Endian[0],data_Big_Endian[1],data_Big_Endian[2]);
//  data_little_Endian[0]=data_Big_Endian[2];
//  data_little_Endian[1]=data_Big_Endian[1];
//  data_little_Endian[2]=data_Big_Endian[0];
  uint32_t UV_count = ((data_Big_Endian[2]*65536) + (data_Big_Endian[1]*256) +data_Big_Endian[0]);
  UVI=(UV_count/2300)*1;
  printf("uv_raw :[%d] UVI: [%f] \n",UV_count,UVI);
  return UVI;

}
uint8_t *ALS(void)
{
  //static int data[3]={0,0,0};
  //uint8_t data_Big_Endian[3];//  data= bus_read(LTR390_RD, LTR390_UVSDATA ,3);
  HAL_I2C_Mem_Read(&hi2c3,LTR390_RD,LTR390_ALSDATA,I2C_MEMADD_SIZE_8BIT,data_Big_Endian,3,HAL_MAX_DELAY);
  printf("ALS_raw %d %d %d \r\n",data_Big_Endian[2],data_Big_Endian[1],data_Big_Endian[0]);
  printf("uv_raw %d  \r\n",((data_Big_Endian[2]*65536) + (data_Big_Endian[1]*256) +data_Big_Endian[0]));
//  bus_read(LTR390_RD,LTR390_ALSDATA,data,3);
  return data_Big_Endian;

}
//int8_t bus_write(uint8_t dev_addr, uint8_t targetAddress,uint8_t txBuff )
//{
//  I2C_TransferSeq_TypeDef i2cTransfer;
//  I2C_TransferReturn_TypeDef result;
//  uint8_t txBuffer[2];
//
//  txBuffer[0] = targetAddress;
//  txBuffer[1] = txBuff;
//
//  // Initialize I2C transfer
//  i2cTransfer.addr          = dev_addr;
//  i2cTransfer.flags         = I2C_FLAG_WRITE;
//  i2cTransfer.buf[0].data   = txBuffer;
//  i2cTransfer.buf[0].len    = 2;
//
//
//  result = I2C_TransferInit(I2C0, &i2cTransfer);
//
//  // Send data
//  while (result == i2cTransferInProgress) {
//    result = I2C_Transfer(I2C0);
//  }
//
//  if (result == i2cTransferDone)
//          {
//            return 0;
//          }
//      else{
////           printf("err %x:\n",txBuffer[1]);
////            printf("i2c_write_fail");
////            printf("err %x:\n",txBuffer[1]);
//            return 1;
//          }
//}
