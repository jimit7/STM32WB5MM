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
#define RESET_ENABLE_CMD                     0x66
#define RESET_EXECUTE_CMD                    0x99
#define flash_address_map 0x1FF000
#define mode_address_map 0x00
#define start_address_Temp 0x1000
#define end_address_Temp 0xFEFFF
#define start_address_UV 0xFF000
#define end_address_UV 0x1FEFFF

//QSPI_AutoPollingTypeDef sConfig;
uint8_t FL_QUADSPI_Init(void);
uint8_t QSPI_AutoPollingMemReady(void);
uint8_t Write_Enable(void);
uint8_t Chip_Erase(void);
uint8_t Quad_Enable(void);
uint8_t QSPI_AutoPollingMemReady(void);
uint8_t Quad_Read(uint8_t *pData,uint32_t Read_ADDr,uint32_t Size);
uint8_t Quad_Write(uint8_t *pData,uint32_t Write_ADDr,uint32_t Size);
uint8_t QSPI_Sector_Erase(uint32_t Address);
uint32_t Read_flash_address(uint8_t select);
uint8_t QSPI_Configuration(void);
uint8_t QSPI_ResetChip(void);
void update_address(uint32_t add,uint8_t select);
#endif /* APP_FLASH_H_ */
