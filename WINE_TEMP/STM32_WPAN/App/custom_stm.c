/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    App/custom_stm.c
 * @author  MCD Application Team
 * @brief   Custom Example Service.
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
#include "common_blesvc.h"
#include "custom_stm.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct{
  uint16_t  CustomBat_SerHdle;                    /**< Battery_Service handle */
  uint16_t  CustomLevelHdle;                  /**< level handle */
  uint16_t  CustomTempHdle;                    /**< Temperature handle */
  uint16_t  CustomIr_THdle;                  /**< IR_temperature handle */
  uint16_t  CustomA_THdle;                  /**< Analog_Temperature handle */
  uint16_t  CustomS_THdle;                  /**< Set_Temp handle */
  uint16_t  CustomL_THdle;                  /**< Log_Temp handle */
  uint16_t  CustomR_NHdle;                  /**< Range_Notification handle */
  uint16_t  CustomUv_SHdle;                    /**< UV_Sensor handle */
  uint16_t  CustomUv_VHdle;                  /**< UV_Value handle */
  uint16_t  CustomUv_LHdle;                  /**< UV_Log handle */
  uint16_t  CustomOtaHdle;                  /**< BLE_CFG_OTA_REBOOT_CHAR handle */
  uint16_t  CustomAmbi_TempHdle;                  /**< Ambient_Temperature handle */
  uint16_t  CustomT_IntHdle;                  /**< TIME_INTERVAL handle */
  uint16_t  CustomUtHdle;                    /**< Utility handle */
  uint16_t  CustomAcclxHdle;                  /**< Acclerometer handle */
  uint16_t  CustomTdHdle;                  /**< Time_Date handle */
  uint16_t  CustomEcHdle;                  /**< EOC handle */
  uint16_t  CustomPwr_GdHdle;                  /**< PWR_GD handle */
  uint16_t  CustomSoftware_Revision_StringHdle;                  /**< Software_Revision_String handle */
  uint16_t  CustomOp_MHdle;                    /**< OperationMode handle */
  uint16_t  CustomMdHdle;                  /**< MODE handle */
/* USER CODE BEGIN Context */
	/* Place holder for Characteristic Descriptors Handle*/

/* USER CODE END Context */
}CustomContext_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define UUID_128_SUPPORTED  1

#if (UUID_128_SUPPORTED == 1)
#define BM_UUID_LENGTH  UUID_TYPE_128
#else
#define BM_UUID_LENGTH  UUID_TYPE_16
#endif

#define BM_REQ_CHAR_SIZE    (3)

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
#define CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET         2
#define CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET              1
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
uint8_t SizeLevel = 1;
uint8_t SizeIr_T = 4;
uint8_t SizeA_T = 1;
uint8_t SizeS_T = 4;
uint8_t SizeL_T = 1;
uint8_t SizeR_N = 1;
uint8_t SizeUv_V = 5;
uint8_t SizeUv_L = 247;
uint8_t SizeOta = 3;
uint8_t SizeAmbi_Temp = 4;
uint8_t SizeT_Int = 1;
uint8_t SizeAcclx = 1;
uint8_t SizeTd = 7;
uint8_t SizeEc = 1;
uint8_t SizePwr_Gd = 1;
uint8_t SizeSoftware_Revision_String = 10;
uint8_t SizeMd = 1;

/**
 * START of Section BLE_DRIVER_CONTEXT
 */
static CustomContext_t CustomContext;

/**
 * END of Section BLE_DRIVER_CONTEXT
 */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *pckt);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
/* USER CODE BEGIN PFD */

/* USER CODE END PFD */

/* Private functions ----------------------------------------------------------*/

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
    uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
    uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
    uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

/* Hardware Characteristics Service */
/*
 The following 128bits UUIDs have been generated from the random UUID
 generator:
 D973F2E0-B19E-11E2-9E96-0800200C9A66: Service 128bits UUID
 D973F2E1-B19E-11E2-9E96-0800200C9A66: Characteristic_1 128bits UUID
 D973F2E2-B19E-11E2-9E96-0800200C9A66: Characteristic_2 128bits UUID
 */
