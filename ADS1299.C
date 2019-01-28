/**
	@filename ADS1299.C
	@brief    ADS1299 config module and read data module
	@author   Qiao Yu
	Copyright (c) 2018 EPIC. All rights reserved.
*/

#include "ADS1299.h"
#include "time.h"
#include "uart.h"

/***********SPI Send Function**************
	@param  spidata: Data to send,
	@retval void
******************************************/
void SPI_send(uchar spidata)
{
	uchar i;
	ASCLK = 0;  
	DelayUS(1);
	for (i=0;i<8;i++)		
   {
  	if(spidata&0x80) // 1000 0000 
			DIN = 1;
		else
		DIN = 0;
		ASCLK = 1;      
		spidata<<= 1;
		DelayUS(2);
		ASCLK = 0;     //2  down-edge, DIN -> ASD  
		DelayUS(2);
   }
}
/***********SPI Send Function**************
	@param  void
	@retval Data of uchar type from register
******************************************/
uchar SPI_read(void)
{
    uchar j,d;
	for (j=0;j<8;j++)		
	{	 
		d=d<<1;
		ASCLK = 0;
		DelayUS(2);
		ASCLK=1;   ////1  up-edge, ASD -> DOUT -> mcu
		DelayUS(2);
		if(DOUT==1)	d=d|0x01;
	}
	ASCLK=0;
	return d;
}

/***********Write A Register**************
	@param  x: Addr of register,
			y: Num of register,
			d: Data to send,
	@retval void
****************************************/
void WREG(uchar x, uchar y, uchar d)
{  
	x=x|0x40;    //  010 0 0000  start with 010 
	y=y&0x1F;    //  000 1 1111  start with 000
	CS=0;        //  stays low before and during spi
	SPI_send(x);
	DelayUS(8);
	SPI_send(y);
	DelayUS(2);
	SPI_send(d);
	CS=1;
}

/***********Read A Register**************
	@param  x: Addr of register,
			y: Num of register,
	@retval Data of uchar type from register
****************************************/
uchar RREG(uchar x, uchar y)		
{
	x=x|0x20;
	y=y&0x1F;
	CS=0;
	ASCLK = 0;
	DelayMS(1);
	SPI_send(x);//
	DelayMS(8);
	SPI_send(y);//
	DelayMS(2);
	CS=1;
	return SPI_read();
}

/***********Read Data from ADC channel**************
	@param  ch,the buffer to store the data from channel
	@retval Return status to read,0 is successful.
***************************************************/
uchar RDATAC(UINT32 *ch)
{	
	uchar i,j;
	UINT32 temp=0;
	ASCLK=0;
	CS=0; 
	for(i=0;i<7;i++){
		for(j=0;j<24;j++){
			temp=temp<<1;
			ASCLK=0;
			DelayUS(2);
			ASCLK=1;                //DOUT->  rising edge. 
			DelayUS(2);
			if(DOUT==1)	temp=temp|0x01;      //rightest bit = 1
		}
		*(ch+i)=temp;     //ch[0] ~ ch[6]
		temp=0;
		j=0;
	}
	CS=1;
	ASCLK=0;
	return 0;
}

/***********Read Data from ADC channel**************
	@param  void
	@retval Data of uchar type from channel
***************************************************/
void Read_ADS1299()
{
	UINT32 ch[7]={0};
	uchar EEG_Data[3]={0};   //data in the first channel
	if(DRDY==0){
		RDATAC(ch);
		EEG_Data[0]=(ch[1]&0xff0000)>>16;
		EEG_Data[1]=(ch[1]&0x00ff00)>>8;
		EEG_Data[2]= ch[1]&0x0000ff;
		Send_ADS1299(EEG_Data);
	}
    else
      UartSendString("DRDY invalid\r\n", 14);  
}

/***********Init ADS1299 IC**************
	@param  void
	@retval void

****************************************/
void Init_ADS1299()
{
	PWDN=1;			
	START=0;    
	ASCLK=0;       //seems a bit unnecessary   
	CS=1;				   //stays low during SPI ; otherwise the serial interface is reset
	RESET=1;		   
	DelayMS(200);
	
	RESET=0;		
	DelayUS(2);
	RESET=1;		
	DelayUS(20);
	
	CS=0;				
	SPI_send(0x11);  //SDATAC command: Stop Read Data Continuously mode,so that we can write the registers
	CS=1;
	
	DelayUS(2);
	WREG(0x03,0x00,0xEC);  //Configures either an internal or exteral reference and BIAS operation  1110 1100
	WREG(0x01,0x00,0x96);  //This register configures the DAISY_EN bit, clock, and data rate
	WREG(0x02,0x00,0xD4);  //This register configures the test signal generation
	WREG(0x05,0x00,0x00);  //Configures the power mode, PGA gain, and multiplexer settings channels
	WREG(0x0D,0x00,0x01);  //Route channel 1 positive signal into BIAS channel
	WREG(0x0E,0x00,0x01);  //Route channel 1 negative signal into BIAS derivation
	START=0;
	DelayMS(1);
	
	START=1;
	CS=0;
	SPI_send(0x10);
	CS=1;
	DelayMS(1);
}

/***********Init ADS1299 IC**************
	@param  Point of data to send 
	@retval Error value
****************************************/
uchar Send_ADS1299(uchar *Txdata)
{
    UartSendString("st", 2);
    UartSendString(Txdata, 3);
	return 0;
}
