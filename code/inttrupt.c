/***********************************************************
** xxxxxx Co.Ltd CopyRight
**
**File: inttrupt.c
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
 uInt8_PUB glb_i2c0_rcv_first=FALSE_PUB;
uInt8_PUB i2c_receive, dac_update;
uInt32_PUB glb_sys_tick=0;
static uInt8_PUB rx_count = 0; // i2c 接口接收数据计数器
static uInt8_PUB i2cwrite_Byte = 0; // I2C 接口写数据缓存
static uInt8_PUB register_addr = 0; // I2C 接口读写数据的 寄存器地址,0和1数据上电初始化已经缓存
//static uInt8_PUB i2cA0read_Byte[2] = {0}; //i2c 接口读数据缓存
static uint8_t InvalidData = 0xFF;

static uint8_t I2C_Addr = 0;   //I2C寻址地址
/**
**extern
**/
extern uInt8_PUB glb_sram_qsfp_A0_memory[256];
extern uInt8_PUB glb_sram_qsfp_A2_lo_memory[PUBCOM_TABLE_PAGE_SIZE];
extern uInt8_PUB glb_sram_qsfp_A2_memory[PUBCOM_MAX_UPPER_PAGE][PUBCOM_TABLE_PAGE_SIZE];

extern uInt8_PUB glb_sram_map_gn1160[PUBCOM_GN1160_NUM][PUBCOM_GN1160_REG_SIZE];


extern Bool_PUB glb_passward_enable;
extern Bool_PUB glb_erase_fw_enable;
extern Bool_PUB glb_update_dac_enable;
extern Bool_PUB glb_update_reg_enable;

extern Bool_PUB glb_update_lomem_flash;
extern Bool_PUB glb_update_A2lomem_flash;
extern Bool_PUB glb_update_himem_flash[PUBCOM_MAX_UPPER_PAGE];

extern uInt8_PUB glb_sram_map_dac[32];


const uInt8_PUB glb_passwrd[]={'u','u','a','a'};

const uInt8_PUB glb_erase[]={'e','r','a','s'};

uInt8_PUB glb_erase_rcv[4]={0,0,0,0};
uInt8_PUB glb_erase_rcv_page00[4]={0,0,0,0};
volatile uInt8_PUB i2c0_rcv_data = 0;
Bool_PUB glb_timer1_notice_updata = FALSE_PUB;

static uInt8_PUB pRdData = 0;
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
**Name	  :   validate_password()
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
/*****************************************************************
**Name	  :   validate_password()
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
static void intrrupt_validate_password( void )
{
	if ( intrrupt_memcmp( &glb_sram_qsfp_A2_lo_memory[PUBCOM_PAGE_OFFSET_PWD_START], (uInt8_PUB *)glb_passwrd, 4 ) == 0 )
    {	
        glb_passward_enable = TRUE_PUB;
    }
    else
    {	/* invalid password - disables all password controlled access */
        glb_passward_enable = FALSE_PUB;
    }
}
/*****************************************************************
**Name	  :   validate_password()
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
#if 0
static void intrrupt_validate_erase_fwcmd( void )
{
	if ( intrrupt_memcmp( &glb_sram_qsfp_A2_memory[PUBCOM_UPPAGE_00][PUBCOM_UPMEM_PAGE00_OFFSET_ERASE_START-PUBCOM_TABLE_PAGE_SIZE], (uInt8_PUB *)glb_erase, 4 ) == 0 )
    {	
        glb_erase_fw_enable = TRUE_PUB;
    }
    else
    {	/* invalid password - disables all password controlled access */
        glb_erase_fw_enable = FALSE_PUB;
    }
}
#endif
/*****************************************************************
**Name	  :   validate_password()
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
static void intrrupt_validate_erase_fwcmd_of_page( uInt8_PUB *pbuf)
{
	if ( intrrupt_memcmp( pbuf, (uInt8_PUB *)glb_erase, 4 ) == 0 )
    {	
        glb_erase_fw_enable = TRUE_PUB;
    }
    else
    {	/* invalid password - disables all password controlled access */
        glb_erase_fw_enable = FALSE_PUB;
    }
}
/******************************************************************************
 * Function:    I2CReadByte
 * Description: I2CReadByte
 * Input:       void
 * Output:      void
 * Return:      void
 * History:     2022-08-26  |   zyj   |  Alpha Release         
 ******************************************************************************/
