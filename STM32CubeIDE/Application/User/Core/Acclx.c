/*
 * Acclx.c
 *
 *  Created on: Apr. 14, 2023
 *      Author: Jimitp
 */
#include "Acclx.h"
#include "i2c.h"

//AccelData output;
//uint8_t //Acclx_read(uint8_t address ,uint8_t *Buffer ,uint8_t numBytes)
//{
//  I2C_TransferSeq_TypeDef i2cTransfer;
//  I2C_TransferReturn_TypeDef result;
//  uint8_t tx[1];
//  tx[0]=address;
//  // Initialize I2C transfer
//  i2cTransfer.addr          = ACCEL_ADDRESS;
//  i2cTransfer.flags         = I2C_FLAG_WRITE_READ; // must write target address before reading
//  i2cTransfer.buf[0].data   = tx;
//  i2cTransfer.buf[0].len    = 1;
//  i2cTransfer.buf[1].data   = Buffer;
//  i2cTransfer.buf[1].len    = numBytes;
//
//  result = I2C_TransferInit(I2C0, &i2cTransfer);
//
//  // Send data
//  while (result == i2cTransferInProgress) {
//      result = I2C_Transfer(I2C0);
////     printf(" tranfer in progress \r \n");
//  }
//
//  if (result == i2cTransferDone)
//    {
//      // LED1 ON and infinite while loop to indicate I2C transmission problem
////      for(int i=0;i<numBytes;i++)
////        {
////          printf(" R:%x \n", Buffer[i]);
////        }
//      return 0;
//    }
//  else
//    {
//      printf("i2c_write_fail");
//      return 1;
//    }
//}
typedef union U1
{
  uint8_t data[2];
  int16_t word;
} MY_CONV;
MY_CONV dataconver;

AccelData output;
////uint8_t Acclx_Write(uint8_t address ,uint8_t Buffer)
//{
//  I2C_TransferSeq_TypeDef i2cTransfer;
//  I2C_TransferReturn_TypeDef result;
//  uint8_t data[2];
//  data[0]=address;
//  data[1]=Buffer;
//      // Initialize I2C transfer
//      i2cTransfer.addr          = ACCEL_ADDRESS;
//      i2cTransfer.flags         = I2C_FLAG_WRITE; // must write target address before reading
//      i2cTransfer.buf[0].data   = data;
//      i2cTransfer.buf[0].len    = 2;
//      result = I2C_TransferInit(I2C0, &i2cTransfer);
//      // Send data
//      while (result == i2cTransferInProgress) {
//        result = I2C_Transfer(I2C0);
//        ////printf(" tranfer in progress \r \n");
//      }
//
//      if (result == i2cTransferDone)
//               {
////          for(int i=0;i<numBytes;i++)
////            {
////              //printf(" W:%x ", Buffer[i]);
////            }
//                return 0;
//               }
//           else
//             {
//               //printf("i2c_write_fail");
//               return 1;
//               }
//}

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
void Accel_6D_Init(void)
{
	uint8_t cmd[1];
	cmd[0]=0x00;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_TEMP_CFG_REG,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x77;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_CTRL_REG1,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x00;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_CTRL_REG2,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x40;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_CTRL_REG3,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x00;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_CTRL_REG4,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x08;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_CTRL_REG5,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x00;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_CTRL_REG6,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);

	cmd[0]=0x28;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_INT1_THS,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x01;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_INT1_DURATION,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	cmd[0]=0x7F;
	HAL_I2C_Mem_Write(&hi2c3,ACCEL_ADDRESS,ACCEL_INT1_CFG,I2C_MEMADD_SIZE_8BIT,cmd,1,HAL_MAX_DELAY);
	// write 0x00 CTRL 5
}
void AccelRead(void)
{
//  MY_CONV dataconver;
  uint8_t data[1];
  //Acclx_read(ACCEL_OUT_X_L,data,1);
  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_X_L,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
  dataconver.data[0]=data[0];
  //Acclx_read(ACCEL_OUT_X_H,data,1);
  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_X_H,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
  dataconver.data[1]=data[0];
  output.RawAccelX=dataconver.word;

  //Acclx_read(ACCEL_OUT_Y_L,data,1);
  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_Y_L,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
  dataconver.data[0]=data[0];
  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_Y_H,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
  //Acclx_read(ACCEL_OUT_Y_H,data,1);
  dataconver.data[1]=data[0];
  output.RawAccelY=dataconver.word;

  //Acclx_read(ACCEL_OUT_Z_L,data,1);
  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_Z_L,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
  dataconver.data[0]=data[0];
  //Acclx_read(ACCEL_OUT_Z_H,data,1);
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
void Intr_Status(void)
	{
	uint8_t data[1];
	  //Acclx_read(ACCEL_OUT_X_L,data,1);
	  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_INT1_SRC,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
	  printf("INT_SRC %d \n",data[0]);
	}
//void AccelRead(AccelData *output)
//{
////  MY_CONV dataconver;
//  uint8_t data[1];
//  //Acclx_read(ACCEL_OUT_X_L,data,1);
//  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_X_L,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
//  dataconver.data[0]=data[0];
//  //Acclx_read(ACCEL_OUT_X_H,data,1);
//  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_X_H,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
//  dataconver.data[1]=data[0];
//  output->RawAccelX=dataconver.word;
//
//  //Acclx_read(ACCEL_OUT_Y_L,data,1);
//  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_Y_L,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
//  dataconver.data[0]=data[0];
//  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_Y_H,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
//  //Acclx_read(ACCEL_OUT_Y_H,data,1);
//  dataconver.data[1]=data[0];
//  output->RawAccelY=dataconver.word;
//
//  //Acclx_read(ACCEL_OUT_Z_L,data,1);
//  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_Z_L,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
//  dataconver.data[0]=data[0];
//  //Acclx_read(ACCEL_OUT_Z_H,data,1);
//  HAL_I2C_Mem_Read(&hi2c3,ACCEL_ADDRESS,ACCEL_OUT_Z_H,I2C_MEMADD_SIZE_8BIT,data,1,HAL_MAX_DELAY);
//  dataconver.data[1]=data[0];
//  output->RawAccelZ=dataconver.word;
//
//  output->AccelXScratch = ((float)(output->RawAccelX/64))*0.004f *100;
//  output->AccelYScratch = ((float)(output->RawAccelY/64))*0.004f *100;
//  output->AccelZScratch = ((float)(output->RawAccelZ/64))*0.004f *100;
//
//
//    output->AccelX = (int16_t)output->AccelXScratch;
//    output->AccelY = (int16_t)output->AccelYScratch;
//    output->AccelZ = (int16_t)output->AccelZScratch;
//
//    printf("Accelerometer X:%d Y:%d Z:%d \n",output->AccelX,output->AccelY,output->AccelZ);
//
//}
