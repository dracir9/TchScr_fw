//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"

// USER PROTOTYPES
// USER FUNCTIONS

// $[Library Includes]
// [Library Includes]$

//==============================================================================
// enter_DefaultMode_from_RESET
//==============================================================================
extern void enter_DefaultMode_from_RESET(void)
{
	// $[Config Calls]
	WDT_0_enter_DefaultMode_from_RESET();
	PORTS_0_enter_DefaultMode_from_RESET();
	PORTS_1_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	ADC_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	TIMER01_0_enter_DefaultMode_from_RESET();
	TIMER16_2_enter_DefaultMode_from_RESET();
	TIMER16_3_enter_DefaultMode_from_RESET();
	TIMER_SETUP_0_enter_DefaultMode_from_RESET();
	SMBUS_0_enter_DefaultMode_from_RESET();
	EXTINT_0_enter_DefaultMode_from_RESET();
	INTERRUPT_0_enter_DefaultMode_from_RESET();
	// [Config Calls]$

}

extern void PORTS_0_enter_DefaultMode_from_RESET(void)
{
	// $[P0 - Port 0 Pin Latch]
	/***********************************************************************
	 - P0.0 is high. Set P0.0 to drive or float high
	 - P0.1 is low. Set P0.1 to drive low
	 - P0.2 is high. Set P0.2 to drive or float high
	 - P0.3 is low. Set P0.3 to drive low
	 - P0.4 is high. Set P0.4 to drive or float high
	 - P0.5 is high. Set P0.5 to drive or float high
	 - P0.6 is high. Set P0.6 to drive or float high
	 - P0.7 is high. Set P0.7 to drive or float high
	 ***********************************************************************/
	P0 = P0_B0__HIGH | P0_B1__LOW | P0_B2__HIGH | P0_B3__LOW | P0_B4__HIGH
			| P0_B5__HIGH | P0_B6__HIGH | P0_B7__HIGH;
	// [P0 - Port 0 Pin Latch]$

	// $[P0MDOUT - Port 0 Output Mode]
	/***********************************************************************
	 - P0.0 output is open-drain
	 - P0.1 output is push-pull
	 - P0.2 output is open-drain
	 - P0.3 output is push-pull
	 - P0.4 output is open-drain
	 - P0.5 output is open-drain
	 - P0.6 output is open-drain
	 - P0.7 output is open-drain
	 ***********************************************************************/
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__PUSH_PULL
			| P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B3__PUSH_PULL
			| P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN
			| P0MDOUT_B6__OPEN_DRAIN | P0MDOUT_B7__OPEN_DRAIN;
	// [P0MDOUT - Port 0 Output Mode]$

	// $[P0MDIN - Port 0 Input Mode]
	// [P0MDIN - Port 0 Input Mode]$

	// $[P0SKIP - Port 0 Skip]
	/***********************************************************************
	 - P0.0 pin is skipped by the crossbar
	 - P0.1 pin is skipped by the crossbar
	 - P0.2 pin is skipped by the crossbar
	 - P0.3 pin is skipped by the crossbar
	 - P0.4 pin is skipped by the crossbar
	 - P0.5 pin is skipped by the crossbar
	 - P0.6 pin is skipped by the crossbar
	 - P0.7 pin is skipped by the crossbar
	 ***********************************************************************/
	P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED | P0SKIP_B2__SKIPPED
			| P0SKIP_B3__SKIPPED | P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED
			| P0SKIP_B6__SKIPPED | P0SKIP_B7__SKIPPED;
	// [P0SKIP - Port 0 Skip]$

	// $[P0MASK - Port 0 Mask]
	// [P0MASK - Port 0 Mask]$

	// $[P0MAT - Port 0 Match]
	// [P0MAT - Port 0 Match]$

}

extern void PBCFG_0_enter_DefaultMode_from_RESET(void)
{
	// $[XBR2 - Port I/O Crossbar 2]
	/***********************************************************************
	 - Weak Pullups enabled 
	 - Crossbar enabled
	 ***********************************************************************/
	XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;
	// [XBR2 - Port I/O Crossbar 2]$

	// $[PRTDRV - Port Drive Strength]
	// [PRTDRV - Port Drive Strength]$

	// $[XBR0 - Port I/O Crossbar 0]
	/***********************************************************************
	 - UART I/O unavailable at Port pin
	 - SPI I/O unavailable at Port pins
	 - SMBus 0 I/O routed to Port pins
	 - CP0 unavailable at Port pin
	 - Asynchronous CP0 unavailable at Port pin
	 - CP1 unavailable at Port pin
	 - Asynchronous CP1 unavailable at Port pin
	 - SYSCLK unavailable at Port pin
	 ***********************************************************************/
	XBR0 = XBR0_URT0E__DISABLED | XBR0_SPI0E__DISABLED | XBR0_SMB0E__ENABLED
			| XBR0_CP0E__DISABLED | XBR0_CP0AE__DISABLED | XBR0_CP1E__DISABLED
			| XBR0_CP1AE__DISABLED | XBR0_SYSCKE__DISABLED;
	// [XBR0 - Port I/O Crossbar 0]$

	// $[XBR1 - Port I/O Crossbar 1]
	// [XBR1 - Port I/O Crossbar 1]$

}

