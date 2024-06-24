/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_stm.h
  * @author  MCD Application Team
  * @brief   Header for custom_stm.c module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOM_STM_H
#define CUSTOM_STM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  /* Battery_Service */
  CUSTOM_STM_LEVEL,
  /* Temperature */
  CUSTOM_STM_IR_T,
  CUSTOM_STM_A_T,
  CUSTOM_STM_S_T,
  CUSTOM_STM_L_T,
  CUSTOM_STM_R_N,
  /* UV_Sensor */
  CUSTOM_STM_UV_V,
  CUSTOM_STM_UV_L,
  CUSTOM_STM_OTA,
  CUSTOM_STM_AMBI_TEMP,
  CUSTOM_STM_T_INT,
  /* Utility */
  CUSTOM_STM_ACCLX,
  CUSTOM_STM_TD,
  CUSTOM_STM_EC,
  CUSTOM_STM_PWR_GD,
  CUSTOM_STM_SOFTWARE_REVISION_STRING,
  /* OperationMode */
  CUSTOM_STM_MD,
} Custom_STM_Char_Opcode_t;

typedef enum
{
  /* level */
  CUSTOM_STM_LEVEL_READ_EVT,
  /* IR_temperature */
  CUSTOM_STM_IR_T_READ_EVT,
  CUSTOM_STM_IR_T_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_IR_T_NOTIFY_DISABLED_EVT,
  /* Analog_Temperature */
  CUSTOM_STM_A_T_READ_EVT,
  /* Set_Temp */
  CUSTOM_STM_S_T_WRITE_NO_RESP_EVT,
  /* Log_Temp */
  CUSTOM_STM_L_T_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_L_T_NOTIFY_DISABLED_EVT,
  /* Range_Notification */
  CUSTOM_STM_R_N_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_R_N_NOTIFY_DISABLED_EVT,
  /* UV_Value */
  CUSTOM_STM_UV_V_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_UV_V_NOTIFY_DISABLED_EVT,
  /* UV_Log */
  CUSTOM_STM_UV_L_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_UV_L_NOTIFY_DISABLED_EVT,
  /* BLE_CFG_OTA_REBOOT_CHAR */
  CUSTOM_STM_OTA_WRITE_NO_RESP_EVT,
  /* Ambient_Temperature */
  CUSTOM_STM_AMBI_TEMP_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_AMBI_TEMP_NOTIFY_DISABLED_EVT,
  /* TIME_INTERVAL */
  CUSTOM_STM_T_INT_READ_EVT,
  CUSTOM_STM_T_INT_WRITE_NO_RESP_EVT,
  /* Acclerometer */
  CUSTOM_STM_ACCLX_READ_EVT,
  /* Time_Date */
  CUSTOM_STM_TD_READ_EVT,
  CUSTOM_STM_TD_WRITE_NO_RESP_EVT,
  /* EOC */
  CUSTOM_STM_EC_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_EC_NOTIFY_DISABLED_EVT,
  /* PWR_GD */
  CUSTOM_STM_PWR_GD_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_PWR_GD_NOTIFY_DISABLED_EVT,
  /* Software_Revision_String */
  CUSTOM_STM_SOFTWARE_REVISION_STRING_READ_EVT,
  /* MODE */
  CUSTOM_STM_MD_READ_EVT,
  CUSTOM_STM_MD_WRITE_NO_RESP_EVT,

  CUSTOM_STM_BOOT_REQUEST_EVT
} Custom_STM_Opcode_evt_t;

typedef struct
{
  uint8_t * pPayload;
  uint8_t   Length;
} Custom_STM_Data_t;

typedef struct
{
  Custom_STM_Opcode_evt_t       Custom_Evt_Opcode;
  Custom_STM_Data_t             DataTransfered;
  uint16_t                      ConnectionHandle;
  uint8_t                       ServiceInstance;
} Custom_STM_App_Notification_evt_t;

/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
extern uint8_t SizeLevel;
extern uint8_t SizeIr_T;
extern uint8_t SizeA_T;
extern uint8_t SizeS_T;
extern uint8_t SizeL_T;
extern uint8_t SizeR_N;
extern uint8_t SizeUv_V;
extern uint8_t SizeUv_L;
extern uint8_t SizeOta;
extern uint8_t SizeAmbi_Temp;
extern uint8_t SizeT_Int;
extern uint8_t SizeAcclx;
extern uint8_t SizeTd;
extern uint8_t SizeEc;
extern uint8_t SizePwr_Gd;
extern uint8_t SizeSoftware_Revision_String;
extern uint8_t SizeMd;

/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros -----------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ------------------------------------------------------- */
void SVCCTL_InitCustomSvc(void);
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification);
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode,  uint8_t *pPayload);
/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*CUSTOM_STM_H */
