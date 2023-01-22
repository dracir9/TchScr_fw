//=========================================================
// src/myProject_main.c: generated by Hardware Configurator
//
// This file will be updated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!!
//=========================================================

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB1_Register_Enums.h>                  // SFR declarations
#include <MATH.H>
#include "InitDevice.h"
#include "TouchScreen.h"
#include "UART.h"
// $[Generated Includes]
// [Generated Includes]$

int16_t dx = 320;
int16_t rx_min = 110;
int16_t rx_max = 780;

int16_t dy = 480;
int16_t ry_min = 65;
int16_t ry_max = 885;

uint16_t p_min = 100;
uint16_t p_max = 20000;

bool touchIRQ = false;
bool buttonIRQ = false;
bool flipXY = false;
bool sendEvent = false;
volatile bool IS_READY = true;

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
  // $[SiLabs Startup]
  // [SiLabs Startup]$
}
//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
	Vec2 point;
	bool valid = false;
	bool pressed = false;
	bool lastPressed = false;
	uint16_t lastP = 0;

	// Call hardware initialization routine
	enter_DefaultMode_from_RESET();

	TCON_IE0 = 0;	// Clear external interrupt

	// Enable interrupts
	IE_EA = 1;

	while (1)
	{
		if (wakeFromIdle)
		{
			wakeFromIdle = false;
			startTchRead();
		}
		if (readComplete)
		{
			readComplete = false;
			valid = true;
			if (touchPoint.z > p_min && touchPoint.z < p_max)
			{
				if (touchPoint.z < lastP)	// Pressure is decreasing
				{
					// Interpolate
					//point.x = x_min + (point.x - rx_min) * (x_max - x_min)/(rx_max-rx_min); // -> 41 Cycles
					//point.x = (point.x - rx_min) * x_max/(rx_max-rx_min); // ->28 Cycles
					//point.x = (int32_t)(point.x - rx_min) * x_max/(rx_max-rx_min); // ->130 Cycles
					//point.x = point.x * a + rx_min; // -> 200
					if (flipXY)
					{
						point.x = (int32_t)(touchPoint.y - rx_min) * dx/rx_max; // ->125 Cycles
						point.y = (int32_t)(touchPoint.x - ry_min) * dy/ry_max;
					}
					else
					{
						point.x = (int32_t)(touchPoint.x - rx_min) * dx/rx_max; // ->125 Cycles
						point.y = (int32_t)(touchPoint.y - ry_min) * dy/ry_max;
					}

					if (abs(LAST_POINT.x - point.x) > 2 || abs(LAST_POINT.y - point.y) > 2)
						valid = false;
					else
						pressed = true;

					LAST_POINT = point;
				}
				else
					valid = false;

				lastP = touchPoint.z;

				startTchRead();
			}
			else
			{
				lastP = 0;
				pressed = false;
				setIdle();
			}

			if (valid)
			{
				if (!lastPressed && pressed) touchState = TCH_PRESS;
				else if (lastPressed && pressed) touchState = TCH_HOLD;
				else if (lastPressed && !pressed) touchState = TCH_RELEASE;
				else touchState = TCH_FREE;

				checkButtons(point.x, point.y);

				// Send notifications
				// Touch interrupts enabled and cool down ready
				if (touchIRQ && IS_READY)
				{
					sendEvent = true;
					IS_READY = false;
					TMR2CN0 |= TMR2CN0_TR2__RUN;		// Enable cool down timer
				}

				// Button interrupts
				if (buttonIRQ && activeBtn != EVNT_IDLE)
					sendEvent = true;

				if (sendEvent)
				{
					while(UART_BUSY);
					UART_DATA_OUT[0] = activeBtn;
					UART_DATA_OUT[1] = ((uint8_t*)&point.x)[1];
					UART_DATA_OUT[2] = ((uint8_t*)&point.x)[0];
					UART_DATA_OUT[3] = ((uint8_t*)&point.y)[1];
					UART_DATA_OUT[4] = ((uint8_t*)&point.y)[0];
					UART_Write(0, 5);
					sendEvent = false;
				}

				lastPressed = pressed;
			}
		}

		if (DATA_READY)
		{
			switch (CMD_ID)
			{
			case TCH_CMD_CAL:
				((uint8_t*)&dx)[1] = UART_DATA_IN[0];
				((uint8_t*)&dx)[0] = UART_DATA_IN[1];
				((uint8_t*)&rx_min)[1] = UART_DATA_IN[2];
				((uint8_t*)&rx_min)[0] = UART_DATA_IN[3];
				((uint8_t*)&rx_max)[1] = UART_DATA_IN[4];
				((uint8_t*)&rx_max)[0] = UART_DATA_IN[5];
				((uint8_t*)&dy)[1] = UART_DATA_IN[6];
				((uint8_t*)&dy)[0] = UART_DATA_IN[7];
				((uint8_t*)&ry_min)[1] = UART_DATA_IN[8];
				((uint8_t*)&ry_min)[0] = UART_DATA_IN[9];
				((uint8_t*)&ry_max)[1] = UART_DATA_IN[10];
				((uint8_t*)&ry_max)[0] = UART_DATA_IN[11];
				rx_max = rx_max - rx_min;
				ry_max = ry_max - ry_min;
				break;

			case TCH_CMD_THR:
				((uint8_t*)&p_min)[1] = UART_DATA_IN[0];
				((uint8_t*)&p_min)[0] = UART_DATA_IN[1];
				((uint8_t*)&p_max)[1] = UART_DATA_IN[2];
				((uint8_t*)&p_max)[0] = UART_DATA_IN[3];
				break;

			case TCH_CMD_BTN:
				editButton(UART_DATA_IN[0], UART_DATA_IN[1], UART_DATA_IN[2],
						UART_DATA_IN[4] | UART_DATA_IN[5] << 8,
						UART_DATA_IN[6] | UART_DATA_IN[7] << 8,
						UART_DATA_IN[8] | UART_DATA_IN[9] << 8,
						UART_DATA_IN[10] | UART_DATA_IN[11] << 8);
				break;

			case TCH_CMD_IRQ:
				touchIRQ = UART_DATA_IN[0] & 0x01;
				buttonIRQ = UART_DATA_IN[0] & 0x02;
				flipXY = UART_DATA_IN[0] & 0x04;
				TMR2RLL = UART_DATA_IN[1];
				TMR2RLH = UART_DATA_IN[2];
				break;

			// Commands that return a response
			case TCH_CMD_LEV:
				while(UART_BUSY);
				UART_DATA_OUT[0] = activeBtn;
				UART_DATA_OUT[1] = ((uint8_t*)&point.x)[1];
				UART_DATA_OUT[2] = ((uint8_t*)&point.x)[0];
				UART_DATA_OUT[3] = ((uint8_t*)&point.y)[1];
				UART_DATA_OUT[4] = ((uint8_t*)&point.y)[0];
				UART_Write(0x01, 5);
			}
			DATA_READY = false;
		}
	}
}

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN::TF2L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
	TMR2CN0 &= ~0x80;					// Clear Timer2 interrupt-pending flag
	TMR2CN0 &= ~(TMR2CN0_TR2__RUN);		// Disable timeout timer
	IS_READY = true;
}
