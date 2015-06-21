//-----------------------------------------------------------------------------
// ATmega328p SPI 
//		- with detailed descriptions and explanations.
//	Author: Vicente Cunha
//	Date: june 2015
//	License: WTFPL
//-----------------------------------------------------------------------------
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
// Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
//
// Everyone is permitted to copy and distribute verbatim or modified
// copies of this license document, and changing it is allowed as long
// as the name is changed.
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.
//-----------------------------------------------------------------------------
#include <avr/io.h>
#ifndef _SPI_
#define _SPI_
//-----------------------------------------------------------------------------
// This file has many include dependencies, which are automatically determined
//	by Atmel® Studio. If you know what you're doing and desire to work outside
//	Atmel Studio, the ./include directory has the necessary dependencies.
//-----------------------------------------------------------------------------
// At the end of this include file, macros are set for a 'standard' SPI pinout:
//						********************************
//						*	ATmega328p as SPI Master.  *
//						*			SPI mode 0.        *
//						*SCK	->	PB5(Arduino pin 13)*
//						*MISO	->	PB4(Arduino pin 12)*
//						*MOSI	->	PB3(Arduino pin 11)*
//						*SS		->	PB2(Arduino pin 10)*
//						********************************
// This pinout can be altered, and support for multiple SPI busses can be added
//	by modifying this include file appropriately.
//-----------------------------------------------------------------------------
// Full description of registers and SPI functions are given, so that new
//	macros and functions can be easily defined by the user.
// Need help with modifying this file? Find me in github.com/vicentecunha
//-----------------------------------------------------------------------------
// Before we begin: a brief graphic reference on SPI modes.
//
//	+-----------+-------------------+-------------------+-------------------+
//	|SPI Mode	|Conditions			|Leading Edge		|Trailing Edge		|
//	+-----------+-------------------+-------------------+-------------------+
//	|		0	|CPOL = 0, CPHA = 0	|Sample (Rising)	|Setup (Falling)	|
//	|		1	|CPOL = 0, CPHA = 1	|Setup (Rising)		|Sample (Falling)	|
//	|		2	|CPOL = 1, CPHA = 0	|Sample (Falling)	|Setup (Rising)		|
//	|		3	|CPOL = 1, CPHA = 1	|Setup (Falling)	|Sample (Rising)	|
//	+-----------+-------------------+-------------------+-------------------+
//
// SPI Transfer Format with CPHA = 0
// +--
// |					+---+	+---+	+---+	+---+	+---+	+---+	+---+	
// |SCK (CPOL = 0)		|	|	|	|	|	|	|	|	|	|	|	|	|	|	
// |mode 0			----+	+---+	+---+	+---+	+---+	+---+	+---+	+--
// |				----+	+---+	+---+	+---+	+---+	+---+	+---+	+--
// |SCK (CPOL = 1)  	|	|	|	|	|	|	|	|	|	|	|	|	|	|	
// |mode 2				+---+	+---+	+---+	+---+	+---+	+---+	+---+
// +--                  |		|		|		|		|		|		|
// +--                  |		|		|		|		|		|		|
// |					I		I		I		I		I		I		I
// |SAMPLE I        	I		I		I		I		I		I		I
// |MOSI/MISO       ----I-------I-------I-------I-------I-------I-------I------		
// +--					|		|		|		|		|		|		|
// +--                  |		|		|		|		|		|		|
// |			  -+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-
// |CHANGE 0       +-|	   |-|	   |-|	   |-|	   |-|	   |-|	   |-|	   |-+
// |MOSI PIN         +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+
// |			     +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ 
// |CHANGE 0	  ---|	   |-|	   |-|	   |-|	   |-|	   |-|	   |-|	   |---
// |MISO PIN         +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+
// +--					|		|		|		|		|		|		|
// +--                  |		|		|		|		|		|		|
// |			---+	|		|		|		|		|		|		|	 +-
// |/SS            |	|		|		|		|		|		|		|	 |
// |               +---------------------------------------------------------+
// +--					|		|		|		|		|		|		|
// MSB first(DORD=0)   MSB    Bit 6   Bit 5   Bit 4   Bit 3    ...     LSB
// LSB first(DORD=1)   LSB    Bit 1   Bit 2   Bit 3   Bit 4    ...     MSB
//
// SPI Transfer Format with CPHA = 1
// +--
// |				+---+	+---+	+---+	+---+	+---+	+---+	+---+	
// |SCK (CPOL = 0)	|	|	|	|	|	|	|	|	|	|	|	|	|	|	
// |mode 1		----+	+---+	+---+	+---+	+---+	+---+	+---+	+--
// |			----+	+---+	+---+	+---+	+---+	+---+	+---+	+--
// |SCK (CPOL = 1) 	|	|	|	|	|	|	|	|	|	|	|	|	|	|	
// |mode 3			+---+	+---+	+---+	+---+	+---+	+---+	+---+
// +--                  |		|		|		|		|		|		|
// +--                  |		|		|		|		|		|		|
// |					I		I		I		I		I		I		I
// |SAMPLE I        	I		I		I		I		I		I		I
// |MOSI/MISO       ----I-------I-------I-------I-------I-------I-------I------		
// +--					|		|		|		|		|		|		|
// +--                  |		|		|		|		|		|		|
// |			  -+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-
// |CHANGE 0       +-|	   |-|	   |-|	   |-|	   |-|	   |-|	   |-|	   |-+
// |MOSI PIN         +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+
// |			     +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ 
// |CHANGE 0	  ---|	   |-|	   |-|	   |-|	   |-|	   |-|	   |-|	   |---
// |MISO PIN         +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+
// +--					|		|		|		|		|		|		|
// +--                  |		|		|		|		|		|		|
// |			---+	|		|		|		|		|		|		|	 +-
// |/SS            |	|		|		|		|		|		|		|	 |
// |               +---------------------------------------------------------+
// +--					|		|		|		|		|		|		|
// MSB first(DORD=0)   MSB    Bit 6   Bit 5   Bit 4   Bit 3    ...     LSB
// LSB first(DORD=1)   LSB    Bit 1   Bit 2   Bit 3   Bit 4    ...     MSB
//-----------------------------------------------------------------------------
// Register-wise macros:
//-----------------------------------------------------------------------------
// SPCR - SPI Control Register
//					+---------------------------------------+
//	Bit Number		|7   |6	  |5   |4   |3   |2   |1   |0   |
//					+---------------------------------------+
//	Bit Name		|SPIE|SPE |DORD|MSTR|CPOL|CPHA|SPR1|SPR0|
//					+---------------------------------------+
//	R/W				|R/W |R/W |R/W |R/W |R/W |R/W |R/W |R/W |
//					+---------------------------------------+
//	Initial Value	|0   |0   |0   |0   |0   |0   |0   |0   |
//					+---------------------------------------+
//-----------------------------------------------------------------------------
// SPIE - SPI Interrupt Enable
//	1 -> SPI interrupt is executed, if:
//			- SPIF   (SPI Interrupt Flag) bit in SPSR Register is set;
//			- I (Global Interrupt Enable) bit in SREG Register is set.
//	0 -> SPI interrupt is not executed.
//-----------------------------------------------------------------------------
#define GLOBAL_INTERRUPT_ENABLE		SREG |=  (1 << I)
#define GLOBAL_INTERRUPT_DISABLE	SREG &= ~(1 << I)
#define SPI_INTERRUPT_ENABLE		SPCR |=  (1 << SPIE);\
									GLOBAL_INTERRUPT_ENABLE
