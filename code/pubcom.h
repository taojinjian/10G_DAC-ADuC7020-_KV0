/***********************************************************
** Hani Co.Ltd CopyRight
**
**File: pubcom.h
**
**Des: 1-type def
**
**Author: Hani , created in 29/12/2014 Rev1.0
**
**Rev: xxxx, Modifed in ..................... Rev1.x
**
**
***********************************************************/
#ifndef _PUB_COM_H_
#define _PUB_COM_H_
    
#include "pubtype.h"
//#include <ADuC7023.h>
#include <ADuC7020.H>

#define SW_INT                 1  
#define T0_INT                 2  
#define T1_INT                 3  
#define T2_INT                 4  
#define FLASH_INT              5  
#define ADC_INT                6 
#define PLL_INT                7 
#define I2C0M_INT              8 
#define I2C0S_INT              9 
#define I2C1M_INT              10 
#define I2C1S_INT              11 
#define SPI_INT                12 
#define XIRQ0_INT              13 
#define COMP_INT               14 
#define PSM_INT                15
#define XIRQ1_INT              16 
#define PLA0_INT               17 
#define XIRQ2_INT              18 
#define XIRQ3_INT              19 
#define PLA1_INT               20 
#define PWM_INT                21

// Bit Definitions
#define BIT0  0x01
#define BIT1  0x02
#define BIT2  0x04
#define BIT3  0x08
#define BIT4  0x10
#define BIT5  0x20
#define BIT6  0x40
#define BIT7  0x80
#define BIT8  0x100
#define BIT9  0x200
#define BIT10 0x400
#define BIT11 0x800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000
#define BIT16 0x10000

#define DEBUG 0                     // 0 to 2  
enum _PAGE_INDEX
{
    PUBCOM_UPPAGE_00,
    PUBCOM_UPPAGE_01,
    PUBCOM_UPPAGE_02,
    PUBCOM_UPPAGE_03,
    PUBCOM_UPPAGE_99=99,
     PUBCOM_UPPAGE_100=100,
};
#define PUBCOM_MAX_UPPER_PAGE		(4)
#define PUBCOM_TABLE_LOMEM_PAGE_RW_START			(86)
#define PUBCOM_TABLE_LOMEM_PAGE_MAX_ADDR			(127)
#define PUBCOM_TABLE_PAGE_SIZE			(128)
#define PUBCOM_FLASH_PAGE_SIZE			(512)
#define PUBCOM_FUN_TABLE_LOMEM_BASE     (0x8000)
//#define PUBCOM_FUN_TABLE_LOMEM_BASE     (0xe000)
#define PUBCOM_FUN_TABLE_A2LOWMEM_BASE     (PUBCOM_FUN_TABLE_LOMEM_BASE+(PUBCOM_FLASH_PAGE_SIZE*2))
#define PUBCOM_FUN_TABLE_UPMEM__PAGE00_BASE     (PUBCOM_FUN_TABLE_A2LOWMEM_BASE+PUBCOM_FLASH_PAGE_SIZE)
#define PUBCOM_FUN_TABLE_UPMEM__PAGE01_BASE     (PUBCOM_FUN_TABLE_UPMEM__PAGE00_BASE+PUBCOM_FLASH_PAGE_SIZE)
#define PUBCOM_FUN_TABLE_UPMEM__PAGE02_BASE     (PUBCOM_FUN_TABLE_UPMEM__PAGE01_BASE+PUBCOM_FLASH_PAGE_SIZE)
#define PUBCOM_FUN_TABLE_UPMEM__PAGE03_BASE     (PUBCOM_FUN_TABLE_UPMEM__PAGE02_BASE+PUBCOM_FLASH_PAGE_SIZE)
#define PUBCOM_FUN_TABLE_VALID_STATUS_BASE     	(PUBCOM_FUN_TABLE_UPMEM__PAGE03_BASE+127)
#define PUBCOM_FUN_TABLE_VALID_STATUS_VALUE     	(0x99)
#define PUBCOM_FLASH_TABLE_LOMEM_BASE     		(0x80000+PUBCOM_FUN_TABLE_LOMEM_BASE)

#define DDTABLE_PAGE_OFFSET_PWD_START		119		/*!< offset into A2 page - start of user EEPROM */

#define PUBCOM_PAGE_OFFSET_PWD_START		(123)
#define PUBCOM_PAGE_OFFSET_PWD_END			(126)

#define PUBCOM_UPMEM_PAGE00_OFFSET_ERASE_START		(250)
#define PUBCOM_UPMEM_PAGE00_OFFSET_ERASE_END		(253)

#define PUBCOM_TABLE_UPMEM_PAGE03_RW_START			(226)

#define PUBCOM_PAGE_OFFSET_DAC_DBG_START		(128)
#define PUBCOM_PAGE_OFFSET_DAC_DBG_END		(143)
#define PUBCOM_PAGE_OFFSET_REG_WR_START		(144)
#define PUBCOM_PAGE_OFFSET_REG_WR_END		(147)

typedef enum _E_I2CBG_TX{
E_I2CBG_TX1,
E_I2CBG_TX2,
E_I2CBG_TX3,
E_I2CBG_TX4,
E_I2CBG_TX_MAX,
}E_I2CBG_TXn;

#define PUBCOM_GN1160_NUM			(E_I2CBG_TX_MAX)
#define PUBCOM_GN1160_REG_SIZE	(64)

#define PUBCOM_HIGH_MAX        4085

#define PUBCOM_DEVICE_ADDR0	(0xA0)
#define PUBCOM_DEVICE_ADDRA2	(0xA2)
#define PUBCOM_DEVICE_ADDR_INVALID	(0xAC)

#define PUBCOM_LOOKUP_TABLE_NUM		(4)
/**********************flash.c function***********************/
void init_flash_for_qsfp_table(void);
void init_sram_for_qsfp_table(void);
void flash_update_qsfp_table(void);
void init_module(void);
Bool_PUB flash_erase(unsigned short int addr);
void init_gn1160_with_flash(void);

/**********************misc.c function*********************************/
void misc_monitor(void);
void misc_diag(void);
void misc_disable_channel(void);
Bool_PUB misc_get_lpmode_status(void);
Bool_PUB misc_get_modsel_status(void);
void  misc_set_intl_status(Bool_PUB intterupt);
void misc_specail_for_jatg(void);
void misc_specail_erase_fwcmd(void);
void misc_lookup_table(void);
void misc_update_dac(void);
void misc_register_wr(void);
/***************************i2cbg.c function*********************************/
Bool_PUB i2cbg_read_nbytes(E_I2CBG_TXn tx,uInt8_PUB *pdata,uInt8_PUB lenght,uInt8_PUB reg_address);
Bool_PUB i2cbg_write_nbytes(E_I2CBG_TXn tx,uInt8_PUB *pdata,uInt8_PUB lenght,uInt8_PUB reg_address);
void i2cbg_specail_for_jtag(void);

#endif

