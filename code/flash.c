/***********************************************************
** xxxxxx Co.Ltd CopyRight
**
**File: flash.c
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
uInt8_PUB glb_sram_qsfp_A0_memory[256];
uInt8_PUB glb_sram_qsfp_A2_memory[PUBCOM_MAX_UPPER_PAGE][PUBCOM_TABLE_PAGE_SIZE];
uInt8_PUB glb_sram_qsfp_A2_lo_memory[PUBCOM_TABLE_PAGE_SIZE];
volatile uint16_t  I2CBusy = 0;
					
const uInt8_PUB glb_code_gn1160_regvalue_memory[PUBCOM_GN1160_NUM][PUBCOM_GN1160_REG_SIZE]=
{
#if 1
	{
		0x01, 0xC4, 0x0F, 0x02, 0x00, 0x00, 0x8A, 0xB4, 0x0A, 0xB4, 0x73, 0x99, 0x00, 0x00, 0x00, 0x00, 
		0x01, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, (0x00|0x04), (0x00|0xE2), (0x00|0X03), 
		(0x00|0XE8), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 
		0x1F, 0x1F, 0x00, (0x00|(1<<5)), 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, (0x1E|(1<<5)), ((0xD1&(~(1<<3)))&(~(1<<6))), 0x00,
	},
	{
		0x01, 0xC4, 0x0F, 0x02, 0x00, 0x00, 0x8A, 0xB4, 0x0A, 0xB4, 0x73, 0x99, 0x00, 0x00, 0x00, 0x00, 
		0x01, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, (0x00|0x04), (0x00|0xE2), (0x00|0X03), 
		(0x00|0XE8), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 
		0x1F, 0x1F, 0x00, (0x00|(1<<5)), 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, (0x1E|(1<<5)), ((0xD1&(~(1<<3)))&(~(1<<6))), 0x00,
	},
	{
		0x01, 0xC4, 0x0F, 0x02, 0x00, 0x00, 0x8A, 0xB4, 0x0A, 0xB4, 0x73, 0x99, 0x00, 0x00, 0x00, 0x00, 
		0x01, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, (0x00|0x04), (0x00|0xE2), (0x00|0X03), 
		(0x00|0XE8), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 
		0x1F, 0x1F, 0x00, (0x00|(1<<5)), 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, (0x1E|(1<<5)), ((0xD1&(~(1<<3)))&(~(1<<6))), 0x00,
	},
	{
		0x01, 0xC4, 0x0F, 0x02, 0x00, 0x00, 0x8A, 0xB4, 0x0A, 0xB4, 0x73, 0x99, 0x00, 0x00, 0x00, 0x00, 
		0x01, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, (0x00|0x04), (0x00|0xE2), (0x00|0X03), 
		(0x00|0XE8), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 
		0x1F, 0x1F, 0x00, (0x00|(1<<5)), 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, (0x1E|(1<<5)), ((0xD1&(~(1<<3)))&(~(1<<6))), 0x00,
	},
#else  /*default register value of ic*/
	{
		0x01, 0xC4, 0x0F, 0x02, 0x00, 0x00, 0x8A, 0xB4, 0x0A, 0xB4, 0x73, 0x99, 0x00, 0x00, 0x00, 0x00, 
		0x01, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 
		0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x1E, 0xD1, 0x00,
	},
	{
		0x01, 0xC4, 0x0F, 0x02, 0x00, 0x00, 0x8A, 0xB4, 0x0A, 0xB4, 0x73, 0x99, 0x00, 0x00, 0x00, 0x00, 
		0x01, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 
		0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x1E, 0xD1, 0x00,
	},
	{
		0x01, 0xC4, 0x0F, 0x02, 0x00, 0x00, 0x8A, 0xB4, 0x0A, 0xB4, 0x73, 0x99, 0x00, 0x00, 0x00, 0x00, 
		0x01, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 
		0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x1E, 0xD1, 0x00,
	},
	{
		0x01, 0xC4, 0x0F, 0x02, 0x00, 0x00, 0x8A, 0xB4, 0x0A, 0xB4, 0x73, 0x99, 0x00, 0x00, 0x00, 0x00, 
		0x01, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 
		0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x1E, 0xD1, 0x00,
	},
