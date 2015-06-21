//-----------------------------------------------------------------------------
// ATmega328p SPI 
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
#ifndef SPI_H
#define SPI_H
//-----------------------------------------------------------------------------
//	Interrupt related macros:
//-----------------------------------------------------------------------------
#define GLOBAL_INTERRUPT_ENABLE		SREG |=  (1 << I)
#define GLOBAL_INTERRUPT_DISABLE	SREG &= ~(1 << I)
#define SPI_INTERRUPT_ENABLE		SPCR |=  (1 << SPIE);\
						GLOBAL_INTERRUPT_ENABLE
#define SPI_INTERRUPT_DISABLE		SPCR &= ~(1 << SPIE)
//-----------------------------------------------------------------------------
//	SPI enabling macros:
//-----------------------------------------------------------------------------
#define SPI_ENABLE	SPCR |=  (1 << SPE)
#define SPI_DISABLE	SPCR &= ~(1 << SPE)
//-----------------------------------------------------------------------------
//	Bit order macros:
//-----------------------------------------------------------------------------
#define SPI_LSB_FIRST	SPCR |=  (1 << DORD)
#define SPI_MSB_FIRST	SPCR &= ~(1 << DORD)
//-----------------------------------------------------------------------------
//	Master/Slave selection macros:
//-----------------------------------------------------------------------------
#define SPI_MASTER	SPCR |=  (1 << MSTR)
#define SPI_SLAVE	SPCR &= ~(1 << MSTR)
//-----------------------------------------------------------------------------
//	CPOL/CPHA macros:
//-----------------------------------------------------------------------------
#define SPI_CPOL_1	SPCR |=  (1 << CPOL)
#define SPI_CPOL_0	SPCR &= ~(1 << CPOL)
#define SPI_CPHA_1	SPCR |=  (1 << CPHA)
#define SPI_CPHA_0	SPCR &= ~(1 << CPHA)
//-----------------------------------------------------------------------------
//	SPI flags macros:
//-----------------------------------------------------------------------------
char SPI_DUMMY;
#define SPI_CLEAR_INTERRUPT_FLAG	SPI_DUMMY = SPIF; SPI_DUMMY = SPDR
#define SPI_CLEAR_WRITE_COLLISION_FLAG	SPI_DUMMY = SPIF; SPI_DUMMY = SPDR
//-----------------------------------------------------------------------------
//	SPI clock macros:
//-----------------------------------------------------------------------------
#define SPI_DOUBLE_SPEED	SPSR |=  (1 << SPI2X)
#define SPI_SINGLE_SPEED	SPSR &= ~(1 << SPI2X)
#define SPI_CLOCK_DIVIDER_2	SPCR &= ~((1 << SPR1)|(1 << SPR0));\
							SPI_DOUBLE_SPEED
#define SPI_CLOCK_DIVIDER_4	SPCR &= ~((1 << SPR1)|(1 << SPR0));\
							SPI_SINGLE_SPEED
#define SPI_CLOCK_DIVIDER_8	SPCR &= ~(1 << SPR1); SPCR |= (1 << SPR0);\
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
// 		*****************************
// 		*SCK  -> PB5(Arduino pin 13)*
// 		*MISO -> PB4(Arduino pin 12)*
// 		*MOSI -> PB3(Arduino pin 11)*
// 		*SS   -> PB2(Arduino pin 10)*
// 		*****************************
//-----------------------------------------------------------------------------
#define SPI_PIN_CONFIG	DDRB |= (1<<DDB2)|(1<<DDB3)|(1<<DDB5);\
						DDRB &= ~(1 << DDB4)
//-----------------------------------------------------------------------------
//	Common usage macros:
//-----------------------------------------------------------------------------
#define SPI_SET_SS			PORTB |=  (1 << PORTB2)
#define SPI_LOWER_SS			PORTB &= ~(1 << PORTB2)
#define SPI_TRANSMIT_BYTE(BYTE)		SPDR = BYTE
#define SPI_WAIT_TRANSMISSION		while(!(SPSR & (1<<SPIF)))
#define SPI_RECEIVED_BYTE		SPDR
#endif