#line 1 "code\\misc.c"












 



 
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



#line 19 "code\\misc.c"



 


 




 








 



 
uInt8_PUB glb_sram_map_gn1160[(E_I2CBG_TX_MAX)][(64)];
Bool_PUB glb_sram_map_gn1160_status[(E_I2CBG_TX_MAX)];

const uInt8_PUB glb_flash_map_gn1160[(E_I2CBG_TX_MAX)][(64)];
uInt8_PUB glb_sram_map_dac[32];



 
extern uInt8_PUB glb_sram_qsfp_lo_memory[(128)];
extern const uInt8_PUB glb_code_qsfp_up_memory[(4)][(128)];
extern uInt8_PUB glb_sram_qsfp_up_memory[(4)][(128)];
extern uInt32_PUB glb_sys_tick;
extern Bool_PUB glb_erase_fw_enable;
extern Bool_PUB glb_passward_enable;
extern Bool_PUB glb_update_dac_enable ;
extern Bool_PUB glb_update_reg_enable;



 


 



 















 
Bool_PUB misc_get_lpmode_status(void)
{
	return ((*(volatile unsigned long *) 0xFFFFF430) &(0x01)) == 0 ? (1) : (0);
}












 
Bool_PUB misc_get_modsel_status(void)
{
	return ((*(volatile unsigned long *) 0xFFFFF430) &(0x02)) == 0 ? (1) : (0);
}












 
void  misc_set_intl_status(Bool_PUB intterupt)
{
	if(intterupt == (1))
		(*(volatile unsigned long *) 0xFFFFF420) &=(~0x00400000); 
	else
		(*(volatile unsigned long *) 0xFFFFF420) |=(0x00400000);
}












 
static uInt16_PUB misc_memcp( uInt8_PUB * dst, uInt8_PUB * src, uInt16_PUB len )
{
    while( len > 0 )
    {
	*dst=* src;
        dst++;
        src++;
        len--;
    }

    return ( len );
}












 
void misc_test_slave_i2c(uInt8_PUB channel)
{
	static uInt32_PUB tick = 0;

	if(((*(volatile unsigned long *) 0xFFFFF420)&0x02000000)!=0x02000000)
		(*(volatile unsigned long *) 0xFFFFF420) |= 0x02000000; 

	if(((*(volatile unsigned long *) 0xFFFFF420)&0x04000000)!=0x04000000)
		(*(volatile unsigned long *) 0xFFFFF420) |= 0x04000000; 

	if(((*(volatile unsigned long *) 0xFFFFF420)&0x08000000)!=0x08000000)
		(*(volatile unsigned long *) 0xFFFFF420) |= 0x08000000; 
	
	
	
	
	
	

	if(((*(volatile unsigned long *) 0xFFFFF420)&0x00020000)==0x02000000)
		(*(volatile unsigned long *) 0xFFFFF420) &= (~0x00020000);	 
	if(((*(volatile unsigned long *) 0xFFFFF420)&0x00040000)==0x04000000)
		(*(volatile unsigned long *) 0xFFFFF420) &= (~0x00040000);	 
	if(((*(volatile unsigned long *) 0xFFFFF420)&0x00080000)==0x08000000)
		(*(volatile unsigned long *) 0xFFFFF420) &= (~0x000880000);	 
	
	
	
	if(glb_sys_tick>(tick+10))
	{
		tick = glb_sys_tick;

		
		
		
			
	}
}












 
void misc_update_sram_map_gn1160_register(uInt8_PUB channel)
{
	uInt8_PUB ch = 0;
	uInt8_PUB buf[(64)]={0};

	
	
	for(ch=0;ch<(E_I2CBG_TX_MAX);ch++)
	{
#line 239 "code\\misc.c"
		
		
		
		if((1)== i2cbg_read_nbytes((E_I2CBG_TXn)ch,buf,sizeof(buf),0x00))
		{
			if((buf[1]&0xC0)==0xC0)
			{
				misc_memcp(&glb_sram_map_gn1160[ch][0],buf,sizeof(buf));
				glb_sram_map_gn1160_status[ch]=(1);
				
			}
		}
		









 

	}
}













 
Bool_PUB misc_adc(uInt8_PUB adc_ch,uInt16_PUB *adc_value)
{
	uInt8_PUB cnt;
	uInt16_PUB adcread[4];
	volatile uInt16_PUB timeout =0xfff;
		
	(*(volatile unsigned long *) 0xFFFF0504) = adc_ch;

	for(cnt=0; cnt<4; cnt++)
	{
		(*(volatile unsigned long *) 0xFFFF0500) = 0xA3;              
		while(!(*(volatile unsigned long *) 0xFFFF050C)) 
		{
			if(timeout--)
			 	continue;
			else 
				return (0);
		}            
		adcread[cnt]=((*(volatile unsigned long *) 0xFFFF0510)>>16);
	}

	 
	if(adcread[3] > 4085) adcread[3] = 0;     
	*adc_value = adcread[3];
	return (1);
}












 
Bool_PUB glb_temperatur_first_sample = (0);