#define COPY_BATTERY_SERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0x18,0x0f,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_LEVEL_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x2a,0x19,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_TEMPERATURE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x61,0xc8,0x32,0x12,0x93,0xa2,0x44,0x57,0xa0,0xd1,0x30,0x8b,0x99,0xb4,0x92,0x3d)
#define COPY_IR_TEMPERATURE_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x3a,0x0a,0xc3,0x22,0x21,0x9b,0x45,0x60,0x8f,0xd8,0x7c,0xf4,0x48,0xda,0xd5,0x53)
#define COPY_ANALOG_TEMPERATURE_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x5a,0xba,0x8c,0xe2,0x6a,0x73,0x4b,0x04,0x8c,0x03,0x82,0x7a,0x9a,0x3a,0x46,0xd0)
#define COPY_SET_TEMP_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x0a,0x24,0x4b,0xc1,0x07,0x45,0x43,0xb8,0xa9,0xdf,0xb4,0x08,0xc4,0xa8,0xdf,0x8c)
#define COPY_LOG_TEMP_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0xd5,0x72,0xe7,0xc1,0x6a,0x16,0x44,0xa4,0x8b,0x1d,0x8b,0x05,0x43,0xd4,0xbf,0x63)
#define COPY_RANGE_NOTIFICATION_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x5a,0x1a,0x48,0x07,0xd9,0x6c,0x4f,0x13,0xb9,0x45,0x5a,0xb1,0xfa,0x5b,0x4d,0xb3)
#define COPY_UV_SENSOR_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0xfe,0x59,0x5b,0x56,0x2f,0xb8,0x11,0xee,0xbe,0x56,0x02,0x42,0xac,0x12,0x00,0x02)
#define COPY_UV_VALUE_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0xfe,0x59,0x5e,0x30,0x2f,0xb8,0x11,0xee,0xbe,0x56,0x02,0x42,0xac,0x12,0x00,0x02)
#define COPY_UV_LOG_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0xfe,0x59,0x5f,0x84,0x2f,0xb8,0x11,0xee,0xbe,0x56,0x02,0x42,0xac,0x12,0x00,0x02)
#define COPY_BLE_CFG_OTA_REBOOT_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0xfe,0x11,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_AMBIENT_TEMPERATURE_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0xfe,0x59,0x88,0x6a,0x2f,0xb8,0x11,0xee,0xbe,0x56,0x02,0x42,0xac,0x12,0x00,0x02)
#define COPY_TIME_INTERVAL_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0xfe,0x59,0x9f,0x84,0x2f,0xb8,0x11,0xee,0xbe,0x56,0x02,0x42,0xac,0x12,0x00,0x02)
#define COPY_UTILITY_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0xfe,0x59,0x71,0xc2,0x2f,0xb8,0x11,0xee,0xbe,0x56,0x02,0x42,0xac,0x12,0x00,0x02)
#define COPY_ACCLEROMETER_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0xfe,0x59,0x72,0xc6,0x2f,0xb8,0x11,0xee,0xbe,0x56,0x02,0x42,0xac,0x12,0x00,0x02)
#define COPY_TIME_DATE_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0xfe,0x59,0x74,0x24,0x2f,0xb8,0x11,0xee,0xbe,0x56,0x02,0x42,0xac,0x12,0x00,0x02)
#define COPY_EOC_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0xfe,0x59,0x75,0x32,0x2f,0xb8,0x11,0xee,0xbe,0x56,0x02,0x42,0xac,0x12,0x00,0x02)
#define COPY_PWR_GD_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0xfe,0x59,0x78,0x84,0x2f,0xb8,0x11,0xee,0xbe,0x56,0x02,0x42,0xac,0x12,0x00,0x02)
#define COPY_OPERATIONMODE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x2b,0xd0,0xc3,0x69,0xe8,0x5f,0x44,0xe9,0x8c,0x2c,0xfb,0x5b,0xff,0x07,0x6f,0x5f)
#define COPY_MODE_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x25,0xf4,0x94,0x52,0x8a,0x54,0x42,0x85,0xb8,0xbe,0xee,0xf5,0x81,0x37,0xca,0x59)

/* USER CODE BEGIN PF */

