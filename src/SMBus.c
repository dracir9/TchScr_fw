/*
 * SMBus.c
 *
 *  Created on: 5 ago. 2021
 *      Author: Ricard
 */

#include "SMBus.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Global holder for SMBus master data. All transmit data is read from here
// while in master mode
// Master->Slave
uint8_t SMB_DATA_OUT_MASTER[5] = {0};

// Global holder for SMBus slave data. All receive data is written here
// while slave mode
// Slave<-Master
uint8_t SMB_DATA_IN_SLAVE[12] = {0};
uint8_t TCH_CMD = 0;

// Global holder for SMBus slave data. All transmit data is read from here
// while in slave mode
// Slave->Master
uint8_t SMB_DATA_OUT_SLAVE[5] = {0};

volatile bool DATA_READY = false;		// Data received in slave mode

uint8_t TARGET;							// Target SMBus slave address

volatile bool SMB_BUSY = false;			// Software flag to indicate when the
										// SMB_Read() or SMB_Write() functions
										// have claimed the SMBus

volatile bool IS_SLAVE = false;

uint8_t nMasterWrite = 0;
uint8_t nSlaveReceive = 0;
uint8_t nSlaveSend = 0;

Vec2 LAST_POINT;

uint8_t NUM_ERRORS = 0;						// Counter for the number of errors.

void SMB_Write(uint8_t target, uint8_t cnt)
{
	while(SMB_BUSY);					// Wait for SMBus to be free.
	if (IS_SLAVE || DATA_READY) return;
	SMB_BUSY = true;					// Claim SMBus (set to busy)
	TARGET = target;
	nMasterWrite = cnt;
	SMB0CN0_STA = 1;					// Start transfer
}

