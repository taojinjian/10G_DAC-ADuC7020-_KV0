/***********************************************************
** xxxxxx Co.Ltd CopyRight
**
**File: init.c
**
**Des: 
**
**Author: Hani , created in 03/03/2017 Rev1.0
**
**Rev: xxxx, Modifed in ..................... Rev1.x
**
**
***********************************************************/

/**
**include
**/
#include "pubcom.h"

/**
*********************def
**/
/**
**type def
**/


/**
**control def
**/






/**
*********************val
**/

/**
**global
**/



/**
**extern
**/
 extern uInt8_PUB glb_i2c0_rcv_first;


/**
*********************fun
**/
/**
**local
**/

/**
**extern
**/

/*****************************************************************
**Name	  :   init_delay_us()
**
**Function:
**
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
static void init_delay_us(uInt32_PUB us)
{
	us = us*10;
	while(us--);
}

/*****************************************************************
**Name    :   init_cpu_clock()
**
**Function:
**
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
void init_cpu_clock(void)
{
//		PLLKEY1 = 0xAA;
//    PLLCON = 0x01;   // PLL ģʽ
//    PLLKEY2 = 0x55;
    // Configure CPU Clock for 41.78MHz, CD=0
    POWKEY1 = 0x01;						
    POWCON = 0x00;
    POWKEY2 = 0xF4;	

//    // Enable  I2C0,  Peripherals with 41.78MHz clock
//    POWKEY3 = 0x76;			 			
//    POWCON1 = 0x004;					
//    POWKEY4 = 0xB1;
}

/*****************************************************************
**Name    :   init_i2c_slave_to_host()
**
**Function:
**
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
void init_i2c_slave_to_host(void)
{
    /*
        SCL     P1.0
        SDA     P1.1
        */

	/*Configure P1.0 and P1.1 for I2C mode*/
	GP1CON |= 0x00000022;				// Only write to bits 0/1
	
//	I2C0SCON = BIT0 		  			// Enable I2C Slave + Enable Rx interrupt +
//			+ BIT9 + BIT10  			// Enable Rx interrupt + Enable Tx interrupt +
//			+ BIT8
//			+ BIT5;						// Enable "Stop-after-Start-Detected" IRQ.
	
	I2C0CFG = 0x4001;					//BIT0 -> 1   
	I2C0DIVL = 0x3C;
	I2C0DIVH = 0x28;
	
	I2C0ID0  = PUBCOM_DEVICE_ADDR0;					// Configure Slave's I2C network address
	I2C0ID1  = PUBCOM_DEVICE_ADDRA2;		
	//	I2C0FSTA = BIT8;					// Flush Slave Tx FIFO
//	I2C0FSTA &= ~BIT8;
//	I2C0SSTA = 0x00;
//	FIQCLR = 0x00000200; 
	//uiTest = I2C0SCON;
	//IRQEN = BIT9; 						// Enable I2C0 Slave interrupts

        //I2C0ID0 = 0xA0;                      // primary I2C address A0h
        //I2C0ID1 = 0xA2;                      // secondary I2C address A2h
        //I2C0CFG = 0x4201;                    // enable I2C slave, Stop IRQ, disable TX FIFO requst IRQ

//	GP1PAR |= (1<<0);//disable the pull up of p1.0
//	GP1PAR |= (1<<4);//disable the pull up of p1.1
	FIQEN = 0x00000200;     // enable I2C0SIRQ
	glb_i2c0_rcv_first = TRUE_PUB;

    /*set start status*/
    //glb_i2c0_rcv_first = TRUE_PUB;
}

/*****************************************************************
**Name    :   init_adc_for_operate_gn1160()
**
**Function:
**
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
void init_i2c_for_operate_gn1160(void)
{
    /*
        SCL_TX              	P1.3        For Output From mcu
        SDA_TX1         	P0.3        For Input to mcu & Output From mcu
        SDA_TX2     		P1.2        For Input to mcu & Output From mcu
        SDA_TX3     		P0.2        For Input to mcu & Output From mcu
        SDA_TX3     		P0.1        For Input to mcu & Output From mcu
        */
}

