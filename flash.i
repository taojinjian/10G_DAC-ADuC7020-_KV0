#line 1 "code\\flash.c"












 



 
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



#line 19 "code\\flash.c"



 


 




 








 



 
uInt8_PUB glb_sram_qsfp_lo_memory[(128)];
uInt8_PUB glb_sram_qsfp_up_memory[(4)][(128)];

const uInt8_PUB glb_code_gn1160_regvalue_memory[(E_I2CBG_TX_MAX)][(64)]=
{

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
#line 100 "code\\flash.c"
		







 
};

const uInt8_PUB glb_code_qsfp_lo_memory[(128)]=
{
  13,     0,     0,  255,   15,     0,     0,     0,     0,   85,   85,   85,   85,   85,   85,     0,
    0,     0,     0,     0,     0,     0,   37,  167,     0,     0,  133,   91,     0,     0,     0,     0,
    0,     0,     0,     8,     0,     1,     0,     1,     0,   10,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     1,     0,     1,     0,     1,     0,     1,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0

};

const uInt8_PUB glb_code_qsfp_up_memory[(4)][(128)]=
{
	 
	{
		13,  196,     7,     2,     0,     0,     2,   16,   16,     1,  128,     3,  103,     0,     2,     0,
		0,     0,     0,   64,   73,   78,   78,   79,   76,   73,   71,   72,   84,   32,   32,   32,
		32,   32,   32,   32,     7,   68,  124,  127,   84,   82,   45,   81,   81,   49,   51,   84,
		45,   78,   48,   48,  118,   49,   46,   49,   49,   65,  101,  144,   23,  112,   70,   61,
		0,     0,     0,  222,   73,   78,   69,   66,   71,   48,   56,   52,   48,   53,   50,   55,
		32,   32,   32,   32,   49,   52,   48,   56,   49,   52,   32,   32,     8,     0,     0,  167,
		17,   57,   33,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,
		255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255

	},
	 
	{
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
	},
	 
	{
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
		0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
	},
	 
	{
		  80,     0,  246,     0,   75,     0,  251,     0,  255,  255,  255,  255,  255,  255,  255,  255,
		 144,  136,  113,  112,  140,  112,  117,   72,  255,  255,  255,  255,  255,  255,  255,  255,
		 255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,
		  98,   31,     2,   50,   87,  115,     2,  196,  146,  124,   19,  136,  136,  184,   29,   76,
		  98,   31,   15,  141,   87,  115,   17,  115,  255,  255,  255,  255,  255,  255,  255,  255,
		 255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,
		 255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,     0,     0,
		    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0

    }
};	