#define SPI_INTERRUPT_DISABLE		SPCR &= ~(1 << SPIE)
//-----------------------------------------------------------------------------
// SPE - SPI Enable
//	1 -> SPI is enabled. This is required to enable any SPI operations.
//	0 -> SPI is disabled.
//-----------------------------------------------------------------------------
#define SPI_ENABLE	SPCR |=  (1 << SPE)
#define SPI_DISABLE	SPCR &= ~(1 << SPE)
//-----------------------------------------------------------------------------
// DORD - Data Order
//	1 -> LSB of the data word is transmitted first.
//	0 -> MSB of the data word is transmitted first.
//-----------------------------------------------------------------------------
#define SPI_LSB_FIRST	SPCR |=  (1 << DORD)
#define SPI_MSB_FIRST	SPCR &= ~(1 << DORD)
//-----------------------------------------------------------------------------
// MSTR - Master/Slave Select
//	1 -> Master SPI mode.
//		- If SS is configured as an input and is driven low while this bit is
//			set, this bit will be cleared (Slave SPI mode), and SPIF (SPI
//			Interrupt Flag) will be set. The user will have to this bit again
//			to re-enable SPI Master mode.
//	0 -> Slave SPI mode.
//-----------------------------------------------------------------------------
#define SPI_MASTER	SPCR |=  (1 << MSTR)
#define SPI_SLAVE	SPCR &= ~(1 << MSTR)
//-----------------------------------------------------------------------------
// CPOL - Clock Polarity
//	1 -> SCK is high when idle.
//	0 -> SCK is low when idle.
//		+-------+---------------+---------------+
//		|CPOL	|Leading Edge	|Trailing Edge	|
//		+-------+---------------+---------------+
//		|	0	|	Rising		|	Falling		|
//		|	1	|	Falling		|	Rising		|
//		+-------+---------------+---------------+
//-----------------------------------------------------------------------------
#define SPI_CPOL_1	SPCR |=  (1 << CPOL)
#define SPI_CPOL_0	SPCR &= ~(1 << CPOL)
//-----------------------------------------------------------------------------
// CPHA - Clock Phase
//	1 -> Data is sampled on the trailing edge.
//	0 -> Data is sampled on the leading edge.
//		+-------+---------------+---------------+
//		|CPHA	|Leading Edge	|Trailing Edge	|
//		+-------+---------------+---------------+
//		|	0	|	Sample		|	Setup		|
//		|	1	|	Setup		|	Sample		|
//		+-------+---------------+---------------+
//-----------------------------------------------------------------------------
#define SPI_CPHA_1	SPCR |=  (1 << CPHA)
#define SPI_CPHA_0	SPCR &= ~(1 << CPHA)
//-----------------------------------------------------------------------------
// SPSR - SPI Status Register
//					+----------------------------------------+
//	Bit Number		|7   |6	  |5   |4   |3   |2   |1   |0    |
//					+----------------------------------------+
//	Bit Name		|SPIF|WCOL| -- | -- | -- | -- | -- |SPI2X|
//					+----------------------------------------+
//	R/W				|R   |R   |R   |R   |R   |R   |R   |R/W  |
//					+----------------------------------------+
//	Initial Value	|0   |0   |0   |0   |0   |0   |0   |0    |
//					+----------------------------------------+
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// SPIF - SPI Interrupt Flag
//	- When a serial transfer is complete, this bit is set.
//	- An interrupt is generated if:
//		- this bit is set;
//		- I (Global Interrupt Enable) bit in SREG Register is set.
//	- If SS is an input and is driven low when the SPI is in Master mode, this
//		will also set this bit.
//	- This bit is cleared by hardware when executing the corresponding
//		interrupt handling vector.
//	- Alternatively, this bit is cleared by first reading the SPI Status
//		Register with this bit set then accessing the SPI Data Register (SPDR).
//-----------------------------------------------------------------------------
char SPI_DUMMY;
#define SPI_CLEAR_INTERRUPT_FLAG	SPI_DUMMY = SPIF; SPI_DUMMY = SPDR
//-----------------------------------------------------------------------------
// WCOL - Write Collision Flag
//	- This bit is set if the SPI Data Register is written during a data
//		transfer.
//	- This bit (and the SPIF) bit are cleared by first reading the SPI Status
//		Reguster with this bit set, and then accessing the SPI Data Register.
//-----------------------------------------------------------------------------
#define SPI_CLEAR_WRITE_COLLISION_FLAG	SPI_DUMMY = SPIF; SPI_DUMMY = SPDR
//-----------------------------------------------------------------------------
// SPI2X - Double SPI Speed Bit (refer to SPR1,SPR0 - SPI Clock Rate Select)
//	1 -> SPI speed (SCK Frequency) will be doubled when the SPI is in Master
//			mode. This means that the minimum SCK period will be two CPU clock
//			periods. When the SPI is configured as Slave, the SPI is only
//			guaranteed to work at f_osc/4 or lower.
//	0 -> SPI single speed.
//-----------------------------------------------------------------------------
#define SPI_DOUBLE_SPEED	SPSR |=  (1 << SPI2X)
#define SPI_SINGLE_SPEED	SPSR &= ~(1 << SPI2X)
//-----------------------------------------------------------------------------
// SPR1,SPR0 - SPI Clock Rate Select
//	- Control the SCK rate of the device configured as a Master.
//	- No effect on the Slave.
//		+-------+-------+-------+---------------+
//      |SPI2X	|SPR1	|SPR0	|SCK Frequency	|
//      +-------+-------+-------+---------------+
//      |	0	|	0	|	0	|	f_osc/4		|
//      |	0	|	0	|	1	|	f_osc/16    |
//      |	0	|	1	|	0	|	f_osc/64    |
//      |	0	|	1	|	1	|	f_osc/128   |
//      |	1	|	0	|	0	|	f_osc/2     |
//      |	1	|	0	|	1	|	f_osc/8     |
//      |	1	|	1	|	0	|	f_osc/32    |
//      |	1	|	1	|	1	|	f_osc/64    |
//      +-------+---------------+---------------+
//-----------------------------------------------------------------------------
#define SPI_CLOCK_DIVIDER_2		SPCR &= ~((1 << SPR1)|(1 << SPR0));\
									SPI_DOUBLE_SPEED
