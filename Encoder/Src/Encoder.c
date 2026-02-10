/*
 * Encoder.c
 *
 *  Created on: Feb 9, 2026
 *      Author: zeyad
 */

/*============================= Standard Includes =============================*/
#include "Encoder.h"
/*=============================================================================*/

/*============================= Private Variables =============================*/
// External handle from can.c
extern CAN_HandleTypeDef hcan;
extern TIM_HandleTypeDef htim2;
bool angle_is_valid = 1;
/*=============================================================================*/

/*============================= Functions definitions =============================*/

// function used to trigger delay in microseconds 'us' using TIM2
void Encoder_DelayUs(uint16_t us) {
    // Reset the counter to 0
    __HAL_TIM_SET_COUNTER(&htim2, 0);

    // Wait until the counter reaches the desired value
    // Since 1 tick = 1 us, we just wait for the counter to equal 'us'
    while (__HAL_TIM_GET_COUNTER(&htim2) < us);
}

// function used to get raw readings in gray code
uint16_t Encoder_GetRawReadings(void) {
	// first reading
	uint16_t read_raw_1 = (GPIOB->IDR) & ENCODER_MASK;

	// wait for 50 us and take a new reading
	Encoder_DelayUs(ENCODER_DELAY_PERIOD);

	// second reading
	uint16_t read_raw_2 = (GPIOB->IDR) & ENCODER_MASK;

	// check stability
	if(read_raw_1 == read_raw_2) {
		return (read_raw_1); // return correct reading
	}
	else {
		// wait for 50 us and take a new reading
		Encoder_DelayUs(ENCODER_DELAY_PERIOD);

		// third reading
		uint16_t read_raw_3 = (GPIOB->IDR) & ENCODER_MASK;
		if(read_raw_2 == read_raw_3) {
				return (read_raw_2); // return correct reading
		}
		else {
			return ENCODER_ERROR_READING; // return error reading
		}
	};
}

// function used to convert gray code to binary
uint16_t Encoder_GrayToBinary(uint16_t gray_reading) {
	uint16_t binary_reading = gray_reading;
	// converting gray code to binary
	binary_reading ^= (binary_reading >> 1);
	binary_reading ^= (binary_reading >> 2);
	binary_reading ^= (binary_reading >> 4);
	binary_reading ^= (binary_reading >> 8);

	return binary_reading & ENCODER_MASK;
}

// function used to convert from raw reading to actual reading in binary and applying the offset
uint16_t Encoder_ConvertRAW(uint16_t raw_reading) {
	if(raw_reading == ENCODER_ERROR_READING) {
			return ENCODER_ERROR_READING;
	}
	// invert active low to correct gray code
	uint16_t inverted_gray = (~raw_reading) & ENCODER_MASK;

	// convert correction offset to binary
	uint16_t binary_correction_offset = Encoder_GrayToBinary(ENCODER_CORRECTION_OFFSET);

	// convert gray code reading to binary
	uint16_t raw_binary_reading = Encoder_GrayToBinary(inverted_gray);

	// correct reading after calibration
	uint16_t binary_reading = (raw_binary_reading + 1024 - binary_correction_offset) % 1024;
	return binary_reading;
}

// function used to convert the angle from binary to degree
float Encoder_GetAngleInDegree(uint16_t binary_reading) {
	float angle_degree = 0.0f;
	// Check for invalid reading
	if(binary_reading == ENCODER_ERROR_READING) {
	   angle_is_valid = false;
	   return 999.9f;  // Error value
	}
	// convert binary reading to degree
	angle_degree = binary_reading * ENCODER_RAW_CONVERSION;
	// deal with right and left turn
	if(angle_degree > 180.0f) {
		angle_degree = angle_degree - 360.0f;
	}
	// check if the angle exceeded its maximum limit
	if(angle_degree > ENCODER_MAX_STEERING_ANGLE || angle_degree < (ENCODER_MAX_STEERING_ANGLE * -1)) {
		angle_is_valid = false;
	}
	else {
		angle_is_valid = true;
	}
	return angle_degree;
}

// function used to transfer the readings through CAN
void Encoder_SendCAN(float angle_degree) {
	static uint8_t life_counter = 0; // detect if the MCU froze
	CAN_TxHeaderTypeDef txHeader = {0}; // CAN transmit header
	uint32_t txMailbox; // CAN transmit mailbox
	uint8_t data[5]; // 4 Bytes Angle + 1 Byte Counter and flag

	// Setup Header
	txHeader.StdId = ENCODER_CAN_ID; // defined in Encoder.h and DAQ.h
	txHeader.RTR = CAN_RTR_DATA;
	txHeader.IDE = CAN_ID_STD;
	txHeader.DLC = sizeof(data) / sizeof(data[0]); // Sending 5 Bytes
	txHeader.TransmitGlobalTime = DISABLE;

	// convert float to uint32_t
	uint32_t angle_bits = 0;
	memcpy(&angle_bits, &angle_degree, sizeof(float));

	// Fill Data
	data[0] = (angle_bits) & 0xFF;
	data[1] = (angle_bits >> 8) & 0xFF;
	data[2] = (angle_bits >> 16) & 0xFF;
	data[3] = (angle_bits >> 24) & 0xFF;
	data[4] = (angle_is_valid << 7) |(life_counter++ & 0x7F);  // Rolling counter (0-127) + the angle validation bit

	// Send only if Mailbox is free
	if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) > 0) {
	    HAL_CAN_AddTxMessage(&hcan, &txHeader, data, &txMailbox);
	}
}

// The task that will be executed by the scheduler
void Encoder_Task(void * argument) {
	for(;;) {
		// get the gray raw reading
		uint16_t gray_raw_reading = Encoder_GetRawReadings();

		// convert the gray code reading to binary
		uint16_t binary_reading = Encoder_ConvertRAW(gray_raw_reading);

		// get the angle in degree and applying the offset along with the ratio
		float angle_degree = Encoder_GetAngleInDegree(binary_reading);

		// Transfer the readings through CAN
		Encoder_SendCAN(angle_degree);
	}
}