extern void WDT_0_enter_DefaultMode_from_RESET(void)
{
	// $[Watchdog Timer Init Variable Declarations]
	uint32_t i;
	bool ea;
	// [Watchdog Timer Init Variable Declarations]$

	// $[WDTCN - Watchdog Timer Control]
	// Deprecated
	// [WDTCN - Watchdog Timer Control]$

	// $[WDTCN_2 - Watchdog Timer Control]

	// Feed WDT timer before disabling (Erratum WDT_E102)
	WDTCN = 0xA5;

	// Add 2 LFO cycle delay before disabling WDT (Erratum WDT_E102)
	for (i = 0; i < (2 * 3062500UL) / (10000 * 3); i++)
	{
		NOP();
	}

	// Disable WDT
	ea = IE_EA;
	IE_EA = 0;
	WDTCN = 0xDE;
	WDTCN = 0xAD;
	IE_EA = ea;

	// [WDTCN_2 - Watchdog Timer Control]$

}

extern void PORTS_1_enter_DefaultMode_from_RESET(void)
{
	// $[P1 - Port 1 Pin Latch]
	// [P1 - Port 1 Pin Latch]$

	// $[P1MDOUT - Port 1 Output Mode]
	// [P1MDOUT - Port 1 Output Mode]$

	// $[P1MDIN - Port 1 Input Mode]
	// [P1MDIN - Port 1 Input Mode]$

	// $[P1SKIP - Port 1 Skip]
	/***********************************************************************
	 - P1.0 pin is skipped by the crossbar
	 - P1.1 pin is not skipped by the crossbar
	 - P1.2 pin is not skipped by the crossbar
	 - P1.3 pin is not skipped by the crossbar
	 ***********************************************************************/
	P1SKIP = P1SKIP_B0__SKIPPED | P1SKIP_B1__NOT_SKIPPED
			| P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED;
	// [P1SKIP - Port 1 Skip]$

	// $[P1MASK - Port 1 Mask]
	// [P1MASK - Port 1 Mask]$

	// $[P1MAT - Port 1 Match]
	// [P1MAT - Port 1 Match]$

}

extern void ADC_0_enter_DefaultMode_from_RESET(void)
{
	// $[ADC0CN1 - ADC0 Control 1]
	/***********************************************************************
	 - Enable the common mode buffer
	 ***********************************************************************/
	ADC0CN1 = ADC0CN1_ADCMBE__CM_BUFFER_ENABLED;
	// [ADC0CN1 - ADC0 Control 1]$

	// $[ADC0MX - ADC0 Multiplexer Selection]
	// [ADC0MX - ADC0 Multiplexer Selection]$

	// $[ADC0CF - ADC0 Configuration]
	/***********************************************************************
	 - SAR Clock Divider = 0x01
	 - ADC0 operates in 10-bit or 12-bit mode 
	 - The on-chip PGA gain is 0.5
	 - Delayed Track Mode
	 ***********************************************************************/
	ADC0CF = (0x01 << ADC0CF_ADSC__SHIFT) | ADC0CF_AD8BE__NORMAL
			| ADC0CF_ADGN__GAIN_0P5 | ADC0CF_ADTM__TRACK_DELAYED;
	// [ADC0CF - ADC0 Configuration]$

	// $[ADC0AC - ADC0 Accumulator Configuration]
	/***********************************************************************
	 - Right justified. Shifted right by 3 bits
	 - Disable 12-bit mode
	 - ADC0H:ADC0L contain the result of the latest conversion when Burst
	 Mode is disabled
	 - Perform and Accumulate 8 conversions 
	 ***********************************************************************/
	ADC0AC = ADC0AC_ADSJST__RIGHT_SHIFT_3 | ADC0AC_AD12BE__12_BIT_DISABLED
			| ADC0AC_ADAE__ACC_DISABLED | ADC0AC_ADRPT__ACC_8;
	// [ADC0AC - ADC0 Accumulator Configuration]$

	// $[ADC0TK - ADC0 Burst Mode Track Time]
	// [ADC0TK - ADC0 Burst Mode Track Time]$

	// $[ADC0PWR - ADC0 Power Control]
	// [ADC0PWR - ADC0 Power Control]$

	// $[ADC0GTH - ADC0 Greater-Than High Byte]
	// [ADC0GTH - ADC0 Greater-Than High Byte]$

	// $[ADC0GTL - ADC0 Greater-Than Low Byte]
	// [ADC0GTL - ADC0 Greater-Than Low Byte]$

	// $[ADC0LTH - ADC0 Less-Than High Byte]
	// [ADC0LTH - ADC0 Less-Than High Byte]$

	// $[ADC0LTL - ADC0 Less-Than Low Byte]
	// [ADC0LTL - ADC0 Less-Than Low Byte]$

	// $[ADC0CN0 - ADC0 Control 0]
	/***********************************************************************
	 - Enable ADC0 
	 - Enable ADC0 burst mode
	 ***********************************************************************/
	ADC0CN0 |= ADC0CN0_ADEN__ENABLED | ADC0CN0_ADBMEN__BURST_ENABLED;
	// [ADC0CN0 - ADC0 Control 0]$

}

