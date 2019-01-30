/****************************************************************************
* �� �� ��: main.c
* ��    ��: Amo [ www.amoMcu.com ��Ī��Ƭ��]
* ��    ��: 2014-04-08
* ��    ��: 1.0
* ��    ��: ���ô��ڵ������ֲ����ʣ�115200bps 8N1
*           ���ڵ������ָ�CC254x���ַ���ʱ��������᷵�ؽ��յ����ַ���
****************************************************************************/
#define LED2 P1_1       //����P1.1��ΪLED2���ƶ�
#include <uart.h>
#include <time.h>
#include <ADS1299.h>



void main(void)
{	
    InitTime(); 
    InitUart();                                //���ô��ڳ�ʼ������   
    // GPIO��ʼ��
    P0SEL &= ~0x51;     //  *0*0 ***0
    P0DIR |= 0x51;     //  *1*1 ***1
    
    P1SEL &= ~0xF1;     //  0000 ***0
    P1DIR &= ~0x81;     //  0111 ***0
    P1DIR |= 0x70;     //  0111 ***0
    
    
    U0CSR &= ~0x40;                           //��ֹ���ڽ������� 
    
    Init_ADS1299();  ////////////////
	DelayUS(1);	
	START=1;
	DelayUS(15);

    UartSendString("program start\n", 14);     
    DelayMS(200);
        
    DRDY=0;
    
    
    UartSendString("Read_ADS1299()\r\n", 16); 
    if(DRDY!=0)
        UartSendString("DRDY invalid\r\n", 14);  
	while(1)  {
         
		Read_ADS1299();  //////////////if(DRDY==0),translate data in first channel 
	}
       
  

}
