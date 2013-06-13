#include "udp_tabler.h"

udp_tabler::udp_tabler(unsigned int addr,int port,sn76_player* play,event_loop* looper):
udp_port(addr,port,looper),
player(play)
{
}

void udp_tabler::read_callback(uint8_t* buffer,int len)
{
	//ok assume we are receiving a tabling message
	if(len < sizeof(table_msg))
	{
		std::cout << "bad udp message\n";
		return;
	}

	table_msg* msg = (table_msg*)buffer;
	int* vals = (int*)(buffer + sizeof(table_msg));
	int* jumps = vals + msg->length;

	switch(msg->table)
	{
		case 0:
			for(int i=0;i<msg->length;++i)
			{
				player->chip1_ch1.set_amptable(i+msg->offset,(uint8_t)(vals[i]),jumps[i]);
			}
		break;
		case 1:
			for(int i=0;i<msg->length;++i)
			{
				player->chip1_ch1.set_notetable(i+msg->offset,vals[i],jumps[i]);
			}
		break;
		case 2:
			for(int i=0;i<msg->length;++i)
			{
				player->chip1_ch1.set_freqtable(i+msg->offset,vals[i],jumps[i]);
			}
		break;
		case 3:
			for(int i=0;i<msg->length;++i)
			{
				player->chip1_ch2.set_amptable(i+msg->offset,(uint8_t)(vals[i]),jumps[i]);
			}
		break;
		case 4:
			for(int i=0;i<msg->length;++i)
			{
				player->chip1_ch2.set_notetable(i+msg->offset,vals[i],jumps[i]);
			}
		break;
		case 5:
			for(int i=0;i<msg->length;++i)
			{
				player->chip1_ch2.set_freqtable(i+msg->offset,vals[i],jumps[i]);
			}
		break;
		case 6:
			for(int i=0;i<msg->length;++i)
			{
				player->chip1_noise.set_amptable(i+msg->offset,(uint8_t)(vals[i]),jumps[i]);
			}
		break;
		case 7:
			for(int i=0;i<msg->length;++i)
			{
				player->chip1_noise.set_notetable(i+msg->offset,vals[i],jumps[i]);
			}
		break;
		case 8:
			for(int i=0;i<msg->length;++i)
			{
				player->chip1_noise.set_freqtable(i+msg->offset,vals[i],jumps[i]);
			}
		break;
		case 9:
			for(int i=0;i<msg->length;++i)
			{
				player->chip1_noise.set_noise_amp_table(i+msg->offset,(uint8_t)(vals[i]),jumps[i]);
			}
		break;
		case 10:
			for(int i=0;i<msg->length;++i)
			{
				player->chip1_noise.set_wave_table(i+msg->offset,(uint8_t)(vals[i]),jumps[i]);
			}
		break;
		case 11:
			for(int i=0;i<msg->length;++i)
			{
				player->chip2_ch1.set_amptable(i+msg->offset,(uint8_t)(vals[i]),jumps[i]);
			}
		break;
		case 12:
			for(int i=0;i<msg->length;++i)
			{
				player->chip2_ch1.set_notetable(i+msg->offset,vals[i],jumps[i]);
			}
		break;
		case 13:
			for(int i=0;i<msg->length;++i)
			{
				player->chip2_ch1.set_freqtable(i+msg->offset,vals[i],jumps[i]);
			}
		break;
		case 14:
			for(int i=0;i<msg->length;++i)
			{
				player->chip2_ch2.set_amptable(i+msg->offset,(uint8_t)(vals[i]),jumps[i]);
			}
		break;
		case 15:
			for(int i=0;i<msg->length;++i)
			{
				player->chip2_ch2.set_notetable(i+msg->offset,vals[i],jumps[i]);
			}
		break;
		case 16:
			for(int i=0;i<msg->length;++i)
			{
				player->chip2_ch2.set_freqtable(i+msg->offset,vals[i],jumps[i]);
			}
		break;
		case 17:
			for(int i=0;i<msg->length;++i)
			{
				player->chip2_noise.set_amptable(i+msg->offset,(uint8_t)(vals[i]),jumps[i]);
			}
		break;
		case 18:
			for(int i=0;i<msg->length;++i)
			{
				player->chip2_noise.set_notetable(i+msg->offset,vals[i],jumps[i]);
			}
		break;
		case 19:
			for(int i=0;i<msg->length;++i)
			{
				player->chip2_noise.set_freqtable(i+msg->offset,vals[i],jumps[i]);
			}
		break;
		case 20:
			for(int i=0;i<msg->length;++i)
			{
				player->chip2_noise.set_noise_amp_table(i+msg->offset,(uint8_t)(vals[i]),jumps[i]);
			}
		break;
		case 21:
			for(int i=0;i<msg->length;++i)
			{
				player->chip2_noise.set_wave_table(i+msg->offset,(uint8_t)(vals[i]),jumps[i]);
			}
		break;
		default:
			std::cout<<"cannot find the table you wanna set\n";
			return;
		break;
	}

	std::cout << "SET table: " <<msg->table << " starting at: " << msg->offset <<" length: " << msg->length << "\n";
}