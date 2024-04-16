/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    App/custom_app.c
 * @author  MCD Application Team
 * @brief   Custom Example Application (Server)
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "zmod4xxx.h"
#include "zmod4xxx_hal.h"
#include "zmod4xxx_cleaning.h"
#include "hsxxxx.h"
#include "iaq_2nd_gen_ulp.h"
#include "zmod4410_config_iaq2_ulp.h"
#include "LPS25hb.h"
#include "rtc.h"
#include "flash.h"
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* Battery_Service */
  uint8_t               Bat_level_Notification_Status;
  uint8_t               Eoc_Notification_Status;
  uint8_t               Pwr_gd_Notification_Status;
  /* Gases */
  uint8_t               Iaq_in_Notification_Status;
  uint8_t               Relative_ia_Notification_Status;
  uint8_t               Tvoc_Notification_Status;
  uint8_t               Etoh_Notification_Status;
  uint8_t               Co2_Notification_Status;
  /* Enviroment */
  uint8_t               Temp_Notification_Status;
  uint8_t               Humi_Notification_Status;
  uint8_t               Pres_Notification_Status;
  uint8_t               Get_env_log_Notification_Status;
  /* Lightining_Strike */
  uint8_t               Get_lightning_log_Notification_Status;
  uint8_t               Strike_distance_Notification_Status;
  /* Utilities */
  uint8_t               Log_data_tx_status_Notification_Status;
  /* USER CODE BEGIN CUSTOM_APP_Context_t */

  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */
#define ENV_UPDATE_PERIOD       (uint32_t)(90*1000000/CFG_TS_TICK_VAL)
#define Noti_UPDATE_PERIOD       (uint32_t)(1000000/CFG_TS_TICK_VAL)
#define Ts_UPDATE_PERIOD       (uint32_t)(0.5*1000000/CFG_TS_TICK_VAL)
#define TX_UPDATE_PERIOD       (uint32_t)(0.5*1000000/CFG_TS_TICK_VAL)

uint8_t Log_Track=0,LED=0;
static zmod4xxx_dev_t dev;
static uint8_t zmod4xxx_status;
static uint8_t adc_result[ZMOD4410_ADC_DATA_LEN];
static uint8_t prod_data[ZMOD4410_PROD_DATA_LEN];
uint8_t ENV_TIMER_ID,Noti_TIMER_ID,Ts_TIMER_ID,TX_LOG_ID;
static Interface_t  hal;
static char const*  errContext;

/* Algorithm related declarations */
static iaq_2nd_gen_ulp_handle_t  algo_handle;
static iaq_2nd_gen_ulp_results_t algo_results;
static iaq_2nd_gen_ulp_inputs_t  algo_input;

/* Temperature and humidity sensor related declarations */
static HSxxxx_t          hsxxxx;
static HSxxxx_t*         htSensor = NULL;  /* pointer will be set if HSxxxx is found */
static HSxxxx_Results_t  htResults;

float LPS25hb_data[2];
uint8_t Lightining_Strike_Distance,PWR_GD,EOC_v,Battery_percentage;
uint32_t Current_Address,New_Lightining_data=0;


//RTCStatus = 1;

uint8_t Time[7];
uint8_t Log[39];
uint32_t iteration_address1= flash_start_address_Env, iteration_address2= flash_start_address_LT;
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
int  detect_and_configure ( zmod4xxx_dev_t*, int, char const** );
void read_and_verify ( zmod4xxx_dev_t*, uint8_t*, char const* );
void Zmod4410_init(void);
void Get_Zmod4410(void);
void ENV_APPInit(void);
void ENV_CALL_BACK(void);
void Notification(void);
void Noti_CALL_BACK(void);
void INTR_NOTIFY(void);
void TS_Toggle(void);
void TS_CALL_BACK(void);
void TS(void);
void Date_Time(void);
void Log_Data(uint8_t select);
void TX_Log(void);
void Init_Tx_Log(void);
void TX_CALLBACK(void);
void Battery(void);
void IAQ_led_RESET(void);
void IAQ_Blink(uint16_t led,uint16_t led2);
void IAQ_Status(uint8_t state);
/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//const uint8_t instance1 = 1;
//const uint8_t instance2 = 2;
//UTIL_SEQ_TaskParamDef(TX_Log, instance1)
//UTIL_SEQ_TaskParamDef(TX_Log, instance2)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

uint8_t UpdateCharData[247];
uint8_t NotifyCharData[247];

