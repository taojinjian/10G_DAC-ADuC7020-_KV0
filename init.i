#line 1 "code\\init.c"












 



 
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



#line 19 "code\\init.c"



 


 




 








 



 





 




 


 



 













 
static void init_delay_us(uInt32_PUB us)
{
	us = us*10;
	while(us--);
}













 
void init_cpu_clock(void)
{
    
    (*(volatile unsigned long *) 0xFFFF0404) = 0x01;						
    (*(volatile unsigned long *) 0xFFFF0408) = 0x00;
    (*(volatile unsigned long *) 0xFFFF040C) = 0xF4;	

    
    (*(volatile unsigned long *) 0xFFFF0434) = 0x76;			 			
    (*(volatile unsigned long *) 0xFFFF0438) = 0x004;					
    (*(volatile unsigned long *) 0xFFFF043C) = 0xB1;
}













 
void init_i2c_slave_to_host(void)
{
    


 

	 
	(*(volatile unsigned long *) 0xFFFFF400) |= 0x00110000;				
	
	(*(volatile unsigned long *) 0xFFFF0828) = 0x01 		  			
			+ 0x200 + 0x400  			
			+ 0x100;						
	(*(volatile unsigned long *) 0xFFFF083C)  = (0xA0);					
	(*(volatile unsigned long *) 0xFFFF084C) = 0x100;					
	(*(volatile unsigned long *) 0xFFFF084C) &= ~0x100;

	
	

        
        
        


     
    
}













 
void init_i2c_for_operate_gn1160(void)
{
    





 
}













 
void init_adc_for_rx_monitoer(void)
{
    




 
        
	 
	(*(volatile unsigned long *) 0xFFFF048C) = 0x01;                      
    
	 
	(*(volatile unsigned long *) 0xFFFF0500) = 0x20;                      
}












 
void init_qsfp_interface_io_for_control_and_status(void)
{
	




 

	 
	(*(volatile unsigned long *) 0xFFFFF42C) |=0x0;
	(*(volatile unsigned long *) 0xFFFFF420) |=0x40000000;
	(*(volatile unsigned long *) 0xFFFFF420) &=(~0x00400000);
	(*(volatile unsigned long *) 0xFFFFF420) =(0x00400000);
	
	 
	(*(volatile unsigned long *) 0xFFFFF42C) |=0x0;
	(*(volatile unsigned long *) 0xFFFFF420) |=0x80000000;

	 
	(*(volatile unsigned long *) 0xFFFFF43C) |=0x0;
	(*(volatile unsigned long *) 0xFFFFF430) &=(~0x01000000);

	 
	(*(volatile unsigned long *) 0xFFFFF43C) |=0x0;
	(*(volatile unsigned long *) 0xFFFFF430) &=(~0x02000000);
}













 
void init_timer(void)
{
    
    (*(volatile unsigned long *) 0xFFFF0320) = 0x40;                        
    (*(volatile unsigned long *) 0xFFFF0328) = 0xCF;                       
                                        
                                        
                                        
                                        
}












 
void init_enable_interrupt(void)
{















































 
    (*(volatile unsigned long *) 0xFFFF0108) = 0x00000200;     
    
    (*(volatile unsigned long *) 0xFFFF0008) = 0x00000008;     
}












 
void init_reset_gn1160(void)
{
	(*(volatile unsigned long *) 0xFFFFF420) &=(~0x00800000);
	init_delay_us(100);
	(*(volatile unsigned long *) 0xFFFFF420) |=(0x00800000);
}












 
void init_module()
{
	 
	(*(volatile unsigned long *) 0xFFFFF804) |= 0x108;               

	 
	init_cpu_clock();
	init_i2c_slave_to_host();
	init_i2c_for_operate_gn1160();
	init_adc_for_rx_monitoer();
	init_qsfp_interface_io_for_control_and_status();
	init_timer();
	init_reset_gn1160();

	 
	init_flash_for_qsfp_table();
	init_sram_for_qsfp_table();

	 
	init_gn1160_with_flash();
	
	 
	init_enable_interrupt();
}

