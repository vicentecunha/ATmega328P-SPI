# ATmega328P-SPI
A SPI include for the ATmega328p.
-----------------------------------------------------------------------------
 This file has many include dependencies, which are automatically determined
	by Atmel® Studio. If you know what you're doing and desire to work outside
	Atmel Studio, the ./include directory has the necessary dependencies.
-----------------------------------------------------------------------------
 Full description of registers and SPI functions are given, so that new
	macros and functions can be easily defined by the user.
 Need help with modifying this file? Find me in github.com/vicentecunha
-----------------------------------------------------------------------------
 Before we begin: a brief graphic reference on SPI modes.

	+-----------+-------------------+-------------------+-------------------+
	|SPI Mode   |Conditions         |Leading Edge       |Trailing Edge      |
	+-----------+-------------------+-------------------+-------------------+
	|       0   |CPOL = 0, CPHA = 0 |Sample (Rising)    |Setup (Falling)    |
	|       1   |CPOL = 0, CPHA = 1 |Setup (Rising)     |Sample (Falling)   |
	|       2   |CPOL = 1, CPHA = 0 |Sample (Falling)   |Setup (Rising)     |
	|       3   |CPOL = 1, CPHA = 1 |Setup (Falling)    |Sample (Rising)    |
	+-----------+-------------------+-------------------+-------------------+

 SPI Transfer Format with CPHA = 0
 +--
 |                    +---+   +---+   +---+   +---+   +---+   +---+   +---+
 |SCK (CPOL = 0)      |   |   |   |   |   |   |   |   |   |   |   |   |   |
 |mode 0          ----+   +---+   +---+   +---+   +---+   +---+   +---+   +--
 |                ----+   +---+   +---+   +---+   +---+   +---+   +---+   +--
 |SCK (CPOL = 1)      |   |   |   |   |   |   |   |   |   |   |   |   |   |
 |mode 2              +---+   +---+   +---+   +---+   +---+   +---+   +---+
 +--                  |       |       |       |       |       |       |
 +--                  |       |       |       |       |       |       |
 |                    I       I       I       I       I       I       I
 |SAMPLE I            I       I       I       I       I       I       I
 |MOSI/MISO       ----I-------I-------I-------I-------I-------I-------I------
 +--                  |       |       |       |       |       |       |
 +--                  |       |       |       |       |       |       |
 |              -+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-
 |CHANGE 0       +-|     |-|     |-|     |-|     |-|     |-|     |-|     |-+
 |MOSI PIN         +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+
 |                 +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ 
 |CHANGE 0      ---|     |-|     |-|     |-|     |-|     |-|     |-|     |---
 |MISO PIN         +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+
 +--                  |       |       |       |       |       |       |
 +--                  |       |       |       |       |       |       |
 |            ---+    |       |       |       |       |       |       |    +-
 |/SS            |    |       |       |       |       |       |       |    |
 |               +---------------------------------------------------------+
 +--                  |       |       |       |       |       |       |
 MSB first(DORD=0)   MSB    Bit 6   Bit 5   Bit 4   Bit 3    ...     LSB
 LSB first(DORD=1)   LSB    Bit 1   Bit 2   Bit 3   Bit 4    ...     MSB

 SPI Transfer Format with CPHA = 1
 +--
 |                +---+   +---+   +---+   +---+   +---+   +---+   +---+
 |SCK (CPOL = 0)  |   |   |   |   |   |   |   |   |   |   |   |   |   |
 |mode 1      ----+   +---+   +---+   +---+   +---+   +---+   +---+   +---+
 |            ----+   +---+   +---+   +---+   +---+   +---+   +---+   +---+
 |SCK (CPOL = 1)  |   |   |   |   |   |   |   |   |   |   |   |   |   |
 |mode 3          +---+   +---+   +---+   +---+   +---+   +---+   +---+
 +--                  |       |       |       |       |       |       |
 +--                  |       |       |       |       |       |       |
 |                    I       I       I       I       I       I       I
 |SAMPLE I            I       I       I       I       I       I       I
 |MOSI/MISO       ----I-------I-------I-------I-------I-------I-------I------
 +--                  |       |       |       |       |       |       |
 +--                  |       |       |       |       |       |       |
 |              -+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-
 |CHANGE 0       +-|     |-|     |-|     |-|     |-|     |-|     |-|     |-+
 |MOSI PIN         +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+
 |                 +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ 
 |CHANGE 0      ---|     |-|     |-|     |-|     |-|     |-|     |-|     |---
 |MISO PIN         +-----+ +-----+ +-----+ +-----+ +-----+ +-----+ +-----+
 +--                  |       |       |       |       |       |       |
 +--                  |       |       |       |       |       |       |
 |            ---+    |       |       |       |       |       |       |    +-
 |/SS            |    |       |       |       |       |       |       |    |
 |               +---------------------------------------------------------+
 +--                  |       |       |       |       |       |       |
 MSB first(DORD=0)   MSB    Bit 6   Bit 5   Bit 4   Bit 3    ...     LSB
 LSB first(DORD=1)   LSB    Bit 1   Bit 2   Bit 3   Bit 4    ...     MSB
-----------------------------------------------------------------------------
 Register descriptions:
-----------------------------------------------------------------------------
 SPCR - SPI Control Register
                  +---------------------------------------+
  Bit Number      |7   |6   |5   |4   |3   |2   |1   |0   |
                  +---------------------------------------+
  Bit Name        |SPIE|SPE |DORD|MSTR|CPOL|CPHA|SPR1|SPR0|
                  +---------------------------------------+
  R/W             |R/W |R/W |R/W |R/W |R/W |R/W |R/W |R/W |
                  +---------------------------------------+
  Initial Value   |0   |0   |0   |0   |0   |0   |0   |0   |
                  +---------------------------------------+
