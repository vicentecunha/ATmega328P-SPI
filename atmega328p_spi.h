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
// This file has many include dependencies, which are automatically determined
//	by Atmel® Studio. If you know what you're doing and desire to work outside
//	Atmel Studio, the ./include has the necessary dependencies.
//
// This include file considers the following standard SPI pinout:
//	SCK		->	PB5(Arduino pin 13)
//	MISO	->	PB4(Arduino pin 12)
//	MOSI	->	PB3(Arduino pin 11)
//	SS		->	PB2(Arduino pin 10)
//
// This pinout can be altered, and support for multiple SPI busses can be added
//	by modifying this include file appropriately.
//	Need help with modifying this file? Find me in github.com/vicentecunha
//-----------------------------------------------------------------------------