extern void CLOCK_0_enter_DefaultMode_from_RESET(void)
{
	// $[CLKSEL - Clock Select]
	/***********************************************************************
	 - Clock derived from the Internal High-Frequency Oscillator
	 - SYSCLK is equal to selected clock source divided by 1
	 ***********************************************************************/
	CLKSEL = CLKSEL_CLKSL__HFOSC | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	// [CLKSEL - Clock Select]$

}

extern void TIMER01_0_enter_DefaultMode_from_RESET(void)
{
	// $[Timer Initialization]
	//Save Timer Configuration
	uint8_t TCON_save;
	TCON_save = TCON;
	//Stop Timers
	TCON &= ~TCON_TR0__BMASK & ~TCON_TR1__BMASK;

	// [Timer Initialization]$

	// $[TH0 - Timer 0 High Byte]
	/***********************************************************************
	 - Timer 0 High Byte = 0xE0
	 ***********************************************************************/
	TH0 = (0xE0 << TH0_TH0__SHIFT);
	// [TH0 - Timer 0 High Byte]$

	// $[TL0 - Timer 0 Low Byte]
	// [TL0 - Timer 0 Low Byte]$

	// $[TH1 - Timer 1 High Byte]
	/***********************************************************************
	 - Timer 1 High Byte = 0x38
	 ***********************************************************************/
	TH1 = (0x38 << TH1_TH1__SHIFT);
	// [TH1 - Timer 1 High Byte]$

	// $[TL1 - Timer 1 Low Byte]
	/***********************************************************************
	 - Timer 1 Low Byte = 0x9E
	 ***********************************************************************/
	TL1 = (0x9E << TL1_TL1__SHIFT);
	// [TL1 - Timer 1 Low Byte]$

	// $[Timer Restoration]
	//Restore Timer Configuration
	TCON |= (TCON_save & TCON_TR0__BMASK) | (TCON_save & TCON_TR1__BMASK);

	// [Timer Restoration]$

}

extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void)
{
	// $[CKCON0 - Clock Control 0]
	/***********************************************************************
	 - System clock divided by 48
	 - Counter/Timer 0 uses the system clock
	 - Timer 2 high byte uses the clock defined by T2XCLK in TMR2CN0
	 - Timer 2 low byte uses the clock defined by T2XCLK in TMR2CN0
	 - Timer 3 high byte uses the clock defined by T3XCLK in TMR3CN0
	 - Timer 3 low byte uses the clock defined by T3XCLK in TMR3CN0
	 - Timer 1 uses the clock defined by the prescale field, SCA
	 ***********************************************************************/
	CKCON0 = CKCON0_SCA__SYSCLK_DIV_48 | CKCON0_T0M__SYSCLK
			| CKCON0_T2MH__EXTERNAL_CLOCK | CKCON0_T2ML__EXTERNAL_CLOCK
			| CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__EXTERNAL_CLOCK
			| CKCON0_T1M__PRESCALE;
	// [CKCON0 - Clock Control 0]$

	// $[TMOD - Timer 0/1 Mode]
	/***********************************************************************
	 - Mode 2, 8-bit Counter/Timer with Auto-Reload
	 - Mode 1, 16-bit Counter/Timer
	 - Timer Mode
	 - Timer 0 enabled when TR0 = 1 irrespective of INT0 logic level
	 - Timer Mode
	 - Timer 1 enabled when TR1 = 1 irrespective of INT1 logic level
	 ***********************************************************************/
	TMOD = TMOD_T0M__MODE2 | TMOD_T1M__MODE1 | TMOD_CT0__TIMER
			| TMOD_GATE0__DISABLED | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
	// [TMOD - Timer 0/1 Mode]$

	// $[TCON - Timer 0/1 Control]
	/***********************************************************************
	 - INT0 is edge triggered
	 - Start Timer 0 running
	 - Start Timer 1 running
	 ***********************************************************************/
	TCON |= TCON_IT0__EDGE | TCON_TR0__RUN | TCON_TR1__RUN;
	// [TCON - Timer 0/1 Control]$

}

extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void)
{
	// $[EIE1 - Extended Interrupt Enable 1]
	/***********************************************************************
	 - Enable interrupt requests generated by the ADINT flag
	 - Disable ADC0 Window Comparison interrupt
	 - Disable CP0 interrupts
	 - Disable CP1 interrupts
	 - Disable all Port Match interrupts
	 - Disable all PCA0 interrupts
	 - Enable interrupt requests generated by SMB0
	 - Enable interrupt requests generated by the TF3L or TF3H flags
	 ***********************************************************************/
	EIE1 = EIE1_EADC0__ENABLED | EIE1_EWADC0__DISABLED | EIE1_ECP0__DISABLED
			| EIE1_ECP1__DISABLED | EIE1_EMAT__DISABLED | EIE1_EPCA0__DISABLED
			| EIE1_ESMB0__ENABLED | EIE1_ET3__ENABLED;
	// [EIE1 - Extended Interrupt Enable 1]$

	// $[EIP1 - Extended Interrupt Priority 1]
	/***********************************************************************
	 - ADC0 Conversion Complete interrupt set to low priority level
	 - ADC0 Window interrupt set to low priority level
	 - CP0 interrupt set to low priority level
	 - CP1 interrupt set to low priority level
	 - Port Match interrupt set to low priority level
	 - PCA0 interrupt set to low priority level
	 - SMB0 interrupt set to high priority level
	 - Timer 3 interrupts set to low priority level
	 ***********************************************************************/
	EIP1 = EIP1_PADC0__LOW | EIP1_PWADC0__LOW | EIP1_PCP0__LOW | EIP1_PCP1__LOW
			| EIP1_PMAT__LOW | EIP1_PPCA0__LOW | EIP1_PSMB0__HIGH
			| EIP1_PT3__LOW;
	// [EIP1 - Extended Interrupt Priority 1]$

	// $[IE - Interrupt Enable]
	/***********************************************************************
	 - Disable all interrupt sources
	 - Enable interrupt requests generated by the INT0 input
	 - Disable external interrupt 1
	 - Disable all SPI0 interrupts
	 - Disable all Timer 0 interrupt
	 - Enable interrupt requests generated by the TF1 flag
	 - Enable interrupt requests generated by the TF2L or TF2H flags
	 - Disable UART0 interrupt
	 ***********************************************************************/
	IE = IE_EA__DISABLED | IE_EX0__ENABLED | IE_EX1__DISABLED
			| IE_ESPI0__DISABLED | IE_ET0__DISABLED | IE_ET1__ENABLED
			| IE_ET2__ENABLED | IE_ES0__DISABLED;
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

}

extern void SMBUS_0_enter_DefaultMode_from_RESET(void)
{
	// $[SMB0ADR - SMBus 0 Slave Address]
	// [SMB0ADR - SMBus 0 Slave Address]$

	// $[SMB0ADM - SMBus 0 Slave Address Mask]
	// [SMB0ADM - SMBus 0 Slave Address Mask]$

	// $[SMB0TC - SMBus 0 Timing and Pin Control]
	/***********************************************************************
	 - SCL is mapped to the lower-numbered port pin, and SDA is mapped to the
	 higher-numbered port pin
	 - No additional SDA falling edge recognition delay 
	 ***********************************************************************/
	SMB0TC = SMB0TC_SWAP__SDA_HIGH_PIN | SMB0TC_SDD__NONE;
	// [SMB0TC - SMBus 0 Timing and Pin Control]$

	// $[SMB0CF - SMBus 0 Configuration]
	/***********************************************************************
	 - Enable the SMBus module
	 - Enable bus free timeouts
	 - Enable SCL low timeouts
	 - Enable SDA extended setup and hold times
	 ***********************************************************************/
	SMB0CF |= SMB0CF_ENSMB__ENABLED | SMB0CF_SMBFTE__FREE_TO_ENABLED
			| SMB0CF_SMBTOE__SCL_TO_ENABLED | SMB0CF_EXTHOLD__ENABLED;
	// [SMB0CF - SMBus 0 Configuration]$

}

