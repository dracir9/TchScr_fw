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
volatile uint8_t pressCnt = 1;

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
	bool anyHold = false;
	activeBtn = EVNT_IDLE;

	for (i = 0; i < MAX_BUTTONS; i++)
	{
		if (x >= buttonArr[i].xmin && x < buttonArr[i].xmax &&
			y >= buttonArr[i].ymin && y < buttonArr[i].ymax)
		{
			if ((buttonArr[i].trigger & 0x01) && touchState == TCH_PRESS) // Press events
			{
				activeBtn = i | EVNT_PRESS;
			}

			if ((buttonArr[i].trigger & 0x02) && touchState == TCH_HOLD) // Hold Events
			{
				anyHold = true;
				if (lastHoldBtn != i)
				{
					if (pressCnt == 0)
						activeBtn = lastHoldBtn | EVNT_HOLD_END;	// Hold end
					lastHoldBtn = i;
					pressCnt = 8; // Reset count
					TCON_TR1 = 1; // Enable timer
				}
				else if (pressCnt > (buttonArr[i].trigger & 0xF8))
				{
					TCON_TR1 = 0;	// Stop timer to prevent re-triggering
					pressCnt = 0;
					activeBtn = i | EVNT_HOLD_STRT;				// Hold start
					return activeBtn;
				}
			}

			if (buttonArr[i].trigger & 0x04) // Release events
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
		if (lastHoldBtn != -1 && activeBtn == EVNT_IDLE && pressCnt == 0)
		{
			activeBtn = lastHoldBtn | EVNT_HOLD_END;				// Hold end
			pressCnt = 1;
		}
		lastHoldBtn = -1;
	}

	if (touchState == TCH_RELEASE)
		lastPressBtn = -1;

	return activeBtn;
}

void editButton(uint8_t id, uint8_t trg, int16_t xl, int16_t xh, int16_t yl, int16_t yh)
{
	int i = 0;
	if (id == 31)
	{
		for (i = 0; i < MAX_BUTTONS; ++i)
		{
			buttonArr[i].trigger = 0;
		}
	}
	else
	{
		if (id >= MAX_BUTTONS) id = MAX_BUTTONS -1;
		buttonArr[id].trigger = trg;
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
		if (ADC0 == 0)
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
// TIMER1_ISR
//-----------------------------------------------------------------------------
//
// TIMER1 ISR Content goes here. Remember to clear flag bits:
// TCON::TF1 (Timer 1 Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (TIMER1_ISR, TIMER1_IRQn)
{
	// Increment counter
	pressCnt += 4;	// Keep first 2 bits unchanged

	// Start at 14494
	TH1 = 0x38;
	TL1 = 0x9E;

	// Clear flag
	TCON_TF1 = 0;
}