/* USER CODE BEGIN PV */
//volatile int8_t ENV_data[4];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* Battery_Service */
static void Custom_Bat_level_Update_Char(void);
static void Custom_Bat_level_Send_Notification(void);
static void Custom_Eoc_Update_Char(void);
static void Custom_Eoc_Send_Notification(void);
static void Custom_Pwr_gd_Update_Char(void);
static void Custom_Pwr_gd_Send_Notification(void);
/* Gases */
static void Custom_Iaq_in_Update_Char(void);
static void Custom_Iaq_in_Send_Notification(void);
static void Custom_Relative_ia_Update_Char(void);
static void Custom_Relative_ia_Send_Notification(void);
static void Custom_Tvoc_Update_Char(void);
static void Custom_Tvoc_Send_Notification(void);
static void Custom_Etoh_Update_Char(void);
static void Custom_Etoh_Send_Notification(void);
static void Custom_Co2_Update_Char(void);
static void Custom_Co2_Send_Notification(void);
/* Enviroment */
static void Custom_Temp_Update_Char(void);
static void Custom_Temp_Send_Notification(void);
static void Custom_Humi_Update_Char(void);
static void Custom_Humi_Send_Notification(void);
static void Custom_Pres_Update_Char(void);
static void Custom_Pres_Send_Notification(void);
static void Custom_Get_env_log_Update_Char(void);
static void Custom_Get_env_log_Send_Notification(void);
/* Lightining_Strike */
static void Custom_Get_lightning_log_Update_Char(void);
static void Custom_Get_lightning_log_Send_Notification(void);
static void Custom_Strike_distance_Update_Char(void);
static void Custom_Strike_distance_Send_Notification(void);
/* Utilities */
static void Custom_Log_data_tx_status_Update_Char(void);
static void Custom_Log_data_tx_status_Send_Notification(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */

  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* Battery_Service */
    case CUSTOM_STM_BAT_LEVEL_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BAT_LEVEL_READ_EVT */
    	Custom_Bat_level_Send_Notification();
      /* USER CODE END CUSTOM_STM_BAT_LEVEL_READ_EVT */
      break;

    case CUSTOM_STM_BAT_LEVEL_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BAT_LEVEL_NOTIFY_ENABLED_EVT */
    	Custom_Bat_level_Send_Notification();
      /* USER CODE END CUSTOM_STM_BAT_LEVEL_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_BAT_LEVEL_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BAT_LEVEL_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_BAT_LEVEL_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_EOC_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_EOC_NOTIFY_ENABLED_EVT */
    	Custom_Eoc_Send_Notification();

      /* USER CODE END CUSTOM_STM_EOC_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_EOC_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_EOC_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_EOC_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_PWR_GD_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PWR_GD_NOTIFY_ENABLED_EVT */
    	Custom_Pwr_gd_Send_Notification();
      /* USER CODE END CUSTOM_STM_PWR_GD_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_PWR_GD_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PWR_GD_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_PWR_GD_NOTIFY_DISABLED_EVT */
      break;

    /* Gases */
    case CUSTOM_STM_IAQ_IN_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_IAQ_IN_NOTIFY_ENABLED_EVT */
    	Custom_Iaq_in_Send_Notification();
      /* USER CODE END CUSTOM_STM_IAQ_IN_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_IAQ_IN_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_IAQ_IN_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_IAQ_IN_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_RELATIVE_IA_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_RELATIVE_IA_NOTIFY_ENABLED_EVT */
    	Custom_Relative_ia_Send_Notification();
      /* USER CODE END CUSTOM_STM_RELATIVE_IA_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_RELATIVE_IA_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_RELATIVE_IA_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_RELATIVE_IA_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_TVOC_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_TVOC_NOTIFY_ENABLED_EVT */
    	Custom_Tvoc_Send_Notification();
      /* USER CODE END CUSTOM_STM_TVOC_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_TVOC_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_TVOC_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_TVOC_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_ETOH_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_ETOH_NOTIFY_ENABLED_EVT */
    	Custom_Etoh_Send_Notification();
      /* USER CODE END CUSTOM_STM_ETOH_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_ETOH_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_ETOH_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_ETOH_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_CO2_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_CO2_NOTIFY_ENABLED_EVT */
    	Custom_Co2_Send_Notification();
      /* USER CODE END CUSTOM_STM_CO2_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_CO2_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_CO2_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_CO2_NOTIFY_DISABLED_EVT */
      break;

    /* Enviroment */
    case CUSTOM_STM_TEMP_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_TEMP_NOTIFY_ENABLED_EVT */
    	Custom_Temp_Send_Notification();
      /* USER CODE END CUSTOM_STM_TEMP_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_TEMP_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_TEMP_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_TEMP_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_HUMI_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_HUMI_NOTIFY_ENABLED_EVT */
    	Custom_Humi_Send_Notification();
      /* USER CODE END CUSTOM_STM_HUMI_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_HUMI_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_HUMI_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_HUMI_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_PRES_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PRES_NOTIFY_ENABLED_EVT */
    	Custom_Pres_Send_Notification();
      /* USER CODE END CUSTOM_STM_PRES_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_PRES_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PRES_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_PRES_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_GET_ENV_LOG_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_GET_ENV_LOG_NOTIFY_ENABLED_EVT */
    	UTIL_SEQ_PauseTask(1<<CFG_TASK_ENV_ID);
    	HW_TS_Stop(ENV_TIMER_ID);
    	Current_Address=Read_flash_address(1);
    	HW_TS_Start(TX_LOG_ID,TX_UPDATE_PERIOD);
      /* USER CODE END CUSTOM_STM_GET_ENV_LOG_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_GET_ENV_LOG_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_GET_ENV_LOG_NOTIFY_DISABLED_EVT */
		UTIL_SEQ_PauseTask(1<<CFG_LOG_TX_ID);
		HW_TS_Stop(TX_LOG_ID);
		HW_TS_Start(ENV_TIMER_ID,ENV_UPDATE_PERIOD);
      /* USER CODE END CUSTOM_STM_GET_ENV_LOG_NOTIFY_DISABLED_EVT */
      break;

    /* Lightining_Strike */
    case CUSTOM_STM_GET_LIGHTNING_LOG_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_GET_LIGHTNING_LOG_NOTIFY_ENABLED_EVT */
		UTIL_SEQ_PauseTask(1<<CFG_TASK_ENV_ID);
		HW_TS_Stop(ENV_TIMER_ID);
		Current_Address=Read_flash_address(2);
		HW_TS_Start(TX_LOG_ID,TX_UPDATE_PERIOD);
      /* USER CODE END CUSTOM_STM_GET_LIGHTNING_LOG_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_GET_LIGHTNING_LOG_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_GET_LIGHTNING_LOG_NOTIFY_DISABLED_EVT */
		UTIL_SEQ_PauseTask(1<<CFG_LOG_TX_ID);
		HW_TS_Stop(TX_LOG_ID);
		HW_TS_Start(ENV_TIMER_ID,ENV_UPDATE_PERIOD);
      /* USER CODE END CUSTOM_STM_GET_LIGHTNING_LOG_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_STRIKE_DISTANCE_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_STRIKE_DISTANCE_NOTIFY_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_STRIKE_DISTANCE_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_STRIKE_DISTANCE_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_STRIKE_DISTANCE_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_STRIKE_DISTANCE_NOTIFY_DISABLED_EVT */
      break;

    /* Utilities */
    case CUSTOM_STM_TIME_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_TIME_READ_EVT */
		Date_Time();
		Custom_STM_App_Update_Char(CUSTOM_STM_TIME, (uint8_t *)Time);
      /* USER CODE END CUSTOM_STM_TIME_READ_EVT */
      break;

    case CUSTOM_STM_TIME_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_TIME_WRITE_NO_RESP_EVT */
		sTime.Hours=pNotification->DataTransfered.pPayload[0];
		sTime.Minutes=pNotification->DataTransfered.pPayload[1];
		sTime.Seconds=pNotification->DataTransfered.pPayload[2];
		sTime.SecondFraction=0;
		sTime.SubSeconds = 0x0;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_Handler();
		}
		sDate.WeekDay = pNotification->DataTransfered.pPayload[3];
		sDate.Month = pNotification->DataTransfered.pPayload[4];
		sDate.Date = pNotification->DataTransfered.pPayload[5];
		sDate.Year = pNotification->DataTransfered.pPayload[6];  //(1980+20) + value(0-79)
		for(uint8_t i=0;i<pNotification->DataTransfered.Length;i++){
			printf("Input_time[%d]:%d\n",i,pNotification->DataTransfered.pPayload[i]);
		}
		if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_Handler();
		}
      /* USER CODE END CUSTOM_STM_TIME_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_LOG_DATA_TX_STATUS_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_LOG_DATA_TX_STATUS_NOTIFY_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_LOG_DATA_TX_STATUS_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_LOG_DATA_TX_STATUS_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_LOG_DATA_TX_STATUS_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_LOG_DATA_TX_STATUS_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_OTA_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_OTA_WRITE_NO_RESP_EVT */
    	*(uint32_t*)SRAM1_BASE = *(uint32_t*)pNotification->DataTransfered.pPayload;
    	 printf("Reset\n");
    	 NVIC_SystemReset();
      /* USER CODE END CUSTOM_STM_OTA_WRITE_NO_RESP_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_STM_App_Notification_default */

      /* USER CODE END CUSTOM_STM_App_Notification_default */
      break;
  }
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

  /* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_APP_Notification_1 */

  /* USER CODE END CUSTOM_APP_Notification_1 */

  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

    /* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
    case CUSTOM_CONN_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */

      /* USER CODE END CUSTOM_CONN_HANDLE_EVT */
      break;

    case CUSTOM_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */

      /* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_APP_Notification_default */

      /* USER CODE END CUSTOM_APP_Notification_default */
      break;
  }

  /* USER CODE BEGIN CUSTOM_APP_Notification_2 */

  /* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void)
{
  /* USER CODE BEGIN CUSTOM_APP_Init */
	printf("fkgklfkhkkfhlfhkkhffk\n");
	PWR_GD=HAL_GPIO_ReadPin(GPIOC,PWR_GD_Pin);
	EOC_v=HAL_GPIO_ReadPin(GPIOC,EOC_Pin);
	Zmod4410_init();
	LPS25HB_INIT();
	ENV_APPInit();
	Init_Tx_Log();
	Notifiction();
	TS_Toggle();
	Get_Zmod4410();

  /* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/* Battery_Service */
