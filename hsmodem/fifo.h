#pragma once

enum _FIFOUSAGE_ {
	FIFO_RTTYTX = 0,
	PSK_GUI_TX,
	EXT_TX,
	EXT_SPECNB,
	EXT_SPECWB,
};

void init_fifos();
void write_fifo(int pipenum, uint8_t* pdata, int len);
int read_fifo(int pipenum, uint8_t* data, int maxlen);
void fifo_clear(int pipenum);
int fifo_freespace(int pipenum);
int fifo_usedspace(int pipenum);
int fifo_usedpercent(int pipenum);