/*****************************************************************
**Name    :   init_adc_for_rx_monitoer()
**
**Function:
**
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
void init_adc_for_rx_monitoer(void)
{
    /*
        RX_MON1     ADC0        For Input to mcu
        RX_MON2     ADC1        For Input to mcu
        RX_MON3     ADC2        For Input to mcu
        RX_MON4     ADC3        For Input to mcu
        */

	/*Bandgap Reference configuration*/
	REFCON = 0x01;                      // use internal 2.5V reference

	/*ADC configuration*/
	ADCCON = 0x20;                      // powerup ADC
}
/*****************************************************************
**Name    :   init_qsfp_interface_io_for_control_and_status()
**
**Function:
**
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
void init_qsfp_interface_io_for_control_and_status(void)
{
	/*
	INTL            P0.6        For Output from mcu
	RST_LD        P0.7        For Output from mcu
	LPMode        P1.0        For Input to mcu			used check module inplus or outplug
	ModSel         P1.1        For Input to mcu			used to select slave_to_host work
	*/

	/*INTL  default-output-level = 1(means no interrupt), pull-up*/
//	GP0PAR |=0x0;
//	GP0DAT |=0x40000000;
//	GP0DAT &=(~0x00400000);
//	GP0DAT =(0x00400000);
//	
//	/*RST_LD used to reset the gn1160*/
//	GP0PAR |=0x0;
//	GP0DAT |=0x80000000;

//	/*LPMode used to check module inplug*/
//	GP1PAR |=0x0;
//	GP1DAT &=(~0x01000000);

//	/*ModSel used to disable or enable the module by host*/
//	GP1PAR |=0x0;
//	GP1DAT &=(~0x02000000);
	//IRQCONE          |= 0x0C;        // XIRQ1 low edge triggered
}

/*****************************************************************
**Name    :   init_timer()
**
**Function:
**
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
void init_timer(void)
{
    // Timer1 configuration
    T1LD = 0x40;                        // 42000000/32768/20 (for overflow every 20Hz)
    T1CON = 0xCF;                       // count down
                                        // periodic mode
                                        // binary format
                                        // prescale:32768
                                        // source clock: core clock
}
/*****************************************************************
**Name    :   init_enable_interrupt()
**
**Function:
**
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
void init_enable_interrupt(void)
{
/*
Bit
Description
0
All interrupts OR��ed (FIQ only).
1
SWI.
2
Timer0.
3
Timer1.
4
Watchdog timer (Timer 2).
5
Flash control.
6
ADC channel.
7
PLL lock.
8
I2C0 master.
9
I2C0 slave.
10
I2C1 master.
11
I2C1 slave.
12
SPI.
13
External IRQ0.
14
Comparator.
15
PSM.
16
External IRQ1.
17
PLA IRQ0.
18
External IRQ2.
19
External IRQ3.
20
PLA IRQ1.
21
PWM.
*/
   
    //IRQEN = 0x00048008;     // enable Timer1IRQ, XIRQ0, XIRQ1
    IRQEN = 0x00000008;     // enable Timer1IRQ

//	IRQEN |=(1UL << XIRQ1_INT);// enable XIRQ1
}
/*****************************************************************
**Name	  :   InitCPU()
**
**Function:
**
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
void init_reset_gn1160(void)
{
//	GP0DAT &=(~0x00800000);
//	init_delay_us(100);
//	GP0DAT |=(0x00800000);
}
/*****************************************************************
**Name	  :   InitCPU()
**
**Function:
**
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
void init_module()
{
	/*enable flash erase/write*/
	FEEMOD |= 0x108;               

	/*init cpu and device*/
	init_cpu_clock();
	init_i2c_slave_to_host();
//	init_i2c_for_operate_gn1160();
	init_adc_for_rx_monitoer();
	init_qsfp_interface_io_for_control_and_status();
	init_timer();
//	init_reset_gn1160();

	/*init qsfp28 table*/
	init_flash_for_qsfp_table();
	init_sram_for_qsfp_table();

	/*init the gn1160*/
//	init_gn1160_with_flash();
	
	/*must call at the end*/
	init_enable_interrupt();
}

