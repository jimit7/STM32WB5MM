/*
 * Acclx.h
 *
 *  Created on: Apr. 14, 2023
 *      Author: Jimitp
 */

#ifndef ACCLX_H_
#define ACCLX_H_


#include <stdint.h>


#define ACCEL_ADDRESS         0x30
#define ACCEL_STATUS_REG_AUX  0x07
#define ACCEL_OUT_TEMP_L      0x0C
#define ACCEL_OUT_TEMP_H      0x0D
#define ACCEL_INT_COUNTER_REG 0x0E
#define ACCEL_WHO_AM_I      0x0F

#define ACCEL_TEMP_CFG_REG  0x1F
#define ACCEL_CTRL_REG1     0x20
#define ACCEL_CTRL_REG2     0x21
#define ACCEL_CTRL_REG3     0x22
#define ACCEL_CTRL_REG4     0x23
#define ACCEL_CTRL_REG5     0x24
#define ACCEL_CTRL_REG6     0x25
#define ACCEL_REFERENCE_DATACAPTURE 0x26
#define ACCEL_STATUS_REG    0x27
#define ACCEL_OUT_X_L       0x28
#define ACCEL_OUT_X_H       0x29
#define ACCEL_OUT_Y_L       0x2A
#define ACCEL_OUT_Y_H       0x2B
#define ACCEL_OUT_Z_L       0x2C
#define ACCEL_OUT_Z_H       0x2D
#define ACCEL_FIFO_CTRL_REG 0x2E
#define ACCEL_FIFO_SRC_REG  0x2F
#define ACCEL_INT1_CFG      0x30
#define ACCEL_INT1_SRC      0x31
#define ACCEL_INT1_THS      0x32
#define ACCEL_INT1_DURATION 0x33
#define ACCEL_INT2_CFG      0x34
#define ACCEL_INT2_SRC      0x35
#define ACCEL_INT2_THS      0x36
#define ACCEL_INT2_DURATION 0x37
#define ACCEL_CLICK_CFG     0x38
#define ACCEL_CLICK_SRC     0x39
#define ACCEL_CLICK_THS     0x3A
#define ACCEL_TIME_LIMIT    0x3B
#define ACCEL_TIME_LATENCY  0x3C
#define ACCEL_TIME_WINDOW   0x3D
#define ACCEL_Act_THS       0x3E
#define ACCEL_Act_DUR       0x3F

typedef struct S1{int16_t AccelX;
            int16_t AccelY;
            int16_t AccelZ;
            int16_t RawAccelX;
            int16_t RawAccelY;
            int16_t RawAccelZ;
            float AccelXScratch;
            float AccelYScratch;
            float AccelZScratch;}AccelData;
extern AccelData output;
void AccelInit (void);
//void AccelRead(AccelData *output);
void AccelRead(void);

#endif /* ACCLX_H_ */