-----------------------------------------------------------------------------
 SPIE - SPI Interrupt Enable
	1 -> SPI interrupt is executed, if:
			- SPIF   (SPI Interrupt Flag) bit in SPSR Register is set;
			- I (Global Interrupt Enable) bit in SREG Register is set.
	0 -> SPI interrupt is not executed.
-----------------------------------------------------------------------------
 SPE - SPI Enable
	1 -> SPI is enabled. This is required to enable any SPI operations.
	0 -> SPI is disabled.
-----------------------------------------------------------------------------
 DORD - Data Order
	1 -> LSB of the data word is transmitted first.
	0 -> MSB of the data word is transmitted first.
-----------------------------------------------------------------------------
 MSTR - Master/Slave Select
	1 -> Master SPI mode.
		- If SS is configured as an input and is driven low while this bit is
			set, this bit will be cleared (Slave SPI mode), and SPIF (SPI
			Interrupt Flag) will be set. The user will have to this bit again
			to re-enable SPI Master mode.
	0 -> Slave SPI mode.
-----------------------------------------------------------------------------
 CPOL - Clock Polarity
	1 -> SCK is high when idle.
	0 -> SCK is low when idle.
		+-------+---------------+---------------+
		|CPOL   |Leading Edge   |Trailing Edge  |
		+-------+---------------+---------------+
		|   0   |   Rising      |   Falling     |
		|   1   |   Falling     |   Rising      |
		+-------+---------------+---------------+
-----------------------------------------------------------------------------
 CPHA - Clock Phase
	1 -> Data is sampled on the trailing edge.
	0 -> Data is sampled on the leading edge.
		+-------+---------------+---------------+
		|CPHA   |Leading Edge   |Trailing Edge  |
		+-------+---------------+---------------+
		|   0   |   Sample      |   Setup       |
		|   1   |   Setup       |   Sample      |
		+-------+---------------+---------------+
-----------------------------------------------------------------------------
 SPSR - SPI Status Register
                  +----------------------------------------+
  Bit Number      |7   |6   |5   |4   |3   |2   |1   |0    |
                  +----------------------------------------+
  Bit Name        |SPIF|WCOL| -- | -- | -- | -- | -- |SPI2X|
                  +----------------------------------------+
  R/W             |R   |R   |R   |R   |R   |R   |R   |R/W  |
                  +----------------------------------------+
  Initial Value   |0   |0   |0   |0   |0   |0   |0   |0    |
                  +----------------------------------------+
-----------------------------------------------------------------------------
 SPIF - SPI Interrupt Flag
	- When a serial transfer is complete, this bit is set.
	- An interrupt is generated if:
		- this bit is set;
		- I (Global Interrupt Enable) bit in SREG Register is set.
	- If SS is an input and is driven low when the SPI is in Master mode, this
		will also set this bit.
	- This bit is cleared by hardware when executing the corresponding
		interrupt handling vector.
	- Alternatively, this bit is cleared by first reading the SPI Status
		Register with this bit set then accessing the SPI Data Register (SPDR).
-----------------------------------------------------------------------------
 WCOL - Write Collision Flag
	- This bit is set if the SPI Data Register is written during a data
		transfer.
	- This bit (and the SPIF) bit are cleared by first reading the SPI Status
		Reguster with this bit set, and then accessing the SPI Data Register.
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
 SPI2X - Double SPI Speed Bit (refer to SPR1,SPR0 - SPI Clock Rate Select)
	1 -> SPI speed (SCK Frequency) will be doubled when the SPI is in Master
			mode. This means that the minimum SCK period will be two CPU clock
			periods. When the SPI is configured as Slave, the SPI is only
			guaranteed to work at f_osc/4 or lower.
	0 -> SPI single speed.
-----------------------------------------------------------------------------
 SPR1,SPR0 - SPI Clock Rate Select
	- Control the SCK rate of the device configured as a Master.
	- No effect on the Slave.
      +-------+-------+-------+---------------+
      |SPI2X  |SPR1   |SPR0   |SCK Frequency  |
      +-------+-------+-------+---------------+
      |   0   |   0   |   0   |   f_osc/4     |
      |   0   |   0   |   1   |   f_osc/16    |
      |   0   |   1   |   0   |   f_osc/64    |
      |   0   |   1   |   1   |   f_osc/128   |
      |   1   |   0   |   0   |   f_osc/2     |
      |   1   |   0   |   1   |   f_osc/8     |
      |   1   |   1   |   0   |   f_osc/32    |
      |   1   |   1   |   1   |   f_osc/64    |
      +-------+---------------+---------------+
-----------------------------------------------------------------------------
 SPDR - SPI Data Register
                  +---------------------------------------+
  Bit Number      |7   |6   |5   |4   |3   |2   |1   |0   |
                  +---------------------------------------+
  Bit Name        |MSB |    |    |    |    |    |    |LSB |
                  +---------------------------------------+
  R/W             |R/W |R/W |R/W |R/W |R/W |R/W |R/W |R/W |
                  +---------------------------------------+
  Initial Value   |X   |X   |X   |X   |X   |X   |X   |X   |
                  +---------------------------------------+
	- The SPI Data Register is a read/write register used for data transfer
		between the Register File and the SPI Shift Register. Writing to the
		register initiates data transmission. Reading the register causes the
		Shift Register Receive buffer to be read.