void Custom_Bat_level_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bat_level_UC_1*/
  Custom_STM_App_Update_Char(CUSTOM_STM_BAT_LEVEL, (uint8_t *)&Battery_percentage);
  /* USER CODE END Bat_level_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_BAT_LEVEL, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Bat_level_UC_Last*/

  /* USER CODE END Bat_level_UC_Last*/
  return;
}

void Custom_Bat_level_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bat_level_NS_1*/
  Custom_STM_App_Update_Char(CUSTOM_STM_BAT_LEVEL, (uint8_t *)&Battery_percentage);
  /* USER CODE END Bat_level_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_BAT_LEVEL, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Bat_level_NS_Last*/

  /* USER CODE END Bat_level_NS_Last*/

  return;
}

void Custom_Eoc_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Eoc_UC_1*/

  /* USER CODE END Eoc_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_EOC, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Eoc_UC_Last*/

  /* USER CODE END Eoc_UC_Last*/
  return;
}

void Custom_Eoc_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Eoc_NS_1*/

  /* USER CODE END Eoc_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_EOC, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Eoc_NS_Last*/
	Custom_STM_App_Update_Char(CUSTOM_STM_EOC, (uint8_t *)&EOC_v);
  /* USER CODE END Eoc_NS_Last*/

  return;
}

void Custom_Pwr_gd_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Pwr_gd_UC_1*/

  /* USER CODE END Pwr_gd_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_PWR_GD, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Pwr_gd_UC_Last*/
	Custom_STM_App_Update_Char(CUSTOM_STM_EOC, (uint8_t *)&PWR_GD);
  /* USER CODE END Pwr_gd_UC_Last*/
  return;
}

void Custom_Pwr_gd_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Pwr_gd_NS_1*/

  /* USER CODE END Pwr_gd_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_PWR_GD, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Pwr_gd_NS_Last*/
  Custom_STM_App_Update_Char(CUSTOM_STM_EOC, (uint8_t *)&PWR_GD);
  /* USER CODE END Pwr_gd_NS_Last*/

  return;
}

/* Gases */
void Custom_Iaq_in_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Iaq_in_UC_1*/

  /* USER CODE END Iaq_in_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_IAQ_IN, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Iaq_in_UC_Last*/

  /* USER CODE END Iaq_in_UC_Last*/
  return;
}

void Custom_Iaq_in_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Iaq_in_NS_1*/

	uint8_t ENV_data[4];
	STORE_FLOAT_TO_BYTES(algo_results.iaq,ENV_data);
	memcpy(Log+11,ENV_data,sizeof(float));
  /* USER CODE END Iaq_in_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_IAQ_IN, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Iaq_in_NS_Last*/
	Custom_STM_App_Update_Char(CUSTOM_STM_IAQ_IN, (uint8_t *)ENV_data);
  /* USER CODE END Iaq_in_NS_Last*/

  return;
}

void Custom_Relative_ia_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Relative_ia_UC_1*/

  /* USER CODE END Relative_ia_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_RELATIVE_IA, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Relative_ia_UC_Last*/

  /* USER CODE END Relative_ia_UC_Last*/
  return;
}

void Custom_Relative_ia_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Relative_ia_NS_1*/
	uint8_t ENV_data[4];
	STORE_FLOAT_TO_BYTES(algo_results.rel_iaq,ENV_data);
	memcpy(Log+7,ENV_data,sizeof(float));
  /* USER CODE END Relative_ia_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_RELATIVE_IA, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Relative_ia_NS_Last*/
	Custom_STM_App_Update_Char(CUSTOM_STM_RELATIVE_IA, (uint8_t *)ENV_data);
  /* USER CODE END Relative_ia_NS_Last*/

  return;
}