void I2CReadByte(void)  
{
//	i2cA0read_Byte[1] = 0;
//	pRdData = &i2cA0read_Byte;
	uint8_t upmem_page = 0;
	
	switch(I2C_Addr)
	{
		case PUBCOM_DEVICE_ADDR0:
//		if(register_addr<=PUBCOM_TABLE_LOMEM_PAGE_MAX_ADDR)
//		{
//				if((register_addr>=DDTABLE_PAGE_OFFSET_PWD_START)&&(register_addr<=PUBCOM_PAGE_OFFSET_PWD_END))
//					pRdData =  &InvalidData;/*if passward send 0xFF*/
//				else
			pRdData = glb_sram_qsfp_A0_memory[register_addr];
			break;
//		}
			
		case PUBCOM_DEVICE_ADDRA2:									
			upmem_page = glb_sram_qsfp_A2_lo_memory[127];
			if(register_addr <=127)
			{
					pRdData = glb_sram_qsfp_A2_lo_memory[register_addr];
			}		
			else
			{
				if(upmem_page < PUBCOM_MAX_UPPER_PAGE)
        {
						if((upmem_page==PUBCOM_UPPAGE_00)&&(register_addr==255))
							pRdData = InvalidData;
						else
							pRdData = glb_sram_qsfp_A2_memory[upmem_page][register_addr-PUBCOM_TABLE_PAGE_SIZE];
				}
				else
				{
						pRdData =  InvalidData;/*send 0xFF*/
				}
			}

			break;
	//		else if((upmem_page==PUBCOM_UPPAGE_99)&&(glb_passward_enable==TRUE_PUB))
	//		{
	//			if(mem_addr>=(PUBCOM_TABLE_PAGE_SIZE+64))
	//			{
	//				spe_index = mem_addr -(PUBCOM_TABLE_PAGE_SIZE+64);
	//				I2C0STX = glb_sram_map_gn1160[E_I2CBG_TX2][spe_index];
	//			}
	//			else
	//			{
	//				spe_index = mem_addr -(PUBCOM_TABLE_PAGE_SIZE+0);
	//				I2C0STX = glb_sram_map_gn1160[E_I2CBG_TX1][spe_index];
	//			}
	//		}
	//		else if((upmem_page==PUBCOM_UPPAGE_100)&&(glb_passward_enable==TRUE_PUB))
	//		{
	//			if(mem_addr>=(PUBCOM_TABLE_PAGE_SIZE+64))
	//			{
	//				spe_index = mem_addr -(PUBCOM_TABLE_PAGE_SIZE+64);
	//				I2C0STX = glb_sram_map_gn1160[E_I2CBG_TX4][spe_index];
	//			}
	//			else
	//			{
	//				spe_index = mem_addr -(PUBCOM_TABLE_PAGE_SIZE+0);
	//				I2C0STX = glb_sram_map_gn1160[E_I2CBG_TX3][spe_index];
	//			}
	//		}			
		default:break;
	}
	
}

/******************************************************************************
 * Function:    I2CWriteByte
 * Description: I2CWriteByte
 * Input:       void
 * Output:      void
 * Return:      void
 * History:     2022-08-26  |    zyj   |  Alpha Release         
 ******************************************************************************/
