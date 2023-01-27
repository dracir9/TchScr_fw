/*
 * TouchScreen.c
 *
 *  Created on: 10 jul. 2021
 *      Author: Ricard
 */

#include "TouchScreen.h"

volatile uint8_t readTchState;
volatile bool readComplete = false;
volatile Vec3 touchPoint;
volatile uint8_t pressCnt = 254;

int8_t activeBtn = EVNT_IDLE;	// First 3 bits: Event ID || Last 5 bits: Button ID
int8_t lastHoldBtn = -1;
int8_t lastPressBtn = -1;

uint8_t touchState = TCH_FREE;

Button xdata buttonArr[MAX_BUTTONS] = {0};

bool wakeFromIdle = false;

// Updated every conversion
int8_t checkButtons(int16_t x, int16_t y)
{
	uint8_t i = 0;
	uint8_t state = 0;
	bool anyHold = false;
	activeBtn = EVNT_IDLE;

	for (i = 0; i < MAX_BUTTONS; i++)
	{
		if (x >= buttonArr[i].xmin && x < buttonArr[i].xmax &&
			y >= buttonArr[i].ymin && y < buttonArr[i].ymax)
		{
			state = buttonArr[i].state;
			if ((state & 0x01) && touchState == TCH_PRESS) // Press events
			{
				activeBtn = i | EVNT_PRESS;
			}

			if ((state & 0x02) && touchState == TCH_HOLD) // Hold Events
			{
				anyHold = true;
				// If not pressing the same button
				if (lastHoldBtn != i)
				{
					// If button Hold Start was triggered
					if (pressCnt == 255)
						activeBtn = lastHoldBtn | EVNT_HOLD_END;	// Hold end

					// User is pressing this button now
					lastHoldBtn = i;
					pressCnt = buttonArr[i].holdTime; // Reset count
					TCON_TR0 = 1; // Enable timer
				}
				// If timeout exhausted
				else if (!pressCnt)
				{
					// Ensure timer is stopped
					TCON_TR0 = 0;

					// Flag to start hold tick
					pressCnt = 255;
					activeBtn = i | EVNT_HOLD_STRT;				// Hold start
					return activeBtn;
				}
			}

			if ((state & 0x04) && pressCnt == 255)
			{
				activeBtn = i | EVNT_HOLD_TICK;
			}

			if (state & 0x08) // Release events
			{
				if (touchState == TCH_PRESS)
					lastPressBtn = i;
				else if (touchState == TCH_RELEASE && lastPressBtn == i)
					activeBtn = i | EVNT_RELEASE;
			}
		}
	}

	if (!anyHold)
	{
		if (lastHoldBtn != -1 && activeBtn == EVNT_IDLE && pressCnt == 255)
		{
			activeBtn = lastHoldBtn | EVNT_HOLD_END;				// Hold end
			pressCnt = 254;
		}
		lastHoldBtn = -1;
	}

	if (touchState == TCH_RELEASE)
		lastPressBtn = -1;

	return activeBtn;
}

void editButton(uint8_t id, uint8_t trg, uint8_t hold, int16_t xl, int16_t xh, int16_t yl, int16_t yh)
{
	int i = 0;
	if (id == 31)
	{
		for (i = 0; i < MAX_BUTTONS; ++i)
		{
			buttonArr[i].state = 0;
		}
	}
	else
	{
		if (id >= MAX_BUTTONS) id = MAX_BUTTONS -1;
		buttonArr[id].holdTime = hold & 0x7F;
		buttonArr[id].state = trg & 0x0F;
		buttonArr[id].xmin = xl;
		buttonArr[id].xmax = xh;
		buttonArr[id].ymin = yl;
		buttonArr[id].ymax = yh;
	}
}

//SI_REENTRANT_FUNCTION(startTchRead, void, (bool X, bool Y, bool Z))
void startTchRead()
{
	setGPIO_X();
	readTchState = 1;
	ADC0CN0_ADBUSY = SET;
}

