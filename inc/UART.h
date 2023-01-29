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

#define UART_IN_SIZE  40

// Write commands
#define TCH_CMD_CAL		0x01	// Receive calibration values
#define TCH_CMD_THR		0x02	// Pressure threshold
#define TCH_CMD_BTN		0x03	// Modify button
#define TCH_CMD_IRQ		0x04	// Modify touch notifications

// Read commands
#define TCH_CMD_LEV		0x80	// Get Last Event

extern uint8_t UART_DATA_IN[UART_IN_SIZE];

extern volatile uint8_t DATA_READY;

void UART_Write(uint8_t *buff, uint8_t size);
uint8_t UART_GetByte();
uint16_t UART_GetUI16();

#endif /* INC_UART_H_ */
