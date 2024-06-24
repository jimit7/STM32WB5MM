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
#include "Acclx.h"
#include "rtc.h"
#include "flash.h"
#include "UV.h"
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* Battery_Service */
  /* Temperature */
  uint8_t               Ir_t_Notification_Status;
  uint8_t               L_t_Notification_Status;
  uint8_t               R_n_Notification_Status;
  /* UV_Sensor */
  uint8_t               Uv_v_Notification_Status;
  uint8_t               Uv_l_Notification_Status;
  uint8_t               Ambi_temp_Notification_Status;
  /* Utility */
  uint8_t               Ec_Notification_Status;
  uint8_t               Pwr_gd_Notification_Status;
  /* OperationMode */
  /* USER CODE BEGIN CUSTOM_APP_Context_t */

  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define TEMP_UPDATE_PERIOD       (uint32_t)(1000*1000/CFG_TS_TICK_VAL)
#define UV_UPDATE_PERIOD	     (uint32_t)(3*1000*1000/CFG_TS_TICK_VAL)
#define LOG_UPDATE_PERIOD	     (uint32_t)(0.5*1000*1000/CFG_TS_TICK_VAL)
#define Ts_UPDATE_PERIOD       (uint32_t)(0.5*1000000/CFG_TS_TICK_VAL)
#define PWR_UPDATE_PERIOD       (uint32_t)(1*1000000/CFG_TS_TICK_VAL)
//#define flash_address_map 2093056
//#define start_address_Temp 0
//#define end_address_Temp 1040370
//#define start_address_UV 1040384
//#define end_address_UV 2093055
/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

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
uint32_t TEMP_UPDATE_PERIOD;
float *Temperature;
uint8_t Interval=60;


uint8_t T[4];


uint8_t AT[4];
int16_t Temp_lower_limit =-21;
int16_t Temp_upper_limit=86;
uint8_t mode;
int16_t Desire_temp=100;
int8_t opmode[4];
uint8_t global_storage_count=0;
uint8_t Time[7];
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

uint32_t ADC_Volt=0;
uint32_t ADC_Temp=0;
uint8_t Battery_Percentage=0;
uint8_t Dev_Temperature=0;
volatile uint32_t iteration_address1=4096;
volatile uint32_t iteration_address2=1044480;
uint8_t TEMP_TIMER_ID,UV_TIMER_ID,TEMP_LOG_ID,UV_LOG_ID,Ts_TIMER_ID,PWR_TIMER_ID;
AccelData Data;
uint8_t UV_Data;
float f_UV_Data;
uint16_t ADC_Data[2];
char Firmware[]="Wine_UV_5.0.0.06.24";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* Battery_Service */
/* Temperature */
static void Custom_Ir_t_Update_Char(void);
static void Custom_Ir_t_Send_Notification(void);
static void Custom_L_t_Update_Char(void);
static void Custom_L_t_Send_Notification(void);
static void Custom_R_n_Update_Char(void);
static void Custom_R_n_Send_Notification(void);
/* UV_Sensor */
static void Custom_Uv_v_Update_Char(void);
static void Custom_Uv_v_Send_Notification(void);
static void Custom_Uv_l_Update_Char(void);
static void Custom_Uv_l_Send_Notification(void);
static void Custom_Ambi_temp_Update_Char(void);
static void Custom_Ambi_temp_Send_Notification(void);
/* Utility */
static void Custom_Ec_Update_Char(void);
static void Custom_Ec_Send_Notification(void);
static void Custom_Pwr_gd_Update_Char(void);
static void Custom_Pwr_gd_Send_Notification(void);
/* OperationMode */