#endif
		/*

	{
		0x01, 0xC4, 0x0F, 0x02, 0x00, 0x00, 0x8A, 0xB4, 0x0A, 0xB4, 0x73, 0x99, 0x00, 0x00, 0x00, 0x00, 
		0x01, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, |mod_dac 0x00, 0x00|, |bias_dac 0x00, 
		0x00,| 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 
		0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x1E, 0xD1, 0x00,
	},
		*/
};

const uInt8_PUB glb_code_qsfp_A0_memory[256]=
{
		//A0 LO
		13,    0,     0,  255,   15,     0,     0,     0,     0,   85,   85,   85,   85,   85,   85,     0,
    0,     0,     0,     0,     0,     0,   37,  167,     0,     0,  133,   91,     0,     0,     0,     0,
    0,     0,     0,     8,     0,     1,     0,     1,     0,   10,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     1,     0,     1,     0,     1,     0,     1,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
	
	//A0 UP
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,

};

const uInt8_PUB glb_code_qsfp_A2_lo_memory[PUBCOM_TABLE_PAGE_SIZE]=
{
		//

		#if 0
		0x5A,  00,   0xD3,   0,   0x55,    0,   0xD8,    0,   0x8C,   0xA0,   0x75,  0x30, 0x87,   0x8C,   0x7A,  0x44,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     63,   128,    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0

	#endif

	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
0x1C, 0x1F, 0x80, 0xCE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x36, 0xFF,
0x95, 0x40, 0xFF, 0xFF, 0x95, 0x40, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00

};


const uInt8_PUB glb_code_qsfp_A2_up_memory[PUBCOM_MAX_UPPER_PAGE][PUBCOM_TABLE_PAGE_SIZE]=
{
	/*page00*/
	{
	#if 0
		13,  196,     7,     2,     0,     0,     2,   16,   16,     1,  128,     3,  103,     0,     10/*2*/,     0,
		0,     0,     0,   64,   73,   78,   78,   79,   76,   73,   71,   72,   84,   32,   32,   32,
		32,   32,   32,   32,     7,   68,  124,  127,   84,   82,   45,   81,   81,   49,   51,   84,
		45,   78,   48,   48,  118,   49,   46,   49,   49,   65,  101,  144,   23,  112,   70,   61,
		0,     0,     0,  222,   73,   78,   69,   66,   71,   48,   56,   52,   48,   53,   50,   55,
		32,   32,   32,   32,   49,   52,   48,   56,   49,   52,   32,   32,     8,     0,     0,  167,
		17,   57,   33,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,
		255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255
	#else
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA

	#endif
	},
	/*page01*/
	{
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA

	},
	/*page02*/
	{
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA
	},
	/*page03*/
	{
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
	0xAA  ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA

    }
};	

const uInt8_PUB glb_flash_qsfp_up_memory[PUBCOM_MAX_UPPER_PAGE][PUBCOM_TABLE_PAGE_SIZE*4] __attribute__((at(PUBCOM_FLASH_TABLE_LOMEM_BASE))) =
{
	/*page00*/
	{
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,

	},
	/*page01*/
	{
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,

	},
	/*page02*/
	{
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,

	},
	/*page03*/
	{
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,

	}
};

//const uInt8_PUB A2h_test[256] __attribute__((at(0x0008f000)));

Bool_PUB glb_update_lomem_flash=FALSE_PUB;
Bool_PUB glb_update_A2lomem_flash = FALSE_PUB;
Bool_PUB glb_update_himem_flash[PUBCOM_MAX_UPPER_PAGE];


Bool_PUB glb_passward_enable = FALSE_PUB;
Bool_PUB glb_erase_fw_enable = FALSE_PUB;
Bool_PUB glb_update_dac_enable = FALSE_PUB;
Bool_PUB glb_update_reg_enable = FALSE_PUB;

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
**Name	  :   flash_read()
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
Bool_PUB flash_read(unsigned short int addr,unsigned char* value)
{
    uInt32_PUB cnt = 3000;
    
    FEEADR = addr;              // set address
    FEECON = 0x01;              // single read command
    //while(!(FEESTA & 0x01)){}   // wait for command completion
    
    while((cnt>1) && (!(FEESTA & 0x01))) {cnt--;}
    
    *value=(FEEDAT & 0xFF);     // return one byte data

	if(cnt==0)
		return FALSE_PUB;
	else 
		return TRUE_PUB;	
}


