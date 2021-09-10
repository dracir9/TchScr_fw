/*
 * TouchScreen.h
 *
 *  Created on: 10 jul. 2021
 *      Author: Ricard
 */

#ifndef INC_TOUCHSCREEN_H_
#define INC_TOUCHSCREEN_H_

#include <SI_EFM8BB1_Register_Enums.h>

// State macros
#define HIGH	1
#define SET		1
#define ENABLE	1

// State macros
#define LOW		0
#define CLEAR	0
#define DISABLE 0

/**
 * Pin Configuration
 */

#define XP_PIN 	P0_B1
#define XM_PIN 	P0_B7
#define YP_PIN 	P0_B6
#define YM_PIN 	P0_B0

#define XP_PIN_MSK 	P0MDOUT_B1__BMASK
#define XM_PIN_MSK 	P0MDOUT_B7__BMASK
#define YP_PIN_MSK 	P0MDOUT_B6__BMASK
#define YM_PIN_MSK 	P0MDOUT_B0__BMASK

#define X_READ		ADC0MX_ADC0MX__ADC0P0	// ADC channel to use to read X axis (must map to YM or YP pin)
#define Y_READ		ADC0MX_ADC0MX__ADC0P1	// ADC channel to use to read Y axis (must map to XM or XP pin)
#define Z_READ		ADC0MX_ADC0MX__ADC0P1

//
#define TCH_PRESS	1
#define TCH_HOLD	2
#define TCH_RELEASE	3
#define TCH_FREE	4

#define MAX_BUTTONS	20

typedef struct
{
	int16_t x;
	int16_t y;
	uint16_t z;
} Vec3;

typedef struct
{
	int16_t x;
	int16_t y;
} Vec2;

typedef struct
{
	uint8_t trigger;
	int16_t xmin;
	int16_t xmax;
	int16_t ymin;
	int16_t ymax;
} Button;

int8_t checkButtons(int16_t x, int16_t y);
void editButton(uint8_t id, uint8_t trg, int16_t xl, int16_t xh, int16_t yl, int16_t yh);
//SI_REENTRANT_FUNCTION_PROTO(startTchRead, void, (bool X, bool Y, bool Z));
void startTchRead();
void setGPIO_X();
void setGPIO_Y();
void setGPIO_Z();
void setIdle();

extern volatile bool readComplete;
extern volatile Vec3 touchPoint;
extern uint8_t touchState;
extern int8_t activeBtn;
extern bool wakeFromIdle;

#endif /* INC_TOUCHSCREEN_H_ */
