#include <stdint.h>
#include "aoscls_parser.h"
#include "segachip.h"
#include "ltc6903.h"
#include "usb_serial.h"

uint8_t dex;
uint8_t cmd_buff[COMMAND_BUFF_SIZE];


//something to make things a little easier to read
inline uint8_t len() 
{ 
	return cmd_buff[0];
}

//basically this allways has to be a custom function
void execute_command()
{
	uint16_t dac;

	switch(cmd_buff[1])
	{
		case SET_CLOCK:
			if(len() == 5)
			{
				dac = (cmd_buff[4] << 8) | cmd_buff[5];
				set_clock(cmd_buff[2],cmd_buff[3],dac);
				usb_serial_write("clk_exec\n",9);
			}
		break;
		case SET_AMP:
			if(len() == 4)
			{
				set_amp(cmd_buff[2],cmd_buff[3],cmd_buff[4]);
				usb_serial_write("amp_exec\n",9);
			}
		break;
		case SET_FREQ:
			if(len() == 5)
			{
				set_freq(cmd_buff[2],cmd_buff[3],cmd_buff[4],cmd_buff[5]);
				usb_serial_write("frq_exec\n",9);
			}
		break;
		case SET_NOISE:
			if(len() == 3)
			{
				set_noise(cmd_buff[2],cmd_buff[3]);
				usb_serial_write("nzz_exec\n",9);
			}
		break;
		case WRITE_CHIP: //low level for debug
			if(len() == 3)
			{
				select_chip(cmd_buff[2]);
				write_chip(cmd_buff[2],cmd_buff[3]);
				deselect_chip(cmd_buff[2]);
				usb_serial_write("chp_exec\n",9);
			}
		break;
		default:
		break;
	}

	
}


//zero everything out.
void init_aoscls_parser()
{
	dex = 0;
	for(int i=0;i<COMMAND_BUFF_SIZE;++i)
	{
		cmd_buff[i] = 0x00;
	}
}

void aoscls_parser_cin(uint8_t cin)
{
	cmd_buff[dex] = cin;

	//if this is the message length execute the message
	if(dex == cmd_buff[0])
	{
		execute_command();
		//and set us to wait for the next
		dex = 0;
	}
	else
	{
		dex++;
	}
}
