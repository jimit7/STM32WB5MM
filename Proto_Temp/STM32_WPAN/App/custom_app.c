/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_app.c
  * @author  MCD Application Team
  * @brief   Custom Example Application (Server)
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
  /* Utility */
  uint8_t               Ec_Notification_Status;
  uint8_t               Pr_Notification_Status;
  /* USER CODE BEGIN CUSTOM_APP_Context_t */

  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TEMP_UPDATE_PERIOD       (uint32_t)(0.5*1000*1000/CFG_TS_TICK_VAL)
#define UV_UPDATE_PERIOD       (uint32_t)(1*1000*1000/CFG_TS_TICK_VAL)
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
int32_t Temperature;
int16_t Temp_lower_limit =0xffff;
int16_t Temp_upper_limit=0xffff;
uint8_t global_storage_count=0;
uint8_t Time[7];
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

uint32_t ADC_Volt=0;
uint32_t ADC_Temp=0;
int8_t Battery_Percentage=0;
int8_t Dev_Temperature=0;
uint16_t iteration_address1;
uint16_t iteration_address2;
uint8_t TEMP_TIMER_ID,UV_TIMER_ID=1,TEMP_LOG_ID,UV_LOG_ID;
AccelData Data;
int8_t *UV_Data;

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
/* Utility */
static void Custom_Ec_Update_Char(void);
static void Custom_Ec_Send_Notification(void);
static void Custom_Pr_Update_Char(void);
static void Custom_Pr_Send_Notification(void);

/* USER CODE BEGIN PFP */
void Send_Temp_Notification(void);
void log_data(uint8_t select);
void Date_Time(void);
static void TEMP_APP_Init(void);
static void TEMP_Timer_Callback(void);
void Send_UV_Notification(void);
static void UV_APP_Init(void);
static void UV_Timer_Callback(void);

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

      /* USER CODE END CUSTOM_STM_LEVEL_READ_EVT */
      break;

    /* Temperature */
    case CUSTOM_STM_IR_T_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_IR_T_NOTIFY_ENABLED_EVT */
    	printf("Temp_NOTIFY_ENABLED\n");
    	//HW_TS_Stop(UV_TIMER_ID);
    	HW_TS_Start(TEMP_TIMER_ID,TEMP_UPDATE_PERIOD);
      /* USER CODE END CUSTOM_STM_IR_T_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_IR_T_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_IR_T_NOTIFY_DISABLED_EVT */
    	HW_TS_Stop(TEMP_TIMER_ID);
      /* USER CODE END CUSTOM_STM_IR_T_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_A_T_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_A_T_READ_EVT */

      /* USER CODE END CUSTOM_STM_A_T_READ_EVT */
      break;

    case CUSTOM_STM_S_T_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_S_T_WRITE_NO_RESP_EVT */

      /* USER CODE END CUSTOM_STM_S_T_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_L_T_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_L_T_NOTIFY_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_L_T_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_L_T_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_L_T_NOTIFY_DISABLED_EVT */

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
    	printf("UV_NOTIFY_ENABLED\n");
    	//HW_TS_Stop(TEMP_TIMER_ID);
    	HW_TS_Start(UV_TIMER_ID,UV_UPDATE_PERIOD);
      /* USER CODE END CUSTOM_STM_UV_V_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_UV_V_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_UV_V_NOTIFY_DISABLED_EVT */
    	HW_TS_Stop(UV_TIMER_ID);
      /* USER CODE END CUSTOM_STM_UV_V_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_UV_L_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_UV_L_NOTIFY_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_UV_L_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_UV_L_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_UV_L_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_UV_L_NOTIFY_DISABLED_EVT */
      break;

    /* Utility */
    case CUSTOM_STM_ACLX_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_ACLX_READ_EVT */

      /* USER CODE END CUSTOM_STM_ACLX_READ_EVT */
      break;

    case CUSTOM_STM_EC_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_EC_NOTIFY_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_EC_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_EC_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_EC_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_EC_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_PR_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PR_NOTIFY_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_PR_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_PR_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PR_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_PR_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_TD_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_TD_READ_EVT */

      /* USER CODE END CUSTOM_STM_TD_READ_EVT */
      break;

    case CUSTOM_STM_TD_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_TD_WRITE_NO_RESP_EVT */

      /* USER CODE END CUSTOM_STM_TD_WRITE_NO_RESP_EVT */
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
	TEMP_APP_Init();
	 UV_APP_Init();
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

void Custom_Pr_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Pr_UC_1*/

  /* USER CODE END Pr_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_PR, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Pr_UC_Last*/

  /* USER CODE END Pr_UC_Last*/
  return;
}

