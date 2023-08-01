/*
 * flash.c
 *
 *  Created on: Jul 7, 2023
 *      Author: Jimitp
 */

#include "flash.h"

union Byte_conv4a {
	uint32_t Byte_32;
	uint8_t Byte_8[4];
}Byte_4_conv;



uint8_t Chip_Erase(void)
{
	QSPI_CommandTypeDef sCommand;
	uint8_t ret;
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	  sCommand.Instruction       = CHIP_ERASE_CMD;
	  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	  sCommand.DataMode          = QSPI_DATA_NONE;
	  sCommand.DummyCycles       = 0;
	  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	  //sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	  if(Write_Enable()!=HAL_OK)
	  	  		{
	  	  		printf("Write_enable_fail");
	  	  		return ret;
	  	  		}
	  	  ret=HAL_QSPI_Command(&hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	  	  		if (ret != HAL_OK) {
	  	  		  	return ret;
	  	  		  	}
	  	  ret=QSPI_AutoPollingMemReady();
	  	  		if (ret != HAL_OK)
	  	  			{
	  	  			return ret;
	  	  			}
	  	  		return HAL_OK;
}
uint8_t Write_Enable(void)
{
	uint8_t ret;
	QSPI_CommandTypeDef sCommand;
	QSPI_AutoPollingTypeDef sConfig;
	/* Enable write operations */
	  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	  sCommand.Instruction       = WRITE_ENABLE_CMD;
	  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	  sCommand.DataMode          = QSPI_DATA_NONE;
	  sCommand.DummyCycles       = 0;
	  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	  //sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	  ret=HAL_QSPI_Command(&hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);

	  if (ret != HAL_OK) {
	    return ret;
	  }

	  /* Configure automatic polling mode to wait for write enabling */
	  sConfig.Match           = MX25R6435F_SR_WEL;
	  sConfig.Mask            = MX25R6435F_SR_WEL;
	  sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
	  sConfig.StatusBytesSize = 1;
	  sConfig.Interval        = 0x10;
	  sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	  sCommand.Instruction    = READ_STATUS_REG_CMD;
	  sCommand.DataMode       = QSPI_DATA_1_LINE;
	  ret=HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);

	  if (ret != HAL_OK) {
	    return ret;
	  }


	  return HAL_OK;
}
uint8_t Quad_Enable(void)
{
	uint8_t ret,reg;
	QSPI_CommandTypeDef sCommand;
	  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	  sCommand.Instruction       = READ_STATUS_REG_CMD;
	  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	  sCommand.DataMode          = QSPI_DATA_1_LINE;
	  sCommand.DummyCycles       = 0;
	  sCommand.NbData            = 1;
	  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	  //sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	  ret=HAL_QSPI_Command(&hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	  if (ret != HAL_OK) {
	    return ret;
	  }
	  ret=HAL_QSPI_Receive(&hqspi, &reg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	  if (ret != HAL_OK) {
	  	    return ret;
	  	  }
	  if(Write_Enable()!=HAL_OK)
		  {
		  return ret;
		  }
	  SET_BIT(reg, MX25R6435F_SR_QE);
	  sCommand.Instruction = WRITE_STATUS_CFG_REG_CMD;
	  ret=HAL_QSPI_Command(&hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	  if (ret != HAL_OK) {
	  	    return ret;
	  	  }
	  ret=HAL_QSPI_Transmit(&hqspi, &reg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	  if (ret != HAL_OK) {
	 	  	    return ret;
	 	  	  }
	  ret=QSPI_AutoPollingMemReady();
	  if (ret != HAL_OK) {
	 	 	  	    return ret;
	 	 	  	  }
	  sCommand.Instruction = READ_STATUS_REG_CMD;
	  ret=HAL_QSPI_Command(&hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	  	  if (ret != HAL_OK) {
	  	  	    return ret;
	  	  	  }
	 ret=HAL_QSPI_Receive(&hqspi, &reg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	  		  if (ret != HAL_OK) {
	  		  	    return ret;
	  		  	  }
	  		if((reg & MX25R6435F_SR_QE) == 0){
	  		     printf("QUAD enable\n");
	  		  }
	  		return ret;
}

uint8_t QSPI_AutoPollingMemReady(void)
{
	uint8_t ret;
	QSPI_CommandTypeDef sCommand;
	QSPI_AutoPollingTypeDef sConfig;
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = READ_STATUS_REG_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_1_LINE;
	sCommand.DummyCycles       = 0;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	//sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	sConfig.Match           = 0;
	sConfig.Mask            = MX25R6435F_SR_WIP;
	sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval        = 0x10;
	sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
	ret=HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	if(ret != HAL_OK)
	{
		return ret;

	}
	return HAL_OK;
}
uint8_t Quad_Read(uint8_t *pData,uint32_t Read_ADDr,uint32_t Size)
{
	uint8_t ret;
	ret=Quad_Enable();
	if(ret==HAL_OK)
	{
		printf("Quad_enable");

	}
	QSPI_CommandTypeDef sCommand;
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = QUAD_READ;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.Address           = Read_ADDr;
	sCommand.AddressSize 	   = QSPI_ADDRESS_24_BITS;
	sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	sCommand.DummyCycles       = 8;
	sCommand.NbData            = Size;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	ret=HAL_QSPI_Command(&hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
		if (ret != HAL_OK) {
		  	return ret;
		  	}
	ret=HAL_QSPI_Receive(&hqspi,pData, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
		if (ret != HAL_OK) {
		  	return ret;
		  	}
		ret=QSPI_AutoPollingMemReady();
					  if (ret != HAL_OK)
					  {
					 	return ret;
					 	}
				return HAL_OK;
		return HAL_OK;
}
uint8_t Quad_Write(uint8_t *pData,uint32_t Write_ADDr,uint32_t Size)
{
	//QSPI_CommandTypeDef sCommand;
	uint8_t ret;
	if(Write_Enable()!=HAL_OK)
			  {
		printf("Write_enable");
			  }
	QSPI_CommandTypeDef sCommand;
//	ret=Quad_Enable();
//	if(ret==HAL_OK)
//		{
//		printf("Quad_enable");
//		}
	uint8_t aT[] = " ****QSPI communication based on IT****  ****QSPI communication based on IT****  ****QSPI communication based on IT****  ****QSPI communication based on IT****  ****QSPI communication based on IT****  ****QSPI communication based on IT**** ";
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = QUAD_PAGE_PROG_CMD;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.Address           = Write_ADDr;
	sCommand.AddressSize 	   = QSPI_ADDRESS_24_BITS;
	sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = Size;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	ret=HAL_QSPI_Command(&hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
		if (ret != HAL_OK) {
		  	return ret;
		  	}
	ret=HAL_QSPI_Transmit(&hqspi, pData, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
		if (ret != HAL_OK) {
			return ret;
			 }
	ret=QSPI_AutoPollingMemReady();
			  if (ret != HAL_OK)
			  {
			 	return ret;
			 	}
		return HAL_OK;
}
uint8_t QSPI_Sector_Erase(uint32_t Address)
{uint8_t ret;
		QSPI_CommandTypeDef sCommand;
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	  sCommand.Instruction       = SECTOR_ERASE_CMD;
	  sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
	  sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
	  sCommand.Address           = Address;
	  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	  sCommand.DataMode          = QSPI_DATA_NONE;
	  sCommand.DummyCycles       = 0;
	  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	  //sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	  if(Write_Enable()!=HAL_OK)
	  		{
	  		printf("Write_enable_fail");
	  		return ret;
	  		}
	  ret=HAL_QSPI_Command(&hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	  		if (ret != HAL_OK) {
	  		  	return ret;
	  		  	}
	  ret=QSPI_AutoPollingMemReady();
	  		if (ret != HAL_OK)
	  			{
	  			return ret;
	  			}
	  		return HAL_OK;

}

void update_address(uint32_t add,uint8_t select)
{
	 uint8_t current_address[6];
	 Byte_4_conv.Byte_32=add;
	  Quad_Read(&current_address,flash_address_map,6);
	//  printf("Update address  %d %d %d %d %d %d \n",current_address[0],current_address[1],current_address[2],current_address[3],current_address[4],current_address[5]);
	  if(select==1)
	    {
	      for(int8_t i=16;i>=0;i--)                //take of overFlow
	          {
	//            printf("in loop %d \n",dataconverter.word);
	            if(((Byte_4_conv.Byte_32+i)%4096)==0)
	              {
	                if(Byte_4_conv.Byte_32+i==end_address_Temp)
	                  {
	                	QSPI_Sector_Erase(start_address_Temp);
	                    Byte_4_conv.Byte_32=start_address_Temp;
	//                    printf("iterative_end \n");
	                    break;
	                  }
	              else
	                  {
	            	  QSPI_Sector_Erase(Byte_4_conv.Byte_32+i);
	//                    printf("iterative_address_erase %d \n",dataconverter.word+i);
	                    break;
	                  }
	              }
	          }
	      current_address[0]=Byte_4_conv.Byte_8[2];
	      current_address[1]=Byte_4_conv.Byte_8[1];
	      current_address[2]=Byte_4_conv.Byte_8[0];
	//      printf("up_address %d %d %d",current_address[0],current_address[1],current_address[2]);
	    }
	  if(select==2)
	    {
	      if(Byte_4_conv.Byte_32 % 4096==0)
	        {
	          if(Byte_4_conv.Byte_32==end_address_UV)
	              {
	        	  QSPI_Sector_Erase(start_address_UV);
	        	  Byte_4_conv.Byte_32=start_address_UV;
	//               printf("iterative_end \n");
	              }
	          else
	              {
	        	  QSPI_Sector_Erase(Byte_4_conv.Byte_32);
	//               printf("iterative_address_erase %d \n",dataconverter.word);
	              }
	        }
	      current_address[3]=Byte_4_conv.Byte_8[2];
	      current_address[4]=Byte_4_conv.Byte_8[1];
	      current_address[5]=Byte_4_conv.Byte_8[0];
	    }
	  QSPI_Sector_Erase(flash_address_map);
	  Quad_Write(&current_address,flash_address_map,6);

}
uint32_t Read_flash_address(uint8_t select)
{
  uint8_t current_address[6];

  uint32_t add;
  Quad_Read(current_address,2093056,6);
//  printf("current_add %d %d %d %d %d %d \n",current_address[0],current_address[1],current_address[2],current_address[3],current_address[4],current_address[5]);
// printf("select %d \n",select);
  if(select==1)
    {
	  Byte_4_conv.Byte_8[0]=current_address[2];
	  Byte_4_conv.Byte_8[1]=current_address[1];
	  Byte_4_conv.Byte_8[2]=current_address[0];
	  Byte_4_conv.Byte_8[3]=0;
      add=Byte_4_conv.Byte_32;
//      printf("address_1: %ld \n\n",add);
      if(add==0xffffff || add>=end_address_Temp||add<=start_address_Temp)
        {
          //update_address(start_address_Temp,1);
          add=start_address_Temp;
         }
      return add;
    }
  else if(select==0)
    {
	  Byte_4_conv.Byte_8[0]=current_address[5];
	  Byte_4_conv.Byte_8[1]=current_address[4];
	  Byte_4_conv.Byte_8[2]=current_address[3];
	  Byte_4_conv.Byte_8[3]=0;
	  add=Byte_4_conv.Byte_32;

      if(add==0xffffff || add>=end_address_UV || add<=start_address_UV)
              {
                //update_address(start_address_Temp,2);
                 add=start_address_UV;
               }
            return add;
    }
  else
    {
      return(0xffffff);
    }
}