void I2CWriteByte(void) 
{
	uint8_t upmem_page = 0;
	
	switch(I2C_Addr)
	{
		case PUBCOM_DEVICE_ADDR0:
//			if((register_addr<=PUBCOM_TABLE_LOMEM_PAGE_MAX_ADDR))
//				{
						if(glb_passward_enable==TRUE_PUB)
							glb_sram_qsfp_A0_memory[register_addr] = I2C0SRX;               /*get byte data*/
						else
							i2c0_rcv_data = I2C0SRX;/*empty read for flash specail status*/

						/*updata*/
						if(glb_passward_enable==TRUE_PUB)
							glb_update_lomem_flash = TRUE_PUB;
//				}	
		break;
		case PUBCOM_DEVICE_ADDRA2:
		if((register_addr<=PUBCOM_TABLE_LOMEM_PAGE_MAX_ADDR))
				{
					if((register_addr<PUBCOM_TABLE_LOMEM_PAGE_RW_START))
					{
						if(glb_passward_enable==TRUE_PUB)
							glb_sram_qsfp_A2_lo_memory[register_addr] = I2C0SRX;               /*get byte data*/
						else
							i2c0_rcv_data = I2C0SRX;/*empty read for flash specail status*/
					}
					else
					{
						glb_sram_qsfp_A2_lo_memory[register_addr] = I2C0SRX;               /*get byte data*/
					}

					/*updata*/
					if(register_addr<=DDTABLE_PAGE_OFFSET_PWD_START)
					{
						if(glb_passward_enable==TRUE_PUB)
							glb_update_A2lomem_flash = TRUE_PUB;
					}
					/*for password check*/
					else if(register_addr==PUBCOM_PAGE_OFFSET_PWD_END)
					{
						intrrupt_validate_password();
					}
				}
				else
				{						
					upmem_page = glb_sram_qsfp_A2_lo_memory[127];
					if(upmem_page==PUBCOM_UPPAGE_00)
					{
						if((register_addr == 255))
							i2c0_rcv_data = I2C0SRX;/*empty read for flash specail status 0X99*/
						else
						{
							if(register_addr<PUBCOM_UPMEM_PAGE00_OFFSET_ERASE_START)
							{
								if(glb_passward_enable==TRUE_PUB)
									glb_sram_qsfp_A2_memory[upmem_page][register_addr-PUBCOM_TABLE_PAGE_SIZE]=I2C0SRX;
								else
									i2c0_rcv_data = I2C0SRX;/*empty read for flash specail status*/

								if(glb_passward_enable==TRUE_PUB)
									glb_update_himem_flash[upmem_page] = TRUE_PUB;
							}
							else
							{
								if((register_addr>=PUBCOM_UPMEM_PAGE00_OFFSET_ERASE_START)&&(register_addr<=PUBCOM_UPMEM_PAGE00_OFFSET_ERASE_END))
									glb_erase_rcv_page00[register_addr-PUBCOM_UPMEM_PAGE00_OFFSET_ERASE_START]=I2C0SRX;
								else
									i2c0_rcv_data = I2C0SRX;/*empty read for flash specail status*/
							}
						}

						/*judge the erase cmd*/
						if((glb_passward_enable==TRUE_PUB)&&(register_addr==PUBCOM_UPMEM_PAGE00_OFFSET_ERASE_END))
						{
							intrrupt_validate_erase_fwcmd_of_page(glb_erase_rcv_page00);
							misc_specail_erase_fwcmd();
						}
					}
					else if((upmem_page==PUBCOM_UPPAGE_01)||(upmem_page==PUBCOM_UPPAGE_02))
					{
						i2c0_rcv_data = I2C0SRX;/*empty read*/
					}
					else if(upmem_page==PUBCOM_UPPAGE_03)
					{
						if(register_addr<PUBCOM_TABLE_UPMEM_PAGE03_RW_START)
						{
							if(glb_passward_enable==TRUE_PUB)
								glb_sram_qsfp_A2_memory[upmem_page][register_addr-PUBCOM_TABLE_PAGE_SIZE]=I2C0SRX;
							else
								i2c0_rcv_data = I2C0SRX;/*empty read for flash specail status*/
						}
						else
						{
							glb_sram_qsfp_A2_memory[upmem_page][register_addr-PUBCOM_TABLE_PAGE_SIZE]=I2C0SRX;
						}

						if(glb_passward_enable==TRUE_PUB)
							glb_update_himem_flash[upmem_page] = TRUE_PUB;
					}
	//				else if(upmem_page==PUBCOM_UPPAGE_99)
	//				{
	//					if((register_addr-PUBCOM_TABLE_PAGE_SIZE)<(sizeof(glb_sram_map_dac)))
	//						glb_sram_map_dac[register_addr-PUBCOM_TABLE_PAGE_SIZE]=I2C0SRX;
	//					else
	//						i2c0_rcv_data = I2C0SRX;/*empty read for flash specail status*/
	//					
	//					if((glb_passward_enable==TRUE_PUB)&&(register_addr==PUBCOM_PAGE_OFFSET_DAC_DBG_END))
	//					{
	//						glb_update_dac_enable=TRUE_PUB;
	//					}
	//					if((glb_passward_enable==TRUE_PUB)&&(register_addr==PUBCOM_PAGE_OFFSET_REG_WR_END))
	//					{
	//						glb_update_reg_enable=TRUE_PUB;
	//					}
	//				}
	//				else if(upmem_page==PUBCOM_UPPAGE_100)
	//				{
	//					if((register_addr>=PUBCOM_UPMEM_PAGE00_OFFSET_ERASE_START)&&(register_addr<=PUBCOM_UPMEM_PAGE00_OFFSET_ERASE_END))
	//						glb_erase_rcv[register_addr-PUBCOM_UPMEM_PAGE00_OFFSET_ERASE_START]=I2C0SRX;
	//					else
	//						i2c0_rcv_data = I2C0SRX;/*empty read for flash specail status*/
	//					
	//					if((glb_passward_enable==TRUE_PUB)&&(register_addr==PUBCOM_UPMEM_PAGE00_OFFSET_ERASE_END))
	//					{
	//						intrrupt_validate_erase_fwcmd_of_page(glb_erase_rcv);
	//						misc_specail_erase_fwcmd();
	//					}
	//				}
					else
					{
						i2c0_rcv_data = I2C0SRX;/*empty read*/
					}
		break;
					
		default:break;
				}
		}
}

