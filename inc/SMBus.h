/*
 * SMBus.h
 *
 *  Created on: 5 ago. 2021
 *      Author: Ricard
 */

#ifndef INC_SMBUS_H_
#define INC_SMBUS_H_

#include <SI_EFM8BB1_Register_Enums.h>
#include <assert.h>
#include <string.h>
#include "TouchScreen.h"

#define  WRITE					0x00	// SMBus WRITE command
#define  READ					0x01	// SMBus READ command

// Device addresses (7 bits, lsb is a don't care)
#define  MY_ADDR				0x80	//10xx 000x// This device address
#define  ESP_ADDR				0x44	//0100 0100// ESP32 address-Touch notification


// Status vector - top 4 bits only
#define  SMB_MTSTA				0xE0	// (MT) start transmitted
#define  SMB_MTDB				0xC0	// (MT) data byte transmitted
#define  SMB_MRDB				0x80	// (MR) data byte received

#define  SMB_SRADD				0x20	// (SR) slave address received
										// (also could be a lost arbitration)
#define  SMB_SRSTO				0x10	// (SR) STOP detected while SR or ST,
										// or lost arbitration
#define  SMB_SRDB				0x00	// (SR) data byte received, or
										// lost arbitration
#define  SMB_STDB				0x40	// (ST) data byte transmitted
#define  SMB_STSTO				0x50	// (ST) STOP detected during a
										// transaction; bus error
// End status vector definition

// Write commands
#define TCH_CMD_CAL				0x08	// Receive calibration values
#define TCH_CMD_THR				0x09	// Pressure threshold
#define TCH_CMD_BTN				0x0A	// Modify button
#define TCH_CMD_IRQ				0x0B	// Modify touch notifications

extern uint8_t SMB_DATA_OUT_MASTER[4];
extern uint8_t SMB_DATA_IN_SLAVE[12];
extern uint8_t SMB_DATA_OUT_SLAVE[4];

extern volatile bool SMB_BUSY;
extern volatile bool DATA_READY;
extern uint8_t TCH_CMD;

extern Vec2 LAST_POINT;

// Functions
void SMB_Write(uint8_t target, uint8_t cnt);

#endif /* INC_SMBUS_H_ */