void setGPIO_X()
{
	/*
	 * YM - Input
	 * XP - HIGH
	 * YP - Input
	 * XM - LOW
	 */

	// Configure Pins
	P0MDOUT |= XM_PIN_MSK | XP_PIN_MSK;
	P0MDOUT &= ~(YM_PIN_MSK | YP_PIN_MSK);

	P0MDIN |= XM_PIN_MSK | XP_PIN_MSK;
	P0MDIN &= ~(YM_PIN_MSK | YP_PIN_MSK);

	XP_PIN = HIGH;
	XM_PIN = LOW;

	// Configure ADC
	ADC0MX = X_READ;
}

void setGPIO_Y()
{
	/*
	 * YM - LOW
	 * XP - Input
	 * YP - HIGH
	 * XM - Input
	 */

	P0MDOUT |= YM_PIN_MSK | YP_PIN_MSK;
	P0MDOUT &= ~(XM_PIN_MSK | XP_PIN_MSK);

	P0MDIN |= YM_PIN_MSK | YP_PIN_MSK;
	P0MDIN &= ~(XM_PIN_MSK | XP_PIN_MSK);

	YP_PIN = HIGH;
	YM_PIN = LOW;

	// Configure ADC
	ADC0MX = Y_READ;
}

void setGPIO_Z()
{
	/*
	 * YM - Input
	 * XP - Input
	 * YP - HIGH
	 * XM - LOW
	 */
	P0MDOUT |= XM_PIN_MSK | YP_PIN_MSK;
	P0MDOUT &= ~(XP_PIN_MSK | YM_PIN_MSK);

	P0MDIN |= XM_PIN_MSK | YP_PIN_MSK;
	P0MDIN &= ~(XP_PIN_MSK | YM_PIN_MSK);

	XM_PIN = LOW;
	YP_PIN = HIGH;

	// Configure ADC
	ADC0MX = Z_READ;
}

void setIdle()
{
	/*
	 * YM - LOW
	 * XP - Open-Weak PullUp
	 * YP - LOW
	 * XM - Open-Weak PullUp
	 */
	P0MDOUT |= YM_PIN_MSK | YP_PIN_MSK;		// Push pull
	P0MDOUT &= ~(XM_PIN_MSK | XP_PIN_MSK);	// Open drain

	P0MDIN |= YM_PIN_MSK | YP_PIN_MSK | XM_PIN_MSK | XP_PIN_MSK;

	P0 |= XM_PIN_MSK | XP_PIN_MSK;
	P0 &= ~(YM_PIN_MSK | YP_PIN_MSK);

	ADC0MX = ADC0MX_ADC0MX__NONE;

	TCON_IE0 = CLEAR;	// Reset IT pending flag
	IE_EX0 = ENABLE;
}

SI_INTERRUPT(ADC0EOC_ISR, ADC0EOC_IRQn)
{
	static Vec3 readPoint;

	ADC0CN0_ADINT = CLEAR;  // Clear interrupt flag

	// Store current read
	switch (readTchState)
	{
	case 1:	// X Position
		readPoint.x = ADC0;
		setGPIO_Y();
		readTchState = 2;
		break;

	case 2:	// Y Position
		readPoint.y = ADC0;
		setGPIO_Z();
		readTchState = 3;
		break;

	case 3:	// Z Position
		if (!ADC0)
			readPoint.z = -1;
		else
			readPoint.z = (int32_t)(1024-ADC0)*readPoint.x/ADC0 - readPoint.y;
		touchPoint = readPoint;
		readComplete = true;
		return;	// Do not initiate a new conversion
	}

	// Start new read
	ADC0CN0_ADBUSY = SET;
}

//-----------------------------------------------------------------------------
// TIMER0_ISR
//-----------------------------------------------------------------------------
//
// TIMER1 ISR Content goes here. Remember to clear flag bits:
// TCON::TF1 (Timer 1 Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (TIMER0_ISR, TIMER0_IRQn)
{
	// Decrement counter
	pressCnt--;
	if (!pressCnt)
		TCON_TR0 = 0; // Stop timer

	// Start at 14494
	TH0 = 0x38;
	TL0 = 0x9E;

	// Clear flag
	TCON_TF0 = 0;
}