extern void TIMER16_3_enter_DefaultMode_from_RESET(void)
{
	// $[Timer Initialization]
	// Save Timer Configuration
	uint8_t TMR3CN0_TR3_save;
	TMR3CN0_TR3_save = TMR3CN0 & TMR3CN0_TR3__BMASK;
	// Stop Timer
	TMR3CN0 &= ~(TMR3CN0_TR3__BMASK);
	// [Timer Initialization]$

	// $[TMR3CN0 - Timer 3 Control]
	// [TMR3CN0 - Timer 3 Control]$

	// $[TMR3H - Timer 3 High Byte]
	// [TMR3H - Timer 3 High Byte]$

	// $[TMR3L - Timer 3 Low Byte]
	// [TMR3L - Timer 3 Low Byte]$

	// $[TMR3RLH - Timer 3 Reload High Byte]
	/***********************************************************************
	 - Timer 3 Reload High Byte = 0x38
	 ***********************************************************************/
	TMR3RLH = (0x38 << TMR3RLH_TMR3RLH__SHIFT);
	// [TMR3RLH - Timer 3 Reload High Byte]$

	// $[TMR3RLL - Timer 3 Reload Low Byte]
	/***********************************************************************
	 - Timer 3 Reload Low Byte = 0x9E
	 ***********************************************************************/
	TMR3RLL = (0x9E << TMR3RLL_TMR3RLL__SHIFT);
	// [TMR3RLL - Timer 3 Reload Low Byte]$

	// $[TMR3CN0]
	/***********************************************************************
	 - Start Timer 3 running
	 ***********************************************************************/
	TMR3CN0 |= TMR3CN0_TR3__RUN;
	// [TMR3CN0]$

	// $[Timer Restoration]
	// Restore Timer Configuration
	TMR3CN0 |= TMR3CN0_TR3_save;
	// [Timer Restoration]$

}

extern void TIMER16_2_enter_DefaultMode_from_RESET(void)
{
	// $[Timer Initialization]
	// Save Timer Configuration
	uint8_t TMR2CN0_TR2_save;
	TMR2CN0_TR2_save = TMR2CN0 & TMR2CN0_TR2__BMASK;
	// Stop Timer
	TMR2CN0 &= ~(TMR2CN0_TR2__BMASK);
	// [Timer Initialization]$

	// $[TMR2CN0 - Timer 2 Control]
	// [TMR2CN0 - Timer 2 Control]$

	// $[TMR2H - Timer 2 High Byte]
	// [TMR2H - Timer 2 High Byte]$

	// $[TMR2L - Timer 2 Low Byte]
	// [TMR2L - Timer 2 Low Byte]$

	// $[TMR2RLH - Timer 2 Reload High Byte]
	/***********************************************************************
	 - Timer 2 Reload High Byte = 0xE1
	 ***********************************************************************/
	TMR2RLH = (0xE1 << TMR2RLH_TMR2RLH__SHIFT);
	// [TMR2RLH - Timer 2 Reload High Byte]$

	// $[TMR2RLL - Timer 2 Reload Low Byte]
	/***********************************************************************
	 - Timer 2 Reload Low Byte = 0x53
	 ***********************************************************************/
	TMR2RLL = (0x53 << TMR2RLL_TMR2RLL__SHIFT);
	// [TMR2RLL - Timer 2 Reload Low Byte]$

	// $[TMR2CN0]
	// [TMR2CN0]$

	// $[Timer Restoration]
	// Restore Timer Configuration
	TMR2CN0 |= TMR2CN0_TR2_save;
	// [Timer Restoration]$

}

extern void EXTINT_0_enter_DefaultMode_from_RESET(void)
{
	// $[IT01CF - INT0/INT1 Configuration]
	/***********************************************************************
	 - INT0 input is active low
	 - Select P0.2
	 - INT1 input is active low
	 - Select P0.0
	 ***********************************************************************/
	IT01CF = IT01CF_IN0PL__ACTIVE_LOW | IT01CF_IN0SL__P0_2
			| IT01CF_IN1PL__ACTIVE_LOW | IT01CF_IN1SL__P0_0;
	// [IT01CF - INT0/INT1 Configuration]$

}

