#ifndef SEGACHIP_DOT_H
#define SEGACHIP_DOT_H

#define SEGA_DATA_PORT PORTD
#define SEGA_DATA_DDR DDRD

#define SEGA_STATUS_PORT PORTF
#define SEGA_STATUS_DDR DDRF
#define SEGA_ONE_OUTPUT_ENABLE PF5
#define SEGA_ONE_WRITE_ENABLE PF4
#define SEGA_ONE_READY PF0

#define SEGA_TWO_OUTPUT_ENABLE PF7
#define SEGA_TWO_WRITE_ENABLE PF6
#define SEGA_TWO_READY PF1

#define SEGA_CHIP_ONE 0
#define SEGA_CHIP_TWO 1

//-----------------------------------------------------------------------------
//high level stuff
//-----------------------------------------------------------------------------

void set_freq(uint8_t chip,uint8_t channel,uint8_t hi,uint8_t low);
void set_amp(uint8_t chip,uint8_t channel,uint8_t amp);
void set_noise(uint8_t chip,uint8_t noise);

//-----------------------------------------------------------------------------
//	Low level stuff,
//-----------------------------------------------------------------------------

void init_segachips();

void select_chip(int chip);
void write_chip(int chip, uint8_t data);
void deselect_chip(int chip);


#endif