void misc_monitor_temperatur(uInt8_PUB channel)
{
#line 397 "code\\misc.c"
	uInt8_PUB ch =0;
	uInt16_PUB temperatur_raw_adc=0;
	uInt16_PUB temperatur = 0;
	float temperatur_f = 0;
	
	for(ch=0;ch<(E_I2CBG_TX_MAX);ch++)
	{
		if(glb_sram_map_gn1160_status[ch]==(1))
		{
			temperatur_raw_adc = (glb_sram_map_gn1160[ch][0x08]<<8)|(glb_sram_map_gn1160[ch][0x09]<<0);

			




 
			if(temperatur_raw_adc&0x8000)
				temperatur_raw_adc &=(~0x8000);
			else
				temperatur_raw_adc |=(0x8000);

			temperatur_f = temperatur_raw_adc;
			temperatur_f = (temperatur_f * 1.8/65535);
			
			temperatur_f = (temperatur_f-760)/3-40;
			temperatur_f = temperatur_f*256;

			temperatur = (uInt16_PUB)temperatur_f;
			glb_sram_qsfp_lo_memory[22] = (uInt8_PUB)((temperatur>>8)& 0x00FF);	 
			glb_sram_qsfp_lo_memory[23] = (uInt8_PUB)(temperatur & 0x00FF);    	 
		}
	}

}
#line 488 "code\\misc.c"












 
void misc_monitor_voltage(uInt8_PUB channel)
{
	uInt8_PUB ch =0;

	uInt16_PUB voltage_raw_adc=0;
	uInt16_PUB voltage =0;
	float voltage_float = 0.00;
		
	for(ch=0;ch<(E_I2CBG_TX_MAX);ch++)
	{
		if(glb_sram_map_gn1160_status[ch]==(1))
		{
			voltage_raw_adc = (glb_sram_map_gn1160[ch][0x0A]<<8)|(glb_sram_map_gn1160[ch][0x0B]<<0);

			

 
			voltage_float = voltage_raw_adc*1.8/65535;
			voltage_float = 4.0 * voltage_float;
			voltage_float = (1000*voltage_float)/0.1;
			voltage = (uInt16_PUB)voltage_float;
			
			glb_sram_qsfp_lo_memory[26] = (uInt8_PUB)((voltage>>8)& 0x00FF); 	 
			glb_sram_qsfp_lo_memory[27] = (uInt8_PUB)(voltage & 0x00FF);    	 
		}
	}
}












 
void misc_monitor_txbias(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	uInt16_PUB txbias_raw_adc=0;
	uInt16_PUB txbias = 0;
	float txbias_f = 0;
	
	for(ch=0;ch<(E_I2CBG_TX_MAX);ch++)
	{
		if(glb_sram_map_gn1160_status[ch]==(1))
		{
			txbias_raw_adc = (glb_sram_map_gn1160[ch][0x0c]<<8)|(glb_sram_map_gn1160[ch][0x0d]<<0);

			




 
			
			txbias_f = (txbias_raw_adc * 1.8/65535);
			txbias_f = txbias_f*320.8/4700;
			txbias_f = txbias_f*1000;
			txbias_f = txbias_f/2;

			txbias = (uInt16_PUB)txbias_f;
			glb_sram_qsfp_lo_memory[42+ch*2] = (uInt8_PUB)((txbias>>8)& 0x00FF);	 
			glb_sram_qsfp_lo_memory[43+ch*2] = (uInt8_PUB)(txbias & 0x00FF);    	 
		}
	}
}












 
void misc_monitor_txmod(uInt8_PUB channel)
{
	uInt8_PUB i =0;
	for(i=0;i<(E_I2CBG_TX_MAX);i++)
	{
		if(glb_sram_map_gn1160_status[i]==(1))
		{


		}
	}
}












 
void misc_monitor_txpow(uInt8_PUB channel)
{
	uInt8_PUB i =0;
	for(i=0;i<(E_I2CBG_TX_MAX);i++)
	{
		if(glb_sram_map_gn1160_status[i]==(1))
		{


		}
	}
}












 
Bool_PUB glb_first_sample_rxpow[(E_I2CBG_TX_MAX)] = {(0),(0),(0),(0)};
void misc_monitor_rxpow(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	
	uInt16_PUB pow_res;
	uInt16_PUB sample_value[4]; 
	uInt8_PUB cnt;
	static uInt16_PUB rxpow_value_for_average[(E_I2CBG_TX_MAX)][8]={{0}};
	static uInt8_PUB rxpow_index_for_average[(E_I2CBG_TX_MAX)]={0};
	float  data_f=0;
	const uInt8_PUB adc_channel_map[(E_I2CBG_TX_MAX)]={0,1,2,3};
	volatile uInt16_PUB timeout =0xfff;

	
	for(ch=0;ch<(E_I2CBG_TX_MAX);ch++)
	{
		 
		(*(volatile unsigned long *) 0xFFFF0504) = adc_channel_map[ch];

		 
		for(cnt=0; cnt<4; cnt++)
		{
			 
			(*(volatile unsigned long *) 0xFFFF0500)=0xA3;    

			 
			while(!(*(volatile unsigned long *) 0xFFFF050C))
			{
				if(timeout--)
				 	continue;
				else 
					return ;
			}           
			sample_value[cnt]=((*(volatile unsigned long *) 0xFFFF0510)>>16);
		}

		


 
		 
		if(sample_value[3] > 4085) 
			sample_value[3] = 0;    

		 
		rxpow_value_for_average[ch][rxpow_index_for_average[ch]] = sample_value[3];
		(rxpow_index_for_average[ch])++;

		if(glb_first_sample_rxpow[ch] == (0))
		{
			for(cnt=0; cnt<8; cnt++)
			{
				rxpow_value_for_average[ch][cnt]=sample_value[3];
			}
			glb_first_sample_rxpow[ch] = (1);
		}
	
		  
		rxpow_index_for_average[ch] &= 0x07;                 
		for(cnt=0; cnt<8; cnt++)
		{
			data_f += rxpow_value_for_average[ch][cnt];
		}
		data_f = data_f/8;                   

		 


		 
		glb_sram_qsfp_lo_memory[34+ch*2] = (uInt8_PUB)(pow_res>>8);              
		glb_sram_qsfp_lo_memory[35+ch*2] = (uInt8_PUB)(pow_res & 0x00FF); 
	}
}












 