/*****************************************************************
**Name	  :   flash_write()
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
Bool_PUB flash_write(unsigned short int addr, unsigned char data){

    uInt32_PUB cnt = 3000;

    FEEADR = addr;              // set address
    FEEDAT = data;              // set data value
    FEECON = 0x02;              // write command
    //while(!(FEESTA & 0x01)){}   // wait for command completion

    while((cnt>1) && (!(FEESTA & 0x01))) {cnt--;}

	if(cnt==0)
		return FALSE_PUB;
	else 
		return TRUE_PUB;
    
}


/*****************************************************************
**Name	  :   flash_erase()
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
Bool_PUB flash_erase(unsigned short int addr){
    
    uInt32_PUB cnt = 3000;
    
    FEEADR = addr;              // set address
    FEECON = 0x05;              // page erase command
    //while(!(FEESTA & 0x01)){}   // wait for command completion
    
    while((cnt>1) && (!(FEESTA & 0x01))) {cnt--;}
	
	if(cnt==0)
		return FALSE_PUB;
	else 
		return TRUE_PUB;

}

/*****************************************************************
**Name	  :   init_flash_with_qsfp_default_table_code()
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
void init_flash_with_qsfp_default_table_code(void)
{
	uInt16_PUB i =0;
	uInt16_PUB j =0;

	/*copy A0 low_memroy from code*/
	for(i=0;i<256;i++)
		glb_sram_qsfp_A0_memory[i]=glb_code_qsfp_A0_memory[i];
	
	/*copy A2 low_memroy from code*/
	for(i=0;i<PUBCOM_TABLE_PAGE_SIZE;i++)
		glb_sram_qsfp_A2_lo_memory[i]=glb_code_qsfp_A2_lo_memory[i];

	/*copy A2 up_memroy from code*/
	for(j=0;j<PUBCOM_MAX_UPPER_PAGE;j++)
	{
		for(i=0;i<PUBCOM_TABLE_PAGE_SIZE;i++)
			glb_sram_qsfp_A2_memory[j][i]=glb_code_qsfp_A2_up_memory[j][i];
	}
}
/*****************************************************************
**Name	  :   init_flash_for_qsfp_table()
**
**Function:	check qsfp-table no flash data, create the table from code; if there are data, read to qsfp ram table
**
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
void init_flash_for_qsfp_table(void)
{
	Bool_PUB rc =  FALSE_PUB;

	uInt8_PUB trytime=5;
	uInt8_PUB value=0;

	int i=0;
	int j=0;

	trytime = 3;
	JUMP_CONTINUE0:
	trytime--;
	if(trytime==0)
		return;       
	rc = flash_read(PUBCOM_FUN_TABLE_VALID_STATUS_BASE,&value);
	if(rc==FALSE_PUB)
		goto JUMP_CONTINUE0;

	if(value!=PUBCOM_FUN_TABLE_VALID_STATUS_VALUE)// check A2h[130]
	{    
		init_flash_with_qsfp_default_table_code();        // load defalt data table

		trytime = 3;
		JUMP_CONTINUE1:
		trytime--;
		if(trytime==0)
			return;
		/*A0  memory*/	    
		flash_erase(PUBCOM_FUN_TABLE_LOMEM_BASE);    // erase page, F000 thru F1FF
		for(i=0; i<256; i++){
			rc = flash_write(PUBCOM_FUN_TABLE_LOMEM_BASE+(i*4), glb_sram_qsfp_A0_memory[i]);
			if(rc==FALSE_PUB)
				goto JUMP_CONTINUE1;
		}
		/*A2 low memory*/	
		flash_erase(PUBCOM_FUN_TABLE_A2LOWMEM_BASE);    // 
		for(i=0; i<PUBCOM_TABLE_PAGE_SIZE; i++){
			rc = flash_write(PUBCOM_FUN_TABLE_A2LOWMEM_BASE+(i*4), glb_sram_qsfp_A2_lo_memory[i]);
			if(rc==FALSE_PUB)
				goto JUMP_CONTINUE1;
		}
		trytime=3;
		JUMP_CONTINUE2:
		trytime--;
		if(trytime==0)
			return;		
		/*up memory page00*/
		glb_sram_qsfp_A2_memory[PUBCOM_UPPAGE_00][127]=PUBCOM_FUN_TABLE_VALID_STATUS_VALUE;/* clear flash init. enable bit*/
		for(j=0;j<PUBCOM_MAX_UPPER_PAGE;j++)
		{
			flash_erase(PUBCOM_FUN_TABLE_UPMEM__PAGE00_BASE+(j*PUBCOM_FLASH_PAGE_SIZE));
			for(i=0; i<PUBCOM_TABLE_PAGE_SIZE; i++)
			{
				rc = flash_write(PUBCOM_FUN_TABLE_UPMEM__PAGE00_BASE+(j*PUBCOM_FLASH_PAGE_SIZE)+(i*4), glb_sram_qsfp_A2_memory[j][i]);
				if(rc==FALSE_PUB)
					goto JUMP_CONTINUE2;
			}
		}
		
		rc = flash_write(PUBCOM_FUN_TABLE_VALID_STATUS_BASE,0x99);

		
	}
}


