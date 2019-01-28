/**
	@filename ADS1299.h
	@brief    ADS1299 config module and read data module
	@author   Qiao Yu
	Copyright (c) 2018 EPIC. All rights reserved.
*/

#ifndef __ADS1299_H_
#define __ADS1299_H_

//#define NO_XSFR_DEFINE ///
#include <ioCC2540.h>

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long UINT32;
#define CS P1_4
#define DIN P1_6
#define DOUT P1_7
#define ASCLK P1_5
#define DRDY P1_0
#define START P0_0
#define PWDN P0_6
#define RESET P0_4



void Init_ADS1299();

uchar RDATAC(UINT32 *ch);
void Read_ADS1299();
uchar Send_ADS1299(uchar *Txdata);
void Read_EEG();

void SPI_send(uchar spidata);
uchar SPI_read(void);
void WREG(uchar x, uchar y, uchar d);
uchar RREG(uchar x, uchar y);

#endif  // __ADS1299_H_
