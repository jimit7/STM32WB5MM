/*
 * flash.h
 *
 *  Created on: Jul 7, 2023
 *      Author: Jimitp
 */

#ifndef APP_FLASH_H_
#define APP_FLASH_H_

#include "quadspi.h"

#define RDID 0x9F
#define WRITE_ENABLE_CMD 0x06
#define MX25R6435F_SR_WIP                    ((uint8_t)0x01)    /*!< Write in progress */
#define MX25R6435F_SR_WEL                    ((uint8_t)0x02)    /*!< Write enable latch */
#define MX25R6435F_SR_BP                     ((uint8_t)0x3C)    /*!< Block protect */
#define MX25R6435F_SR_QE                     ((uint8_t)0x40)    /*!< Quad enable */
#define MX25R6435F_SR_SRWD                   ((uint8_t)0x80)	/*!< Status register write disable */
#define READ_STATUS_REG_CMD                  0x05
#define READ_CFG_REG_CMD                     0x15
#define WRITE_STATUS_CFG_REG_CMD             0x01
#define QUAD_READ 							 0x6B
#define QUAD_PAGE_PROG_CMD                   0x38
#define SECTOR_ERASE_CMD                     0x20
#define CHIP_ERASE_CMD                       0x60
#define flash_address_map 2093056
#define start_address_Temp 0
#define end_address_Temp 1044480
#define start_address_UV 1044480
#define end_address_UV 2093056

//QSPI_AutoPollingTypeDef sConfig;

uint8_t QSPI_AutoPollingMemReady(void);
uint8_t Write_Enable(void);
uint8_t Chip_Erase(void);
uint8_t Quad_Enable(void);
uint8_t QSPI_AutoPollingMemReady(void);
uint8_t Quad_Read(uint8_t *pData,uint32_t Read_ADDr,uint32_t Size);
uint8_t Quad_Write(uint8_t *pData,uint32_t Write_ADDr,uint32_t Size);
uint8_t QSPI_Sector_Erase(uint32_t Address);
uint32_t Read_flash_address(uint8_t select);
void update_address(uint32_t add,uint8_t select);
#endif /* APP_FLASH_H_ */
