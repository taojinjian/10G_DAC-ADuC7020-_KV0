#line 1 "code\\inttrupt.c"












 



 
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



#line 19 "code\\inttrupt.c"



 


 




 








 



 
 uInt8_PUB glb_i2c0_rcv_first=(1);
uInt8_PUB i2c_receive, dac_update;
uInt32_PUB glb_sys_tick=0;




 
extern uInt8_PUB glb_sram_qsfp_lo_memory[(128)];
extern uInt8_PUB glb_sram_qsfp_up_memory[(4)][(128)];
extern uInt8_PUB glb_sram_map_gn1160[(E_I2CBG_TX_MAX)][(64)];

extern Bool_PUB glb_passward_enable;
extern Bool_PUB glb_erase_fw_enable;
extern Bool_PUB glb_update_dac_enable;
extern Bool_PUB glb_update_reg_enable;

extern Bool_PUB glb_update_lomem_flash;
extern Bool_PUB glb_update_himem_flash[(4)];

extern uInt8_PUB glb_sram_map_dac[32];


const uInt8_PUB glb_passwrd[]={'a','a','a','a'};

const uInt8_PUB glb_erase[]={'e','r','a','s'};

uInt8_PUB glb_erase_rcv[4]={0,0,0,0};
uInt8_PUB glb_erase_rcv_page00[4]={0,0,0,0};

