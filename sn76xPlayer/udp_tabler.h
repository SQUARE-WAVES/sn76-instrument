#ifndef UDP_TABLER_DOT_H
#define UDP_TABLER_DOT_H

#include <livelayer\livelayer.h>
#include "sn76player.h"

struct table_msg
{
	int table;
	int offset;
	int length;
};

class udp_tabler : public udp_port
{
	protected:
		sn76_player* player;

		void read_callback(uint8_t* buffer, int len);

	public:
		udp_tabler(unsigned int addr,int port,sn76_player* play,event_loop* looper);
};

#endif