/*
 * Encoder.h
 *
 *  Created on: Feb 9, 2026
 *      Author: zeyad
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

/*============================= Standard Includes =============================*/
#include "main.h"
/*=============================================================================*/

/*============================= Important Defines =============================*/
#define ENCODER_CORRECTION_OFFSET 0x0000  // Hard coded after fixating the encoder to correct the reading
#define ENCODER_DELAY_PERIOD 50 // Delay in us to check if the readings are valid
#define ENCODER_MASK      0x03FF // To extract the first 10 bits from the reading
#define ENCODER_CAN_ID    0x77 // CAN ID for the Encoder message
#define ENCODER_RAW_CONVERSION 0.35156f // The conversion factor from raw to degree 360/1024 = 0.3515
#define ENCODER_MAX_STEERING_ANGLE 135.0f // Maximum steering angle as specified by the document (125.0) and an extra 10 degrees as a safety margin
#define ENCODER_ERROR_READING 0xFFFF //
/*=============================================================================*/

/*============================= Functions Prototypes =============================*/
void Encoder_DelayUs(uint16_t us);
uint16_t Encoder_GetRawReadings(void);
uint16_t Encoder_ConvertRAW(uint16_t raw_reading);
uint16_t Encoder_GrayToBinary(uint16_t gray_reading);
float Encoder_GetAngleInDegree(uint16_t binary_reading);
void Encoder_SendCAN(float angle_degree);
void Encoder_Task(void * argument);
/*=============================================================================*/






#endif /* INC_ENCODER_H_ */