/* USER CODE BEGIN PFP */
void Send_Temp_Notification(void);
void log_data(uint8_t select);
void Date_Time(void);
static void TEMP_APP_Init(void);
static void TEMP_Timer_Callback(void);
void Send_UV_Notification(void);
static void UV_APP_Init(void);
static void UV_Timer_Callback(void);
void Analog_Temp(void);
void Battery(void);
void Tx_log_Temp(void);
void Tx_log_UV(void);
static void Log_Tx_Init(void);
static void Log_Temp_Callback(void);
//tatic void Log_UV_Init(void);
static void Log_UV_Callback(void);
void PWR_EOC(void);
void PWR_EOC_CALLBACK(void);
void PWR_EOC_Send_Noti(void);
void Measure_Temp(void);
void TS_Toggle(void);
void TS_CALL_BACK(void);
void TS(void);
void Init_mode(void);
//void Mode_Callback(void);
//void Update_mode_task(void);
void Update_mode(void);
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
    case CUSTOM_STM_LEVEL_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_LEVEL_READ_EVT */
		printf("\rBattery_level\n");
		Battery();
		UpdateCharData[0]=Battery_Percentage;
		Custom_STM_App_Update_Char(CUSTOM_STM_LEVEL, (uint8_t *)UpdateCharData);
      /* USER CODE END CUSTOM_STM_LEVEL_READ_EVT */
      break;

    /* Temperature */
    case CUSTOM_STM_IR_T_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_IR_T_READ_EVT */
    	Send_Temp_Notification();
      /* USER CODE END CUSTOM_STM_IR_T_READ_EVT */
      break;

    case CUSTOM_STM_IR_T_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_IR_T_NOTIFY_ENABLED_EVT */
		printf("Temp_NOTIFY_ENABLED\n");
		UTIL_SEQ_PauseTask(1<<CFG_UV_ID);
		UTIL_SEQ_PauseTask(1<<CFG_UV_ID);
		UTIL_SEQ_PauseTask(1<<CFG_LOG_TEMP_ID);
		UTIL_SEQ_PauseTask(1<<CFG_LOG_UV_ID);
		HW_TS_Stop(UV_TIMER_ID);
		HW_TS_Start(TEMP_TIMER_ID,TEMP_UPDATE_PERIOD);
		Send_Temp_Notification();
      /* USER CODE END CUSTOM_STM_IR_T_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_IR_T_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_IR_T_NOTIFY_DISABLED_EVT */
		HW_TS_Stop(TEMP_TIMER_ID);
		UTIL_SEQ_PauseTask(1<<CFG_TEMP_ID);

		Temp_lower_limit=-21;
		Temp_upper_limit=86;
		HAL_GPIO_WritePin(GPIOD, Yellow_Led_Pin|Green_Led_Pin|Red_Led_Pin, GPIO_PIN_SET);
		//HW_TS_Stop(TEMP_TIMER_ID);
      /* USER CODE END CUSTOM_STM_IR_T_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_A_T_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_A_T_READ_EVT */
		printf("\rAnalog_temp\n");
		Analog_Temp();
		UpdateCharData[0]=Dev_Temperature;
		Custom_STM_App_Update_Char(CUSTOM_STM_A_T, (uint8_t *)UpdateCharData);
      /* USER CODE END CUSTOM_STM_A_T_READ_EVT */
      break;

    case CUSTOM_STM_S_T_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_S_T_WRITE_NO_RESP_EVT */

		Desire_temp = pNotification->DataTransfered.pPayload[0]<<8 |pNotification->DataTransfered.pPayload[1];
		Temp_upper_limit=Desire_temp+1;
		Temp_lower_limit=Desire_temp-1;

		printf("Desire_ temp %d, Temp_up:%d, Temp_low:%d\n",Desire_temp,Temp_upper_limit,Temp_lower_limit);
		if(mode==0x01)
			{
			Update_mode();
	    	  // HW_TS_Start(MODE_TIMER_ID,Ts_UPDATE_PERIOD);
	    	}
      /* USER CODE END CUSTOM_STM_S_T_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_L_T_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_L_T_NOTIFY_ENABLED_EVT */
		UTIL_SEQ_PauseTask(1<<CFG_TEMP_ID);
		UTIL_SEQ_PauseTask(1<<CFG_UV_ID);
		UTIL_SEQ_PauseTask(1<<CFG_LOG_UV_ID);
		HW_TS_Stop(UV_TIMER_ID);
		HW_TS_Stop(TEMP_TIMER_ID);
		HW_TS_Start(TEMP_LOG_ID,LOG_UPDATE_PERIOD);
      /* USER CODE END CUSTOM_STM_L_T_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_L_T_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_L_T_NOTIFY_DISABLED_EVT */
		HW_TS_Stop(UV_TIMER_ID);
		HW_TS_Stop(TEMP_TIMER_ID);
		HW_TS_Stop(TEMP_LOG_ID);
		UTIL_SEQ_PauseTask(1<<CFG_LOG_TEMP_ID);
      /* USER CODE END CUSTOM_STM_L_T_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_R_N_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_R_N_NOTIFY_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_R_N_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_R_N_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_R_N_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_R_N_NOTIFY_DISABLED_EVT */
      break;

    /* UV_Sensor */
    case CUSTOM_STM_UV_V_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_UV_V_NOTIFY_ENABLED_EVT */
		UV_init();
		printf("UV_NOTIFY_ENABLED\n");
		UTIL_SEQ_PauseTask(1<<CFG_TEMP_ID);
		UTIL_SEQ_PauseTask(1<<CFG_LOG_TEMP_ID);
		UTIL_SEQ_PauseTask(1<<CFG_LOG_UV_ID);
		HW_TS_Stop(TEMP_TIMER_ID);

		HW_TS_Start(UV_TIMER_ID,UV_UPDATE_PERIOD);
		Send_UV_Notification();
      /* USER CODE END CUSTOM_STM_UV_V_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_UV_V_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_UV_V_NOTIFY_DISABLED_EVT */
		printf("UV_NOTIFY_DISABLED\n");
		//HW_TS_Stop(TEMP_TIMER_ID);
		UTIL_SEQ_PauseTask(1<<CFG_UV_ID);
		HW_TS_Stop(UV_TIMER_ID);
		UV_init();
      /* USER CODE END CUSTOM_STM_UV_V_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_UV_L_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_UV_L_NOTIFY_ENABLED_EVT */
		UTIL_SEQ_PauseTask(1<<CFG_TEMP_ID);
		UTIL_SEQ_PauseTask(1<<CFG_UV_ID);
		UTIL_SEQ_PauseTask(1<<CFG_LOG_TEMP_ID);
		HW_TS_Stop(UV_TIMER_ID);
		HW_TS_Stop(TEMP_TIMER_ID);
		HW_TS_Start(UV_LOG_ID,LOG_UPDATE_PERIOD);
      /* USER CODE END CUSTOM_STM_UV_L_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_UV_L_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_UV_L_NOTIFY_DISABLED_EVT */

		UTIL_SEQ_PauseTask(1<<CFG_LOG_UV_ID);
		HW_TS_Stop(UV_TIMER_ID);
		HW_TS_Stop(TEMP_TIMER_ID);
		HW_TS_Stop(UV_LOG_ID);
      /* USER CODE END CUSTOM_STM_UV_L_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_OTA_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_OTA_WRITE_NO_RESP_EVT */
		*(uint32_t*)SRAM1_BASE = *(uint32_t*)pNotification->DataTransfered.pPayload;
		printf("Reset\n");
		NVIC_SystemReset();
      /* USER CODE END CUSTOM_STM_OTA_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_AMBI_TEMP_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_AMBI_TEMP_NOTIFY_ENABLED_EVT */
		Send_Temp_Notification();
		Custom_STM_App_Update_Char(CUSTOM_STM_AMBI_TEMP, (uint8_t *)AT);
      /* USER CODE END CUSTOM_STM_AMBI_TEMP_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_AMBI_TEMP_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_AMBI_TEMP_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_AMBI_TEMP_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_T_INT_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_T_INT_READ_EVT */
    	Custom_STM_App_Update_Char(CUSTOM_STM_IR_T, (uint8_t *)Interval);
      /* USER CODE END CUSTOM_STM_T_INT_READ_EVT */
      break;

    case CUSTOM_STM_T_INT_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_T_INT_WRITE_NO_RESP_EVT */
    	Interval=pNotification->DataTransfered.pPayload[0];
    	if(mode==0x01)
    	    	{
    				Update_mode();

    	    	}
    	TEMP_UPDATE_PERIOD = (uint32_t)(Interval*1000*1000/CFG_TS_TICK_VAL);

      /* USER CODE END CUSTOM_STM_T_INT_WRITE_NO_RESP_EVT */
      break;

    /* Utility */
    case CUSTOM_STM_ACCLX_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_ACCLX_READ_EVT */
		AccelRead();
		STORE_BE_16(UpdateCharData,output.AccelX);
		STORE_BE_16(UpdateCharData+2,output.AccelY);
		STORE_BE_16(UpdateCharData+4,output.AccelZ);
		printf("ACClex %d %d %d %d %d %d\n",UpdateCharData[0],UpdateCharData[1],UpdateCharData[2],UpdateCharData[3],UpdateCharData[4],UpdateCharData[5],UpdateCharData[6]);
		uint8_t status=Custom_STM_App_Update_Char(CUSTOM_STM_ACCLX, (uint8_t *)UpdateCharData);
		printf("status %d \n",status);
      /* USER CODE END CUSTOM_STM_ACCLX_READ_EVT */
      break;

    case CUSTOM_STM_TD_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_TD_READ_EVT */
		Date_Time();
		Custom_STM_App_Update_Char(CUSTOM_STM_TD, (uint8_t *)Time);
      /* USER CODE END CUSTOM_STM_TD_READ_EVT */
      break;

    case CUSTOM_STM_TD_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_TD_WRITE_NO_RESP_EVT */
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
      /* USER CODE END CUSTOM_STM_TD_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_EC_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_EC_NOTIFY_ENABLED_EVT */
		Custom_STM_App_Update_Char(CUSTOM_STM_EC,&EOC_v);
      /* USER CODE END CUSTOM_STM_EC_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_EC_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_EC_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_EC_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_PWR_GD_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PWR_GD_NOTIFY_ENABLED_EVT */
		Custom_STM_App_Update_Char(CUSTOM_STM_PWR_GD,&PWR_GD);
      /* USER CODE END CUSTOM_STM_PWR_GD_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_PWR_GD_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PWR_GD_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_PWR_GD_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_SOFTWARE_REVISION_STRING_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_SOFTWARE_REVISION_STRING_READ_EVT */
		puts(Firmware);
		Custom_STM_App_Update_Char(CUSTOM_STM_SOFTWARE_REVISION_STRING,(uint8_t*)&Firmware);
      /* USER CODE END CUSTOM_STM_SOFTWARE_REVISION_STRING_READ_EVT */
      break;

    /* OperationMode */
    case CUSTOM_STM_MD_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_MD_READ_EVT */

    	Custom_STM_App_Update_Char(CUSTOM_STM_MD, &mode);

      /* USER CODE END CUSTOM_STM_MD_READ_EVT */
      break;

    case CUSTOM_STM_MD_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_MD_WRITE_NO_RESP_EVT */
    	mode=pNotification->DataTransfered.pPayload[0];
    	if(mode==0x01)
    	{
    		Update_mode();
    		printf("mode %d\n",mode);
    		//HW_TS_Start(MODE_TIMER_ID,Ts_UPDATE_PERIOD);
    	}
    	else
    	{
    		mode=0;
    		Update_mode();
    		HW_TS_Stop(TEMP_TIMER_ID);
    		HAL_GPIO_WritePin(GPIOD,Green_Led_Pin |Yellow_Led_Pin|Red_Led_Pin, GPIO_PIN_SET);
//    		HW_TS_Start(MODE_TIMER_ID,Ts_UPDATE_PERIOD);
    	}
      /* USER CODE END CUSTOM_STM_MD_WRITE_NO_RESP_EVT */
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
		if(mode != 1)
		{
		HW_TS_Stop(UV_TIMER_ID);
		HW_TS_Stop(TEMP_TIMER_ID);
		HW_TS_Stop(UV_LOG_ID);
		HW_TS_Stop(TEMP_LOG_ID);
		HAL_GPIO_WritePin(GPIOD,Green_Led_Pin |Yellow_Led_Pin|Red_Led_Pin, GPIO_PIN_SET);
		}

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
	//HAL_GPIO_WritePin(GPIOD, Red_Led_Pin, GPIO_PIN_RESET);
	//printf("Wine_temp_board\n");
	TEMP_UPDATE_PERIOD = (uint32_t)(Interval*1000*1000/CFG_TS_TICK_VAL);
	Init_mode();
	TEMP_UPDATE_PERIOD = (uint32_t)(Interval*1000*1000/CFG_TS_TICK_VAL);
	//Update_mode_task();
	UV_init();
	TEMP_APP_Init();
	UV_APP_Init();
	Log_Tx_Init();
	PWR_EOC();
	TS_Toggle();
	puts(Firmware);
	printf("\r\n");
	Analog_Temp();
	if(mode==0x01)
		{
			HW_TS_Start(TEMP_TIMER_ID,TEMP_UPDATE_PERIOD);
			printf("\Auto reload\n");
		}
	//TEMP_APP_Init();
	//HW_TS_Start(UV_TIMER_ID,UV_UPDATE_PERIOD);
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
/* Temperature */
void Custom_Ir_t_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ir_t_UC_1*/

  /* USER CODE END Ir_t_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_IR_T, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Ir_t_UC_Last*/

  /* USER CODE END Ir_t_UC_Last*/
  return;
}

