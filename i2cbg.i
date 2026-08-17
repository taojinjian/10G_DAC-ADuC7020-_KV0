#line 1 "code\\i2cbg.c"












 



 
#line 1 "code\\pubcom.h"












 


    
#line 1 "code\\pubtype.h"












 


    
typedef  char               Int8_PUB;
typedef  short              Int16_PUB;
typedef  int                Int32_PUB;   
typedef  unsigned char      uInt8_PUB;
typedef  unsigned short     uInt16_PUB;
typedef  unsigned int       uInt32_PUB;   

typedef  unsigned char               Bool_PUB;
typedef  unsigned char      Byte_PUB;
    


    
    






#line 18 "code\\pubcom.h"
#line 1 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"












#line 34 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"



#line 44 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"











#line 61 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"











#line 83 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"














#line 108 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"



#line 123 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"



#line 146 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"



#line 169 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"



#line 178 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"



#line 204 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"



#line 222 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"



#line 241 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"



#line 253 "C:\\Keil\\ARM\\INC\\ADI\\ADuC7023.h"
 
typedef void (* tyVctHndlr) (void);

extern tyVctHndlr    IRQ;
extern tyVctHndlr    SWI;
extern tyVctHndlr    FIQ;
extern tyVctHndlr    UNDEF;
extern tyVctHndlr    PABORT;
extern tyVctHndlr    DABORT;
#line 19 "code\\pubcom.h"


#line 38 "code\\pubcom.h"


enum _PAGE_INDEX
{
    PUBCOM_UPPAGE_00,
    PUBCOM_UPPAGE_01,
    PUBCOM_UPPAGE_02,
    PUBCOM_UPPAGE_03,
    PUBCOM_UPPAGE_99=99,
     PUBCOM_UPPAGE_100=100,
};






#line 63 "code\\pubcom.h"
















typedef enum _E_I2CBG_TX{
E_I2CBG_TX1,
E_I2CBG_TX2,
E_I2CBG_TX3,
E_I2CBG_TX4,
E_I2CBG_TX_MAX,
}E_I2CBG_TXn;










 
void init_flash_for_qsfp_table(void);
void init_sram_for_qsfp_table(void);
void flash_update_qsfp_table(void);
void init_module(void);
Bool_PUB flash_erase(unsigned short int addr);
void init_gn1160_with_flash(void);

 
void misc_monitor(void);
void misc_diag(void);
Bool_PUB misc_get_lpmode_status(void);
Bool_PUB misc_get_modsel_status(void);
void  misc_set_intl_status(Bool_PUB intterupt);
void misc_specail_for_jatg(void);
void misc_specail_erase_fwcmd(void);
void misc_lookup_table(void);
void misc_update_dac(void);
void misc_register_wr(void);
 
Bool_PUB i2cbg_read_nbytes(E_I2CBG_TXn tx,uInt8_PUB *pdata,uInt8_PUB lenght,uInt8_PUB reg_address);
Bool_PUB i2cbg_write_nbytes(E_I2CBG_TXn tx,uInt8_PUB *pdata,uInt8_PUB lenght,uInt8_PUB reg_address);
void i2cbg_specail_for_jtag(void);



#line 19 "code\\i2cbg.c"



 


 


enum E_I2CBG_DIR{
E_I2CBG_DIR_OUT,
E_I2CBG_DIR_IN	
};
enum E_I2CBG_VALUE{
E_I2CBG_VALUE_LO,	
E_I2CBG_VALUE_HI,

};

#line 50 "code\\i2cbg.c"
 										
#line 59 "code\\i2cbg.c"

#line 68 "code\\i2cbg.c"
											



 										
#line 81 "code\\i2cbg.c"

#line 90 "code\\i2cbg.c"


											

 										
#line 103 "code\\i2cbg.c"

#line 112 "code\\i2cbg.c"
											

											
											
 										
#line 125 "code\\i2cbg.c"

