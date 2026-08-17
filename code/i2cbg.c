/***********************************************************
** xxxxxx Co.Ltd CopyRight
**
**File: i2cbg.c
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
#define GN1160_DEVICE_ADDR		(0xA8>>1)

enum E_I2CBG_DIR{
E_I2CBG_DIR_OUT,
E_I2CBG_DIR_IN	
};
enum E_I2CBG_VALUE{
E_I2CBG_VALUE_LO,	
E_I2CBG_VALUE_HI,

};

#define I2CBG_SCL_SET(set_scl_value)			{\
												if(set_scl_value==E_I2CBG_VALUE_HI)\
												{\
													GP1DAT |= 0x08000000;/*P1.3 as an output */\
													GP1DAT |= 0x00080000;	/*set P1.3=1*/\
												}\
												else\
												{\
													GP1DAT |= 0x08000000;/*P1.3 as an output*/\
													GP1DAT &= (~0x00080000);	/*set P1.3=0*/\
												}\
											}
/**sda1 operate api**/										
#define I2CBG_SDA_TX1_SET(set_sda1_value)	{if(set_sda1_value==E_I2CBG_VALUE_HI)\
											{\
												GP0DAT |= 0x00080000;	/*set P0.3=1*/\
											}\
											else\
											{\
												GP0DAT &= (~0x00080000);	/*set P0.3=0*/\
											}}

#define I2CBG_SDA_TX1_DIR(set_sda1_dir)		{if(set_sda1_dir==E_I2CBG_DIR_OUT)\
											{\
												GP0DAT |= 0x08000000;/*P0.3 as an output*/\
											}\
											else\
											{\
												GP0DAT &= (~0x08000000);/*P0.3 as an input */\
											}}
											
#define I2CBG_SDA_TX1_GET					(GP0DAT&0x00000008) != 0 ? 1:0


/**sda2 operate api**/										
#define I2CBG_SDA_TX2_SET(set_sda2_value)	{if(set_sda2_value==E_I2CBG_VALUE_HI)\
											{\
												GP1DAT |= 0x00040000;	/*set P1.2=1*/\
											}\
											else\
											{\
												GP1DAT &= (~0x00040000);	/*set P1.2=0*/\
											}}

#define I2CBG_SDA_TX2_DIR(set_sda2_dir)		{if(set_sda2_dir==E_I2CBG_DIR_OUT)\
											{\
												GP1DAT |= 0x04000000;/*P1.2 as an output*/\
											}\
											else\
											{\
												GP1DAT &= (~0x04000000);/*P1.2 as an input */\
											}}

#define I2CBG_SDA_TX2_GET					(GP1DAT&0x00000004) != 0 ? 1:0
											

/**sda3 operate api**/										
#define I2CBG_SDA_TX3_SET(set_sda3_value)	{if(set_sda3_value==E_I2CBG_VALUE_HI)\
											{\
												GP0DAT |= 0x00040000;	/*set P0.2=1*/\
											}\
											else\
											{\
												GP0DAT &= (~0x00040000);	/*set P0.2=0*/\
											}}

#define I2CBG_SDA_TX3_DIR(set_sda3_dir)		{if(set_sda3_dir==E_I2CBG_DIR_OUT)\
											{\
												GP0DAT |= 0x04000000;/*P0.2 as an output*/\
											}\
											else\
											{\
												GP0DAT &= (~0x04000000);/*P0.2 as an input */\
											}}
											
#define I2CBG_SDA_TX3_GET					(GP0DAT&0x00000004) != 0 ? 1:0
											
											
/**sda4 operate api**/										
#define I2CBG_SDA_TX4_SET(set_sda4_value)	{if(set_sda4_value==1)\
											{\
												GP0DAT |= 0x00020000;	/*set P0.1=1*/\
											}\
											else\
											{\
												GP0DAT &= (~0x00020000);	/*set P0.1=0*/\
											}}

#define I2CBG_SDA_TX4_DIR(set_sda4_dir)		{if(set_sda4_dir==E_I2CBG_DIR_OUT)\
											{\
												GP0DAT |= 0x02000000;/*P0.1 as an output*/\
											}\
											else\
											{\
												GP0DAT &= (~0x02000000);/*P0.1 as an input */\
											}}

#define I2CBG_SDA_TX4_GET					(GP0DAT&0x00000002) != 0 ? 1:0
											
											
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
**Name	  :   i2cbg_delay_us()
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
static void i2cbg_delay_us(uInt32_PUB us)
{
	us = us*10;
	while(us--);
}