void Custom_Tvoc_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Tvoc_UC_1*/

  /* USER CODE END Tvoc_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_TVOC, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Tvoc_UC_Last*/

  /* USER CODE END Tvoc_UC_Last*/
  return;
}

void Custom_Tvoc_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Tvoc_NS_1*/
	uint8_t ENV_data[4];
	STORE_FLOAT_TO_BYTES(algo_results.tvoc,ENV_data);
	memcpy(Log+19,ENV_data,sizeof(float));
  /* USER CODE END Tvoc_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_TVOC, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Tvoc_NS_Last*/
	Custom_STM_App_Update_Char(CUSTOM_STM_TVOC, (uint8_t *)ENV_data);
  /* USER CODE END Tvoc_NS_Last*/

  return;
}

void Custom_Etoh_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Etoh_UC_1*/

  /* USER CODE END Etoh_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_ETOH, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Etoh_UC_Last*/

  /* USER CODE END Etoh_UC_Last*/
  return;
}

void Custom_Etoh_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Etoh_NS_1*/
	uint8_t ENV_data[4];
	STORE_FLOAT_TO_BYTES(algo_results.tvoc,ENV_data);
	memcpy(Log+15,ENV_data,sizeof(float));
  /* USER CODE END Etoh_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_ETOH, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Etoh_NS_Last*/
	Custom_STM_App_Update_Char(CUSTOM_STM_ETOH, (uint8_t *)ENV_data);
  /* USER CODE END Etoh_NS_Last*/

  return;
}

void Custom_Co2_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Co2_UC_1*/

  /* USER CODE END Co2_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_CO2, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Co2_UC_Last*/

  /* USER CODE END Co2_UC_Last*/
  return;
}

void Custom_Co2_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Co2_NS_1*/
	uint8_t ENV_data[4];
	STORE_FLOAT_TO_BYTES(algo_results.eco2,ENV_data);
	memcpy(Log+23,ENV_data,sizeof(float));
  /* USER CODE END Co2_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_CO2, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Co2_NS_Last*/
	Custom_STM_App_Update_Char(CUSTOM_STM_CO2, (uint8_t *)ENV_data);
  /* USER CODE END Co2_NS_Last*/

  return;
}

/* Enviroment */
void Custom_Temp_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Temp_UC_1*/

  /* USER CODE END Temp_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_TEMP, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Temp_UC_Last*/

  /* USER CODE END Temp_UC_Last*/
  return;
}

void Custom_Temp_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Temp_NS_1*/
	uint8_t ENV_data[4];
	STORE_FLOAT_TO_BYTES(algo_results.temperature,ENV_data);
	memcpy(Log+27,ENV_data,sizeof(float));
  /* USER CODE END Temp_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_TEMP, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Temp_NS_Last*/
	Custom_STM_App_Update_Char(CUSTOM_STM_TEMP, (uint8_t *)ENV_data);
  /* USER CODE END Temp_NS_Last*/

  return;
}

void Custom_Humi_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Humi_UC_1*/

  /* USER CODE END Humi_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_HUMI, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Humi_UC_Last*/
	Custom_STM_App_Update_Char(CUSTOM_STM_HUMI, (uint8_t *)UpdateCharData);
  /* USER CODE END Humi_UC_Last*/
  return;
}

void Custom_Humi_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Humi_NS_1*/
	uint8_t ENV_data[4];
	STORE_FLOAT_TO_BYTES(htResults.humidity,ENV_data);
	memcpy(Log+31,ENV_data,sizeof(float));
  /* USER CODE END Humi_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_HUMI, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Humi_NS_Last*/
	Custom_STM_App_Update_Char(CUSTOM_STM_HUMI, (uint8_t *)ENV_data);
  /* USER CODE END Humi_NS_Last*/

  return;
}

void Custom_Pres_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Pres_UC_1*/

  /* USER CODE END Pres_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_PRES, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Pres_UC_Last*/

  /* USER CODE END Pres_UC_Last*/
  return;
}

void Custom_Pres_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Pres_NS_1*/
	uint8_t ENV_data[4];
	STORE_FLOAT_TO_BYTES(LPS25hb_data[0],ENV_data);
	memcpy(Log+35,ENV_data,sizeof(float));
  /* USER CODE END Pres_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_PRES, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Pres_NS_Last*/
	Custom_STM_App_Update_Char(CUSTOM_STM_PRES, (uint8_t *)ENV_data);
  /* USER CODE END Pres_NS_Last*/

  return;
}

void Custom_Get_env_log_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Get_env_log_UC_1*/

  /* USER CODE END Get_env_log_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_GET_ENV_LOG, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Get_env_log_UC_Last*/

  /* USER CODE END Get_env_log_UC_Last*/
  return;
}

void Custom_Get_env_log_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Get_env_log_NS_1*/

  /* USER CODE END Get_env_log_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_GET_ENV_LOG, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Get_env_log_NS_Last*/

  /* USER CODE END Get_env_log_NS_Last*/

  return;
}

/* Lightining_Strike */
void Custom_Get_lightning_log_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Get_lightning_log_UC_1*/

  /* USER CODE END Get_lightning_log_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_GET_LIGHTNING_LOG, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Get_lightning_log_UC_Last*/

  /* USER CODE END Get_lightning_log_UC_Last*/
  return;
}

void Custom_Get_lightning_log_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Get_lightning_log_NS_1*/

  /* USER CODE END Get_lightning_log_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_GET_LIGHTNING_LOG, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Get_lightning_log_NS_Last*/

  /* USER CODE END Get_lightning_log_NS_Last*/

  return;
}

void Custom_Strike_distance_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Strike_distance_UC_1*/

  /* USER CODE END Strike_distance_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_STRIKE_DISTANCE, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Strike_distance_UC_Last*/

  /* USER CODE END Strike_distance_UC_Last*/
  return;
}