void misc_monitor(void)
{
	uInt8_PUB ch = 0;

	
	{
		misc_update_sram_map_gn1160_register(ch);
		misc_monitor_temperatur(ch);
		misc_monitor_voltage(ch);
		misc_monitor_txbias(ch);
		misc_monitor_rxpow(ch);

		
		
	}
}












 
void misc_diag_temperatur(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	uInt8_PUB offset_addr = 6;

	uInt8_PUB msb = 0;
	uInt8_PUB lsb = 0;
	uInt8_PUB addr = 128;

	uInt8_PUB result;
	uInt16_PUB monitor = 0;

	uInt16_PUB high_alarm =	(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+1];
	uInt16_PUB low_alarm =		(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+2]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+3];
	uInt16_PUB high_warning =	(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+4]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+5];
	uInt16_PUB low_warning =	(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+6]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+7];
			
	for(ch=0;ch<1;ch++)
	{
		msb = glb_sram_qsfp_lo_memory[22];
		lsb = glb_sram_qsfp_lo_memory[23];
		monitor = (msb<<8) + lsb;

		if (monitor>high_alarm) result=5;
		else if (monitor>high_warning && monitor<=high_alarm) result=4;
		else if (monitor<=high_warning && monitor>=low_warning) result=3;
		else if (monitor<low_warning && monitor>= low_alarm) result=2;
		else result=1;

		offset_addr = 6;
		
		switch (result)
		{
			case 5:
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x20);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x10);	 
			break;
			case 4:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] |= 0x20;		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x10);	 
			break;
			case 3:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x20);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x10);	 
			break;
			case 2:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x20);;		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x10);	 
			break;
			case 1:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x20);;		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x10);	 
			break;
		}
	}
}












 
void misc_diag_voltage(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	uInt8_PUB offset_addr = 7;
	uInt8_PUB msb = 0;
	uInt8_PUB lsb = 0;
	uInt8_PUB addr = 144;

	uInt8_PUB result;
	uInt16_PUB monitor = 0;

	uInt16_PUB high_alarm =	(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+1];
	uInt16_PUB low_alarm =		(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+2]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+3];
	uInt16_PUB high_warning =	(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+4]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+5];
	uInt16_PUB low_warning =	(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+6]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+7];
			
	for(ch=0;ch<1;ch++)
	{
		msb = glb_sram_qsfp_lo_memory[26];
		lsb = glb_sram_qsfp_lo_memory[27];
		monitor = (msb<<8) + lsb;

		if (monitor>high_alarm) result=5;
		else if (monitor>high_warning && monitor<=high_alarm) result=4;
		else if (monitor<=high_warning && monitor>=low_warning) result=3;
		else if (monitor<low_warning && monitor>= low_alarm) result=2;
		else result=1;

		offset_addr = 7;
		
		switch (result)
		{
			case 5:
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x20);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x10);	 
			break;
			case 4:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] |= 0x20;		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x10);	 
			break;
			case 3:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x20);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x10);	 
			break;
			case 2:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x20);;		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x10);	 
			break;
			case 1:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x20);;		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x10);	 
			break;
		}
	}
}












 
void misc_diag_txbias(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	uInt8_PUB offset_addr = 7;
	uInt8_PUB msb = 0;
	uInt8_PUB lsb = 0;
	uInt8_PUB addr = 184;

	uInt8_PUB result;
	uInt16_PUB monitor = 0;

	uInt16_PUB high_alarm =	(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+1];
	uInt16_PUB low_alarm =		(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+2]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+3];
	uInt16_PUB high_warning =	(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+4]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+5];
	uInt16_PUB low_warning =	(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+6]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+7];
			
	for(ch=0;ch<(E_I2CBG_TX_MAX);ch++)
	{
		msb = glb_sram_qsfp_lo_memory[42+ch*2];
		lsb = glb_sram_qsfp_lo_memory[43+ch*2];
		monitor= (msb<<8) + lsb;
		
		if((ch==0)||(ch==1))
			offset_addr = 11;
		else
			offset_addr = 12;
			

		if (monitor>high_alarm) result=5;
		else if (monitor>high_warning && monitor<=high_alarm) result=4;
		else if (monitor<=high_warning && monitor>=low_warning) result=3;
		else if (monitor<low_warning && monitor>= low_alarm) result=2;
		else result=1;

		if((ch==0)||(ch==2))
		{
			switch (result)
			{
				case 5:
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x20);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x10);	 
				break;
				case 4:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] |= 0x20;		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x10);	 
				break;
				case 3:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x20);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x10);	 
				break;
				case 2:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x20);;		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x10);	 
				break;
				case 1:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x20);;		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x10);	 
				break;
			}
		}
		else
		{
			switch (result)
			{
				case 5:
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x8);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x4);	 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x2);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x1);	 
				break;
				case 4:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x8);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x4);	 
				glb_sram_qsfp_lo_memory[offset_addr] |= 0x2;		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x1);	 
				break;
				case 3:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x8);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x4);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x2);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x1);	 
				break;
				case 2:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x8);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x4);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x2);;		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x1);	 
				break;
				case 1:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x8);		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x4);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x2);;		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x1);	 
				break;
			}

		}
	}
}












 
void misc_diag_txmod(uInt8_PUB channel)
{
	uInt8_PUB i =0;
	for(i=0;i<(E_I2CBG_TX_MAX);i++)
	{
		if(glb_sram_map_gn1160_status[i]==(1))
		{


		}
	}
}












 
void misc_diag_txpow(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	uInt8_PUB offset_addr = 7;
	uInt8_PUB msb = 0;
	uInt8_PUB lsb = 0;
	uInt8_PUB addr = 176;

	uInt8_PUB result;
	uInt16_PUB monitor = 0;

	uInt16_PUB high_alarm =	(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+1];
	uInt16_PUB low_alarm =		(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+2]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+3];
	uInt16_PUB high_warning =	(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+4]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+5];
	uInt16_PUB low_warning =	(glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+6]<<8) + glb_code_qsfp_up_memory[PUBCOM_UPPAGE_03][addr+7];
			
	for(ch=0;ch<(E_I2CBG_TX_MAX);ch++)
	{
		msb = glb_sram_qsfp_lo_memory[34+ch*2];
		lsb = glb_sram_qsfp_lo_memory[35+ch*2];
		monitor= (msb<<8) + lsb;
		
		if((ch==0)||(ch==1))
			offset_addr = 9;
		else
			offset_addr = 10;
			

		if (monitor>high_alarm) result=5;
		else if (monitor>high_warning && monitor<=high_alarm) result=4;
		else if (monitor<=high_warning && monitor>=low_warning) result=3;
		else if (monitor<low_warning && monitor>= low_alarm) result=2;
		else result=1;

		if((ch==0)||(ch==2))
		{
			switch (result)
			{
				case 5:
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x20);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x10);	 
				break;
				case 4:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] |= 0x20;		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x10);	 
				break;
				case 3:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x20);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x10);	 
				break;
				case 2:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x20);;		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x10);	 
				break;
				case 1:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x80);		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x40);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x20);;		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x10);	 
				break;
			}
		}
		else
		{
			switch (result)
			{
				case 5:
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x8);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x4);	 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x2);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x1);	 
				break;
				case 4:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x8);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x4);	 
				glb_sram_qsfp_lo_memory[offset_addr] |= 0x2;		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x1);	 
				break;
				case 3:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x8);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x4);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x2);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x1);	 
				break;
				case 2:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x8);		 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x4);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x2);;		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x1);	 
				break;
				case 1:
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x8);		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x4);	 
				glb_sram_qsfp_lo_memory[offset_addr] &= (~0x2);;		 
				glb_sram_qsfp_lo_memory[offset_addr] |= (0x1);	 
				break;
			}

		}
	}
}












 
void misc_diag_rxpow(uInt8_PUB channel)
{
	uInt8_PUB i =0;
	for(i=0;i<(E_I2CBG_TX_MAX);i++)
	{
		if(glb_sram_map_gn1160_status[i]==(1))
		{


		}
	}
}












 
void misc_specail_for_jatg(void)
{
	if(misc_get_lpmode_status()==(0))
	{
		i2cbg_specail_for_jtag();
		while(misc_get_lpmode_status()==(0))
		{

		}
	}
}












 
void misc_specail_erase_fwcmd(void)
{
	if(glb_erase_fw_enable==(1))
	{
		(*(volatile unsigned long *) 0xFFFF010C) = 0xffffffff;     
		(*(volatile unsigned long *) 0xFFFF000C) = 0xffffffff;     

		
		flash_erase(0x0000); 

		glb_passward_enable=(0);
		while((1))flash_erase(0x0000); 
	}
}












 
void misc_diag(void)
{
	uInt8_PUB ch = 0;

	
	{
		misc_diag_temperatur(ch);
		misc_diag_voltage(ch);
		misc_diag_txbias(ch);
		
		
		misc_diag_rxpow(ch);
	}
}












 