/*****************************************************************
**Name	  :   i2cbg_start()
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
Bool_PUB i2cbg_start(E_I2CBG_TXn tx)
{
	if(tx==E_I2CBG_TX1)
	{
		I2CBG_SDA_TX1_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX1_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SDA_TX1_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX2)
	{
		I2CBG_SDA_TX2_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX2_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SDA_TX2_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX3)
	{
		I2CBG_SDA_TX3_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX3_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SDA_TX3_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX4)
	{
		I2CBG_SDA_TX4_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX4_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SDA_TX4_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
	}
	else
	{
		return FALSE_PUB;
	}
	
	return TRUE_PUB;
}
/*****************************************************************
**Name	  :   i2cbg_read_nbytes()
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
Bool_PUB i2cbg_write(E_I2CBG_TXn tx,uInt8_PUB data)
{
	uInt8_PUB bit_cnt=0;
	
	if(tx==E_I2CBG_TX1)
	{
		I2CBG_SDA_TX1_DIR(E_I2CBG_DIR_OUT);i2cbg_delay_us(1);
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);i2cbg_delay_us(2);

		for(bit_cnt=0;bit_cnt<8;bit_cnt++)
		{
			if(data & (1<<(7-bit_cnt)))
				I2CBG_SDA_TX1_SET(E_I2CBG_VALUE_HI)
			else
				I2CBG_SDA_TX1_SET(E_I2CBG_VALUE_LO)
			i2cbg_delay_us(2);

			I2CBG_SCL_SET(E_I2CBG_VALUE_HI);i2cbg_delay_us(2);
			I2CBG_SCL_SET(E_I2CBG_VALUE_LO);i2cbg_delay_us(2);
		}
	}
	else if(tx==E_I2CBG_TX2)
	{
		I2CBG_SDA_TX2_DIR(E_I2CBG_DIR_OUT);i2cbg_delay_us(1);
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);i2cbg_delay_us(2);

		for(bit_cnt=0;bit_cnt<8;bit_cnt++)
		{
			if(data & (1<<(7-bit_cnt)))
				I2CBG_SDA_TX2_SET(E_I2CBG_VALUE_HI)
			else
				I2CBG_SDA_TX2_SET(E_I2CBG_VALUE_LO)
			i2cbg_delay_us(2);

			I2CBG_SCL_SET(E_I2CBG_VALUE_HI);i2cbg_delay_us(2);
			I2CBG_SCL_SET(E_I2CBG_VALUE_LO);i2cbg_delay_us(2);
		}
	}
	else if(tx==E_I2CBG_TX3)
	{
		I2CBG_SDA_TX3_DIR(E_I2CBG_DIR_OUT);i2cbg_delay_us(1);
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);i2cbg_delay_us(2);

		for(bit_cnt=0;bit_cnt<8;bit_cnt++)
		{
			if(data & (1<<(7-bit_cnt)))
				I2CBG_SDA_TX3_SET(E_I2CBG_VALUE_HI)
			else
				I2CBG_SDA_TX3_SET(E_I2CBG_VALUE_LO)
			i2cbg_delay_us(2);

			I2CBG_SCL_SET(E_I2CBG_VALUE_HI);i2cbg_delay_us(2);
			I2CBG_SCL_SET(E_I2CBG_VALUE_LO);i2cbg_delay_us(2);
		}
	}
	else if(tx==E_I2CBG_TX4)
	{
		I2CBG_SDA_TX4_DIR(E_I2CBG_DIR_OUT);i2cbg_delay_us(1);
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);i2cbg_delay_us(2);

		for(bit_cnt=0;bit_cnt<8;bit_cnt++)
		{
			if(data & (1<<(7-bit_cnt)))
				I2CBG_SDA_TX4_SET(E_I2CBG_VALUE_HI)
			else
				I2CBG_SDA_TX4_SET(E_I2CBG_VALUE_LO)
			i2cbg_delay_us(2);

			I2CBG_SCL_SET(E_I2CBG_VALUE_HI);i2cbg_delay_us(2);
			I2CBG_SCL_SET(E_I2CBG_VALUE_LO);i2cbg_delay_us(2);
		}
	}
	else
	{
		return FALSE_PUB;
	}
	
	return TRUE_PUB;
}
/*****************************************************************
**Name	  :   i2cbg_waitack()
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
Bool_PUB i2cbg_waitack(E_I2CBG_TXn tx)
{
	uInt16_PUB err_time = 0;
	
	if(tx==E_I2CBG_TX1)
	{
		I2CBG_SDA_TX1_DIR(E_I2CBG_DIR_IN);i2cbg_delay_us(1);
		I2CBG_SDA_TX1_SET(E_I2CBG_VALUE_HI);i2cbg_delay_us(2);
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);i2cbg_delay_us(2);

		while(I2CBG_SDA_TX1_GET)
		{
			err_time++;
			if(err_time>250)
			{
				return FALSE_PUB;
			}
		}

		i2cbg_delay_us(1);
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		return TRUE_PUB;
	}
	else if(tx==E_I2CBG_TX2)
	{
		I2CBG_SDA_TX2_DIR(E_I2CBG_DIR_IN);i2cbg_delay_us(1);
		I2CBG_SDA_TX2_SET(E_I2CBG_VALUE_HI);i2cbg_delay_us(2);
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);i2cbg_delay_us(2);

		while(I2CBG_SDA_TX2_GET)
		{
			err_time++;
			if(err_time>250)
			{
				return FALSE_PUB;
			}
		}

		i2cbg_delay_us(1);
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		return TRUE_PUB;
	}
	else if(tx==E_I2CBG_TX3)
	{
		I2CBG_SDA_TX3_DIR(E_I2CBG_DIR_IN);i2cbg_delay_us(1);
		I2CBG_SDA_TX3_SET(E_I2CBG_VALUE_HI);i2cbg_delay_us(2);
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);i2cbg_delay_us(2);

		while(I2CBG_SDA_TX3_GET)
		{
			err_time++;
			if(err_time>250)
			{
				return FALSE_PUB;
			}
		}

		i2cbg_delay_us(1);
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		return TRUE_PUB;
	}
	else if(tx==E_I2CBG_TX4)
	{
		I2CBG_SDA_TX4_DIR(E_I2CBG_DIR_IN);i2cbg_delay_us(1);
		I2CBG_SDA_TX4_SET(E_I2CBG_VALUE_HI);i2cbg_delay_us(2);
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);i2cbg_delay_us(2);

		while(I2CBG_SDA_TX4_GET)
		{
			err_time++;
			if(err_time>250)
			{
				return FALSE_PUB;
			}
		}

		i2cbg_delay_us(1);
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		return TRUE_PUB;
	}
	else
	{
		return FALSE_PUB;
	}
}
/*****************************************************************
**Name	  :   i2cbg_read()
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
uInt8_PUB i2cbg_read(E_I2CBG_TXn tx)
{
	uInt8_PUB i=0;
	uInt8_PUB rcv_sda = 0;
	
	if(tx==E_I2CBG_TX1)
	{
		I2CBG_SDA_TX1_DIR(E_I2CBG_DIR_IN);
		i2cbg_delay_us(1);

		for(i=0;i<8;i++)
		{
			I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
			i2cbg_delay_us(2);
			
			I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
			i2cbg_delay_us(1);

			rcv_sda<<=1;

			if(I2CBG_SDA_TX1_GET)rcv_sda++;

			i2cbg_delay_us(1);
		}
	}
	else if(tx==E_I2CBG_TX2)
	{
		I2CBG_SDA_TX2_DIR(E_I2CBG_DIR_IN);
		i2cbg_delay_us(1);

		for(i=0;i<8;i++)
		{
			I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
			i2cbg_delay_us(2);
			
			I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
			i2cbg_delay_us(1);

			rcv_sda<<=1;

			if(I2CBG_SDA_TX2_GET)rcv_sda++;

			i2cbg_delay_us(1);
		}
	}
	else if(tx==E_I2CBG_TX3)
	{
		I2CBG_SDA_TX3_DIR(E_I2CBG_DIR_IN);
		i2cbg_delay_us(1);

		for(i=0;i<8;i++)
		{
			I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
			i2cbg_delay_us(2);
			
			I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
			i2cbg_delay_us(1);

			rcv_sda<<=1;

			if(I2CBG_SDA_TX3_GET)rcv_sda++;

			i2cbg_delay_us(1);
		}
	}
	else if(tx==E_I2CBG_TX4)
	{
		I2CBG_SDA_TX4_DIR(E_I2CBG_DIR_IN);
		i2cbg_delay_us(1);

		for(i=0;i<8;i++)
		{
			I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
			i2cbg_delay_us(2);
			
			I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
			i2cbg_delay_us(1);

			rcv_sda<<=1;

			if(I2CBG_SDA_TX4_GET)rcv_sda++;

			i2cbg_delay_us(1);
		}
	}
	else
	{
		return 0;
	}
	
	return rcv_sda;
}
/*****************************************************************
**Name	  :   i2cbg_send_ack()
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
void i2cbg_send_ack(E_I2CBG_TXn tx)
{
	if(tx==E_I2CBG_TX1)
	{
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX1_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX1_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);

	}
	else if(tx==E_I2CBG_TX2)
	{
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX2_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX2_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX3)
	{
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX3_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX3_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX4)
	{
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX4_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX4_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
	}
	else
	{
		//return FALSE_PUB;
	}
	
	//return TRUE_PUB;
}
/*****************************************************************
**Name	  :   i2cbg_send_nack()
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
void i2cbg_send_nack(E_I2CBG_TXn tx)
{
	if(tx==E_I2CBG_TX1)
	{
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX1_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX1_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX2)
	{
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX2_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX2_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX3)
	{
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX3_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX3_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX4)
	{
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX4_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SDA_TX4_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
	}
	else
	{
		//return FALSE_PUB;
	}
	
	//return TRUE_PUB;
}
/*****************************************************************
**Name	  :   i2cbg_send_nack()
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
void i2cbg_stop(E_I2CBG_TXn tx)
{
	if(tx==E_I2CBG_TX1)
	{
		I2CBG_SDA_TX1_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SDA_TX1_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SDA_TX1_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX2)
	{
		I2CBG_SDA_TX2_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SDA_TX2_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SDA_TX2_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX3)
	{
		I2CBG_SDA_TX3_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SDA_TX3_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SDA_TX3_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX4)
	{
		I2CBG_SDA_TX4_DIR(E_I2CBG_DIR_OUT);
		i2cbg_delay_us(1);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SDA_TX4_SET(E_I2CBG_VALUE_LO);
		i2cbg_delay_us(2);
		
		I2CBG_SCL_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
		
		I2CBG_SDA_TX4_SET(E_I2CBG_VALUE_HI);
		i2cbg_delay_us(2);
	}
	else
	{
		//return FALSE_PUB;
	}
	
	//return TRUE_PUB;
}
/*****************************************************************
**Name	  :   i2cbg_read_nbytes()
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
Bool_PUB i2cbg_read_nbytes(E_I2CBG_TXn tx,uInt8_PUB *pdata,uInt8_PUB lenght,uInt8_PUB reg_address)
{
	uInt8_PUB i =0;

	/*send start single*/
	i2cbg_stop(tx);
	i2cbg_start(tx);

	/*send device addr & write single*/
	i2cbg_write(tx, (GN1160_DEVICE_ADDR<<1)&0xFE);
	if(i2cbg_waitack(tx)==FALSE_PUB)
	{
		i2cbg_stop(tx);
		return FALSE_PUB;
	}

	/*send register addr*/
	i2cbg_write(tx,reg_address);
	if(i2cbg_waitack(tx)==FALSE_PUB)
	{
		i2cbg_stop(tx);
		return FALSE_PUB;
	}
	
	/*send re-start single*/
	//i2cbg_stop(tx);
	i2cbg_start(tx);
	
	/*send device addr & read single*/
	i2cbg_write(tx, ((GN1160_DEVICE_ADDR<<1)|0x01));
	if(i2cbg_waitack(tx)==FALSE_PUB)
	{
		i2cbg_stop(tx);
		return FALSE_PUB;
	}

	/*read data*/
	for(i=0;i<(lenght-1);i++)
	{
		*pdata = i2cbg_read(tx);
		i2cbg_send_ack(tx);
		pdata++;
	}

	/*read last byte data */
	*pdata = i2cbg_read(tx);
	i2cbg_send_nack(tx);

	/*send stop*/
	i2cbg_stop(tx);

	return TRUE_PUB;
}