void Custom_Strike_distance_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Strike_distance_NS_1*/

  /* USER CODE END Strike_distance_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_STRIKE_DISTANCE, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Strike_distance_NS_Last*/
  uint8_t Blink;
  if (Lightining_Strike_Distance < 5) {
          Blink = 6;
      } else if (Lightining_Strike_Distance < 15) {
          Blink = 4;
      } else {
          Blink = 2;
          }
  	  Lightning_led(100,Blink);
	Custom_STM_App_Update_Char(CUSTOM_STM_STRIKE_DISTANCE, (uint8_t *)&Lightining_Strike_Distance);
  /* USER CODE END Strike_distance_NS_Last*/

  return;
}

/* Utilities */
void Custom_Log_data_tx_status_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Log_data_tx_status_UC_1*/

  /* USER CODE END Log_data_tx_status_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_LOG_DATA_TX_STATUS, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Log_data_tx_status_UC_Last*/

  /* USER CODE END Log_data_tx_status_UC_Last*/
  return;
}

void Custom_Log_data_tx_status_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Log_data_tx_status_NS_1*/

  /* USER CODE END Log_data_tx_status_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_LOG_DATA_TX_STATUS, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Log_data_tx_status_NS_Last*/

  /* USER CODE END Log_data_tx_status_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/
void Notifiction(void)
{
	UTIL_SEQ_RegTask( 1<< CFG_TASK_INTR_ID, UTIL_SEQ_RFU,INTR_NOTIFY);
	HW_TS_Create(CFG_TIM_PROC_ID_ISR,&(Noti_TIMER_ID),hw_ts_SingleShot, Noti_CALL_BACK);
}
void ENV_APPInit(void)
{
	UTIL_SEQ_RegTask( 1<< CFG_TASK_ENV_ID, UTIL_SEQ_RFU,Get_Zmod4410); //Get_Zmod4410); //register task
	HW_TS_Create(CFG_TIM_PROC_ID_ISR,&(ENV_TIMER_ID), hw_ts_Repeated, ENV_CALL_BACK);

	HW_TS_Start(ENV_TIMER_ID,ENV_UPDATE_PERIOD);//	 UTIL_SEQ_SetTask( 1<<CFG_TASK_BME_ID, CFG_SCH_PRIO_0);  // set to be run and in main.c it start execution by calling UTIL_SEQ_RunTask in while(1) loop
}
void TS_Toggle(void)
{
	UTIL_SEQ_RegTask( 1<< CFG_TASK_TS_ID, UTIL_SEQ_RFU,TS); //Get_Zmod4410); //register task
	HW_TS_Create(CFG_TIM_PROC_ID_ISR,&(Ts_TIMER_ID), hw_ts_SingleShot, TS_CALL_BACK);
}
void TS_CALL_BACK(void)
{
	UTIL_SEQ_SetTask(1<<CFG_TASK_TS_ID,CFG_SCH_PRIO_0);
}
void TS(void)
{
	HAL_GPIO_WritePin(GPIOA,TS_Pin,GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA,TS_Pin,GPIO_PIN_SET);
	printf("EO_v_interrupt %d \r\n",EOC_v);
}
void Noti_CALL_BACK(void)
{
	UTIL_SEQ_SetTask(1<<CFG_TASK_INTR_ID,CFG_SCH_PRIO_0);
}
void ENV_CALL_BACK(void)
{
	//	 printf("call_back\n");
	// uint16_t el=linkTimer(ENV_TIMER_ID);

	uint32_t status = UTIL_SEQ_IsPauseTask( 1<<CFG_TASK_ENV_ID);
		if(status==1)
		{
			UTIL_SEQ_ResumeTask(1<<CFG_TASK_ENV_ID);
		}
	UTIL_SEQ_SetTask( 1<<CFG_TASK_ENV_ID, CFG_SCH_PRIO_0);

}
void Zmod4410_init(void)
{
	uint8_t ret;
	ret = HAL__Init(&hal);
	if (ret) {
		HAL_HandleError(ret, "Hardware initialization");
	}
	// * and configure the sensor for the IAQ_2nd_Gen_ULP algorithm */
	dev.read = &_i2c_read_reg;
	dev.write =_i2c_write_reg;
	dev.delay_ms=Delay;
	dev.i2c_addr = ZMOD4410_I2C_ADDR;
	dev.pid = ZMOD4410_PID;
	dev.init_conf = &zmod_iaq2_ulp_sensor_cfg[INIT];
	dev.meas_conf = &zmod_iaq2_ulp_sensor_cfg[MEASUREMENT];
	dev.prod_data = prod_data;

	ret = detect_and_configure ( &dev, ZMOD4410_PROD_DATA_LEN, &errContext );
	if ( ret ) {
		HAL_HandleError ( ret, errContext );
	}

	/* Try finding a Renesas temperature and humidity sensor - example will work
	 * without it, but results will be less accurate. */
	ret = HSxxxx_Init ( &hsxxxx, &hal );
	if ( ret ) {
		printf ( "No temperature/humidity sensor found, using on-chip" );
		printf ( " temperature sensor and 50%% relative humidity!\n\n" );
	}
	else {
		htSensor = &hsxxxx;
		printf ( "Found %s humdity & temperature sensor\n\n", HSxxxx_Name ( htSensor ) );
	}
	/* Set default values for temperature and humidity: These values will be used
	 * if no sensor is detected or if reading the sensor fails.
	 * The temperature value of -300°C causes the algo to use the on-chip temperature
	 * measurement of the gas sensor. However, an external temperaturue and humidity
	 * sensor provides better accuracy and is the preferred input source. */
	htResults . temperature = -300;
	htResults . humidity    =  50;

	/* One-time initialization of the algorithm. Handle passed to calculation
	 * function. */
	ret = init_iaq_2nd_gen_ulp(&algo_handle);
	if (ret) {
		HAL_HandleError(ret, "Algorithm initialization");
	}

}