/*****************************************************************
**Name	  :   init_sram_for_qsfp_table()
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
void init_sram_for_qsfp_table(void)
{
	Bool_PUB rc = FALSE_PUB;
	uInt8_PUB value=0;
	uInt8_PUB trytime=5;

	uInt32_PUB i=0;
	uInt32_PUB j=0;

	/*init flash update status*/
	glb_update_lomem_flash = FALSE_PUB;
	for(j=0; j<PUBCOM_MAX_UPPER_PAGE;j++)
	{
		glb_update_himem_flash[j] = FALSE_PUB;
	}

	/*Map A0 memroy  data to SRAM*/
	for(i=0; i<256; i++)
	{
		trytime = 3;
		JUMP_CONTINUE0:
		trytime--;
		if(trytime==0)
			return;
		rc =flash_read(PUBCOM_FUN_TABLE_LOMEM_BASE+(i*4),&value);
		if(rc==FALSE_PUB)
			goto JUMP_CONTINUE0;
		glb_sram_qsfp_A0_memory[i]=(uInt8_PUB)(value);
	}
	
	for(i=0; i<PUBCOM_TABLE_PAGE_SIZE; i++)
	{
		trytime = 3;
		JUMP_CONTINUE2:
		trytime--;
		if(trytime==0)
			return;
		rc =flash_read(PUBCOM_FUN_TABLE_A2LOWMEM_BASE+(i*4),&value);
		if(rc==FALSE_PUB)
			goto JUMP_CONTINUE2;
		glb_sram_qsfp_A2_lo_memory[i]=(uInt8_PUB)(value);
	}
	
	
	

	/*Load A2h up data to Flash, start address 0x0008F400*/
	for(j=0; j<PUBCOM_MAX_UPPER_PAGE;j++)
	{
		for(i=0; i<PUBCOM_TABLE_PAGE_SIZE; i++)
		{
			trytime = 3;
			JUMP_CONTINUE1:
			trytime--;
			if(trytime==0)
				return;
			rc =flash_read(PUBCOM_FUN_TABLE_UPMEM__PAGE00_BASE+(j*PUBCOM_FLASH_PAGE_SIZE)+(i*4),&value);
			if(rc==FALSE_PUB)
				goto JUMP_CONTINUE1;
			glb_sram_qsfp_A2_memory[j][i]=(uInt8_PUB)value;
		}
	}
}