/*****************************************************************
**Name	  :   FIQ_Handler()是
**
**Function:
**1
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
void FIQ_Handler (void) __irq
#if 0
{
    static uInt8_PUB mem_addr = 0x00;
    static uInt8_PUB upmem_page = 0x00;
    uInt8_PUB i2c0_rcv_data = 0;
    Int32_PUB status = I2C0SSTA; 
     
    /***** Slave TX *****/
    if((status & 0x0004) == 0x0004){                    // slave transmitter interrupt?
        I2C0FSTA |= 0x100;                              // flush TX FIFO
        if((status&0x1800)==0){                         // ID decode bit is 00?
       	    I2CSTX = A0h[byte_addr0];                   // set TX data
       	    byte_addr0++;                               // increment byte address
       	    I2CSTX = A0h[byte_addr0];                   // set TX data
        }
        else{
       	    I2CSTX = A2h[byte_addr2];                   // set TX data
       	    byte_addr2++;                               // increment byte address
       	    I2CSTX = A2h[byte_addr2];                   // set TX data
        }
        I2CCFG = 0x4001;                                // clear bit9, enable Slave Tx FIFO request interrupt
    }        

    /***** Slave RX *****/
    if((status & BIT3) == BIT3)/*slave receiver intterupt*/
    {                    
        if(glb_i2c0_rcv_first == TRUE_PUB)/*first slave-receiver INT?*/
        {                          
            if((status&0x1800)==0) 
                mem_addr=I2C0SRX;   /*get mem offset address*/

            glb_i2c0_rcv_first = FALSE_PUB;
        }
        else
        {
            if((status&0x1800)==0)
            {
                if(mem_addr<=PUBCOM_TABLE_LOMEM_PAGE_MAX_ADDR)
                {
                    glb_sram_qsfp_A0_memory[mem_addr] = I2C0SRX;               /*get byte data*/
                    mem_addr++;                           /*increment address*/
                }
                else
                {
                    upmem_page = glb_sram_qsfp_A0_memory[127];
                    if(upmem_page<PUBCOM_MAX_UPPER_PAGE)
                    {
                        glb_sram_qsfp_A2_memory[upmem_page][mem_addr]=I2C0SRX;
                    }
                    else
                    {
                           i2c0_rcv_data = I2C0SRX;/*empty read*/
                    }
                }
            }
        }
    }

    /***** Stop Condition *****/
    if((status & BIT10) == BIT10)/*STOP detect*/
    {                   
        I2C0FSTA |= BIT8;   /*flush TX FIFO*/
        glb_i2c0_rcv_first = TRUE_PUB;

        /*enable stop condition BIT8 & slave enable BIT0*/
        if(I2C0SCON == (BIT8|BIT0))
        {
            /*if equl the device address A0*/
            if((status&0x1800)==0)
                mem_addr--;       /*decrement byte address*/
            
            I2C0SCON = 0x4201;/*enable stop condition BIT8 & slave enable BIT0, disable the salve tx fifo request interrupt */ 
        }
    }

    return;
}
#else
{
	volatile uInt32_PUB FIQSTATUS = 0;
	uInt16_PUB  I2CSSTATUS = 0;
	static uInt8_PUB mem_addr = 0x00;
//	static uInt8_PUB upmem_page = 0x00;
	
	volatile uInt8_PUB spe_index = 0;

	glb_passward_enable = TRUE_PUB;
	
	I2CSSTATUS = I2C0SSTA;
	FIQSTATUS = FIQSTA;
//	if(misc_get_modsel_status()==FALSE_PUB)
//		goto JUMP_NO_SELECT;
	if((I2CSSTATUS & (BIT11 | BIT12)) == 0)  //Received Address Matched ID Register 0
	{					
		I2C_Addr = PUBCOM_DEVICE_ADDR0;	
	}
	else if((I2CSSTATUS & (BIT11 | BIT12)) == BIT11) //Received Address Matched ID Register 1
	{
		I2C_Addr = PUBCOM_DEVICE_ADDRA2;
	}
	
	
	
/*If I2C Slave Rx IRQ*/
	if ((I2CSSTATUS & BIT3) == BIT3) 
	{
		i2cwrite_Byte = I2C0SRX;   /*get mem offset address*/
		if(rx_count == 0)/*first slave-receiver INT?*/
		{                          
			register_addr = i2cwrite_Byte;//I2cRx(pADI_I2C0,SLAVE);
			//glb_i2c0_rcv_first = FALSE_PUB;
			rx_count = 0x01;
			//I2C0STX
			/*flush the tx fifo*/
			I2C0FSTA = BIT8;			// Flush Slave Tx FIFO
			I2C0FSTA &= ~BIT8;
			
			I2CBusy = 1;
			I2CReadByte();

			I2C0STX = pRdData;
			
		}
		else
		{
				I2CWriteByte();
			/*increment address*/			
			if(register_addr == 127)
				register_addr = 0;
			else if(register_addr == 255)
				register_addr = 128;
			else
				register_addr++;					
		}												
	}
	
	/*If I2C Slave Tx IRQ*/
	if((I2CSSTATUS & BIT2) == BIT2) 
	{
		/*flush the tx fifo*/
//		I2C0FSTA = BIT8;			// Flush Slave Tx FIFO
//		I2C0FSTA &= ~BIT8;
		rx_count = 2;
		if(register_addr == 127)
			register_addr = 0;
		else if(register_addr == 255)
			register_addr = 128;
		else
			register_addr++;	
		
		I2CBusy = 1;
		
		I2CReadByte();
		
		I2C0STX = pRdData;	
//		if(mem_addr<=PUBCOM_TABLE_LOMEM_PAGE_MAX_ADDR)
//		{
//			if((mem_addr>=DDTABLE_PAGE_OFFSET_PWD_START)&&(mem_addr<=PUBCOM_PAGE_OFFSET_PWD_END))
//				I2C0STX =  0xFF;/*if passward send 0xFF*/
//			else
//				I2C0STX = glb_sram_qsfp_A0_memory[mem_addr];
//		}
//		else
//		{
//			upmem_page = glb_sram_qsfp_A0_memory[127];
//			if(upmem_page<PUBCOM_MAX_UPPER_PAGE)
//			{
//				if((upmem_page==PUBCOM_UPPAGE_00)&&(mem_addr==255))
//					I2C0STX = 0xFF;
//				else
//					I2C0STX = glb_sram_qsfp_A2_memory[upmem_page][mem_addr-PUBCOM_TABLE_PAGE_SIZE];
//			}
////			else if((upmem_page==PUBCOM_UPPAGE_99)&&(glb_passward_enable==TRUE_PUB))
////			{
////				if(mem_addr>=(PUBCOM_TABLE_PAGE_SIZE+64))
////				{
////					spe_index = mem_addr -(PUBCOM_TABLE_PAGE_SIZE+64);
////					I2C0STX = glb_sram_map_gn1160[E_I2CBG_TX2][spe_index];
////				}
////				else
////				{
////					spe_index = mem_addr -(PUBCOM_TABLE_PAGE_SIZE+0);
////					I2C0STX = glb_sram_map_gn1160[E_I2CBG_TX1][spe_index];
////				}
////			}
////			else if((upmem_page==PUBCOM_UPPAGE_100)&&(glb_passward_enable==TRUE_PUB))
////			{
////				if(mem_addr>=(PUBCOM_TABLE_PAGE_SIZE+64))
////				{
////					spe_index = mem_addr -(PUBCOM_TABLE_PAGE_SIZE+64);
////					I2C0STX = glb_sram_map_gn1160[E_I2CBG_TX4][spe_index];
////				}
////				else
////				{
////					spe_index = mem_addr -(PUBCOM_TABLE_PAGE_SIZE+0);
////					I2C0STX = glb_sram_map_gn1160[E_I2CBG_TX3][spe_index];
////				}
////			}
//			else
//			{
//				I2C0STX = 0xFF;/*send 0xFF*/
//			}		
//	  }      
	}
	
	 	 /*If I2C Stop after Start condition detected IRQ*/
	if ((I2CSSTATUS & BIT10) == BIT10)
	{
		if(rx_count == 1)//the current Register_data after write
		{
			I2C0FSTA = BIT8;			// Flush Slave Tx FIFO
			I2C0FSTA &= ~BIT8;//BIT8 SFLUSH Flush the Slave Transmit FIFO
			I2CReadByte();
			I2C0STX = pRdData;

		}
		
		if(rx_count == 2)
		{
			//Diag_INTL();
		}
        I2CBusy = 0;
        rx_count = 0;
		//glb_i2c0_rcv_first = TRUE_PUB;
	}

	/*If I2C Repeated Start detected IRQ*/
	if ((I2CSSTATUS & BIT13) == BIT13) 
	{
		//I2C0FSTA = BIT8;			// Flush Slave Tx FIFO
		//I2C0FSTA &= ~BIT8;

		//glb_i2c0_rcv_first = TRUE_PUB;
	}
	
//	I2C0SSTA = I2CSSTATUS;
//	IRQCLR = 0x00000200;
	return;
	
//JUMP_NO_SELECT:
//	i2c0_rcv_data = I2C0SRX;
//	i2c0_rcv_data = I2C0SRX;

//	return;
}


