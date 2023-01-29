/*
 * UART.c
 *
 *  Created on: 22 ene. 2023
 *      Author: Ricard
 */

#include "UART.h"
#include "STRING.H"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Global buffer for UART outgoing data. All transmit data is read from here
static uint8_t UART_DATA_OUT[UART_OUT_SIZE] = {0};
// Bytes to write
static int8_t nSend = 0;

// Global buffer for UART incoming data. All received data is written here
uint8_t UART_DATA_IN[UART_IN_SIZE] = {0};
// Bytes expected to receive
static uint8_t nReceive = 0;

// Receiving command
static bool IS_RECEIVING = false;

volatile uint8_t DATA_READY = 0;		// Data received

// Software flag to indicate the UART bus is transmitting
static volatile bool UART_BUSY = false;

static uint8_t BytesSent = 0;
static uint8_t inStPtr = 0;
static uint8_t inEndPtr = 0;
static uint8_t inCnt = 0;

void UART_Write(uint8_t *buff, uint8_t size)
{
	while(UART_BUSY);					// Wait for UART to be free.
	UART_BUSY = true;					// Claim UART (set to busy)

	memcpy(UART_DATA_OUT, buff, size);

	nSend = size;
	// Start transfer
	SBUF0 = size;
}

uint8_t UART_GetByte()
{
	uint8_t byte;

	if (inCnt == 0)
		return 0;

	byte = UART_DATA_IN[inEndPtr++];

	inCnt--;
	if (inEndPtr >= UART_IN_SIZE)
		inEndPtr = 0;

	return byte;
}

uint16_t UART_GetUI16()
{
	uint16_t val;

	if (inCnt <= 1)
		return 0;

	((uint8_t*)&val)[1] = UART_DATA_IN[inEndPtr++];
	if (inEndPtr >= UART_IN_SIZE)
		inEndPtr = 0;

	((uint8_t*)&val)[0] = UART_DATA_IN[inEndPtr++];
	if (inEndPtr >= UART_IN_SIZE)
		inEndPtr = 0;

	inCnt -= 2;

	return val;
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
	uint8_t byte = 0;

	if (SCON0_RI == 1) 							// New Byte received
	{
		SCON0_RI = 0;							// Clear interrupt flag
		// Buffer new byte
		byte = SBUF0;

		if (IS_RECEIVING)
		{
			// Reset timer
			TMR3 = 0;

			//##############################
			// Store in circular queue
			if (inCnt == UART_IN_SIZE)
				return;

			UART_DATA_IN[inStPtr++] = byte;

			inCnt++;
			if (inStPtr >= UART_IN_SIZE)
				inStPtr = 0;
			//##############################

			nReceive--;
			if (nReceive == 0)
			{
				// Disable timeout timer
				TMR3CN0 &= ~TMR3CN0_TR3__RUN;

				IS_RECEIVING = false;
				DATA_READY++;
			}
		}
		else
		{
			switch (byte)
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

			// Read commands
			case TCH_CMD_LEV:
				DATA_READY++;
				return;

			default:
				// Invalid command
				byte = 0;
				return;
			}

			//##############################
			// Store in circular queue
			if (inCnt == UART_IN_SIZE)
				return;

			UART_DATA_IN[inStPtr++] = byte;

			inCnt++;
			if (inStPtr >= UART_IN_SIZE)
				inStPtr = 0;
			//##############################

			IS_RECEIVING = true;

			// Reset timer
			TMR3 = 0;
			TMR3CN0 |= TMR3CN0_TR3__RUN; // Enable timeout timer
		}
	}

	if (SCON0_TI == 1)							// Transfer completed
	{
		SCON0_TI = 0;							// Clear interrupt flag

		if (BytesSent < nSend)						// While data remaining
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
	TMR3CN0 &= ~TMR3CN0_TF3H__BMASK;	// Clear Timer3 interrupt-pending flag
	TMR3CN0 &= ~TMR3CN0_TR3__RUN;		// Disable timeout timer
	IS_RECEIVING = false;

	// Reset queue
	DATA_READY = 0;
	inStPtr = 0;
	inEndPtr = 0;
	inCnt = 0;
}