void Get_Zmod4410(void)
{
	uint8_t ret;
	uint32_t  remaining_period_time = ZMOD4410_IAQ2_ULP_SAMPLE_TIME;
	ret = zmod4xxx_start_measurement(&dev);
	if (ret) {
		HAL_HandleError(ret, "Starting measurement");
	}
	/* If a sensor was detected, read temperature and humidity from it.
	 * Errors occuring during read are ignored: In case of error the
	 * HSxxx API leaves data in the result data structure unmodified. */
	if ( htSensor ) {
		HSxxxx_Measure( htSensor, &htResults );
	}

	/* Wait for the measurement to be complete (for immediate result printing).
	 * Addtional delay is inserted at the end of this loop to keep proper
	 * measurement timing and keep algorithm accuracy. For more information,
	 * read the Programming Manual, section "Interrupt Usage and Measurement
	 * Timing". */
	dev.delay_ms(ZMOD4410_IAQ2_ULP_SEQ_RUN_TIME_WITH_MARGIN);

	remaining_period_time -= ZMOD4410_IAQ2_ULP_SEQ_RUN_TIME_WITH_MARGIN + HSxxxx_MeasurementDuration ( htSensor );

	read_and_verify ( &dev, adc_result, "ZMOD4410" );

	/* Assign algorithm inputs: raw sensor data and ambient conditions. */
	algo_input.adc_result       = adc_result;
	algo_input.humidity_pct     = htResults.humidity;
	algo_input.temperature_degc = htResults.temperature;

	/* Calculate algorithm results. */
	ret = calc_iaq_2nd_gen_ulp(&algo_handle, &dev, &algo_input, &algo_results);

	printf ( "*********** Measurements ***********\n" );
	//	        for (int i = 0; i < 13; i++) {
	//	            printf ( " Rmox[%2d]    = %8.3f kOhm\n", i, algo_results.rmox[i] / 1e3 );
	//	        }
	//printf ( " Rcda        = %8.3f kOhm\n", pow(10, algo_results.Log_nonLog_rcda[0]) / 1e3 );
	printf ( " EtOH        = %8.2f ppm\n", algo_results.etoh );
	printf ( " TVOC        = %8.2f mg/m^3\n", algo_results.tvoc );
	printf ( " IAQ         = %8.1f\n", algo_results.iaq );
	printf ( " eCO2        = %8.0f ppm\n", algo_results.eco2 );
	printf ( " rel_IAQ     = %8.1f\n", algo_results.rel_iaq );
	printf ( " TEMP     = %8.1f\n", algo_results.temperature );
	if ( htSensor ) {
		printf ( " Humidity    = %8.2f %%rH\n", htResults.humidity );
		printf ( " Temperature = %8.2f degC\n", htResults.temperature );
	}

	/* Check validity of the algorithm results. */
	switch (ret) {
	case IAQ_2ND_GEN_ULP_STABILIZATION:
		/* The sensor should run for at least 10 cycles to stabilize.
		 * Algorithm results obtained during this period SHOULD NOT be
		 * considered as valid outputs! */
		printf ( "Warm-Up!\n" );
		break;
	case IAQ_2ND_GEN_ULP_OK:
		printf ( "Valid\n" );
		break;
		/* Notification from Sensor self-check. For more information, read the
		 * Datasheet, section "Conditioning, Sensor Self-Check Status, and
		 * Stability". */
	case IAQ_2ND_GEN_ULP_DAMAGE:
		printf ( "Error: Sensor probably damaged. Algorithm results may be incorrect.\n" );
		break;
		/* Exit program due to unexpected error. */
	default:
		HAL_HandleError(ret, "Algorithm calculation");
	}

	/* Wait for the reaining time to keep proper measurement timing */
	//dev.delay_ms(remaining_period_time);
	ret=LPS25HB_GET_DATA(LPS25hb_data);
	if(ret==0)
	{
		printf("Pressure:%f\n",LPS25hb_data[0]);
	}
	if(Log_Track==5)
		{
			Log_Data(1);
			Log_Track=0;
		}
		else
		{
			Log_Track+=1;
		}
	if(algo_results.iaq<=1.99)
	{
		LED=1;
	}
	else if(algo_results.iaq<=2.99)
	{
		LED=2;
	}
	else if(algo_results.iaq<=3.99)
	{
		LED=3;
	}
	else if(algo_results.iaq<=4.99)
	{
		LED=4;
	}
	else if(algo_results.iaq<=5.00)
	{
		LED=5;
	}
	else{
		LED=0;
	}
	IAQ_Status(LED);
	Battery();
	Custom_Iaq_in_Send_Notification();
	Custom_Relative_ia_Send_Notification();
	Custom_Tvoc_Send_Notification();
	Custom_Etoh_Send_Notification();
	Custom_Co2_Send_Notification();
	/* Enviroment */
	Custom_Temp_Send_Notification();
	Custom_Humi_Send_Notification();
	Custom_Pres_Send_Notification();
	Custom_Bat_level_Send_Notification();
	//static void Custom_Get_bme_Log_Update_Char(void);
	//static void Custom_Get_bme_Log_Send_Notification(void);
}
int detect_and_configure ( zmod4xxx_dev_t*  sensor, int  pd_len, char const** errContext ) {
	uint8_t  track_number [ ZMOD4XXX_LEN_TRACKING ];
	uint8_t ret;
	ret = zmod4xxx_init_sensor(sensor);
	if ( ret ) {
		*errContext = "sensor initialization";
		return ret;
	}

	/* Read product ID and configuration parameters. */
	ret = zmod4xxx_read_sensor_info(sensor);
	if (ret) {
		*errContext = "reading sensor information";
		return ret;
	}

	/* Retrieve sensors unique tracking number and individual trimming information.
	 * Provide this information when requesting support from Renesas.
	 * Otherwise this function is not required for gas sensor operation. */
	ret = zmod4xxx_read_tracking_number(sensor, track_number);
	if (ret) {
		*errContext = "Reading tracking number";
		return ret;
	}
	printf ( "Sensor tracking number: x0000" );
	for (int i = 0; i < sizeof(track_number); i++) {
		printf ( "%02X", track_number[i] );
	}
	printf ( "\n" );
	printf ( "Sensor trimming data:" );
	for (int i = 0; i < pd_len; i++) {
		printf ( " %i", prod_data[i] );
	}
	printf ( "\n" );


	/* Start the cleaning procedure. Check the Datasheet on indications
	 * of usage. IMPORTANT NOTE: The cleaning procedure can be run only once
	 * during the modules lifetime and takes 1 minute (blocking). */
	//    printf("Starting cleaning procedure. This might take up to 1 min ...\n");
	//    ret = zmod4xxx_cleaning_run(sensor);
	//    if (ERROR_CLEANING == ret) {
	//        printf("Skipping cleaning procedure. It has already been performed\n");
	//    } else if (ret) {
	//        *errContext = "sensor cleaning";
	//        return ret;
	//    }
	/* Determine calibration parameters and configure measurement. */
	ret = zmod4xxx_prepare_sensor(sensor);
	if (ret) {
		*errContext = "sensor preparation";
		return ret;
	}
	return 0;
}

