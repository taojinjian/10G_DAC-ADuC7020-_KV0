#line 1 "code\\main.c"












 



 
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



#line 19 "code\\main.c"



 


 




 








 



 





 
extern Bool_PUB glb_timer1_notice_updata ;




 


 



 













 

int main(void)
{
	init_module();

	while(1)
	{
		 
		
		
		 
		

		 
		misc_register_wr();

		 
		misc_update_dac();
		
		 
		flash_update_qsfp_table();

		 
		if(glb_timer1_notice_updata==(1))
		{
			misc_monitor();
			misc_diag();
			misc_lookup_table();
		}
	}
}
