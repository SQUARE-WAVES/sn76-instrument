#include "sn76instrument.h"

sn76_instrument::sn76_instrument(int size,int noise_size):
amp_table(size),
note_table(size),
freq_table(size),
noise_amp_table(noise_size),
wave_table(noise_size)
{
}