/* USER CODE END PF */

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0 *attribute_modified;
  aci_gatt_read_permit_req_event_rp0    *read_req;
  Custom_STM_App_Notification_evt_t     Notification;
  /* USER CODE BEGIN Custom_STM_Event_Handler_1 */

  /* USER CODE END Custom_STM_Event_Handler_1 */

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch (event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      blecore_evt = (evt_blecore_aci*)event_pckt->data;
      switch (blecore_evt->ecode)
      {
        case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */
          attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;
          if (attribute_modified->Attr_Handle == (CustomContext.CustomIr_THdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1 */

            /* USER CODE END CUSTOM_STM_Service_2_Char_1 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_2_Char_1_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_IR_T_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_IR_T_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_default */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomIr_THdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomL_THdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_4 */

            /* USER CODE END CUSTOM_STM_Service_2_Char_4 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_4_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_2_Char_4_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_4_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_2_Char_4_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_L_T_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_4_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_2_Char_4_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_4_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_2_Char_4_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_L_T_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_4_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_2_Char_4_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_4_default */

                /* USER CODE END CUSTOM_STM_Service_2_Char_4_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomL_THdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomR_NHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_5 */

            /* USER CODE END CUSTOM_STM_Service_2_Char_5 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_5_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_2_Char_5_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_5_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_2_Char_5_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_R_N_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_5_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_2_Char_5_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_5_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_2_Char_5_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_R_N_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_5_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_2_Char_5_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_5_default */

                /* USER CODE END CUSTOM_STM_Service_2_Char_5_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomR_NHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomUv_VHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1 */

            /* USER CODE END CUSTOM_STM_Service_3_Char_1 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_3_Char_1_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_3_Char_1_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_UV_V_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_3_Char_1_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_3_Char_1_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_UV_V_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_3_Char_1_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_default */

                /* USER CODE END CUSTOM_STM_Service_3_Char_1_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomUv_VHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomUv_LHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2 */

            /* USER CODE END CUSTOM_STM_Service_3_Char_2 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_3_Char_2_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_3_Char_2_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_UV_L_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_3_Char_2_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_3_Char_2_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_UV_L_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_3_Char_2_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2_default */

                /* USER CODE END CUSTOM_STM_Service_3_Char_2_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomUv_LHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomAmbi_TempHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4 */

            /* USER CODE END CUSTOM_STM_Service_3_Char_4 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_3_Char_4_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_3_Char_4_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_AMBI_TEMP_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_3_Char_4_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_3_Char_4_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_AMBI_TEMP_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_3_Char_4_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4_default */

                /* USER CODE END CUSTOM_STM_Service_3_Char_4_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomAmbi_TempHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomEcHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3 */

            /* USER CODE END CUSTOM_STM_Service_4_Char_3 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_4_Char_3_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_3_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_EC_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_3_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_3_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_EC_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_3_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3_default */

                /* USER CODE END CUSTOM_STM_Service_4_Char_3_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomEcHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomPwr_GdHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4 */

            /* USER CODE END CUSTOM_STM_Service_4_Char_4 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_4_Char_4_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_4_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_PWR_GD_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_4_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_4_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_PWR_GD_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_4_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_default */

                /* USER CODE END CUSTOM_STM_Service_4_Char_4_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomPwr_GdHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomS_THdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_3_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
				Notification.Custom_Evt_Opcode = CUSTOM_STM_S_T_WRITE_NO_RESP_EVT;
				Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
				Notification.DataTransfered.pPayload=attribute_modified->Attr_Data;
				Custom_STM_App_Notification(&Notification);
            /* USER CODE END CUSTOM_STM_Service_2_Char_3_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
          } /* if (attribute_modified->Attr_Handle == (CustomContext.CustomS_THdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (attribute_modified->Attr_Handle == (CustomContext.CustomOtaHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_3_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
				Notification.Custom_Evt_Opcode = CUSTOM_STM_OTA_WRITE_NO_RESP_EVT;
				Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
				Notification.DataTransfered.pPayload=attribute_modified->Attr_Data;
				Custom_STM_App_Notification(&Notification);
            /* USER CODE END CUSTOM_STM_Service_3_Char_3_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
          } /* if (attribute_modified->Attr_Handle == (CustomContext.CustomOtaHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (attribute_modified->Attr_Handle == (CustomContext.CustomT_IntHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_5_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
            Notification.Custom_Evt_Opcode = CUSTOM_STM_T_INT_WRITE_NO_RESP_EVT;
            Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
            Notification.DataTransfered.pPayload=attribute_modified->Attr_Data;
            Custom_STM_App_Notification(&Notification);
            /* USER CODE END CUSTOM_STM_Service_3_Char_5_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
          } /* if (attribute_modified->Attr_Handle == (CustomContext.CustomT_IntHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (attribute_modified->Attr_Handle == (CustomContext.CustomTdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
				Notification.Custom_Evt_Opcode = CUSTOM_STM_TD_WRITE_NO_RESP_EVT;
				Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
				Notification.DataTransfered.pPayload=attribute_modified->Attr_Data;
				Custom_STM_App_Notification(&Notification);
            /* USER CODE END CUSTOM_STM_Service_4_Char_2_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
          } /* if (attribute_modified->Attr_Handle == (CustomContext.CustomTdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (attribute_modified->Attr_Handle == (CustomContext.CustomMdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_1_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
            Notification.Custom_Evt_Opcode = CUSTOM_STM_MD_WRITE_NO_RESP_EVT;
            Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
            Notification.DataTransfered.pPayload=attribute_modified->Attr_Data;
            Custom_STM_App_Notification(&Notification);
            /* USER CODE END CUSTOM_STM_Service_5_Char_1_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
          } /* if (attribute_modified->Attr_Handle == (CustomContext.CustomMdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */
          break;

        case ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE :
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */
          read_req = (aci_gatt_read_permit_req_event_rp0*)blecore_evt->data;
          if (read_req->Attribute_Handle == (CustomContext.CustomLevelHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */
				Notification.Custom_Evt_Opcode=CUSTOM_STM_LEVEL_READ_EVT;
				Custom_STM_App_Notification(&Notification);
            /*USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomLevelHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (read_req->Attribute_Handle == (CustomContext.CustomIr_THdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */
            Notification.Custom_Evt_Opcode=CUSTOM_STM_IR_T_READ_EVT;
            Custom_STM_App_Notification(&Notification);
            /*USER CODE END CUSTOM_STM_Service_2_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_2_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomIr_THdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (read_req->Attribute_Handle == (CustomContext.CustomA_THdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_2_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */
				Notification.Custom_Evt_Opcode=CUSTOM_STM_A_T_READ_EVT;
				Custom_STM_App_Notification(&Notification);
            /*USER CODE END CUSTOM_STM_Service_2_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_2_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_2_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomA_THdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (read_req->Attribute_Handle == (CustomContext.CustomT_IntHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_3_Char_5_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */
            Notification.Custom_Evt_Opcode=CUSTOM_STM_T_INT_READ_EVT;
            Custom_STM_App_Notification(&Notification);
            /*USER CODE END CUSTOM_STM_Service_3_Char_5_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_3_Char_5_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_3_Char_5_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomT_IntHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (read_req->Attribute_Handle == (CustomContext.CustomAcclxHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */
				Notification.Custom_Evt_Opcode=CUSTOM_STM_ACCLX_READ_EVT;
				Custom_STM_App_Notification(&Notification);
            /*USER CODE END CUSTOM_STM_Service_4_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_4_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomAcclxHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (read_req->Attribute_Handle == (CustomContext.CustomTdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */
				Notification.Custom_Evt_Opcode=CUSTOM_STM_TD_READ_EVT;
				Custom_STM_App_Notification(&Notification);
            /*USER CODE END CUSTOM_STM_Service_4_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_4_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomTdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (read_req->Attribute_Handle == (CustomContext.CustomSoftware_Revision_StringHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_4_Char_5_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */
				Notification.Custom_Evt_Opcode=CUSTOM_STM_SOFTWARE_REVISION_STRING_READ_EVT;
				Custom_STM_App_Notification(&Notification);
            /*USER CODE END CUSTOM_STM_Service_4_Char_5_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_4_Char_5_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_4_Char_5_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomSoftware_Revision_StringHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (read_req->Attribute_Handle == (CustomContext.CustomMdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_5_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */
            Notification.Custom_Evt_Opcode=CUSTOM_STM_MD_READ_EVT;
            Custom_STM_App_Notification(&Notification);
            /*USER CODE END CUSTOM_STM_Service_5_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_5_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_5_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomMdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_END */
          break;

        case ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */
          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */
          break;
        /* USER CODE BEGIN BLECORE_EVT */

        /* USER CODE END BLECORE_EVT */
        default:
          /* USER CODE BEGIN EVT_DEFAULT */

          /* USER CODE END EVT_DEFAULT */
          break;
      }
      /* USER CODE BEGIN EVT_VENDOR*/

      /* USER CODE END EVT_VENDOR*/
      break; /* HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE */

      /* USER CODE BEGIN EVENT_PCKT_CASES*/

      /* USER CODE END EVENT_PCKT_CASES*/

    default:
      /* USER CODE BEGIN EVENT_PCKT*/

      /* USER CODE END EVENT_PCKT*/
      break;
  }

  /* USER CODE BEGIN Custom_STM_Event_Handler_2 */

  /* USER CODE END Custom_STM_Event_Handler_2 */

  return(return_value);
}/* end Custom_STM_Event_Handler */

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void SVCCTL_InitCustomSvc(void)
{

  Char_UUID_t  uuid;
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  uint8_t max_attr_record;

  /* USER CODE BEGIN SVCCTL_InitCustomSvc_1 */

  /* USER CODE END SVCCTL_InitCustomSvc_1 */

  /**
   *  Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(Custom_STM_Event_Handler);

  /**
   *          Battery_Service
   *
   * Max_Attribute_Records = 1 + 2*1 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for Battery_Service +
   *                                2 for level +
   *                              = 3
   *
   * This value doesn't take into account number of descriptors manually added
   * In case of descriptors added, please update the max_attr_record value accordingly in the next SVCCTL_InitService User Section
   */
  max_attr_record = 3;

  /* USER CODE BEGIN SVCCTL_InitService */
	/* max_attr_record to be updated if descriptors have been added */

  /* USER CODE END SVCCTL_InitService */

  COPY_BATTERY_SERVICE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             max_attr_record,
                             &(CustomContext.CustomBat_SerHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: Bat_ser, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: Bat_ser \n\r");
  }

  /**
   *  level
   */
  COPY_LEVEL_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomBat_SerHdle,
                          UUID_TYPE_128, &uuid,
                          SizeLevel,
                          CHAR_PROP_READ,
                          ATTR_PERMISSION_ENCRY_READ,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomLevelHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : LEVEL, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : LEVEL \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service1_Char1/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service1_Char1 */

  /**
   *          Temperature
   *
   * Max_Attribute_Records = 1 + 2*5 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for Temperature +
   *                                2 for IR_temperature +
   *                                2 for Analog_Temperature +
   *                                2 for Set_Temp +
   *                                2 for Log_Temp +
   *                                2 for Range_Notification +
   *                                1 for IR_temperature configuration descriptor +
   *                                1 for Log_Temp configuration descriptor +
   *                                1 for Range_Notification configuration descriptor +
   *                              = 14
   *
   * This value doesn't take into account number of descriptors manually added
   * In case of descriptors added, please update the max_attr_record value accordingly in the next SVCCTL_InitService User Section
   */
  max_attr_record = 14;

  /* USER CODE BEGIN SVCCTL_InitService */
	/* max_attr_record to be updated if descriptors have been added */

  /* USER CODE END SVCCTL_InitService */

  COPY_TEMPERATURE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             max_attr_record,
                             &(CustomContext.CustomTempHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: temp, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: temp \n\r");
  }

  /**
   *  IR_temperature
   */
  COPY_IR_TEMPERATURE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomTempHdle,
                          UUID_TYPE_128, &uuid,
                          SizeIr_T,
                          CHAR_PROP_READ | CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomIr_THdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : IR_T, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : IR_T \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service2_Char1/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service2_Char1 */
  /**
   *  Analog_Temperature
   */
  COPY_ANALOG_TEMPERATURE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomTempHdle,
                          UUID_TYPE_128, &uuid,
                          SizeA_T,
                          CHAR_PROP_READ,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomA_THdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : A_T, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : A_T \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service2_Char2/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service2_Char2 */
  /**
   *  Set_Temp
   */
  COPY_SET_TEMP_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomTempHdle,
                          UUID_TYPE_128, &uuid,
                          SizeS_T,
                          CHAR_PROP_WRITE_WITHOUT_RESP,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomS_THdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : S_T, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : S_T \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service2_Char3/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service2_Char3 */
  /**
   *  Log_Temp
   */
  COPY_LOG_TEMP_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomTempHdle,
                          UUID_TYPE_128, &uuid,
                          SizeL_T,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomL_THdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : L_T, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : L_T \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service2_Char4/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service2_Char4 */
  /**
   *  Range_Notification
   */
  COPY_RANGE_NOTIFICATION_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomTempHdle,
                          UUID_TYPE_128, &uuid,
                          SizeR_N,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_DONT_NOTIFY_EVENTS,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomR_NHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : R_N, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : R_N \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service2_Char5/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service2_Char5 */

  /**
   *          UV_Sensor
   *
   * Max_Attribute_Records = 1 + 2*5 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for UV_Sensor +
   *                                2 for UV_Value +
   *                                2 for UV_Log +
   *                                2 for BLE_CFG_OTA_REBOOT_CHAR +
   *                                2 for Ambient_Temperature +
   *                                2 for TIME_INTERVAL +
   *                                1 for UV_Value configuration descriptor +
   *                                1 for UV_Log configuration descriptor +
   *                                1 for Ambient_Temperature configuration descriptor +
   *                              = 14
   *
   * This value doesn't take into account number of descriptors manually added
   * In case of descriptors added, please update the max_attr_record value accordingly in the next SVCCTL_InitService User Section
   */
  max_attr_record = 14;

  /* USER CODE BEGIN SVCCTL_InitService */
	/* max_attr_record to be updated if descriptors have been added */

  /* USER CODE END SVCCTL_InitService */

  COPY_UV_SENSOR_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             max_attr_record,
                             &(CustomContext.CustomUv_SHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: UV_s, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: UV_s \n\r");
  }

  /**
   *  UV_Value
   */
  COPY_UV_VALUE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomUv_SHdle,
                          UUID_TYPE_128, &uuid,
                          SizeUv_V,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_DONT_NOTIFY_EVENTS,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomUv_VHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : UV_V, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : UV_V \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service3_Char1/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service3_Char1 */
  /**
   *  UV_Log
   */
  COPY_UV_LOG_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomUv_SHdle,
                          UUID_TYPE_128, &uuid,
                          SizeUv_L,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_DONT_NOTIFY_EVENTS,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomUv_LHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : UV_L, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : UV_L \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service3_Char2/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service3_Char2 */
  /**
   *  BLE_CFG_OTA_REBOOT_CHAR
   */
  COPY_BLE_CFG_OTA_REBOOT_CHAR_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomUv_SHdle,
                          UUID_TYPE_128, &uuid,
                          SizeOta,
                          CHAR_PROP_WRITE_WITHOUT_RESP,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomOtaHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : OTA, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : OTA \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service3_Char3/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service3_Char3 */
  /**
   *  Ambient_Temperature
   */
  COPY_AMBIENT_TEMPERATURE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomUv_SHdle,
                          UUID_TYPE_128, &uuid,
                          SizeAmbi_Temp,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_DONT_NOTIFY_EVENTS,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomAmbi_TempHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : AMBI_TEMP, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : AMBI_TEMP \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service3_Char4/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service3_Char4 */
  /**
   *  TIME_INTERVAL
   */
  COPY_TIME_INTERVAL_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomUv_SHdle,
                          UUID_TYPE_128, &uuid,
                          SizeT_Int,
                          CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RESP,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomT_IntHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : T_INT, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : T_INT \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service3_Char5/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service3_Char5 */

  /**
   *          Utility
   *
   * Max_Attribute_Records = 1 + 2*5 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for Utility +
   *                                2 for Acclerometer +
   *                                2 for Time_Date +
   *                                2 for EOC +
   *                                2 for PWR_GD +
   *                                2 for Software_Revision_String +
   *                                1 for EOC configuration descriptor +
   *                                1 for PWR_GD configuration descriptor +
   *                              = 13
   *
   * This value doesn't take into account number of descriptors manually added
   * In case of descriptors added, please update the max_attr_record value accordingly in the next SVCCTL_InitService User Section
   */
  max_attr_record = 13;

  /* USER CODE BEGIN SVCCTL_InitService */
	/* max_attr_record to be updated if descriptors have been added */

  /* USER CODE END SVCCTL_InitService */

  COPY_UTILITY_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             max_attr_record,
                             &(CustomContext.CustomUtHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: UT, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: UT \n\r");
  }

  /**
   *  Acclerometer
   */
  COPY_ACCLEROMETER_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomUtHdle,
                          UUID_TYPE_128, &uuid,
                          SizeAcclx,
                          CHAR_PROP_READ,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomAcclxHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : ACCLX, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : ACCLX \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service4_Char1/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service4_Char1 */
  /**
   *  Time_Date
   */
  COPY_TIME_DATE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomUtHdle,
                          UUID_TYPE_128, &uuid,
                          SizeTd,
                          CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RESP,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomTdHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : TD, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : TD \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service4_Char2/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service4_Char2 */
  /**
   *  EOC
   */
  COPY_EOC_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomUtHdle,
                          UUID_TYPE_128, &uuid,
                          SizeEc,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_DONT_NOTIFY_EVENTS,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomEcHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : EC, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : EC \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service4_Char3/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service4_Char3 */
  /**
   *  PWR_GD
   */
  COPY_PWR_GD_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomUtHdle,
                          UUID_TYPE_128, &uuid,
                          SizePwr_Gd,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_DONT_NOTIFY_EVENTS,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomPwr_GdHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : PWR_GD, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : PWR_GD \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service4_Char4/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service4_Char4 */
  /**
   *  Software_Revision_String
   */
  uuid.Char_UUID_16 = 0x2a28;
  ret = aci_gatt_add_char(CustomContext.CustomUtHdle,
                          UUID_TYPE_16, &uuid,
                          SizeSoftware_Revision_String,
                          CHAR_PROP_READ,
                          ATTR_PERMISSION_ENCRY_READ,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomSoftware_Revision_StringHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : SOFTWARE_REVISION_STRING, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : SOFTWARE_REVISION_STRING \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service4_Char5/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service4_Char5 */

  /**
   *          OperationMode
   *
   * Max_Attribute_Records = 1 + 2*1 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for OperationMode +
   *                                2 for MODE +
   *                              = 3
   *
   * This value doesn't take into account number of descriptors manually added
   * In case of descriptors added, please update the max_attr_record value accordingly in the next SVCCTL_InitService User Section
   */
  max_attr_record = 3;

  /* USER CODE BEGIN SVCCTL_InitService */
	/* max_attr_record to be updated if descriptors have been added */

  /* USER CODE END SVCCTL_InitService */

  COPY_OPERATIONMODE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             max_attr_record,
                             &(CustomContext.CustomOp_MHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: OP_m, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: OP_m \n\r");
  }

  /**
   *  MODE
   */
  COPY_MODE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomOp_MHdle,
                          UUID_TYPE_128, &uuid,
                          SizeMd,
                          CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RESP,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomMdHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : MD, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : MD \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service5_Char1/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service5_Char1 */

  /* USER CODE BEGIN SVCCTL_InitCustomSvc_2 */

  /* USER CODE END SVCCTL_InitCustomSvc_2 */

  return;
}

/**
 * @brief  Characteristic update
 * @param  CharOpcode: Characteristic identifier
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 *
 */
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  /* USER CODE BEGIN Custom_STM_App_Update_Char_1 */

  /* USER CODE END Custom_STM_App_Update_Char_1 */

  switch (CharOpcode)
  {

    case CUSTOM_STM_LEVEL:
      ret = aci_gatt_update_char_value(CustomContext.CustomBat_SerHdle,
                                       CustomContext.CustomLevelHdle,
                                       0, /* charValOffset */
                                       SizeLevel, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value LEVEL command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value LEVEL command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_1_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_1_Char_1*/
      break;

    case CUSTOM_STM_IR_T:
      ret = aci_gatt_update_char_value(CustomContext.CustomTempHdle,
                                       CustomContext.CustomIr_THdle,
                                       0, /* charValOffset */
                                       SizeIr_T, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value IR_T command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value IR_T command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_2_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_2_Char_1*/
      break;

    case CUSTOM_STM_A_T:
      ret = aci_gatt_update_char_value(CustomContext.CustomTempHdle,
                                       CustomContext.CustomA_THdle,
                                       0, /* charValOffset */
                                       SizeA_T, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value A_T command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value A_T command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_2_Char_2*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_2_Char_2*/
      break;

    case CUSTOM_STM_S_T:
      ret = aci_gatt_update_char_value(CustomContext.CustomTempHdle,
                                       CustomContext.CustomS_THdle,
                                       0, /* charValOffset */
                                       SizeS_T, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value S_T command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value S_T command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_2_Char_3*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_2_Char_3*/
      break;

    case CUSTOM_STM_L_T:
      ret = aci_gatt_update_char_value(CustomContext.CustomTempHdle,
                                       CustomContext.CustomL_THdle,
                                       0, /* charValOffset */
                                       SizeL_T, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value L_T command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value L_T command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_2_Char_4*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_2_Char_4*/
      break;

    case CUSTOM_STM_R_N:
      ret = aci_gatt_update_char_value(CustomContext.CustomTempHdle,
                                       CustomContext.CustomR_NHdle,
                                       0, /* charValOffset */
                                       SizeR_N, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value R_N command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value R_N command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_2_Char_5*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_2_Char_5*/
      break;

    case CUSTOM_STM_UV_V:
      ret = aci_gatt_update_char_value(CustomContext.CustomUv_SHdle,
                                       CustomContext.CustomUv_VHdle,
                                       0, /* charValOffset */
                                       SizeUv_V, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value UV_V command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value UV_V command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_3_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_3_Char_1*/
      break;

    case CUSTOM_STM_UV_L:
      ret = aci_gatt_update_char_value(CustomContext.CustomUv_SHdle,
                                       CustomContext.CustomUv_LHdle,
                                       0, /* charValOffset */
                                       SizeUv_L, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value UV_L command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value UV_L command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_3_Char_2*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_3_Char_2*/
      break;

    case CUSTOM_STM_OTA:
      ret = aci_gatt_update_char_value(CustomContext.CustomUv_SHdle,
                                       CustomContext.CustomOtaHdle,
                                       0, /* charValOffset */
                                       SizeOta, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value OTA command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value OTA command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_3_Char_3*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_3_Char_3*/
      break;

    case CUSTOM_STM_AMBI_TEMP:
      ret = aci_gatt_update_char_value(CustomContext.CustomUv_SHdle,
                                       CustomContext.CustomAmbi_TempHdle,
                                       0, /* charValOffset */
                                       SizeAmbi_Temp, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value AMBI_TEMP command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value AMBI_TEMP command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_3_Char_4*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_3_Char_4*/
      break;

    case CUSTOM_STM_T_INT:
      ret = aci_gatt_update_char_value(CustomContext.CustomUv_SHdle,
                                       CustomContext.CustomT_IntHdle,
                                       0, /* charValOffset */
                                       SizeT_Int, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value T_INT command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value T_INT command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_3_Char_5*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_3_Char_5*/
      break;

    case CUSTOM_STM_ACCLX:
      ret = aci_gatt_update_char_value(CustomContext.CustomUtHdle,
                                       CustomContext.CustomAcclxHdle,
                                       0, /* charValOffset */
                                       SizeAcclx, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value ACCLX command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value ACCLX command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_4_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_4_Char_1*/
      break;

    case CUSTOM_STM_TD:
      ret = aci_gatt_update_char_value(CustomContext.CustomUtHdle,
                                       CustomContext.CustomTdHdle,
                                       0, /* charValOffset */
                                       SizeTd, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value TD command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value TD command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_4_Char_2*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_4_Char_2*/
      break;

    case CUSTOM_STM_EC:
      ret = aci_gatt_update_char_value(CustomContext.CustomUtHdle,
                                       CustomContext.CustomEcHdle,
                                       0, /* charValOffset */
                                       SizeEc, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value EC command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value EC command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_4_Char_3*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_4_Char_3*/
      break;

    case CUSTOM_STM_PWR_GD:
      ret = aci_gatt_update_char_value(CustomContext.CustomUtHdle,
                                       CustomContext.CustomPwr_GdHdle,
                                       0, /* charValOffset */
                                       SizePwr_Gd, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value PWR_GD command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value PWR_GD command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_4_Char_4*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_4_Char_4*/
      break;

    case CUSTOM_STM_SOFTWARE_REVISION_STRING:
      ret = aci_gatt_update_char_value(CustomContext.CustomUtHdle,
                                       CustomContext.CustomSoftware_Revision_StringHdle,
                                       0, /* charValOffset */
                                       SizeSoftware_Revision_String, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value SOFTWARE_REVISION_STRING command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value SOFTWARE_REVISION_STRING command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_4_Char_5*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_4_Char_5*/
      break;

    case CUSTOM_STM_MD:
      ret = aci_gatt_update_char_value(CustomContext.CustomOp_MHdle,
                                       CustomContext.CustomMdHdle,
                                       0, /* charValOffset */
                                       SizeMd, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value MD command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value MD command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_5_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_5_Char_1*/
      break;

    default:
      break;
  }

  /* USER CODE BEGIN Custom_STM_App_Update_Char_2 */

  /* USER CODE END Custom_STM_App_Update_Char_2 */

  return ret;
}