/* This function read the gas sensor results and checks for result validity. */
void read_and_verify ( zmod4xxx_dev_t*  sensor, uint8_t*  result, char const*  id ) {
	/* Verify completion of measurement sequence. */
	uint8_t ret;
	ret = zmod4xxx_read_status(sensor, &zmod4xxx_status);
	if (ret) {
		HAL_HandleError(ret, "Reading sensor status");
	}
	/* Check if measurement is running. */
	if (zmod4xxx_status & STATUS_SEQUENCER_RUNNING_MASK) {
		/* Check if reset during measurement occured. For more information,
		 * read the Programming Manual, section "Error Codes". */
		ret = zmod4xxx_check_error_event(sensor);
		switch (ret) {
		case ERROR_POR_EVENT:
			HAL_HandleError(ret, "Result readout: Unexpected sensor reset!");
			break;
		case ZMOD4XXX_OK:
			HAL_HandleError(ret, "Reading result: Wrong sensor setup!");
			break;
		default:
			HAL_HandleError(ret, "Reading result: Unknown error!");
			break;
		}
	}
	/* Read sensor ADC output. */
	ret = zmod4xxx_read_adc_result(sensor, result);
	if (ret) {
		HAL_HandleError(ret, "Reading ADC results");
	}

	/* Check validity of the ADC results. For more information, read the
	 * Programming Manual, section "Error Codes". */
	ret = zmod4xxx_check_error_event(sensor);
	if (ret) {
		HAL_HandleError(ret, "Reading sensor status");
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	printf("interrupt \n");
	if(GPIO_Pin == PWR_GD_Pin)
	{

		PWR_GD=HAL_GPIO_ReadPin(GPIOC,PWR_GD_Pin);
		printf("PWR_GD_interrupt %d \r\n",PWR_GD);

	}
	if(GPIO_Pin == EOC_Pin)
	{
		printf("interrupt 2\n");
		EOC_v=HAL_GPIO_ReadPin(GPIOC,EOC_Pin);
		PWR_GD=HAL_GPIO_ReadPin(GPIOC,PWR_GD_Pin);
		HW_TS_Start(Ts_TIMER_ID,Ts_UPDATE_PERIOD);

	}
	if(GPIO_Pin == Lightining_intr_Pin)
	{
		Lightining_Strike_Distance=AS3935_reading_distance();
		New_Lightining_data=(Lightining_Strike_Distance!=0xff)?1:0;
		printf("Lighting_Stirke_Distance %d & %d \n",Lightining_Strike_Distance,New_Lightining_data);
	}
	HW_TS_Start(Noti_TIMER_ID,Noti_UPDATE_PERIOD);//Noti_UPDATE_PERIOD
	//UTIL_SEQ_SetTask(1<<CFG_TASK_INTR_ID,CFG_SCH_PRIO_1);



}
void INTR_NOTIFY(void)
{

	if(New_Lightining_data==1){
		Custom_Strike_distance_Send_Notification();
		Log_Data(2);
		New_Lightining_data=0;
	}
	else
	{
	Custom_Pwr_gd_Send_Notification();
	Custom_Eoc_Send_Notification();
	Battery();
	Custom_Bat_level_Send_Notification();
	IAQ_Status(LED);
	}

}
void Date_Time(void)
{
	if (HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
		printf("Time_fail\n");
	}
	if (HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{

		printf("Date_fail\n");
		Error_Handler();
	}
	Time[0]=sTime.Hours;
	Time[1]=sTime.Minutes;
	Time[2]=sTime.Seconds;
	Time[3]=sDate.WeekDay;
	Time[4]=sDate.Date;
	Time[5]=sDate.Month;
	Time[6]=sDate.Year;
	for(uint8_t i=0;i<7;i++){
		printf("T[%d]:%d\n",i,Time[i]);
	}
}

void Log_Data(uint8_t select)
{
	uint32_t current_add = Read_flash_address(select);
	Date_Time();
	memcpy(Log,Time,sizeof(Time));
	if(select == 1)
	{

		if(current_add>=flash_end_address_Env)
		{
			update_address(flash_end_address_Env,1);
			current_add=Read_flash_address(1);
			Quad_Write(Log,current_add,39);
			update_address(current_add+39,1);
		}
		else
		{
			Quad_Write(&Log,current_add,39);
			update_address(current_add+39,1);
		}

	}
	if(select == 2)
	{
		uint8_t data[8];
		memcpy(data,Time,sizeof(Time));
		Log[7]=Lightining_Strike_Distance;
		if(current_add>=flash_end_address_Env)
		{
			update_address(flash_end_address_LT,1);
			current_add=Read_flash_address(2);
			Quad_Write(data,current_add,8);
			update_address(current_add+8,2);
		}
		else
		{
			Quad_Write(&Log,current_add,8);
			update_address(current_add+8,2);
		}
	}
}

void TX_Log(void)
{
	//uint8_t select =*(uint8_t*) Select;
	if(Current_Address <= flash_end_address_Env)
	{
		//uint32_t add=Read_flash_address(1);
		//printf("current_add %d \n",add);
		uint32_t data=Current_Address-iteration_address1;
		printf("Trasmiting Wine Temperature Log Data \n");
		if(data>247)
		{
			// uint8_t read_buffer[247];
			Quad_Read(NotifyCharData,iteration_address1,247);
			Custom_STM_App_Update_Char(CUSTOM_STM_GET_ENV_LOG, (uint8_t *)NotifyCharData);
			// sl_bt_gatt_server_notify_all(gattdb_Log,247,read_buffer);
			iteration_address1+=247;
		}
		if(data<=247 && data!=0)
		{
			uint8_t read_buffer[data];
			Quad_Read(read_buffer,iteration_address1,data);
			Custom_STM_App_Update_Char(CUSTOM_STM_GET_ENV_LOG, (uint8_t *)NotifyCharData);
			//sl_bt_gatt_server_notify_all(gattdb_Log,data,read_buffer);
			iteration_address1+=data;
		}
		if(data==0)
		{
			iteration_address1=flash_start_address_Env;    // reset initial address
			UTIL_SEQ_PauseTask(1<<CFG_LOG_TX_ID);
			HW_TS_Stop(TX_LOG_ID);
			HW_TS_Start(ENV_TIMER_ID,ENV_UPDATE_PERIOD);
			// sl_simple_timer_stop(&Log_tx);
		}
	}
	else if(Current_Address >= flash_start_address_LT && Current_Address <= flash_end_address_LT)
	{
		uint32_t add=Read_flash_address(2);
		printf("current_add %d \n",add);
		uint32_t data=Current_Address-iteration_address2;
		printf("Trasmiting Wine Temperature Log Data \n");
		if(data>247)
		{
			// uint8_t read_buffer[247];
			Quad_Read(NotifyCharData,iteration_address2,247);
			Custom_STM_App_Update_Char(CUSTOM_STM_GET_LIGHTNING_LOG, (uint8_t *)NotifyCharData);
			// sl_bt_gatt_server_notify_all(gattdb_Log,247,read_buffer);
			iteration_address1+=247;
		}
		if(data<=247 && data!=0)
		{
			uint8_t read_buffer[data];
			Quad_Read(read_buffer,iteration_address2,data);
			Custom_STM_App_Update_Char(CUSTOM_STM_GET_LIGHTNING_LOG, (uint8_t *)NotifyCharData);
			//sl_bt_gatt_server_notify_all(gattdb_Log,data,read_buffer);
			iteration_address1+=data;
		}
		if(data==0)
		{
			iteration_address2=flash_start_address_LT;    // reset initial address
			UTIL_SEQ_PauseTask(1<<CFG_LOG_TX_ID);
			HW_TS_Stop(TX_LOG_ID);
			HW_TS_Start(ENV_TIMER_ID,ENV_UPDATE_PERIOD);
			// sl_simple_timer_stop(&Log_tx);
		}
	}
	else
	{
		printf("Error: Wrong Address %d \n",Current_Address);
	}
}
void Init_Tx_Log(void)
{
	UTIL_SEQ_RegTask(1 << CFG_LOG_TX_ID, UTIL_SEQ_RFU,TX_Log);
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(TX_LOG_ID), hw_ts_Repeated,TX_CALLBACK);
}
void TX_CALLBACK(void)
{
//	UTIL_SEQ_PauseTask(1<<CFG_TASK_ENV_ID);
	uint32_t status = UTIL_SEQ_IsPauseTask( 1<<CFG_LOG_TX_ID);
	if(status==1)
	{
		UTIL_SEQ_ResumeTask(1<<CFG_LOG_TX_ID);
	}
	UTIL_SEQ_SetTask(1<<CFG_LOG_TX_ID,CFG_SCH_PRIO_0);
}

void Battery(void)
{
	uint16_t Voltage,Battery;
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&Battery,1);
	Voltage = ((Battery *3300)/4096);
	Battery_percentage=(uint8_t)(((Voltage-3000)/1200)*100);

}
void Lightning_led(uint16_t delay,uint8_t No_flash)
{
  for(uint8_t i=0;i<No_flash;i++)
    {
	    HAL_GPIO_WritePin(GPIOD,WHT_Pin,GPIO_PIN_SET);
	  	HAL_Delay(delay);
	  	HAL_GPIO_WritePin(GPIOD,WHT_Pin,GPIO_PIN_RESET);
    }
}
void IAQ_Status(uint8_t state)
{
	switch(state){
	case 1:
		if(PWR_GD==0){
			HAL_GPIO_WritePin(GPIOD,GRN_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,YEL_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,RED_Pin,GPIO_PIN_SET);
		}
		else
		{	IAQ_led_RESET();
		for(uint8_t i=0;i<3;i++)
		{
			IAQ_Blink(GRN_Pin,GRN_Pin);
		}
		}
		break;
	case 2:
		if(PWR_GD==0){
			HAL_GPIO_WritePin(GPIOD,YEL_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,GRN_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,RED_Pin,GPIO_PIN_SET);
		}
		else
		{	IAQ_led_RESET();
		for(uint8_t i=0;i<3;i++)
		{
			IAQ_Blink(GRN_Pin,YEL_Pin);
		}
		}
		break;
	case 3:
		if(PWR_GD==0){
			HAL_GPIO_WritePin(GPIOD,YEL_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,GRN_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,RED_Pin,GPIO_PIN_SET);
		}
		else
		{
			IAQ_led_RESET();
			for(uint8_t i=0;i<3;i++)
			{
				IAQ_Blink(YEL_Pin,YEL_Pin);
			}
		}
		break;
	case 4:
		if(PWR_GD==0){
			HAL_GPIO_WritePin(GPIOD,YEL_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,GRN_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,RED_Pin,GPIO_PIN_RESET);
		}
		else
		{	IAQ_led_RESET();
		for(uint8_t i=0;i<3;i++)
		{
			IAQ_Blink(YEL_Pin,RED_Pin);
		}
		}
		break;
	case 5:
		if(PWR_GD==0){
			HAL_GPIO_WritePin(GPIOD,YEL_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GRN_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,RED_Pin,GPIO_PIN_RESET);
		}
		else
		{	IAQ_led_RESET();
		for(uint8_t i=0;i<3;i++)
		{
			IAQ_Blink(RED_Pin,RED_Pin);
		}
		}
		break;
	default:
		IAQ_led_RESET();
		break;
	}}
void IAQ_Blink(uint16_t led,uint16_t led2)
	{
		HAL_GPIO_WritePin(GPIOD,led,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,led2,GPIO_PIN_RESET);
		HAL_Delay(250);
		IAQ_led_RESET();
	}
void IAQ_led_RESET(void)
{
	HAL_GPIO_WritePin(GPIOD,GRN_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,YEL_Pin,GPIO_PIN_SET);
}
/* USER CODE END FD_LOCAL_FUNCTIONS*/