void Custom_Ir_t_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ir_t_NS_1*/

  /* USER CODE END Ir_t_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_IR_T, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Ir_t_NS_Last*/

  /* USER CODE END Ir_t_NS_Last*/

  return;
}

void Custom_L_t_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN L_t_UC_1*/

  /* USER CODE END L_t_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_L_T, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN L_t_UC_Last*/

  /* USER CODE END L_t_UC_Last*/
  return;
}

void Custom_L_t_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN L_t_NS_1*/

  /* USER CODE END L_t_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_L_T, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN L_t_NS_Last*/

  /* USER CODE END L_t_NS_Last*/

  return;
}

void Custom_R_n_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN R_n_UC_1*/

  /* USER CODE END R_n_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_R_N, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN R_n_UC_Last*/

  /* USER CODE END R_n_UC_Last*/
  return;
}

void Custom_R_n_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN R_n_NS_1*/

  /* USER CODE END R_n_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_R_N, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN R_n_NS_Last*/

  /* USER CODE END R_n_NS_Last*/

  return;
}

/* UV_Sensor */
void Custom_Uv_v_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Uv_v_UC_1*/

  /* USER CODE END Uv_v_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_UV_V, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Uv_v_UC_Last*/

  /* USER CODE END Uv_v_UC_Last*/
  return;
}