//-----------------------------------------------------------------------------
// SMBUS0_ISR
//-----------------------------------------------------------------------------
//
// SMBUS0 ISR Content goes here. Remember to clear flag bits:
// SMB0CN::SI (SMBus Interrupt Flag)
//
// SMBus ISR state machine
// - MultiMaster implementation
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(SMBUS0_ISR, SMBUS0_IRQn)
{
	bool M_FAIL = 0;					// Used by the master states to flag a
										// bus error condition

	static uint8_t masterBytesSent;
	static uint8_t slaveBytesReceived;
	static uint8_t slaveBytesSent;

	static bool arbitration_lost = 0;	// Used by the ISR to flag whether
										// arbitration was lost and the transfer
										// should be rescheduled

	switch (SMB0CN0 & 0xF0)				// Status vector
	{
	// SMB0CN_MASTER ---------------------------------------------------------------

	// Master Transmitter/Receiver: START condition transmitted.
	case SMB_MTSTA:
		SMB0DAT = TARGET & 0xFE;		// Load address + R/W bit
		SMB0CN0_STA = 0;				// Manually clear START bit
		masterBytesSent = 0;

		break;

	// Master Transmitter: Data byte transmitted
	case SMB_MTDB:

		if(SMB0CN0_ARBLOST == 0)		// Check for a bus error
		{
			if(SMB0CN0_ACK)				// Slave SMB0CN_ACK?
			{
				if(masterBytesSent < nMasterWrite)	// If there are bytes remaining
				{						// Send data byte
					SMB0DAT = SMB_DATA_OUT_MASTER[masterBytesSent++];
				}
				else                    // All bytes sent
				{
					SMB0CN0_STO = 1;	// Set SMB0CN_STO to terminate transfer
					SMB_BUSY = false;	// And free SMBus interface
				}
			}
			else						// If slave NACK,
			{
				SMB0CN0_STO = 1;		// Send STOP condition, followed
				if (NUM_ERRORS < 0)
				{
					SMB0CN0_STA = 1;	// By a START
					NUM_ERRORS++;		// Indicate error
				}
				else
				{
					SMB_BUSY = false;				// Free SMBus
					NUM_ERRORS = 0;					// Indicate an error occurred
				}
			}
		}
		else
		{
			M_FAIL = 1;					// If a bus error occurs, reset
		}

		break;

	// Master Receiver: byte received
	case SMB_MRDB: // We never request a read
		M_FAIL = 1;						// If a bus error occurs, reset

		break;

	// ----------------------------------------------------------------------
	// SLAVE ----------------------------------------------------------------

	// Slave Receiver: Start+Address received
	case SMB_SRADD:

		if (SMB0CN0_ARBLOST == 1)
		{
			arbitration_lost = 1;		// Indicate lost arbitration
		}

		SMB0CN0_STA = 0;				// Clear SMB0CN_STA bit
		slaveBytesReceived = 0;
		slaveBytesSent = 0;

		if((SMB0DAT & 0xCE) == (MY_ADDR & 0xCE))// Decode address
		{									// If the received address matches,
			IS_SLAVE = true;
			SMB_BUSY = false;
			if((SMB0DAT & 0x01) == READ)	// If the transfer is a master READ
			{
				if ((SMB0DAT & 0x30) == 0)	// ADDR: 1000 xxx1
				{
					nSlaveSend = 5;
					SMB_DATA_OUT_SLAVE[0] = activeBtn;
					SMB_DATA_OUT_SLAVE[1] = ((uint8_t*)&LAST_POINT.x)[1];
					SMB_DATA_OUT_SLAVE[2] = ((uint8_t*)&LAST_POINT.x)[0];
					SMB_DATA_OUT_SLAVE[3] = ((uint8_t*)&LAST_POINT.y)[1];
					SMB_DATA_OUT_SLAVE[4] = ((uint8_t*)&LAST_POINT.y)[0];
				}
				// Prepare outgoing byte
				SMB0DAT = SMB_DATA_OUT_SLAVE[slaveBytesSent++];
			}
			else
			{
				if (DATA_READY)			// Data not processed yet
				{
					SMB0CN0_ACK = 0;
					break;
				}

				TCH_CMD = SMB0DAT >> 4;
				switch (TCH_CMD)
				{
				case TCH_CMD_CAL:		// ADDR: 1000 xxx0
					nSlaveReceive = 12;
					break;

				case TCH_CMD_THR:		// ADDR: 1001 xxx0
					nSlaveReceive = 4;
					break;

				case TCH_CMD_BTN:		// ADDR: 1010 xxx0
					nSlaveReceive = 12;
					break;

				case TCH_CMD_IRQ:		// ADDR: 1011 xxx0
					nSlaveReceive = 3;
					break;
				}
			}
			SMB0CN0_ACK = 1;			// SMB0CN_ACK the received slave address
		}
		else							// If received slave address does not
		{								// match,
			SMB0CN0_ACK = 0;			// NACK received address
		}

		break;

	// Slave Receiver: Data received
	case SMB_SRDB:

		if(SMB0CN0_ARBLOST == 0)		// No bus error
		{
			// Store incoming data
			SMB_DATA_IN_SLAVE[slaveBytesReceived++] = SMB0DAT;

			if (slaveBytesReceived >= nSlaveReceive)
			{
				DATA_READY = true;		// Indicate new data received
				IS_SLAVE = false;
			}
			SMB0CN0_ACK = 1;			// SMB0CN_ACK received data
		}
		else							// Bus error detected
		{
			SMB0CN0_STA = 0;
			SMB0CN0_STO = 0;
			SMB0CN0_ACK = 0;
		}

		break;

	// Slave Receiver: Stop received while either a Slave Receiver or Slave
	// Transmitter
	case SMB_SRSTO:

		if(arbitration_lost == 1)
		{
			SMB0CN0_STA = 1;	// The SMB0CN_ARBLOST bit indicated the master
			// lost arbitration
			// reschedule the transfer

			arbitration_lost = 0;
		}

		SMB0CN0_STO = 0;		// SMB0CN_STO must be cleared by software when
		// a STOP is detected as a slave
		break;

	// Slave Transmitter: Data byte transmitted
	case SMB_STDB:
        if (SMB0CN0_ACK == 1)	// If Master SMB0CN0_ACK's, send the next byte
        {
           if (slaveBytesSent < nSlaveSend)
           {
              // Prepare next outgoing byte
              SMB0DAT = SMB_DATA_OUT_SLAVE[slaveBytesSent++];
           }
           else
           {
        	   IS_SLAVE = false;
           }
        }
		break;

	// Slave Transmitter: Arbitration lost, Stop detected
	//
	// This state will only be entered on a bus error condition.
	// In normal operation, the slave is no longer sending data or has
	// data pending when a STOP is received from the master, so the SMB0CN_TXMODE
	// bit is cleared and the slave goes to the SRSTO state.
	case SMB_STSTO:				// SMB0CN_STO must be cleared by software when
		SMB0CN0_STO = 0;		// a STOP is detected as a slave
		break;

	// Undefined cases
	default:
		M_FAIL = 1;// Indicate failed transfer
				   // and handle at end of ISR
		break;

	} // end switch

	if(M_FAIL)// If the transfer failed,
	{
		SMB0CF &= ~0x80;				// Reset communication
		SMB0CF |= 0x80;
		SMB0CN0_STA = 0;
		SMB0CN0_STO = 0;
		SMB0CN0_ACK = 0;

		SMB_BUSY = false;				// Free SMBus
		IS_SLAVE = false;

		M_FAIL = 0;

		NUM_ERRORS = 0;					// Indicate an error occurred
	}

	SMB0CN0_SI = 0;						// Clear interrupt flag
}

//-----------------------------------------------------------------------------
// TIMER3_ISR
//-----------------------------------------------------------------------------
//
// TIMER3 ISR Content goes here. Remember to clear flag bits:
// TMR3CN::TF3H (Timer # High Byte Overflow Flag)
// TMR3CN::TF3L (Timer # Low Byte Overflow Flag)
//
// A Timer3 interrupt indicates an SMBus SCL low timeout.
// The SMBus is disabled and re-enabled here
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER3_ISR, TIMER3_IRQn)
{
	SMB0CF &= ~0x80;					// Disable SMBus
	SMB0CF |= 0x80;						// Re-enable SMBus
	TMR3CN0 &= ~0x80;					// Clear Timer3 interrupt-pending flag
	SMB0CN0_STA = 0;
	SMB_BUSY = false;					// Free SMBus
	IS_SLAVE = false;
}