/*****************************************************************
**Name	  :   i2cbg_write_nbytes()
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
Bool_PUB i2cbg_write_nbytes(E_I2CBG_TXn tx,uInt8_PUB *pdata,uInt8_PUB lenght,uInt8_PUB reg_address)
{
	uInt8_PUB i =0;

	/*send start single*/
	i2cbg_stop(tx);
	i2cbg_start(tx);

	/*send device addr & write single*/
	i2cbg_write(tx,((GN1160_DEVICE_ADDR<<1)&0xFE));
	
	if(i2cbg_waitack(tx)==FALSE_PUB)
	{
		i2cbg_stop(tx);
		return FALSE_PUB;
	}

	/*send register addr*/
	i2cbg_write(tx,reg_address);
	if(i2cbg_waitack(tx)==FALSE_PUB)
	{
		i2cbg_stop(tx);
		return FALSE_PUB;
	}
	
	/*write data*/
	for(i=0;i<(lenght-1);i++)
	{
		i2cbg_write(tx,*pdata);
		if(i2cbg_waitack(tx)==FALSE_PUB)
		{
			i2cbg_stop(tx);
			return FALSE_PUB;
		}
		pdata++;
	}

	/*write last byte data */
	i2cbg_write(tx,*pdata);
	if(i2cbg_waitack(tx)==FALSE_PUB)
	{
		i2cbg_stop(tx);
		return FALSE_PUB;
	}

	/*send stop*/
	i2cbg_stop(tx);

	return TRUE_PUB;
}
/*****************************************************************
**Name	  :   i2cbg_specail_for_jtag()
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
void i2cbg_specail_for_jtag(void)
{
	I2CBG_SDA_TX1_DIR(E_I2CBG_DIR_IN);
	I2CBG_SDA_TX2_DIR(E_I2CBG_DIR_IN);
	I2CBG_SDA_TX3_DIR(E_I2CBG_DIR_IN);
	I2CBG_SDA_TX4_DIR(E_I2CBG_DIR_IN);

	//
	//GP0PAR=0x2222000;

}

