/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_app.h
  * @author  MCD Application Team
  * @brief   Header for custom_app.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#ifndef CUSTOM_APP_H
#define CUSTOM_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mlx90632.h"
#include "flash.h"
#include "Acclx.h"
#include "adc.h"
#include "rtc.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  CUSTOM_CONN_HANDLE_EVT,
  CUSTOM_DISCON_HANDLE_EVT,
} Custom_App_Opcode_Notification_evt_t;

typedef struct
{
  Custom_App_Opcode_Notification_evt_t     Custom_Evt_Opcode;
  uint16_t                                 ConnectionHandle;
} Custom_App_ConnHandle_Not_evt_t;
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define STORE_LE_16(buf, val)    ( ((buf)[0] =  (int8_t) (val)    ) , \
                                   ((buf)[1] =  (int8_t) (val>>8) ) )

#define STORE_LE_32(buf, val)    ( ((buf)[0] =  (int8_t) (val)     ) , \
                                   ((buf)[1] =  (int8_t) (val>>8)  ) , \
                                   ((buf)[2] =  (int8_t) (val>>16) ) , \
                                   ((buf)[3] =  (int8_t) (val>>24) ) )

#define STORE_BE_32(buf, val)    ( ((buf)[3] =  (int8_t) (val)     ) , \
                                   ((buf)[2] =  (int8_t) (val>>8)  ) , \
                                   ((buf)[1] =  (int8_t) (val>>16) ) , \
                                   ((buf)[0] =  (int8_t) (val>>24) ) )

#define STORE_16_BE(buf, val)    ((int16_t)val = ((buf)[0]<<8|(buf)[1]))

/* USER CODE END EM */

/* Exported functions ---------------------------------------------*/
void Custom_APP_Init(void);
void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification);
/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /* CUSTOM_APP_H */