#endif
/*****************************************************************
**Name	  :   IRQ_Handler()
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
void IRQ_Handler(void) __irq
{
  	unsigned long IRQSTATUS = 0;
	
	IRQSTATUS = IRQSTA;	   			// Read off IRQSTA register
	if ((IRQSTATUS & 0x8) == 0x8)	// Timer1 interrupt source
	{
		T1CLRI = 0xFF;				// Clear current timer Interrupt

		glb_timer1_notice_updata = TRUE_PUB;

		glb_sys_tick++;
	}
	
//	if ((IRQSTATUS & (1UL << XIRQ1_INT)) == (1UL << XIRQ1_INT))	// XIRQ1 interrupt source
//	{
//   		IRQCLR = (0x1UL << XIRQ1_INT);

//		if((IRQCONE&0x0C)==0x0C)
//			IRQCONE |= 0x08;        // XIRQ1 up edge triggered
//		else
//			IRQCONE |= 0x0C;        // XIRQ1 low edge triggered
//	}

	if(misc_get_modsel_status()==TRUE_PUB)
	{
		if(I2C0ID0!=PUBCOM_DEVICE_ADDR0)
			I2C0ID0 = PUBCOM_DEVICE_ADDR0;
	}
	else
	{
		if(I2C0ID0==PUBCOM_DEVICE_ADDR0)
			I2C0ID0 = PUBCOM_DEVICE_ADDR_INVALID;
	}
}