const uInt16_PUB glb_lktb_mod[(E_I2CBG_TX_MAX)][(4)][2]=
{
	 
	 
	{
		{0x1b20+0x8000,0x7e5},
		{0x175a+0x8000,0x834},
		{0xcd2+0x8000,0x76c},
		{0x3ce+OFFSET_16BIT_RES,0x640},
	},
	 
	{
		{0x1b20+0x8000,0x7e5},
		{0x175a+0x8000,0x834},
		{0xcd2+0x8000,0x76c},
		{0x3ce+OFFSET_16BIT_RES,0x640},
	},
	 
	{
		{0x1b20+0x8000,0x7e5},
		{0x175a+0x8000,0x834},
		{0xcd2+0x8000,0x76c},
		{0x3ce+OFFSET_16BIT_RES,0x640},
	},
	 
	{
		{0x1b20+0x8000,0x7e5},
		{0x175a+0x8000,0x834},
		{0xcd2+0x8000,0x76c},
		{0x3ce+OFFSET_16BIT_RES,0x640},
	},
};
const uInt16_PUB glb_lktb_bias[(E_I2CBG_TX_MAX)][(4)][2]=
{
	 
	 
	{
		{0x1b20+0x8000,0x578},
		{0x175a+0x8000,0x4b0},
		{0xcd2+0x8000,0x3e8},
		{0x3ce+OFFSET_16BIT_RES,0x300},
	},
	 
	{
		{0x1b20+0x8000,0x578},
		{0x175a+0x8000,0x4b0},
		{0xcd2+0x8000,0x3e8},
		{0x3ce+OFFSET_16BIT_RES,0x300},
	},
	 
	{
		{0x1b20+0x8000,0x578},
		{0x175a+0x8000,0x4b0},
		{0xcd2+0x8000,0x3e8},
		{0x3ce+OFFSET_16BIT_RES,0x300},
	},
	 
	{
		{0x1b20+0x8000,0x578},
		{0x175a+0x8000,0x4b0},
		{0xcd2+0x8000,0x3e8},
		{0x3ce+OFFSET_16BIT_RES,0x300},
	},
};
const uInt16_PUB glb_lktb_jitter[(E_I2CBG_TX_MAX)][(4)][2]=
{
	 
	 
	{
		{0x1b20+0x8000,0x28},
		{0x175a+0x8000,0x28},
		{0xcd2+0x8000,0x28},
		{0x3ce+OFFSET_16BIT_RES,0x46},
	},
	 
	{
		{0x1b20+0x8000,0x28},
		{0x175a+0x8000,0x28},
		{0xcd2+0x8000,0x28},
		{0x3ce+OFFSET_16BIT_RES,0x46},
	},
	 
	{
		{0x1b20+0x8000,0x28},
		{0x175a+0x8000,0x28},
		{0xcd2+0x8000,0x28},
		{0x3ce+OFFSET_16BIT_RES,0x46},
	},
	 
	{
		{0x1b20+0x8000,0x28},
		{0x175a+0x8000,0x28},
		{0xcd2+0x8000,0x28},
		{0x3ce+OFFSET_16BIT_RES,0x46},
	},
};
void misc_lookup_table(void)
{
	uInt8_PUB ch =0;
	uInt8_PUB i_bias =0;
	uInt8_PUB i_mod =0;
	uInt8_PUB i_jit =0;
	volatile uInt8_PUB i_max =0;
	
	volatile uInt8_PUB buf_dac[2];
	volatile uInt16_PUB module_temperature = 0;
	volatile uInt16_PUB value_dac = 0;
		
	 
	

	for(ch =0;ch <E_I2CBG_TX_MAX;ch ++)
	{
		module_temperature = (glb_sram_map_gn1160[ch][0x08]<<8)|(glb_sram_map_gn1160[ch][0x09]<<0);
		if(module_temperature&0x8000)
			module_temperature &=(~0x8000);
		else
			module_temperature |=(0x8000);
			
		 
		i_max = (4);
		for(i_bias=0;i_bias<i_max;i_bias++)
		{
			if(i_bias>=(i_max-1))
			{
				break;
			}
			else if(module_temperature<glb_lktb_bias[ch][i_bias+1][0])
			{
				break;
			}
		}

		 
		i_max = (4);
		for(i_mod=0;i_mod<i_max;i_mod++)
		{
			if(i_mod>=(i_max-1))
			{
				break;
			}
			else if(module_temperature< glb_lktb_mod[ch][i_mod+1][0])
			{
				break;
			}
		}
		
		 
		i_max = (4);
		for(i_jit=0;i_jit<i_max;i_jit++)
		{
			if(i_jit>=(i_max-1))
			{
				break;
			}
			else if(module_temperature<glb_lktb_jitter[ch][i_jit+1][0])
			{
				break;
			}
		}		
		
		 
		value_dac = glb_lktb_bias[ch][i_bias][1];
		buf_dac[0] = (value_dac>>8)&0xFF;
		buf_dac[1] = (value_dac>>0)&0xFF;		
		i2cbg_write_nbytes((E_I2CBG_TXn)ch,(uInt8_PUB *)buf_dac,sizeof(buf_dac),0x1D);

		 
		value_dac = glb_lktb_mod[ch][i_mod][1];
		buf_dac[0] = (value_dac>>8)&0xFF;
		buf_dac[1] = (value_dac>>0)&0xFF;	
		i2cbg_write_nbytes((E_I2CBG_TXn)ch,(uInt8_PUB *)buf_dac,sizeof(buf_dac),0x1F);

		 
		value_dac = glb_lktb_jitter[ch][i_mod][1];
		buf_dac[0] = (value_dac>>0)&0xFF;
		i2cbg_write_nbytes((E_I2CBG_TXn)ch,(uInt8_PUB *)buf_dac,1,0x19);
	}
}
#line 1545 "code\\misc.c"












 
void misc_update_dac(void)
{
	uInt8_PUB ch =0;
	
	volatile uInt8_PUB buf_dac[2];

	Bool_PUB rc = (0);
	
	if(glb_passward_enable==(1))
	{
		if(glb_update_dac_enable==(1))
		{
			for(ch=0;ch<(E_I2CBG_TX_MAX);ch++)
			{
				 
				buf_dac[0] = glb_sram_map_dac[4*ch+0]&0x0f;
				buf_dac[1] = glb_sram_map_dac[4*ch+1]&0xff;		
				rc=i2cbg_write_nbytes((E_I2CBG_TXn)ch,(uInt8_PUB *)buf_dac,sizeof(buf_dac),0x1D);
				if(rc == (0))
					return;
				
				 
				buf_dac[0] = glb_sram_map_dac[4*ch+2]&0x0f;
				buf_dac[1] = glb_sram_map_dac[4*ch+3]&0xff;	
				rc=i2cbg_write_nbytes((E_I2CBG_TXn)ch,(uInt8_PUB *)buf_dac,sizeof(buf_dac),0x1F);
				if(rc == (0))
					return;
			}
			glb_update_dac_enable = (0);
		}
	}
}












 
void misc_register_wr(void)
{
	uInt8_PUB ch =0;
	uInt8_PUB opeate_mode = 1;
	uInt8_PUB reg_addr = 0;
	uInt8_PUB reg_value= 0;

	Bool_PUB rc = (0);
	
	if(glb_passward_enable==(1))
	{
		if(glb_update_reg_enable==(1))
		{
			ch=glb_sram_map_dac[(144)-128];
			opeate_mode=glb_sram_map_dac[(144)-128+1];
			reg_addr=glb_sram_map_dac[(144)-128+2];
			reg_value=glb_sram_map_dac[(144)-128+3];

			if((ch<(E_I2CBG_TX_MAX))&&(opeate_mode==1))
			{
				rc=i2cbg_write_nbytes((E_I2CBG_TXn)ch,&reg_value,1,reg_addr);
				if(rc == (0))
					return;
			}

			glb_update_reg_enable = (0);
		}
	}
}