#define SPI_CLOCK_DIVIDER_4		SPCR &= ~((1 << SPR1)|(1 << SPR0));\
									SPI_SINGLE_SPEED
#define SPI_CLOCK_DIVIDER_8		SPCR &= ~(1 << SPR1); SPCR |= (1 << SPR0);\
									SPI_DOUBLE_SPEED
#define SPI_CLOCK_DIVIDER_16	SPCR &= ~(1 << SPR1); SPCR |= (1 << SPR0);\
									SPI_SINGLE_SPEED
#define SPI_CLOCK_DIVIDER_32	SPCR |= (1 << SPR1); SPCR &= ~(1 << SPR0);\
									SPI_DOUBLE_SPEED
#define SPI_CLOCK_DIVIDER_64	SPCR |= (1 << SPR1); SPCR &= ~(1 << SPR0);\
									SPI_SINGLE_SPEED
#define SPI_CLOCK_DIVIDER_128	SPCR |= ((1 << SPR1)|(1 << SPR0));\
									SPI_SINGLE_SPEED
									
//-----------------------------------------------------------------------------
//	Mode selection macros:
//-----------------------------------------------------------------------------
#define SPI_MODE_0	SPI_CPOL_0; SPI_CPHA_0
#define SPI_MODE_1	SPI_CPOL_0; SPI_CPHA_1
#define SPI_MODE_2	SPI_CPOL_1; SPI_CPHA_0
#define SPI_MODE_3	SPI_CPOL_1; SPI_CPHA_1
//-----------------------------------------------------------------------------
//	Pin configuration macros:
//-----------------------------------------------------------------------------
#define SPI_DEFAULT_PINOUT	SPI_MASTER; DDRB |= (1<<DDB2)|(1<<DDB3)|(1<<DDB5);\
								DDRB &= ~(1 << DDB4)
//-----------------------------------------------------------------------------
//	Common usage macros:
//-----------------------------------------------------------------------------
#define SPI_SET_SS					PORTB |=  (1 << PORTB2)
#define SPI_LOWER_SS				PORTB &= ~(1 << PORTB2)
#define SPI_TRANSMIT_BYTE(BYTE)		SPDR = BYTE
#define SPI_WAIT_TRANSMISSION		while(!(SPSR & (1<<SPIF)))
#define SPI_RECEIVED_BYTE			SPDR