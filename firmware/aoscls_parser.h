#ifndef AOSCLS_PARSER_DOT_H
#define AOSCLS_PARSER_DOT_H

#define COMMAND_BUFF_SIZE 64

#define SET_CLOCK 0
#define SET_AMP 1 //4/0x01/chip/chan/amp
#define SET_FREQ 2
#define SET_NOISE 3
#define WRITE_CHIP 4

void init_aoscls_parser();
void aoscls_parser_cin(uint8_t cin);


#endif