void Custom_Uv_v_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Uv_v_NS_1*/

  /* USER CODE END Uv_v_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_UV_V, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Uv_v_NS_Last*/

  /* USER CODE END Uv_v_NS_Last*/

  return;
}

void Custom_Uv_l_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Uv_l_UC_1*/

  /* USER CODE END Uv_l_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_UV_L, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Uv_l_UC_Last*/

  /* USER CODE END Uv_l_UC_Last*/
  return;
}

void Custom_Uv_l_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Uv_l_NS_1*/

  /* USER CODE END Uv_l_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_UV_L, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Uv_l_NS_Last*/

  /* USER CODE END Uv_l_NS_Last*/

  return;
}

void Custom_Ambi_temp_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ambi_temp_UC_1*/

  /* USER CODE END Ambi_temp_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_AMBI_TEMP, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Ambi_temp_UC_Last*/

  /* USER CODE END Ambi_temp_UC_Last*/
  return;
}

void Custom_Ambi_temp_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ambi_temp_NS_1*/

  /* USER CODE END Ambi_temp_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_AMBI_TEMP, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Ambi_temp_NS_Last*/

  /* USER CODE END Ambi_temp_NS_Last*/

  return;
}

/* Utility */
void Custom_Ec_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ec_UC_1*/

  /* USER CODE END Ec_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_EC, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Ec_UC_Last*/

  /* USER CODE END Ec_UC_Last*/
  return;
}

