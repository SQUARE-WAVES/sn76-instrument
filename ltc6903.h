#ifndef LTCSIXTYNINEOHTHREE_DOT_H
#define LTCSIXTYNINEOHTHREE_DOT_H

//-----------------------------------------------------------------------------
// CONFIGURABLE PINS AND PORTS
//-----------------------------------------------------------------------------
#define CLOCK_ONE 0
#define CLOCK_ONE_PORT PORTB
#define CLOCK_ONE_DDR DDRB
#define CLOCK_ONE_OE PB4
#define CLOCK_ONE_CS PB0

#define CLOCK_TWO 1
#define CLOCK_TWO_PORT PORTB
#define CLOCK_TWO_DDR DDRB
#define CLOCK_TWO_OE PB6
#define CLOCK_TWO_CS PB5



//-----------------------------------------------------------------------------
// CONSTANT DEFINITIONS
//-----------------------------------------------------------------------------
#define ALL_ON 0x00
#define NORMAL 0x02
#define INVERTING 0x01
#define OFF 0x03

//-----------------------------------------------------------------------------
// THE ACTUAL STUFF
//-----------------------------------------------------------------------------
void init_clocks();
void set_clock(int clock,uint8_t oct, uint16_t dac);
void write_clock(int clock,uint8_t hi, uint8_t low);
void set_clock_mode(int clock,uint8_t mode);

void clock_on(int clock);
void clock_off(int clock);

#endif