#line 134 "code\\i2cbg.c"


											
											


 








 



 





 





 


 



 












 
static void i2cbg_delay_us(uInt32_PUB us)
{
	us = us*10;
	while(us--);
}













 
Bool_PUB i2cbg_start(E_I2CBG_TXn tx)
{
	if(tx==E_I2CBG_TX1)
	{
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x08000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x08000000); }};
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00080000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00080000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX2)
	{
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x04000000); }};
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00040000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00040000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX3)
	{
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x04000000); }};
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00040000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00040000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX4)
	{
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x02000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x02000000); }};
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_VALUE_HI==1) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00020000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00020000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{if(E_I2CBG_VALUE_LO==1) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00020000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00020000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
	}
	else
	{
		return (0);
	}
	
	return (1);
}












 
Bool_PUB i2cbg_write(E_I2CBG_TXn tx,uInt8_PUB data)
{
	uInt8_PUB bit_cnt=0;
	
	if(tx==E_I2CBG_TX1)
	{
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x08000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x08000000); }};i2cbg_delay_us(1);
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);

		for(bit_cnt=0;bit_cnt<8;bit_cnt++)
		{
			if(data & (1<<(7-bit_cnt)))
				{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00080000); }}
			else
				{if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00080000); }}
			i2cbg_delay_us(2);

			{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);
			{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);
		}
	}
	else if(tx==E_I2CBG_TX2)
	{
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x04000000); }};i2cbg_delay_us(1);
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);

		for(bit_cnt=0;bit_cnt<8;bit_cnt++)
		{
			if(data & (1<<(7-bit_cnt)))
				{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00040000); }}
			else
				{if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00040000); }}
			i2cbg_delay_us(2);

			{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);
			{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);
		}
	}
	else if(tx==E_I2CBG_TX3)
	{
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x04000000); }};i2cbg_delay_us(1);
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);

		for(bit_cnt=0;bit_cnt<8;bit_cnt++)
		{
			if(data & (1<<(7-bit_cnt)))
				{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00040000); }}
			else
				{if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00040000); }}
			i2cbg_delay_us(2);

			{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);
			{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);
		}
	}
	else if(tx==E_I2CBG_TX4)
	{
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x02000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x02000000); }};i2cbg_delay_us(1);
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);

		for(bit_cnt=0;bit_cnt<8;bit_cnt++)
		{
			if(data & (1<<(7-bit_cnt)))
				{if(E_I2CBG_VALUE_HI==1) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00020000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00020000); }}
			else
				{if(E_I2CBG_VALUE_LO==1) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00020000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00020000); }}
			i2cbg_delay_us(2);

			{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);
			{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);
		}
	}
	else
	{
		return (0);
	}
	
	return (1);
}












 
Bool_PUB i2cbg_waitack(E_I2CBG_TXn tx)
{
	uInt16_PUB err_time = 0;
	
	if(tx==E_I2CBG_TX1)
	{
		{if(E_I2CBG_DIR_IN==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x08000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x08000000); }};i2cbg_delay_us(1);
		{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00080000); }};i2cbg_delay_us(2);
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);

		while(((*(volatile unsigned long *) 0xFFFFF420)&0x00000008) != 0 ? 1:0)
		{
			err_time++;
			if(err_time>250)
			{
				return (0);
			}
		}

		i2cbg_delay_us(1);
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		return (1);
	}
	else if(tx==E_I2CBG_TX2)
	{
		{if(E_I2CBG_DIR_IN==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x04000000); }};i2cbg_delay_us(1);
		{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00040000); }};i2cbg_delay_us(2);
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);

		while(((*(volatile unsigned long *) 0xFFFFF430)&0x00000004) != 0 ? 1:0)
		{
			err_time++;
			if(err_time>250)
			{
				return (0);
			}
		}

		i2cbg_delay_us(1);
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		return (1);
	}
	else if(tx==E_I2CBG_TX3)
	{
		{if(E_I2CBG_DIR_IN==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x04000000); }};i2cbg_delay_us(1);
		{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00040000); }};i2cbg_delay_us(2);
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);

		while(((*(volatile unsigned long *) 0xFFFFF420)&0x00000004) != 0 ? 1:0)
		{
			err_time++;
			if(err_time>250)
			{
				return (0);
			}
		}

		i2cbg_delay_us(1);
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		return (1);
	}
	else if(tx==E_I2CBG_TX4)
	{
		{if(E_I2CBG_DIR_IN==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x02000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x02000000); }};i2cbg_delay_us(1);
		{if(E_I2CBG_VALUE_HI==1) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00020000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00020000); }};i2cbg_delay_us(2);
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };i2cbg_delay_us(2);

		while(((*(volatile unsigned long *) 0xFFFFF420)&0x00000002) != 0 ? 1:0)
		{
			err_time++;
			if(err_time>250)
			{
				return (0);
			}
		}

		i2cbg_delay_us(1);
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		return (1);
	}
	else
	{
		return (0);
	}
}












 
uInt8_PUB i2cbg_read(E_I2CBG_TXn tx)
{
	uInt8_PUB i=0;
	uInt8_PUB rcv_sda = 0;
	
	if(tx==E_I2CBG_TX1)
	{
		{if(E_I2CBG_DIR_IN==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x08000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x08000000); }};
		i2cbg_delay_us(1);

		for(i=0;i<8;i++)
		{
			{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
			i2cbg_delay_us(2);
			
			{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
			i2cbg_delay_us(1);

			rcv_sda<<=1;

			if(((*(volatile unsigned long *) 0xFFFFF420)&0x00000008) != 0 ? 1:0)rcv_sda++;

			i2cbg_delay_us(1);
		}
	}
	else if(tx==E_I2CBG_TX2)
	{
		{if(E_I2CBG_DIR_IN==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x04000000); }};
		i2cbg_delay_us(1);

		for(i=0;i<8;i++)
		{
			{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
			i2cbg_delay_us(2);
			
			{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
			i2cbg_delay_us(1);

			rcv_sda<<=1;

			if(((*(volatile unsigned long *) 0xFFFFF430)&0x00000004) != 0 ? 1:0)rcv_sda++;

			i2cbg_delay_us(1);
		}
	}
	else if(tx==E_I2CBG_TX3)
	{
		{if(E_I2CBG_DIR_IN==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x04000000); }};
		i2cbg_delay_us(1);

		for(i=0;i<8;i++)
		{
			{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
			i2cbg_delay_us(2);
			
			{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
			i2cbg_delay_us(1);

			rcv_sda<<=1;

			if(((*(volatile unsigned long *) 0xFFFFF420)&0x00000004) != 0 ? 1:0)rcv_sda++;

			i2cbg_delay_us(1);
		}
	}
	else if(tx==E_I2CBG_TX4)
	{
		{if(E_I2CBG_DIR_IN==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x02000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x02000000); }};
		i2cbg_delay_us(1);

		for(i=0;i<8;i++)
		{
			{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
			i2cbg_delay_us(2);
			
			{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
			i2cbg_delay_us(1);

			rcv_sda<<=1;

			if(((*(volatile unsigned long *) 0xFFFFF420)&0x00000002) != 0 ? 1:0)rcv_sda++;

			i2cbg_delay_us(1);
		}
	}
	else
	{
		return 0;
	}
	
	return rcv_sda;
}












 
void i2cbg_send_ack(E_I2CBG_TXn tx)
{
	if(tx==E_I2CBG_TX1)
	{
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x08000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x08000000); }};
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00080000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);

	}
	else if(tx==E_I2CBG_TX2)
	{
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x04000000); }};
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00040000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX3)
	{
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x04000000); }};
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00040000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX4)
	{
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x02000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x02000000); }};
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_VALUE_LO==1) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00020000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00020000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
	}
	else
	{
		
	}
	
	
}












 
void i2cbg_send_nack(E_I2CBG_TXn tx)
{
	if(tx==E_I2CBG_TX1)
	{
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x08000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x08000000); }};
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00080000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX2)
	{
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x04000000); }};
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00040000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX3)
	{
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x04000000); }};
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00040000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX4)
	{
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x02000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x02000000); }};
		i2cbg_delay_us(1);
		
		{if(E_I2CBG_VALUE_HI==1) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00020000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00020000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
	}
	else
	{
		
	}
	
	
}












 
void i2cbg_stop(E_I2CBG_TXn tx)
{
	if(tx==E_I2CBG_TX1)
	{
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x08000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x08000000); }};
		i2cbg_delay_us(1);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00080000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00080000); }};
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX2)
	{
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x04000000); }};
		i2cbg_delay_us(1);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00040000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00040000); }};
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX3)
	{
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x04000000); }};
		i2cbg_delay_us(1);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00040000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00040000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00040000); }};
		i2cbg_delay_us(2);
	}
	else if(tx==E_I2CBG_TX4)
	{
		{if(E_I2CBG_DIR_OUT==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x02000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x02000000); }};
		i2cbg_delay_us(1);
		
		{ if(E_I2CBG_VALUE_LO==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{if(E_I2CBG_VALUE_LO==1) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00020000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00020000); }};
		i2cbg_delay_us(2);
		
		{ if(E_I2CBG_VALUE_HI==E_I2CBG_VALUE_HI) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) |= 0x00080000; } else { (*(volatile unsigned long *) 0xFFFFF430) |= 0x08000000; (*(volatile unsigned long *) 0xFFFFF430) &= (~0x00080000); } };
		i2cbg_delay_us(2);
		
		{if(E_I2CBG_VALUE_HI==1) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x00020000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x00020000); }};
		i2cbg_delay_us(2);
	}
	else
	{
		
	}
	
	
}












 
Bool_PUB i2cbg_read_nbytes(E_I2CBG_TXn tx,uInt8_PUB *pdata,uInt8_PUB lenght,uInt8_PUB reg_address)
{
	uInt8_PUB i =0;

	 
	i2cbg_stop(tx);
	i2cbg_start(tx);

	 
	i2cbg_write(tx, ((0xA8>>1)<<1)&0xFE);
	if(i2cbg_waitack(tx)==(0))
	{
		i2cbg_stop(tx);
		return (0);
	}

	 
	i2cbg_write(tx,reg_address);
	if(i2cbg_waitack(tx)==(0))
	{
		i2cbg_stop(tx);
		return (0);
	}
	
	 
	
	i2cbg_start(tx);
	
	 
	i2cbg_write(tx, (((0xA8>>1)<<1)|0x01));
	if(i2cbg_waitack(tx)==(0))
	{
		i2cbg_stop(tx);
		return (0);
	}

	 
	for(i=0;i<(lenght-1);i++)
	{
		*pdata = i2cbg_read(tx);
		i2cbg_send_ack(tx);
		pdata++;
	}

	 
	*pdata = i2cbg_read(tx);
	i2cbg_send_nack(tx);

	 
	i2cbg_stop(tx);

	return (1);
}













 
Bool_PUB i2cbg_write_nbytes(E_I2CBG_TXn tx,uInt8_PUB *pdata,uInt8_PUB lenght,uInt8_PUB reg_address)
{
	uInt8_PUB i =0;

	 
	i2cbg_stop(tx);
	i2cbg_start(tx);

	 
	i2cbg_write(tx,(((0xA8>>1)<<1)&0xFE));
	
	if(i2cbg_waitack(tx)==(0))
	{
		i2cbg_stop(tx);
		return (0);
	}

	 
	i2cbg_write(tx,reg_address);
	if(i2cbg_waitack(tx)==(0))
	{
		i2cbg_stop(tx);
		return (0);
	}
	
	 
	for(i=0;i<(lenght-1);i++)
	{
		i2cbg_write(tx,*pdata);
		if(i2cbg_waitack(tx)==(0))
		{
			i2cbg_stop(tx);
			return (0);
		}
		pdata++;
	}

	 
	i2cbg_write(tx,*pdata);
	if(i2cbg_waitack(tx)==(0))
	{
		i2cbg_stop(tx);
		return (0);
	}

	 
	i2cbg_stop(tx);

	return (1);
}












 
void i2cbg_specail_for_jtag(void)
{
	{if(E_I2CBG_DIR_IN==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x08000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x08000000); }};
	{if(E_I2CBG_DIR_IN==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF430) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF430) &= (~0x04000000); }};
	{if(E_I2CBG_DIR_IN==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x04000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x04000000); }};
	{if(E_I2CBG_DIR_IN==E_I2CBG_DIR_OUT) { (*(volatile unsigned long *) 0xFFFFF420) |= 0x02000000; } else { (*(volatile unsigned long *) 0xFFFFF420) &= (~0x02000000); }};

	
	

}