void Custom_Ec_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ec_NS_1*/

  /* USER CODE END Ec_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_EC, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Ec_NS_Last*/

  /* USER CODE END Ec_NS_Last*/

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

  /* USER CODE END Pwr_gd_NS_Last*/

  return;
}

/* OperationMode */

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/
void Send_Temp_Notification(void)
{

	Temperature=Temp();
	printf("T %f %d %d %d %d\n",Temperature[0],T[0],T[1],T[2],T[3]);

	uint8_t light_noti[1];
	memcpy(T,&Temperature[0],4);
	memcpy(AT,&Temperature[1],4);
	//printf("T %d %d %d %d\n",T[0],T[1],T[2],T[3]);
	Custom_STM_App_Update_Char(CUSTOM_STM_IR_T,T);
	Custom_STM_App_Update_Char(CUSTOM_STM_AMBI_TEMP,T);
	if(global_storage_count>300)
	{
		global_storage_count=0;
		//HW_TS_Stop(TEMP_TIMER_ID);
		log_data(17);

	}
	else
	{

		global_storage_count+=1;
	}

	if(Temp_lower_limit >-21 && Temp_upper_limit <86)
	{
		if(Temperature[0]>=Temp_lower_limit && Temperature[0]<=Temp_upper_limit)
		{
			////Green_led //send notification
			printf("In range\n");
			light_noti[1]=1;

			HAL_GPIO_WritePin(GPIOD, Green_Led_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, Yellow_Led_Pin|Red_Led_Pin, GPIO_PIN_SET);
			Custom_STM_App_Update_Char(CUSTOM_STM_R_N,light_noti);
		}
		else if(Temperature[0]<Temp_lower_limit)
		{
			light_noti[1]=2;
			printf("lower range\n");

			HAL_GPIO_WritePin(GPIOD, Yellow_Led_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, Green_Led_Pin|Red_Led_Pin, GPIO_PIN_SET);
			Custom_STM_App_Update_Char(CUSTOM_STM_R_N,light_noti);
		}
		else if(Temperature[0]>Temp_upper_limit)
		{
			//Yellow_led
			printf("Upper range\n");
			light_noti[1]=3;

			HAL_GPIO_WritePin(GPIOD, Red_Led_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, Yellow_Led_Pin|Green_Led_Pin, GPIO_PIN_SET);
			Custom_STM_App_Update_Char(CUSTOM_STM_R_N,light_noti);
		}
		else
		{
			light_noti[1]=0;
			printf("outoff range\n");
			//Custom_STM_App_Update_Char(CUSTOM_STM_R_N,light_noti);
			Temp_lower_limit=-21;
			Temp_upper_limit=86;
			HAL_GPIO_WritePin(GPIOD, Yellow_Led_Pin|Green_Led_Pin|Red_Led_Pin, GPIO_PIN_SET);
			Custom_STM_App_Update_Char(CUSTOM_STM_R_N,light_noti);

		}
	}

}