Bool_PUB glb_timer1_notice_updata = (0);


 


 



 













 
static uInt8_PUB intrrupt_memcmp( uInt8_PUB * str1, uInt8_PUB * str2, uInt8_PUB len )
{
    while( len > 0 )
    {
        if ( *str1 != *str2 )
        {
            break;
        }
        str1++;
        str2++;
        len--;
    }

    return ( len );
}












 
static void intrrupt_validate_password( void )
{
	if ( intrrupt_memcmp( &glb_sram_qsfp_lo_memory[(123)], (uInt8_PUB *)glb_passwrd, 4 ) == 0 )
    {	
        glb_passward_enable = (1);
    }
    else
    {	 
        glb_passward_enable = (0);
    }
}












 
#line 164 "code\\inttrupt.c"












 
static void intrrupt_validate_erase_fwcmd_of_page( uInt8_PUB *pbuf)
{
	if ( intrrupt_memcmp( pbuf, (uInt8_PUB *)glb_erase, 4 ) == 0 )
    {	
        glb_erase_fw_enable = (1);
    }
    else
    {	 
        glb_erase_fw_enable = (0);
    }
}












 
void FIQ_Handler (void) __irq
#line 280 "code\\inttrupt.c"
{
	volatile uInt32_PUB FIQSTATUS = 0;
	uInt16_PUB  I2CSSTATUS = 0;
	static uInt8_PUB mem_addr = 0x00;
	static uInt8_PUB upmem_page = 0x00;
	volatile uInt8_PUB i2c0_rcv_data = 0;
	volatile uInt8_PUB spe_index = 0;

	I2CSSTATUS = (*(volatile unsigned long *) 0xFFFF082C);
	FIQSTATUS = (*(volatile unsigned long *) 0xFFFF0100);
	if(misc_get_modsel_status()==(0))
		goto JUMP_NO_SELECT;

	 
	if ((I2CSSTATUS & 0x04) == 0x04) 
	{
		if(((*(volatile unsigned long *) 0xFFFF084C)&0x3)!=0x3)
		{
			if(mem_addr<=(127))
			{
				if((mem_addr>=119)&&(mem_addr<=(126)))
					(*(volatile unsigned long *) 0xFFFF0834) =  0xFF; 
				else
					(*(volatile unsigned long *) 0xFFFF0834) = glb_sram_qsfp_lo_memory[mem_addr];
			}
			else
			{
				upmem_page = glb_sram_qsfp_lo_memory[127];
				if(upmem_page<(4))
				{
					if((upmem_page==PUBCOM_UPPAGE_00)&&(mem_addr==255))
						(*(volatile unsigned long *) 0xFFFF0834) = 0xFF;
					else
						(*(volatile unsigned long *) 0xFFFF0834) = glb_sram_qsfp_up_memory[upmem_page][mem_addr-(128)];
				}
				else if((upmem_page==PUBCOM_UPPAGE_99)&&(glb_passward_enable==(1)))
				{
					if(mem_addr>=((128)+64))
					{
						spe_index = mem_addr -((128)+64);
						(*(volatile unsigned long *) 0xFFFF0834) = glb_sram_map_gn1160[E_I2CBG_TX2][spe_index];
					}
					else
					{
						spe_index = mem_addr -((128)+0);
						(*(volatile unsigned long *) 0xFFFF0834) = glb_sram_map_gn1160[E_I2CBG_TX1][spe_index];
					}
				}
				else if((upmem_page==PUBCOM_UPPAGE_100)&&(glb_passward_enable==(1)))
				{
					if(mem_addr>=((128)+64))
					{
						spe_index = mem_addr -((128)+64);
						(*(volatile unsigned long *) 0xFFFF0834) = glb_sram_map_gn1160[E_I2CBG_TX4][spe_index];
					}
					else
					{
						spe_index = mem_addr -((128)+0);
						(*(volatile unsigned long *) 0xFFFF0834) = glb_sram_map_gn1160[E_I2CBG_TX3][spe_index];
					}
				}
				else
				{
					(*(volatile unsigned long *) 0xFFFF0834) = 0xFF; 
				}
			}
			
			 
			if(mem_addr<255)
				mem_addr++;       
		}
	}

	 
	if ((I2CSSTATUS & 0x08) == 0x08) 
	{
		if(glb_i2c0_rcv_first == (1)) 
		{                          
			mem_addr=(*(volatile unsigned long *) 0xFFFF0830);    
			glb_i2c0_rcv_first = (0);
		}
		else
		{
			if((mem_addr<=(127)))
			{
				if((mem_addr<(86)))
				{
					if(glb_passward_enable==(1))
						glb_sram_qsfp_lo_memory[mem_addr] = (*(volatile unsigned long *) 0xFFFF0830);                
					else
						i2c0_rcv_data = (*(volatile unsigned long *) 0xFFFF0830); 
				}
				else
				{
					glb_sram_qsfp_lo_memory[mem_addr] = (*(volatile unsigned long *) 0xFFFF0830);                
				}

				 
				if(mem_addr<=119)
				{
					if(glb_passward_enable==(1))
						glb_update_lomem_flash = (1);
				}
				 
				else if(mem_addr==(126))
				{
					intrrupt_validate_password();
				}
			}
			else
			{
				upmem_page = glb_sram_qsfp_lo_memory[127];
				if(upmem_page==PUBCOM_UPPAGE_00)
				{
					if((mem_addr==255))
						i2c0_rcv_data = (*(volatile unsigned long *) 0xFFFF0830); 
					else
					{
						if(mem_addr<(250))
						{
							if(glb_passward_enable==(1))
								glb_sram_qsfp_up_memory[upmem_page][mem_addr-(128)]=(*(volatile unsigned long *) 0xFFFF0830);
							else
								i2c0_rcv_data = (*(volatile unsigned long *) 0xFFFF0830); 

							if(glb_passward_enable==(1))
								glb_update_himem_flash[upmem_page] = (1);
						}
						else
						{
							if((mem_addr>=(250))&&(mem_addr<=(253)))
								glb_erase_rcv_page00[mem_addr-(250)]=(*(volatile unsigned long *) 0xFFFF0830);
							else
								i2c0_rcv_data = (*(volatile unsigned long *) 0xFFFF0830); 
						}
					}

					 
					if((glb_passward_enable==(1))&&(mem_addr==(253)))
					{
						intrrupt_validate_erase_fwcmd_of_page(glb_erase_rcv_page00);
						misc_specail_erase_fwcmd();
					}
				}
				else if((upmem_page==PUBCOM_UPPAGE_01)||(upmem_page==PUBCOM_UPPAGE_02))
				{
					i2c0_rcv_data = (*(volatile unsigned long *) 0xFFFF0830); 
				}
				else if(upmem_page==PUBCOM_UPPAGE_03)
				{
					if(mem_addr<(226))
					{
						if(glb_passward_enable==(1))
							glb_sram_qsfp_up_memory[upmem_page][mem_addr-(128)]=(*(volatile unsigned long *) 0xFFFF0830);
						else
							i2c0_rcv_data = (*(volatile unsigned long *) 0xFFFF0830); 
					}
					else
					{
						glb_sram_qsfp_up_memory[upmem_page][mem_addr-(128)]=(*(volatile unsigned long *) 0xFFFF0830);
					}

					if(glb_passward_enable==(1))
						glb_update_himem_flash[upmem_page] = (1);
				}
				else if(upmem_page==PUBCOM_UPPAGE_99)
				{
					if((mem_addr-(128))<(sizeof(glb_sram_map_dac)))
						glb_sram_map_dac[mem_addr-(128)]=(*(volatile unsigned long *) 0xFFFF0830);
					else
						i2c0_rcv_data = (*(volatile unsigned long *) 0xFFFF0830); 
					
					if((glb_passward_enable==(1))&&(mem_addr==(143)))
					{
						glb_update_dac_enable=(1);
					}
					if((glb_passward_enable==(1))&&(mem_addr==(147)))
					{
						glb_update_reg_enable=(1);
					}
				}
				else if(upmem_page==PUBCOM_UPPAGE_100)
				{
					if((mem_addr>=(250))&&(mem_addr<=(253)))
						glb_erase_rcv[mem_addr-(250)]=(*(volatile unsigned long *) 0xFFFF0830);
					else
						i2c0_rcv_data = (*(volatile unsigned long *) 0xFFFF0830); 
					
					if((glb_passward_enable==(1))&&(mem_addr==(253)))
					{
						intrrupt_validate_erase_fwcmd_of_page(glb_erase_rcv);
						misc_specail_erase_fwcmd();
					}
				}
				else
				{
					i2c0_rcv_data = (*(volatile unsigned long *) 0xFFFF0830); 
				}
			}
						
			 
			if(mem_addr<255)
				mem_addr++;
		}
	}
	
   	  
	if ((I2CSSTATUS & 0x400) == 0x400)
	{
		(*(volatile unsigned long *) 0xFFFF084C) = 0x100;			
		(*(volatile unsigned long *) 0xFFFF084C) &= ~0x100;

		glb_i2c0_rcv_first = (1);
	}

	 
	if ((I2CSSTATUS & 0x2000) == 0x2000) 
	{
		
		

		glb_i2c0_rcv_first = (1);
	}
	return;
	
JUMP_NO_SELECT:
	i2c0_rcv_data = (*(volatile unsigned long *) 0xFFFF0830);
	i2c0_rcv_data = (*(volatile unsigned long *) 0xFFFF0830);

	return;
}













 
void IRQ_Handler(void) __irq
{
  	unsigned long IRQSTATUS = 0;
	
	IRQSTATUS = (*(volatile unsigned long *) 0xFFFF0000);	   			
	if ((IRQSTATUS & 0x8) == 0x8)	
	{
		(*(volatile unsigned long *) 0xFFFF032C) = 0xFF;				

		glb_timer1_notice_updata = (1);
	}

	if(misc_get_modsel_status()==(1))
	{
		if((*(volatile unsigned long *) 0xFFFF083C)!=(0xA0))
			(*(volatile unsigned long *) 0xFFFF083C) = (0xA0);
	}
	else
	{
		if((*(volatile unsigned long *) 0xFFFF083C)==(0xA0))
			(*(volatile unsigned long *) 0xFFFF083C) = (0xAC);
	}

	glb_sys_tick++;
}