void Custom_Pr_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Pr_NS_1*/

  /* USER CODE END Pr_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_PR, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Pr_NS_Last*/

  /* USER CODE END Pr_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/
void Send_Temp_Notification(void)
{
	printf("Temp_sense_call\n");

	Temperature=(int32_t)(Temp()*100);
	int8_t Temp[4];
	uint8_t light_noti[1];
	STORE_BE_32(Temp,Temperature);
	Custom_STM_App_Update_Char(CUSTOM_STM_IR_T,Temp);
	if(global_storage_count>75)
	{
		global_storage_count=0;
		log_data(17);

	}
	else
	{
		global_storage_count+=1;
	}

	if(Temp_lower_limit !=0xffff && Temp_upper_limit !=0xffff)
	{
		if(Temperature>=Temp_lower_limit && Temperature<=Temp_lower_limit)
		{
			////Green_led //send notification
			light_noti[1]=1;
			Custom_STM_App_Update_Char(CUSTOM_STM_R_N,light_noti);
			HAL_GPIO_WritePin(GPIOD, Green_Led_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, Yellow_Led_Pin|Blue_Led_Pin|Red_Led_Pin, GPIO_PIN_SET);
		}
		else if(Temperature<Temp_lower_limit)
		{
			light_noti[1]=2;
			Custom_STM_App_Update_Char(CUSTOM_STM_R_N,light_noti);
			HAL_GPIO_WritePin(GPIOD, Yellow_Led_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, Green_Led_Pin|Blue_Led_Pin|Red_Led_Pin, GPIO_PIN_SET);
		}
		else if(Temperature>Temp_upper_limit)
		{
			//Yellow_led
			light_noti[1]=3;
			Custom_STM_App_Update_Char(CUSTOM_STM_R_N,light_noti);
			HAL_GPIO_WritePin(GPIOD, Red_Led_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, Yellow_Led_Pin|Blue_Led_Pin|Green_Led_Pin, GPIO_PIN_SET);
		}
		else
		{
			light_noti[1]=0;
			Custom_STM_App_Update_Char(CUSTOM_STM_R_N,light_noti);
			Temp_lower_limit=0xffff;
			Temp_upper_limit=0xffff;
			HAL_GPIO_WritePin(GPIOD, Yellow_Led_Pin|Blue_Led_Pin|Green_Led_Pin|Red_Led_Pin, GPIO_PIN_SET);

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

	 STORE_LE_32(log,Temperature);
	 STORE_LE_16(log+4,Data.AccelX);
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
  if(select==16)
      {
        //UV_Data=UV();  to be added
        log[0]=UV_Data[0];
        log[1]=UV_Data[1];
        log[2]=UV_Data[2];
        STORE_LE_16(log+3,Data.AccelX);
        STORE_LE_16(log+5,Data.AccelY);
        STORE_LE_16(log+7,Data.AccelZ);
        log[9]=Time[0];
        log[10]=Time[1];
        log[11]=Time[2];
        log[12]=Time[3];
        log[13]=Time[4];
        log[14]=Time[5];
        log[15]=Time[6];
        if(current_add>=end_address_UV)
          {
           update_address(end_address_UV,1);
           current_add=Read_flash_address(2);
           Quad_Write(log,current_add,16);
           update_address(current_add+16,2);
          }
        else
           {
      	  Quad_Write(log,current_add,16);
            update_address(current_add+16,2);
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
	    	Time[6]=sDate.Year -20;
	    	for(uint8_t i=0;i<7;i++){
	    		printf("T[%d]:%d\n",i,Time[i]);
	    	}
}

static void TEMP_APP_Init(void)
{

  UTIL_SEQ_RegTask( 1<<CFG_TEMP_ID, UTIL_SEQ_RFU, Send_Temp_Notification);
  /* Create timer to get the AccGyroMag params and update charecteristic */
  uint8_t A = HW_TS_Create(CFG_TIM_PROC_ID_ISR,&TEMP_TIMER_ID,hw_ts_Repeated,TEMP_Timer_Callback);

 return;
}
static void TEMP_Timer_Callback(void)
{
  UTIL_SEQ_SetTask(1<<CFG_TEMP_ID, CFG_SCH_PRIO_0);
}
static void UV_Timer_Callback(void)
{

	UTIL_SEQ_SetTask(1<CFG_UV_ID,CFG_SCH_PRIO_0);
}
void Send_UV_Notification(void)
{
	UV_Data=UV();
	Custom_STM_App_Update_Char(CUSTOM_STM_UV_V,UV_Data);
	if(global_storage_count>75)
		{
			global_storage_count=0;
			log_data(16);

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
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &UV_TIMER_ID,hw_ts_Repeated,UV_Timer_Callback);
	 return;
}


/* USER CODE END FD_LOCAL_FUNCTIONS*/