void log_data(uint8_t select)
{
	uint8_t log[select];
	uint32_t current_add=Read_flash_address((select%2));
	printf("current_log_add %d\n",current_add);
	Date_Time();
	AccelRead();

	if(select==17)
	{

		memcpy(log,T,sizeof(T));
		STORE_BE_16(log+4,Data.AccelX);
		STORE_LE_16(log+6,Data.AccelY);
		STORE_LE_16(log+8,Data.AccelZ);
		log[10]=Time[0];
		log[11]=Time[1];
		log[12]=Time[2];
		log[13]=Time[3];
		log[14]=Time[4];
		log[15]=Time[5];

		log[16]=Time[6];
		if(current_add>=end_address_Temp)
		{
			update_address(end_address_Temp,1);
			current_add=Read_flash_address(1);
			Quad_Write(log,current_add,17);
			update_address(current_add+17,1);
		}
		else
		{
			Quad_Write(&log,current_add,17);
			update_address(current_add+17,1);
		}
	}
	if(select==14)
	{
		//UV_Data=UV();  to be added
		log[0]=UV_Data;
		//        log[1]=UV_Data[1];
		//        log[2]=UV_Data[2];
		STORE_LE_16(log+2,Data.AccelX);
		STORE_LE_16(log+4,Data.AccelY);
		STORE_LE_16(log+6,Data.AccelZ);
		log[7]=Time[0];
		log[8]=Time[1];
		log[9]=Time[2];
		log[10]=Time[3];
		log[11]=Time[4];
		log[12]=Time[5];
		log[13]=Time[6];
		if(current_add>=end_address_UV)
		{
			update_address(end_address_UV,2);
			current_add=Read_flash_address(2);
			Quad_Write(log,current_add,14);
			update_address(current_add+14,2);
		}
		else
		{
			Quad_Write(log,current_add,14);
			update_address(current_add+14,2);
		}


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

static void TEMP_APP_Init(void)
{

	UTIL_SEQ_RegTask( 1<<CFG_TEMP_ID, UTIL_SEQ_RFU, Send_Temp_Notification);
	/* Create timer to get the AccGyroMag params and update charecteristic */
	HW_TS_ReturnStatus_t r=HW_TS_Create(CFG_TIM_PROC_ID_ISR, &TEMP_TIMER_ID,hw_ts_Repeated,TEMP_Timer_Callback);
	TEMP_UPDATE_PERIOD = (uint32_t)(Interval*1000*1000/CFG_TS_TICK_VAL);

	printf("\r TEMP_APP_INIT %d\n",r);
	return;
}
static void TEMP_Timer_Callback(void)
{
	UTIL_SEQ_PauseTask(1<<CFG_UV_ID);
	UTIL_SEQ_PauseTask(1<<CFG_LOG_UV_ID);
	UTIL_SEQ_PauseTask(1<<CFG_LOG_TEMP_ID);
	uint32_t status = UTIL_SEQ_IsPauseTask(1<<CFG_TEMP_ID);
	//printf("status %d \n",status);
	if(status==1)
	{
		UTIL_SEQ_ResumeTask(1<<CFG_TEMP_ID);
	}
	UTIL_SEQ_SetTask(1<<CFG_TEMP_ID, CFG_SCH_PRIO_2);
	// printf("\r TEMP_APP_CALLBACK\n");
}


static void UV_Timer_Callback(void)
{
	UTIL_SEQ_PauseTask(1<<CFG_TEMP_ID);
	UTIL_SEQ_PauseTask(1<<CFG_LOG_UV_ID);
	UTIL_SEQ_PauseTask(1<<CFG_LOG_TEMP_ID);
	uint32_t status = UTIL_SEQ_IsPauseTask(1<<CFG_UV_ID);
	if(status==1)
	{
		UTIL_SEQ_ResumeTask(1<<CFG_UV_ID);
	}
	UTIL_SEQ_SetTask(1<<CFG_UV_ID, CFG_SCH_PRIO_2);
	printf("\r UV_APP_CALLBACK\n");
}
void Send_UV_Notification(void)
{
	f_UV();
	//UV();//printf("F_UV %f \n",f_UV_Data);
	UV_Data = (uint8_t)f_UVI;
	uint8_t UV_TX[sizeof(f_UVI)];
	memcpy(UV_TX,&f_UVI,sizeof(f_UVI));
	memcpy(&f_UV_Data,UV_TX,sizeof(f_UV_Data));
	printf("\r UV_sense_call[%f]= %x %x %x %x\n",f_UV_Data, UV_TX[0],UV_TX[1],UV_TX[2],UV_TX[3]);

	Custom_STM_App_Update_Char(CUSTOM_STM_UV_V,UV_TX);
	if(global_storage_count>300)
	{
		global_storage_count=0;
		log_data(14);

	}
	else
	{
		global_storage_count+=1;
	}
}
static void UV_APP_Init(void)
{

	UTIL_SEQ_RegTask( 1<<CFG_UV_ID, UTIL_SEQ_RFU, Send_UV_Notification);
	/* Create timer to get the AccGyroMag params and update charecteristic */
	HW_TS_ReturnStatus_t r=HW_TS_Create(CFG_TIM_PROC_ID_ISR, &UV_TIMER_ID,hw_ts_Repeated,UV_Timer_Callback);
	printf("\r UV_APP_Init %d \n",r);
	return;
}

void Battery(void)
{


	//ADC_Select_Battery();
	uint16_t Voltage,Battery;
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)ADC_Data ,2);
	//HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&Battery,1);
	Voltage = ((ADC_Data[0] *3300)/4096)*4;
	printf("voltage %d \n",Voltage);
	Battery =(Voltage-3000)/12;
	//voltag(ADC_Data*0.80586080586080586080586080586081)*4;// adcx(3300/4096);
	Battery_Percentage=(uint8_t)(Battery);
	printf("Battery_Percentage %d %d\n",Battery_Percentage,Battery);
	HAL_ADC_Stop_DMA(&hadc1);
	if(EOC_v ==1 && Voltage <3500)
	{
		HAL_GPIO_WritePin(GPIOA,TS_Pin,GPIO_PIN_RESET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOA,TS_Pin,GPIO_PIN_SET);
	}

}
void Analog_Temp(void)
{
	//	  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	//	  HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&ADC_Temp,1);
	//ADC_Select_Temp();
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)ADC_Data ,2);
	printf("ADC_Temp:%d :%d\n",ADC_Data[0],ADC_Data[1]);
	float sample =((((ADC_Data[1] *3300)/4096)*122)/100);
	Dev_Temperature=(sample-400)/19.5;
	printf("AnalogTemp %d \n",Dev_Temperature);
	HAL_ADC_Stop_DMA(&hadc1);
}
void Tx_log_Temp(void)
{

	// uint8_t sim= *(uint8_t*) select;
	uint32_t add=Read_flash_address(1);
	printf("current_add %d \n",add);


	uint32_t data=add-iteration_address1;
	printf("Trasmiting Wine Temperature Log Data \n");
	if(data>247)
	{
		// uint8_t read_buffer[247];
		Quad_Read(NotifyCharData,iteration_address1,247);
		Custom_STM_App_Update_Char(CUSTOM_STM_L_T, (uint8_t *)NotifyCharData);
		// sl_bt_gatt_server_notify_all(gattdb_Log,247,read_buffer);
		iteration_address1+=247;
	}
	if(data<=247 && data!=0)
	{
		uint8_t read_buffer[data];
		Quad_Read(read_buffer,iteration_address1,data);
		Custom_STM_App_Update_Char(  CUSTOM_STM_UV_L, (uint8_t *)NotifyCharData);
		//sl_bt_gatt_server_notify_all(gattdb_Log,data,read_buffer);
		iteration_address1+=data;
	}
	if(data==0)
	{
		iteration_address1=start_address_Temp;    // reset initial address
		HW_TS_Stop(TEMP_LOG_ID);
		// sl_simple_timer_stop(&Log_tx);
	}

}
void Tx_log_UV(void)
{
	uint32_t add=Read_flash_address(0);
	uint32_t data=add-iteration_address2;
	printf("Trasmiting UV Sensor Log Data \n");
	if(data>247)
	{
		//uint8_t read_buffer[247];
		Quad_Read(NotifyCharData,iteration_address2,247);
		Custom_STM_App_Update_Char(CUSTOM_STM_UV_L, (uint8_t *)NotifyCharData);
		iteration_address2 +=247;
	}
	if(data<=247 && data!=0)
	{
		uint8_t read_buffer[data];
		Quad_Read(read_buffer,iteration_address2,data);
		Custom_STM_App_Update_Char(CUSTOM_STM_UV_L, (uint8_t *)NotifyCharData);
		iteration_address2+=data;

	}
	if(data==0)
	{
		iteration_address2=start_address_UV;    // reset initial address
		HW_TS_Stop(UV_LOG_ID);
	}
}
static void Log_Tx_Init(void)
{
	UTIL_SEQ_RegTask(1 << CFG_LOG_TEMP_ID, UTIL_SEQ_RFU,Tx_log_Temp);
	UTIL_SEQ_RegTask(1 << CFG_LOG_UV_ID, UTIL_SEQ_RFU,Tx_log_UV);
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(TEMP_LOG_ID), hw_ts_Repeated,
			Log_Temp_Callback); //CALL BACK
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(UV_LOG_ID), hw_ts_Repeated,
			Log_UV_Callback);
}
static void Log_Temp_Callback(void)
{
	UTIL_SEQ_PauseTask(1<<CFG_TEMP_ID);
	UTIL_SEQ_PauseTask(1<<CFG_UV_ID);
	UTIL_SEQ_PauseTask(1<<CFG_LOG_UV_ID);
	uint32_t status = UTIL_SEQ_IsPauseTask( 1<<CFG_LOG_TEMP_ID);
	if(status==1)
	{
		UTIL_SEQ_ResumeTask(1<<CFG_LOG_TEMP_ID);
	}
	UTIL_SEQ_SetTask(1<<CFG_LOG_TEMP_ID,CFG_SCH_PRIO_2);
}
static void Log_UV_Callback(void)
{
	UTIL_SEQ_PauseTask(1<<CFG_TEMP_ID);
	UTIL_SEQ_PauseTask(1<<CFG_UV_ID);
	UTIL_SEQ_PauseTask(1<<CFG_LOG_TEMP_ID);
	uint32_t status = UTIL_SEQ_IsPauseTask( 1<<CFG_LOG_UV_ID);
	if(status==1)
	{
		UTIL_SEQ_ResumeTask(1<<CFG_LOG_UV_ID);
	}
	UTIL_SEQ_SetTask(1<<CFG_LOG_UV_ID,CFG_SCH_PRIO_2);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	printf("interrupt \n");
	if(GPIO_Pin == PWR_Pin)
	{
		//printf("interrupt 1\n");
		//uint8_t PWR_GooD[1];
		PWR_GD=HAL_GPIO_ReadPin(GPIOC,PWR_Pin);
		printf("PWR_GD_interrupt %d \r\n",PWR_GD);

	}
	if(GPIO_Pin == EOC_Pin)
	{
		//printf("interrupt 2\n");
		//uint8_t EOC_v[1];
		EOC_v=HAL_GPIO_ReadPin(GPIOC,EOC_Pin);
		PWR_GD=HAL_GPIO_ReadPin(GPIOC,PWR_Pin);


		printf("EOC_interrupt %d \r\n",EOC_v);
		if(EOC_v==1)
		{
			HW_TS_Start(Ts_TIMER_ID,Ts_UPDATE_PERIOD);
		}}
	HW_TS_Start(PWR_TIMER_ID,PWR_UPDATE_PERIOD);
	//	Custom_STM_App_Update_Char(CUSTOM_STM_PWR_GD,(uint8_t*)&PWR_GD);
	//	Custom_STM_App_Update_Char(CUSTOM_STM_EC,(uint8_t*)&EOC_v);


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
	Custom_STM_App_Update_Char(CUSTOM_STM_PWR_GD,(uint8_t*)&PWR_GD);
	Custom_STM_App_Update_Char(CUSTOM_STM_EC,(uint8_t*)&EOC_v);
	printf("EO_v_interrupt %d \r\n",EOC_v);
}
void PWR_EOC(void)
{
	UTIL_SEQ_RegTask( 1<<CFG_PWR_EC_ID, UTIL_SEQ_RFU,TS); //Get_Zmod4410); //register task
	HW_TS_Create(CFG_TIM_PROC_ID_ISR,&(PWR_TIMER_ID), hw_ts_SingleShot, PWR_EOC_CALLBACK);
}
void PWR_EOC_CALLBACK(void)
{
	UTIL_SEQ_SetTask(1<<CFG_PWR_EC_ID,CFG_SCH_PRIO_0);
}
void PWR_EOC_Send_Noti(void)
{
	Custom_STM_App_Update_Char(CUSTOM_STM_PWR_GD,(uint8_t*)&PWR_GD);
	Custom_STM_App_Update_Char(CUSTOM_STM_EC,(uint8_t*)&EOC_v);
}

