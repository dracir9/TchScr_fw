/*
 * UART.c
 *
 *  Created on: 22 ene. 2023
 *      Author: Ricard
 */

#include "UART.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Global buffer for UART outgoing data. All transmit data is read from here
uint8_t UART_DATA_OUT[UART_OUT_SIZE] = {0};
// Bytes to write
uint8_t nSend = 0;

// Global buffer for UART incoming data. All receive data is written here
uint8_t UART_DATA_IN[UART_IN_SIZE] = {0};
// Bytes expected to receive
uint8_t nReceive = 0;
// Receiving command
bool IS_RECEIVING = false;

uint8_t CMD_ID = 0;

volatile bool DATA_READY = false;		// Data received

// Software flag to indicate the UART bus is transmitting
volatile bool UART_BUSY = false;

Vec2 LAST_POINT;

void UART_Write(uint8_t command, uint8_t size)
{
	while(UART_BUSY);					// Wait for UART to be free.
	UART_BUSY = true;					// Claim UART (set to busy)
	nSend = size;
	SBUF0 = command;					// Start transfer
}


//-----------------------------------------------------------------------------
// UART0_ISR
//-----------------------------------------------------------------------------
//
// UART0 ISR Content goes here. Remember to clear flag bits:
// SCON0::RI (Receive Interrupt Flag)
// SCON0::TI (Transmit Interrupt Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(UART0_ISR, UART0_IRQn)
{
	static uint8_t BytesSent;
	static uint8_t BytesReceived;


	if (SCON0_RI == 1) 							// New Byte received
	{
		SCON0_RI = 0;							// Clear interrupt flag

		if (DATA_READY)							// Data not processed yet
			return;

		if (IS_RECEIVING)
		{
			UART_DATA_IN[BytesReceived++] = SBUF0;
			if (BytesReceived >= nReceive) {
				DATA_READY = true;
				IS_RECEIVING = false;
			}
		}
		else
		{
			CMD_ID = SBUF0;		// First byte should be the command ID

			switch (CMD_ID)
			{
			case TCH_CMD_CAL:
				nReceive = 12;
				break;

			case TCH_CMD_THR:
				nReceive = 4;
				break;

			case TCH_CMD_BTN:
				nReceive = 12;
				break;

			case TCH_CMD_IRQ:
				nReceive = 3;
				break;

			default:			// Invalid command
				CMD_ID = 0;
				return;
			}
			IS_RECEIVING = true;
			BytesReceived = 0;
		}
	}

	if (SCON0_TI == 1)							// Transfer completed
	{
		SCON0_TI = 0;							// Clear interrupt flag

		if (BytesSent <= nSend)						// While data remaining
		{
			SBUF0 = UART_DATA_OUT[BytesSent++];		// Transmit byte
		}
		else
		{
			BytesSent = 0;
			UART_BUSY = false;					// Free UART
		}
	}
}

//-----------------------------------------------------------------------------
// TIMER3_ISR
//-----------------------------------------------------------------------------
//
// TIMER3 ISR Content goes here. Remember to clear flag bits:
// TMR3CN::TF3H (Timer # High Byte Overflow Flag)
// TMR3CN::TF3L (Timer # Low Byte Overflow Flag)
//
// UART receive timeout
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER3_ISR, TIMER3_IRQn)
{
	TMR3CN0 &= ~0x80;					// Clear Timer3 interrupt-pending flag
	IS_RECEIVING = false;
}
