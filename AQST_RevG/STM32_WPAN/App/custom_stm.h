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
  CUSTOM_STM_BAT_LEVEL,
  CUSTOM_STM_EOC,
  CUSTOM_STM_PWR_GD,
  /* Gases */
  CUSTOM_STM_IAQ_IN,
  CUSTOM_STM_RELATIVE_IA,
  CUSTOM_STM_TVOC,
  CUSTOM_STM_ETOH,
  CUSTOM_STM_CO2,
  /* Enviroment */
  CUSTOM_STM_TEMP,
  CUSTOM_STM_HUMI,
  CUSTOM_STM_PRES,
  CUSTOM_STM_GET_ENV_LOG,
  /* Lightining_Strike */
  CUSTOM_STM_GET_LIGHTNING_LOG,
  CUSTOM_STM_STRIKE_DISTANCE,
  /* Utilities */
  CUSTOM_STM_TIME,
  CUSTOM_STM_LOG_DATA_TX_STATUS,
  CUSTOM_STM_OTA,
} Custom_STM_Char_Opcode_t;

typedef enum
{
  /* Battery_percentage */
  CUSTOM_STM_BAT_LEVEL_READ_EVT,
  CUSTOM_STM_BAT_LEVEL_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_BAT_LEVEL_NOTIFY_DISABLED_EVT,
  /* EOC */
  CUSTOM_STM_EOC_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_EOC_NOTIFY_DISABLED_EVT,
  /* PWR_Good */
  CUSTOM_STM_PWR_GD_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_PWR_GD_NOTIFY_DISABLED_EVT,
  /* IAQ_Index */
  CUSTOM_STM_IAQ_IN_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_IAQ_IN_NOTIFY_DISABLED_EVT,
  /* Relative_IAQ_Index */
  CUSTOM_STM_RELATIVE_IA_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_RELATIVE_IA_NOTIFY_DISABLED_EVT,
  /* TVOC */
  CUSTOM_STM_TVOC_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_TVOC_NOTIFY_DISABLED_EVT,
  /* ETOH */
  CUSTOM_STM_ETOH_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_ETOH_NOTIFY_DISABLED_EVT,
  /* ECO2 */
  CUSTOM_STM_CO2_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_CO2_NOTIFY_DISABLED_EVT,
  /* Temperature */
  CUSTOM_STM_TEMP_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_TEMP_NOTIFY_DISABLED_EVT,
  /* Humidity */
  CUSTOM_STM_HUMI_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_HUMI_NOTIFY_DISABLED_EVT,
  /* Pressure */
  CUSTOM_STM_PRES_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_PRES_NOTIFY_DISABLED_EVT,
  /* Get_ENV_log */
  CUSTOM_STM_GET_ENV_LOG_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_GET_ENV_LOG_NOTIFY_DISABLED_EVT,
  /* Get_Lightning_log */
  CUSTOM_STM_GET_LIGHTNING_LOG_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_GET_LIGHTNING_LOG_NOTIFY_DISABLED_EVT,
  /* Strike_distance */
  CUSTOM_STM_STRIKE_DISTANCE_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_STRIKE_DISTANCE_NOTIFY_DISABLED_EVT,
  /* Time */
  CUSTOM_STM_TIME_READ_EVT,
  CUSTOM_STM_TIME_WRITE_NO_RESP_EVT,
  /* Log_data_tx_status */
  CUSTOM_STM_LOG_DATA_TX_STATUS_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_LOG_DATA_TX_STATUS_NOTIFY_DISABLED_EVT,
  /* OTA */
  CUSTOM_STM_OTA_WRITE_NO_RESP_EVT,

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
extern uint8_t SizeBat_Level;
extern uint8_t SizeEoc;
extern uint8_t SizePwr_Gd;
extern uint8_t SizeIaq_In;
extern uint8_t SizeRelative_Ia;
extern uint8_t SizeTvoc;
extern uint8_t SizeEtoh;
extern uint8_t SizeCo2;
extern uint8_t SizeTemp;
extern uint8_t SizeHumi;
extern uint8_t SizePres;
extern uint8_t SizeGet_Env_Log;
extern uint8_t SizeGet_Lightning_Log;
extern uint8_t SizeStrike_Distance;
extern uint8_t SizeTime;
extern uint8_t SizeLog_Data_Tx_Status;
extern uint8_t SizeOta;

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