void Init_mode(void)
{
	int16_t temp_value;
	Quad_Read(opmode,mode_address_map,4);
	temp_value = opmode[3]<<8 | opmode[2];
	if(opmode[0]==0x01)
	{
		mode=opmode[0];
		Interval = (opmode[1]==0xff) ? Interval:opmode[1];
		Desire_temp = (temp_value < -20 || temp_value > 85) ? Desire_temp : temp_value;
		Temp_upper_limit=Desire_temp+1;
		Temp_lower_limit=Desire_temp-1;
	}
	else
	{
		mode=0;
	}
printf(" mode:%d, int:%d,Desire_temp:%d \n",mode,Interval,Desire_temp);
}

void Update_mode(void)
{
	opmode[0]=mode;
	opmode[1]=Interval;
	memcpy(opmode+2,&Desire_temp,2);
	QSPI_Sector_Erase(mode_address_map);
	Quad_Write(opmode,mode_address_map,4);
	printf("data %d %d %d %d \n",opmode[0],opmode[1],opmode[2],opmode[3]);
	Quad_Read(opmode,mode_address_map,4);
	printf("data %d %d %d %d \n",opmode[0],opmode[1],opmode[2],opmode[3]);

}
//void Update_mode_task(void){
//
//	 UTIL_SEQ_RegTask( 1<<CFG_TASK_MODE_UPDATE_ID, UTIL_SEQ_RFU,Update_mode);
//	 	/* Create timer to get the AccGyroMag params and update charecteristic */
//	 HW_TS_ReturnStatus_t r=HW_TS_Create(CFG_TIM_PROC_ID_ISR, &MODE_TIMER_ID,hw_ts_SingleShot,Mode_Callback);
//
//}
//
//void Mode_Callback(void)
//{
//	UTIL_SEQ_SetTask(1<<CFG_TASK_MODE_UPDATE_ID,CFG_SCH_PRIO_0);
//}
/* USER CODE END FD_LOCAL_FUNCTIONS*/