/*****************************************************************
**Name	  :   flash_update_qsfp_table()
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
void flash_update_qsfp_table(void)
{
	Bool_PUB rc = FALSE_PUB;
	int i=0;
	int j=0;

	if(glb_passward_enable==TRUE_PUB)
	{
		if((glb_update_lomem_flash == TRUE_PUB)&&(I2CBusy == 0))
		{
			FIQEN = 0x00000000;     // disable I2C0SIRQ
			/*low memory*/	    
			rc = flash_erase(PUBCOM_FUN_TABLE_LOMEM_BASE); 
			if(rc==FALSE_PUB)
			{
				FIQEN = 0x00000200;     // enable I2C0SIRQ
				return;
			}
			/*A0 high memory*/
			rc = flash_erase(PUBCOM_FUN_TABLE_LOMEM_BASE + PUBCOM_FLASH_PAGE_SIZE); 
			if(rc==FALSE_PUB)
			{
				FIQEN = 0x00000200;     // enable I2C0SIRQ
				return;
			}
			
			for(i=0; i<256; i++)
			{
				rc = flash_write(PUBCOM_FUN_TABLE_LOMEM_BASE+(i * 4), glb_sram_qsfp_A0_memory[i]);
				if(rc==FALSE_PUB)
				{
					FIQEN = 0x00000200;     // enable I2C0SIRQ
					return;
				}
			}
			glb_update_lomem_flash = FALSE_PUB;
			FIQEN = 0x00000200;     // enable I2C0SIRQ
		}
		
		if((glb_update_A2lomem_flash == TRUE_PUB)&&(I2CBusy == 0))
		{
			FIQEN = 0x00000000;     // disable I2C0SIRQ
			/*A2 low memory*/	    
			rc = flash_erase(PUBCOM_FUN_TABLE_A2LOWMEM_BASE); 
			if(rc==FALSE_PUB)
			{
				FIQEN = 0x00000200;     // enable I2C0SIRQ
				return;
			}
			
			for(i=0; i<PUBCOM_TABLE_PAGE_SIZE; i++)
			{
				rc = flash_write(PUBCOM_FUN_TABLE_A2LOWMEM_BASE+(i*4), glb_sram_qsfp_A2_lo_memory[i]);
				if(rc==FALSE_PUB)
				{
					FIQEN = 0x00000200;     // enable I2C0SIRQ
					return;
				}
			}
			glb_update_A2lomem_flash = FALSE_PUB;
			FIQEN = 0x00000200;     // enable I2C0SIRQ
		}	
		
		
		
	    
		for(j=0; j<PUBCOM_MAX_UPPER_PAGE;j++)
		{
			if((glb_update_himem_flash[j] == TRUE_PUB)&&(I2CBusy == 0))
			{
				FIQEN = 0x00000000;     // disable I2C0SIRQ
				/*high memory*/
		    rc = flash_erase(PUBCOM_FUN_TABLE_UPMEM__PAGE00_BASE+(j*PUBCOM_FLASH_PAGE_SIZE));
				if(rc==FALSE_PUB)
				{
					FIQEN = 0x00000200;     // enable I2C0SIRQ
					return;
				}
									
				for(i=0; i<PUBCOM_TABLE_PAGE_SIZE; i++)
				{
		      rc = flash_write(PUBCOM_FUN_TABLE_UPMEM__PAGE00_BASE+(j*PUBCOM_FLASH_PAGE_SIZE)+(i*4), glb_sram_qsfp_A2_memory[j][i]);
					if(rc==FALSE_PUB)
					{
						FIQEN = 0x00000200;     // enable I2C0SIRQ
						return;
					}
		    }

				glb_update_himem_flash[j] = FALSE_PUB;
				FIQEN = 0x00000200;     // enable I2C0SIRQ
			}
		}
	}
	else
	{
		glb_update_lomem_flash = FALSE_PUB;
		glb_update_A2lomem_flash = FALSE_PUB;
		for(j=0; j<PUBCOM_MAX_UPPER_PAGE;j++)
		{
			glb_update_himem_flash[j] = FALSE_PUB;
		}
		FIQEN = 0x00000200;     // enable I2C0SIRQ
	}
}
/*****************************************************************
**Name	  :   flash_update_qsfp_table()
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
void init_gn1160_with_flash(void)
{
	int ch = 0;
	int i=0;
	uInt8_PUB buf[PUBCOM_GN1160_REG_SIZE]={0};
	
	for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
	{
		for(i=0;i<PUBCOM_GN1160_REG_SIZE;i++)
		{
			buf[i] = glb_code_gn1160_regvalue_memory[ch][i];
		}
		i2cbg_write_nbytes((E_I2CBG_TXn)ch,buf,PUBCOM_GN1160_REG_SIZE,0);
	}

}