const uInt8_PUB glb_flash_qsfp_up_memory[(4)][(128)*4] __attribute__((at((0x80000+(0xe000))))) =
{
	 
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



Bool_PUB glb_update_lomem_flash=(0);
Bool_PUB glb_update_himem_flash[(4)];


Bool_PUB glb_passward_enable = (0);
Bool_PUB glb_erase_fw_enable = (0);
Bool_PUB glb_update_dac_enable = (0);
Bool_PUB glb_update_reg_enable = (0);



 
 extern uInt8_PUB glb_i2c0_rcv_first;




 


 



 














 
Bool_PUB flash_read(unsigned short int addr,unsigned char* value)
{
    uInt32_PUB cnt = 3000;
    
    (*(volatile unsigned long *) 0xFFFFF810) = addr;              
    (*(volatile unsigned long *) 0xFFFFF808) = 0x01;              
    
    
    while((cnt>1) && (!((*(volatile unsigned long *) 0xFFFFF800) & 0x01))) {cnt--;}
    
    *value=((*(volatile unsigned long *) 0xFFFFF80C) & 0xFF);     

	if(cnt==0)
		return (0);
	else 
		return (1);	
}














 
Bool_PUB flash_write(unsigned short int addr, unsigned char data){

    uInt32_PUB cnt = 3000;

    (*(volatile unsigned long *) 0xFFFFF810) = addr;              
    (*(volatile unsigned long *) 0xFFFFF80C) = data;              
    (*(volatile unsigned long *) 0xFFFFF808) = 0x02;              
    

    while((cnt>1) && (!((*(volatile unsigned long *) 0xFFFFF800) & 0x01))) {cnt--;}

	if(cnt==0)
		return (0);
	else 
		return (1);
    
}














 
Bool_PUB flash_erase(unsigned short int addr)
{

	uInt8_PUB trytime= 3;
	
	uInt32_PUB cnt = 3000;

	trytime = 4;
	JUMP_CONTINUE0:
	trytime--;
	if(trytime==0)
		return (0);
	
	(*(volatile unsigned long *) 0xFFFFF810) = addr;              
	(*(volatile unsigned long *) 0xFFFFF808) = 0x05;              
	

	cnt = 3000;
	while((cnt>1) && (!((*(volatile unsigned long *) 0xFFFFF800) & 0x01))) {cnt--;}

	if(cnt==0)
		goto JUMP_CONTINUE0;
	else 
		return (1);

}













 
void init_flash_with_qsfp_default_table_code(void)
{
	uInt16_PUB i =0;
	uInt16_PUB j =0;

	 
	for(i=0;i<(128);i++)
		glb_sram_qsfp_lo_memory[i]=glb_code_qsfp_lo_memory[i];

	 
	for(j=0;j<(4);j++)
	{
		for(i=0;i<(128);i++)
			glb_sram_qsfp_up_memory[j][i]=glb_code_qsfp_up_memory[j][i];
	}
}












 
void init_flash_for_qsfp_table(void)
{
	Bool_PUB rc =  (0);

	uInt8_PUB trytime=5;
	uInt8_PUB value=0;

	int i=0;
	int j=0;

	trytime = 3;
	JUMP_CONTINUE0:
	trytime--;
	if(trytime==0)
		return;
	rc = flash_read((((0xe000)+(512))+127*4),&value);
	if(rc==(0))
		goto JUMP_CONTINUE0;

	if(value!=(0x99))
	{    
		init_flash_with_qsfp_default_table_code();        

		trytime = 3;
		JUMP_CONTINUE1:
		trytime--;
		if(trytime==0)
			return;
		 	    
		flash_erase((0xe000));    
		for(i=0; i<(128); i++){
			rc = flash_write((0xe000)+(i*4), glb_sram_qsfp_lo_memory[i]);
			if(rc==(0))
				goto JUMP_CONTINUE1;
		}

		trytime=3;
		JUMP_CONTINUE2:
		trytime--;
		if(trytime==0)
		return;
		 
		glb_sram_qsfp_up_memory[PUBCOM_UPPAGE_00][127]=(0x99); 
		for(j=0;j<(4);j++)
		{
			flash_erase(((0xe000)+(512))+(j*(512)));
			for(i=0; i<(128); i++)
			{
				flash_write(((0xe000)+(512))+(j*(512))+(i*4), glb_sram_qsfp_up_memory[j][i]);
				if(rc==(0))
					goto JUMP_CONTINUE2;
			}
		}
	}
}














 
void init_sram_for_qsfp_table(void)
{
	Bool_PUB rc = (0);
	uInt8_PUB value=0;
	uInt8_PUB trytime=5;

	uInt32_PUB i=0;
	uInt32_PUB j=0;

	 
	glb_update_lomem_flash = (0);
	for(j=0; j<(4);j++)
	{
		glb_update_himem_flash[j] = (0);
	}

	 
	for(i=0; i<(128); i++)
	{
		trytime = 3;
		JUMP_CONTINUE0:
		trytime--;
		if(trytime==0)
			return;
		rc =flash_read((0xe000)+(i*4),&value);
		if(rc==(0))
			goto JUMP_CONTINUE0;
		glb_sram_qsfp_lo_memory[i]=(uInt8_PUB)(value);
	}

	 
	for(j=0; j<(4);j++)
	{
		for(i=0; i<(128); i++)
		{
			trytime = 3;
			JUMP_CONTINUE1:
			trytime--;
			if(trytime==0)
				return;
			rc =flash_read(((0xe000)+(512))+(j*(512))+(i*4),&value);
			if(rc==(0))
				goto JUMP_CONTINUE1;
			glb_sram_qsfp_up_memory[j][i]=(uInt8_PUB)value;
		}
	}
}














 
void flash_update_qsfp_table(void)
{
	Bool_PUB rc = (0);
	int i=0;
	int j=0;

	if(glb_passward_enable==(1))
	{
		if((glb_update_lomem_flash == (1))&&(glb_i2c0_rcv_first==(1)))
		{
			 	    
			rc = flash_erase((0xe000)); 
			if(rc==(0))
				return;
			
			for(i=0; i<(128); i++)
			{
				rc = flash_write((0xe000)+(i*4), glb_sram_qsfp_lo_memory[i]);
				if(rc==(0))
					return;
			}
			glb_update_lomem_flash = (0);
		}
	    
		for(j=0; j<(4);j++)
		{
			if((glb_update_himem_flash[j] == (1))&&(glb_i2c0_rcv_first==(1)))
			{
				 
		            rc = flash_erase(((0xe000)+(512))+(j*(512)));
				if(rc==(0))
					return;
									
		            for(i=0; i<(128); i++)
		            {
		                	rc = flash_write(((0xe000)+(512))+(j*(512))+(i*4), glb_sram_qsfp_up_memory[j][i]);
					if(rc==(0))
						return;
		            }

				glb_update_himem_flash[j] = (0);
			}
		}
	}
	else
	{
		glb_update_lomem_flash = (0);
		for(j=0; j<(4);j++)
		{
			glb_update_himem_flash[j] = (0);
		}
	}
}












 
void init_gn1160_with_flash(void)
{
	int ch = 0;
	int i=0;
	uInt8_PUB buf[(64)]={0};
	
	for(ch=0;ch<(E_I2CBG_TX_MAX);ch++)
	{
		for(i=0;i<(64);i++)
		{
			buf[i] = glb_code_gn1160_regvalue_memory[ch][i];
		}
		i2cbg_write_nbytes((E_I2CBG_TXn)ch,buf,(64),0);
	}

}

