/*
 * UART.h
 *
 *  Created on: 22 ene. 2023
 *      Author: Ricard
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include <SI_EFM8BB1_Register_Enums.h>
#include "TouchScreen.h"

#define UART_OUT_SIZE 5

#define UART_IN_SIZE 12

// Write commands
#define TCH_CMD_CAL				0x01	// Receive calibration values
#define TCH_CMD_THR				0x02	// Pressure threshold
#define TCH_CMD_BTN				0x03	// Modify button
#define TCH_CMD_IRQ				0x04	// Modify touch notifications

extern uint8_t UART_DATA_OUT[UART_OUT_SIZE];
extern uint8_t UART_DATA_IN[UART_IN_SIZE];

extern volatile bool DATA_READY;
extern volatile bool UART_BUSY;

extern uint8_t CMD_ID;

extern Vec2 LAST_POINT;

void UART_Write(uint8_t command, uint8_t size);

#endif /* INC_UART_H_ */
