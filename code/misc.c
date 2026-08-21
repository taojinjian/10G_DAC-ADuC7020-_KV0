/***********************************************************
** xxxxxx Co.Ltd CopyRight
**
**File: misc.c
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
uInt8_PUB glb_sram_map_gn1160[PUBCOM_GN1160_NUM][PUBCOM_GN1160_REG_SIZE];
Bool_PUB glb_sram_map_gn1160_status[PUBCOM_GN1160_NUM];

const uInt8_PUB glb_flash_map_gn1160[PUBCOM_GN1160_NUM][PUBCOM_GN1160_REG_SIZE];
uInt8_PUB glb_sram_map_dac[32];

/**
**extern
**/
extern uInt8_PUB glb_sram_qsfp_A0_memory[PUBCOM_TABLE_PAGE_SIZE];
extern uInt8_PUB glb_sram_qsfp_A2_lo_memory[PUBCOM_TABLE_PAGE_SIZE];
extern const uInt8_PUB glb_code_qsfp_A2_up_memory[PUBCOM_MAX_UPPER_PAGE][PUBCOM_TABLE_PAGE_SIZE];
extern uInt8_PUB glb_sram_qsfp_A2_memory[PUBCOM_MAX_UPPER_PAGE][PUBCOM_TABLE_PAGE_SIZE];
extern uInt32_PUB glb_sys_tick;
extern Bool_PUB glb_erase_fw_enable;
extern Bool_PUB glb_passward_enable;
extern Bool_PUB glb_update_dac_enable ;
extern Bool_PUB glb_update_reg_enable;

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
**Name	  :   misc_get_lpmode_status()
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
/* WARNING: P1.0 is configured as I2C SCL by GP1CON in init_i2c_slave_to_host().
   GP1DAT bit0 therefore returns the SCL bus level, NOT the LPMode input.
   Do not use this result until LPMode is wired to a free GPIO and configured. */
Bool_PUB misc_get_lpmode_status(void)
{
	return (GP1DAT &(0x01)) == 0 ? TRUE_PUB : FALSE_PUB;
}
/*****************************************************************
**Name	  :   misc_get_modsel_status()
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
/* WARNING: P1.1 is configured as I2C SDA by GP1CON in init_i2c_slave_to_host().
   GP1DAT bit1 therefore returns the SDA bus level, NOT the ModSel input.
   This used to gate I2C0ID0 in IRQ_Handler() and permanently disabled address
   0xA0; that call site has been removed. Do not use this result until ModSel is
   wired to a free GPIO and configured. */
Bool_PUB misc_get_modsel_status(void)
{
	return (GP1DAT &(0x02)) == 0 ? TRUE_PUB : FALSE_PUB;
}
/*****************************************************************
**Name	  :   misc_set_intl_status()
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
void  misc_set_intl_status(Bool_PUB intterupt)
{
	if(intterupt == TRUE_PUB)
		GP0DAT &=(~0x00400000); 
	else
		GP0DAT |=(0x00400000);
}
/*****************************************************************
**Name	  :   misc_memcmp()
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
/*****************************************************************
**Name	  :   misc_test_i2c()
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
void misc_test_slave_i2c(uInt8_PUB channel)
{
	static uInt32_PUB tick = 0;

	if((GP0DAT&0x02000000)!=0x02000000)
		GP0DAT |= 0x02000000;/*P0.1 as an output*/

	if((GP0DAT&0x04000000)!=0x04000000)
		GP0DAT |= 0x04000000;/*P0.2 as an output*/

	if((GP0DAT&0x08000000)!=0x08000000)
		GP0DAT |= 0x08000000;/*P0.3 as an output*/
	
	//GP0DAT &= (~0x00020000);	/*set P0.1=0*/
	//GP0DAT |= 0x00020000;	/*set P0.1=1*/
	//GP0SET |= 0x00020000;	/*set P0.1=1*/
	//if(GP0PAR!=0x22223333)
	//	GP0PAR =0x22223333;

	if((GP0DAT&0x00020000)==0x02000000)
		GP0DAT &= (~0x00020000);	/*set P0.1=0*/
	if((GP0DAT&0x00040000)==0x04000000)
		GP0DAT &= (~0x00040000);	/*set P0.2=0*/
	if((GP0DAT&0x00080000)==0x08000000)
		GP0DAT &= (~0x000880000);	/*set P0.3=0*/
	//GP0CLR |= 0x00040000;	/*set P0.2=0*/
	//GP0CLR |= 0x00080000;	/*set P0.3=0*/
	
	if(glb_sys_tick>(tick+10))
	{
		tick = glb_sys_tick;

		//if(GP0DAT&0x00020000)
		//	GP0DAT &= (~0x00020000);	/*set P0.1=0*/
		//else
			//GP0DAT |= 0x00020000;	/*set P0.1=1*/
	}
}
/*****************************************************************
**Name	  :   misc_update_sram_map_gn1160_register()
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
void misc_update_sram_map_gn1160_register(uInt8_PUB channel)
{
	uInt8_PUB ch = 0;
	uInt8_PUB buf[PUBCOM_GN1160_REG_SIZE]={0};

	//misc_test_slave_i2c(0);
	
	for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
	{
#if 0
		if(TRUE_PUB== i2cbg_read_nbytes((E_I2CBG_TXn)ch,buf,sizeof(buf),0x00))
		{
			if((buf[1]&0xC0)==0xC0)
			{
				misc_memcp(&glb_sram_map_gn1160[ch][0],buf,sizeof(buf));
				glb_sram_map_gn1160_status[ch]=TRUE_PUB;
				misc_memcp(&glb_sram_qsfp_A2_memory[PUBCOM_UPPAGE_02][0],buf,sizeof(buf));
			}
		}

#else
		//if(ch!=E_I2CBG_TX2)
		//	continue;
		
		if(TRUE_PUB== i2cbg_read_nbytes((E_I2CBG_TXn)ch,buf,sizeof(buf),0x00))
		{
			if((buf[1]&0xC0)==0xC0)
			{
				misc_memcp(&glb_sram_map_gn1160[ch][0],buf,sizeof(buf));
				glb_sram_map_gn1160_status[ch]=TRUE_PUB;
				//misc_memcp(&glb_sram_qsfp_A2_memory[PUBCOM_UPPAGE_02][0],buf,sizeof(buf));
			}
		}
		/*
		buf[0x32] = 0xaa;
		buf[0x33] = 0x88;
		buf[0x34] = 0x77;
		buf[0x35] = 0x66;
		if(TRUE_PUB== i2cbg_write_nbytes((E_I2CBG_TXn)ch,buf,sizeof(buf),0x00))
		{
			buf[0x32] = 0;
			buf[0x33] = 0;
		}
		*/
#endif
	}
}

/*****************************************************************
**Name	  :   misc_adc()
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
Bool_PUB misc_adc(uInt8_PUB adc_ch,uInt16_PUB *adc_value)
{
	uInt8_PUB cnt;
	uInt16_PUB adcread[4];
	volatile uInt16_PUB timeout =0xfff;
		
	ADCCP = adc_ch;

	for(cnt=0; cnt<4; cnt++)
	{
		ADCCON = 0xA3;              // software start, single-end input
		while(!ADCSTA)/*wait for end of conversion*/
		{
			if(timeout--)
			 	continue;
			else 
				return FALSE_PUB;
		}            
		adcread[cnt]=(ADCDAT>>16);
	}

	/*ADC bug fix*/
	if(adcread[3] > PUBCOM_HIGH_MAX) adcread[3] = 0;    /*limits a highest*/
	*adc_value = adcread[3];
	return TRUE_PUB;
}
/*****************************************************************
**Name	  :   misc_monitor_chip_temperatur()
**
**Function:  read ADuC7020 internal temperature sensor, write to A2[96]/[97]
**
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
void misc_monitor_chip_temperatur(void)
{
	uInt16_PUB raw = 0;
	Int16_PUB  val = 0;
	float t = 0;

	if (misc_adc(0x10, &raw) != TRUE_PUB)   /* 0x10 = internal temperature sensor */
		return;

	/* ADuC7020 datasheet: T[°C] = (0x525 - raw) / 1.3, raw = 12-bit ADC result.
	   Negative temperature coefficient (~-1.3mV/°C), 0x525 = 1317 = 0°C reference. */
	t = (float)(0x525 - (Int32_PUB)raw) / 1.3f;

	/* SFF-8472: signed 16-bit two's complement, LSB = 1/256 °C */
	val = (Int16_PUB)(t * 256.0f);

	glb_sram_qsfp_A2_lo_memory[96] = (uInt8_PUB)(((uInt16_PUB)val >> 8) & 0x00FF); /* MSB */
	glb_sram_qsfp_A2_lo_memory[97] = (uInt8_PUB)((uInt16_PUB)val & 0x00FF);        /* LSB */
}
/*****************************************************************
**Name	  :   misc_monitor_chip_voltage()
**
**Function:  read ADuC7020 AVDD/2 channel, write to A2[98]/[99]
**
**Params :
**
**Return  :
**
**Input    :
**
**Output :
*****************************************************************/
void misc_monitor_chip_voltage(void)
{
	uInt16_PUB raw = 0;
	uInt16_PUB val = 0;
	float v = 0;

	if (misc_adc(0x13, &raw) != TRUE_PUB)   /* 0x13 = AVDD/2 self-diagnostic channel */
		return;

	/* VREF = 2.5V internal, 12-bit. AVDD = 2 * (raw * 2.5/4096).
	   SFF-8472 Vcc: 16-bit unsigned, LSB = 0.1 mV (100 uV). */
	v   = raw * 2.5f / 4096.0f;       /* AVDD/2, volts */
	v   = v * 2.0f;                   /* AVDD, volts */
	val = (uInt16_PUB)(v * 10000.0f); /* volts -> 0.1 mV LSB */

	glb_sram_qsfp_A2_lo_memory[98] = (uInt8_PUB)((val >> 8) & 0x00FF); /* MSB */
	glb_sram_qsfp_A2_lo_memory[99] = (uInt8_PUB)(val & 0x00FF);        /* LSB */
}
/*****************************************************************
**Name	  :   misc_monitor_temperatur()
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
Bool_PUB glb_temperatur_first_sample = FALSE_PUB;
#if 1
void misc_monitor_temperatur(uInt8_PUB channel)
{
#if 0
	volatile uInt16_PUB adcread, sum=0, adcread_avr;
	volatile uInt16_PUB delta, temp;
	volatile uInt8_PUB cnt;
	static uInt16_PUB temp_value_for_average[8] = {0x400,0x400,0x400,0x400,0x400,0x400,0x400,0x400};
	static uInt8_PUB temp_index_for_average=0;
	volatile float cal_val =0;
	uInt16_PUB sample_value[4]; 
	
	volatile uInt16_PUB timeout =0xfff;
	
	/*secect ADC channel
	select internal temperature sensor*/
	ADCCP = 0x10; 
	
	/*start A/D conversion*/
	for(cnt=0; cnt<4; cnt++)
	{
		/*software start, single-end input*/
		ADCCON=0xA3;    

		/*wait for end of conversion*/
		while(!ADCSTA)
		{
			if(timeout--)
			 	continue;
			else 
				return ;
		}           
		sample_value[cnt]=(ADCDAT>>16);
	}
		/*limits a highest*/
	if(sample_value[3] > 4085) 
	{
		sample_value[3] = 0;   
		return;
	}
	adcread = sample_value[3];
		
	if(glb_temperatur_first_sample == FALSE_PUB)
	{
		for(cnt=0; cnt<8; cnt++)
		{
			temp_value_for_average[cnt]=adcread;
		}
		glb_temperatur_first_sample = TRUE_PUB;
	}
	
	/*averaging*/
	temp_index_for_average &= 0x07;                  /*reset ring-buffer pointer*/
	temp_value_for_average[temp_index_for_average] = adcread;
	temp_index_for_average++;
	//temp_index_for_average &= 0x07;                  /*reset ring-buffer pointer*/
	for(cnt=0; cnt<8; cnt++)
	{
		sum += temp_value_for_average[cnt];
	}
	adcread_avr = sum>>3; 

	/*
	Vadc = raw_adc*0.61mv
	the formula: T-Tref = (Vadc-Vtref) * K)
	Tref = 25c
	Vtref = 1369mv
	K = 0.2262C/mv
	*/
	cal_val = adcread_avr*0.61;
	cal_val = (cal_val-1369)*0.2262+25;
	cal_val = cal_val*256;
	temp = (uInt16_PUB)cal_val;

	/*save data*/
	glb_sram_qsfp_A0_memory[22] = (uInt8_PUB)(temp>>8);          /*write high-byte*/
	glb_sram_qsfp_A0_memory[23] = (uInt8_PUB)(temp & 0x00FF);    /*write low-byte*/
#else
	uInt8_PUB ch =0;
	uInt16_PUB temperatur_raw_adc=0;
	uInt16_PUB temperatur = 0;
	float temperatur_f = 0;
	
	for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
	{
		if(glb_sram_map_gn1160_status[ch]==TRUE_PUB)
		{
			temperatur_raw_adc = (glb_sram_map_gn1160[ch][0x08]<<8)|(glb_sram_map_gn1160[ch][0x09]<<0);

			/*Vadc = ADCresult * ADCref/(2^ADCresolution-1);
			Isnk = Vadc * 320.8 / Rbias_mon;  (unit:mA)
			Rbias_mon = 4.7K = 4700;

			QSFP : 0~65535 with LSB equal to 2uA
			*/
			if(temperatur_raw_adc&0x8000)
				temperatur_raw_adc &=(~0x8000);
			else
				temperatur_raw_adc |=(0x8000);

			temperatur_f = temperatur_raw_adc;
			temperatur_f = (temperatur_f * 1.8/65535);
			temperatur_f = temperatur_f*1000;
			
			temperatur_f = (temperatur_f-760)/3-40;
			temperatur_f = temperatur_f*256;

			temperatur = (uInt16_PUB)temperatur_f;
			glb_sram_qsfp_A0_memory[22] = (uInt8_PUB)((temperatur>>8)& 0x00FF);	/*write high-byte*/
			glb_sram_qsfp_A0_memory[23] = (uInt8_PUB)(temperatur & 0x00FF);    	/*write low-byte*/
		}
	}
#endif
}
#else
void misc_monitor_temperatur(uInt8_PUB channel)
{
	volatile uInt16_PUB adcread, sum=0, adcread_avr;
	volatile uInt16_PUB delta, temp;
	volatile uInt8_PUB cnt;
	static uInt16_PUB temp_value_for_average[8] = {0x400,0x400,0x400,0x400,0x400,0x400,0x400,0x400};
	static uInt8_PUB temp_index_for_average=0;
	volatile float cal_val =0;

	
	/*secect ADC channel
	select internal temperature sensor*/
	ADCCP = 0x10; 
	
	/*start AD conversion at ADC input channel 0x10*/
	if(FALSE_PUB==misc_adc(0x10,(uInt16_PUB *)&adcread))
		return;
	
	if(glb_temperatur_first_sample == FALSE_PUB)
	{
		for(cnt=0; cnt<8; cnt++)
		{
			temp_value_for_average[cnt]=adcread;
		}
		glb_temperatur_first_sample = TRUE_PUB;
	}
	
	/*averaging*/
	temp_index_for_average &= 0x07;                  /*reset ring-buffer pointer*/
	temp_value_for_average[temp_index_for_average] = adcread;
	temp_index_for_average++;
	temp_index_for_average &= 0x07;                  /*reset ring-buffer pointer*/
	for(cnt=0; cnt<8; cnt++)
	{
		sum += temp_value_for_average[cnt];
	}
	adcread_avr = sum>>3; 

	/*
	Vadc = raw_adc*0.61mv
	the formula: T-Tref = (Vadc-Vtref) * K)
	Tref = 25c
	Vtref = 1369mv
	K = 0.2262C/mv
	*/
	cal_val = adcread_avr*0.61;
	cal_val = (cal_val-1369)*0.2262+25;
	cal_val = cal_val*256;
	temp = (uInt16_PUB)cal_val;

	/*save data*/
	glb_sram_qsfp_A0_memory[22] = (uInt8_PUB)(temp>>8);          /*write high-byte*/
	glb_sram_qsfp_A0_memory[23] = (uInt8_PUB)(temp & 0x00FF);    /*write low-byte*/
}
#endif
/*****************************************************************
**Name	  :   misc_monitor_voltage()
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
void misc_monitor_voltage(uInt8_PUB channel)
{
	uInt8_PUB ch =0;

	uInt16_PUB voltage_raw_adc=0;
	uInt16_PUB voltage =0;
	float voltage_float = 0.00;
		
	for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
	{
		if(glb_sram_map_gn1160_status[ch]==TRUE_PUB)
		{
			voltage_raw_adc = (glb_sram_map_gn1160[ch][0x0A]<<8)|(glb_sram_map_gn1160[ch][0x0B]<<0);

			/*Vadc = ADCresult * ADCref/(2^ADCresolution-1);
			VCC = 4.0 * Vadc; (unit:V)
			*/
			voltage_float = voltage_raw_adc*1.8/65535;
			voltage_float = 4.0 * voltage_float;
			voltage_float = (1000*voltage_float)/0.1;
			voltage = (uInt16_PUB)voltage_float;
			
			glb_sram_qsfp_A0_memory[26] = (uInt8_PUB)((voltage>>8)& 0x00FF); 	/*write high-byte*/
			glb_sram_qsfp_A0_memory[27] = (uInt8_PUB)(voltage & 0x00FF);    	/*write low-byte*/
		}
	}
}
/*****************************************************************
**Name	  :   misc_monitor_txbias()
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
void misc_monitor_txbias(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	uInt16_PUB txbias_raw_adc=0;
	uInt16_PUB txbias = 0;
	float txbias_f = 0;
	
	for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
	{
		if(glb_sram_map_gn1160_status[ch]==TRUE_PUB)
		{
			txbias_raw_adc = (glb_sram_map_gn1160[ch][0x0c]<<8)|(glb_sram_map_gn1160[ch][0x0d]<<0);

			/*Vadc = ADCresult * ADCref/(2^ADCresolution-1);
			Isnk = Vadc * 320.8 / Rbias_mon;  (unit:mA)
			Rbias_mon = 4.7K = 4700;

			QSFP : 0~65535 with LSB equal to 2uA
			*/
			
			txbias_f = (txbias_raw_adc * 1.8/65535);//v
			txbias_f = txbias_f*1000;//mv
			txbias_f = txbias_f*320.8/4700;//mA
			txbias_f = txbias_f*1000;//uA

			txbias_f = txbias_f*3;
			txbias_f = txbias_f/4;
			
			txbias_f = txbias_f/2;

			txbias = (uInt16_PUB)txbias_f;
			glb_sram_qsfp_A0_memory[42+ch*2] = (uInt8_PUB)((txbias>>8)& 0x00FF);	/*write high-byte*/
			glb_sram_qsfp_A0_memory[43+ch*2] = (uInt8_PUB)(txbias & 0x00FF);    	/*write low-byte*/
		}
	}
}
/*****************************************************************
**Name	  :   misc_monitor_txmod()
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
void misc_monitor_txmod(uInt8_PUB channel)
{
	uInt8_PUB i =0;
	for(i=0;i<PUBCOM_GN1160_NUM;i++)
	{
		if(glb_sram_map_gn1160_status[i]==TRUE_PUB)
		{


		}
	}
}
/*****************************************************************
**Name	  :   misc_monitor_txpow()
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
void misc_monitor_txpow(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	uInt16_PUB txpow_raw_adc=0;
	uInt16_PUB txpow = 0;
	float txpow_v = 0;
	float txpow_imon = 0;


	for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
	{
		if(glb_sram_map_gn1160_status[ch]==TRUE_PUB)
		{
			txpow_raw_adc = (glb_sram_map_gn1160[ch][0x0e]<<8)|(glb_sram_map_gn1160[ch][0x0f]<<0);

			/*Vadc = ADCresult * ADCref/(2^ADCresolution-1);
			Isnk = Vadc * 320.8 / Rbias_mon;  (unit:mA)
			Rbias_mon = 4.7K = 4700;

			QSFP : 0~65535 with LSB equal to 2uA
			*/
			
			txpow_v = (txpow_raw_adc * 1.8/65535);//v
			txpow_v = txpow_v*1000;//mv
			txpow_imon = txpow_v/517;//mA

#if 0
			txpow_imon = txpow_imon/1000;//A
			txpow_v = txpow_v/1000;//V
			txpow_v = txpow_imon*txpow_v;//w
			txpow_v = txpow_v*1000000;//uw
			txpow_v = txpow_v*10;

			txpow_v=txpow_v/4;
#else
			txpow_imon = txpow_imon*1000;//uA
			if(ch==0)
			{
				txpow_imon = txpow_imon*1780;
				txpow_imon = txpow_imon/2343;//uw
			}
			else if(ch==1)
			{
				txpow_imon = txpow_imon*1400;
				txpow_imon = txpow_imon/1762;//uw
			}
			else if(ch==2)
			{
				txpow_imon = txpow_imon*1780;
				txpow_imon = txpow_imon/2343;//uw
			}
			else	 if(ch==3)
			{
				txpow_imon = txpow_imon*1600;
				txpow_imon = txpow_imon/2046;//uw
			}
				
			txpow_imon = txpow_imon*10;
#endif
			txpow = (uInt16_PUB)txpow_imon;
			glb_sram_qsfp_A0_memory[50+ch*2] = (uInt8_PUB)((txpow>>8)& 0x00FF);	/*write high-byte*/
			glb_sram_qsfp_A0_memory[51+ch*2] = (uInt8_PUB)(txpow & 0x00FF); 
		}
	}
}
/*****************************************************************
**Name	  :   misc_monitor_txpow()
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
Bool_PUB glb_first_sample_rxpow[PUBCOM_GN1160_NUM] = {FALSE_PUB,FALSE_PUB,FALSE_PUB,FALSE_PUB};
void misc_monitor_rxpow(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	//float pow;
	uInt16_PUB pow_res;
	uInt16_PUB sample_value[4]; 
	uInt8_PUB cnt;
	static uInt16_PUB rxpow_value_for_average[PUBCOM_GN1160_NUM][8]={{0}};
	static uInt8_PUB rxpow_index_for_average[PUBCOM_GN1160_NUM]={0};
	float  data_f=0;
	const uInt8_PUB adc_channel_map[PUBCOM_GN1160_NUM]={0,1,2,3};
	volatile uInt16_PUB timeout =0xfff;

	
	for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
	{
		/*select ADC channel*/
		ADCCP = adc_channel_map[ch];

		/*start A/D conversion*/
		for(cnt=0; cnt<4; cnt++)
		{
			/*software start, single-end input*/
			ADCCON=0xA3;    

			/*wait for end of conversion*/
			while(!ADCSTA)
			{
				if(timeout--)
				 	continue;
				else 
					return ;
			}           
			sample_value[cnt]=(ADCDAT>>16);
		}

		/*ADC bug fix
		continue 4 times sample, but only used the last time sample
		then average by according to valid 8 samples
		*/
		/*limits a highest*/
		if(sample_value[3] > 4085) 
			sample_value[3] = 0;    

		/*averaging*/
		rxpow_value_for_average[ch][rxpow_index_for_average[ch]] = sample_value[3];
		(rxpow_index_for_average[ch])++;

		if(glb_first_sample_rxpow[ch] == FALSE_PUB)
		{
			for(cnt=0; cnt<8; cnt++)
			{
				rxpow_value_for_average[ch][cnt]=sample_value[3];
			}
			glb_first_sample_rxpow[ch] = TRUE_PUB;
		}
	
		 /*reset ring-buffer pointer*/
		rxpow_index_for_average[ch] &= 0x07;                 
		for(cnt=0; cnt<8; cnt++)
		{
			data_f += rxpow_value_for_average[ch][cnt];
		}
		data_f = data_f/8;                   

		/*cal the rx power according to the raw_adc*/
		data_f = sample_value[3];
		if(ch==0)
		{
			data_f =1350*(data_f-0)/588;//uw
		}
		else if(ch==1)
		{
			data_f =1520*(data_f-0)/760;//uw
		}
		else if(ch==2)
		{
			data_f =1520*(data_f-0)/760;//uw
		}
		else if(ch==3)
		{
			data_f =1450*(data_f-0)/760;//uw
		}
		data_f = data_f*10;
		pow_res = data_f;

		/*save data*/
		glb_sram_qsfp_A0_memory[34+ch*2] = (uInt8_PUB)(pow_res>>8);              
		glb_sram_qsfp_A0_memory[35+ch*2] = (uInt8_PUB)(pow_res & 0x00FF); 

		glb_sram_qsfp_A0_memory[74+ch*2] = (uInt8_PUB)(sample_value[3]>>8);              
		glb_sram_qsfp_A0_memory[75+ch*2] = (uInt8_PUB)(sample_value[3] & 0x00FF); 
	}
}
/*****************************************************************
**Name	  :   misc_monitor()
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

void misc_monitor(void)
{
	uInt8_PUB ch = 0;

	//for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
	{
		misc_update_sram_map_gn1160_register(ch);
		misc_monitor_temperatur(ch);
		misc_monitor_voltage(ch);
		misc_monitor_txbias(ch);
		misc_monitor_rxpow(ch);

		//misc_monitor_txmod(ch);
		misc_monitor_txpow(ch);
	}
}
/*****************************************************************
**Name	  :   misc_diag_temperatur()
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
void misc_diag_temperatur(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	uInt8_PUB offset_addr = 6;

	uInt8_PUB msb = 0;
	uInt8_PUB lsb = 0;
	uInt8_PUB addr = 128;

	uInt8_PUB result;
	uInt16_PUB monitor = 0;

	uInt16_PUB high_alarm =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+1];
	uInt16_PUB low_alarm =		(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+2]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+3];
	uInt16_PUB high_warning =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+4]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+5];
	uInt16_PUB low_warning =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+6]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+7];
			
	for(ch=0;ch<1;ch++)
	{
		msb = glb_sram_qsfp_A0_memory[22];
		lsb = glb_sram_qsfp_A0_memory[23];
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
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x80);		/*set high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x20);		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
			break;
			case 4:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= 0x20;		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
			break;
			case 3:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
			break;
			case 2:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x10);	/*set low warning flag bit4*/
			break;
			case 1:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x10);	/*set low warning flag bit4*/
			break;
		}
	}
}
/*****************************************************************
**Name	  :   misc_diag_voltage()
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
void misc_diag_voltage(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	uInt8_PUB offset_addr = 7;
	uInt8_PUB msb = 0;
	uInt8_PUB lsb = 0;
	uInt8_PUB addr = 144;

	uInt8_PUB result;
	uInt16_PUB monitor = 0;

	uInt16_PUB high_alarm =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+1];
	uInt16_PUB low_alarm =		(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+2]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+3];
	uInt16_PUB high_warning =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+4]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+5];
	uInt16_PUB low_warning =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+6]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+7];
			
	for(ch=0;ch<1;ch++)
	{
		msb = glb_sram_qsfp_A0_memory[26];
		lsb = glb_sram_qsfp_A0_memory[27];
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
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x80);		/*set high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x20);		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
			break;
			case 4:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= 0x20;		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
			break;
			case 3:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
			break;
			case 2:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x10);	/*set low warning flag bit4*/
			break;
			case 1:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x10);	/*set low warning flag bit4*/
			break;
		}
	}
}
/*****************************************************************
**Name	  :   misc_diag_txbias()
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
void misc_diag_txbias(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	uInt8_PUB offset_addr = 7;
	uInt8_PUB msb = 0;
	uInt8_PUB lsb = 0;
	uInt8_PUB addr = 184;

	uInt8_PUB result;
	uInt16_PUB monitor = 0;

	uInt16_PUB high_alarm =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+1];
	uInt16_PUB low_alarm =		(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+2]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+3];
	uInt16_PUB high_warning =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+4]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+5];
	uInt16_PUB low_warning =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+6]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+7];
			
	for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
	{
		msb = glb_sram_qsfp_A0_memory[42+ch*2];
		lsb = glb_sram_qsfp_A0_memory[43+ch*2];
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
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x80);		/*set high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x20);		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
				break;
				case 4:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= 0x20;		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
				break;
				case 3:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
				break;
				case 2:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x10);	/*set low warning flag bit4*/
				break;
				case 1:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x10);	/*set low warning flag bit4*/
				break;
			}
		}
		else
		{
			switch (result)
			{
				case 5:
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x8);		/*set high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x2);		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x1);	/*clear low warning flag bit4*/
				break;
				case 4:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x8);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= 0x2;		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x1);	/*clear low warning flag bit4*/
				break;
				case 3:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x8);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x2);		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x1);	/*clear low warning flag bit4*/
				break;
				case 2:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x8);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x2);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x1);	/*set low warning flag bit4*/
				break;
				case 1:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x8);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x2);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x1);	/*set low warning flag bit4*/
				break;
			}

		}
	}
}
/*****************************************************************
**Name	  :   misc_diag_txmod()
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
void misc_diag_txmod(uInt8_PUB channel)
{
	uInt8_PUB i =0;
	for(i=0;i<PUBCOM_GN1160_NUM;i++)
	{
		if(glb_sram_map_gn1160_status[i]==TRUE_PUB)
		{


		}
	}
}
/*****************************************************************
**Name	  :   misc_monitor_txpow()
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
void misc_diag_rxpow(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	uInt8_PUB offset_addr = 7;
	uInt8_PUB msb = 0;
	uInt8_PUB lsb = 0;
	uInt8_PUB addr = 176;

	uInt8_PUB result;
	uInt16_PUB monitor = 0;

	uInt16_PUB high_alarm =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+1];
	uInt16_PUB low_alarm =		(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+2]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+3];
	uInt16_PUB high_warning =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+4]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+5];
	uInt16_PUB low_warning =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+6]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+7];
			
	for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
	{
		msb = glb_sram_qsfp_A0_memory[34+ch*2];
		lsb = glb_sram_qsfp_A0_memory[35+ch*2];
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
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x80);		/*set high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x20);		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
				break;
				case 4:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= 0x20;		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
				break;
				case 3:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
				break;
				case 2:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x10);	/*set low warning flag bit4*/
				break;
				case 1:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x10);	/*set low warning flag bit4*/
				break;
			}
		}
		else
		{
			switch (result)
			{
				case 5:
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x8);		/*set high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x2);		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x1);	/*clear low warning flag bit4*/
				break;
				case 4:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x8);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= 0x2;		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x1);	/*clear low warning flag bit4*/
				break;
				case 3:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x8);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x2);		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x1);	/*clear low warning flag bit4*/
				break;
				case 2:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x8);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x2);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x1);	/*set low warning flag bit4*/
				break;
				case 1:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x8);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x2);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x1);	/*set low warning flag bit4*/
				break;
			}

		}
	}
}
/*****************************************************************
**Name	  :   misc_monitor_txpow()
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
void misc_diag_txpow(uInt8_PUB channel)
{
	uInt8_PUB ch =0;
	uInt8_PUB offset_addr = 7;
	uInt8_PUB msb = 0;
	uInt8_PUB lsb = 0;
	uInt8_PUB addr = 192;

	uInt8_PUB result;
	uInt16_PUB monitor = 0;

	uInt16_PUB high_alarm =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+1];
	uInt16_PUB low_alarm =		(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+2]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+3];
	uInt16_PUB high_warning =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+4]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+5];
	uInt16_PUB low_warning =	(glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+6]<<8) + glb_code_qsfp_A2_up_memory[PUBCOM_UPPAGE_03][addr+7];
			
	for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
	{
		msb = glb_sram_qsfp_A0_memory[50+ch*2];
		lsb = glb_sram_qsfp_A0_memory[51+ch*2];
		monitor= (msb<<8) + lsb;
		
		if((ch==0)||(ch==1))
			offset_addr = 13;
		else
			offset_addr = 14;
			

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
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x80);		/*set high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x20);		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
				break;
				case 4:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= 0x20;		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
				break;
				case 3:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x10);	/*clear low warning flag bit4*/
				break;
				case 2:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x10);	/*set low warning flag bit4*/
				break;
				case 1:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x80);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x40);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x20);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x10);	/*set low warning flag bit4*/
				break;
			}
		}
		else
		{
			switch (result)
			{
				case 5:
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x8);		/*set high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x2);		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x1);	/*clear low warning flag bit4*/
				break;
				case 4:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x8);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] |= 0x2;		/*set high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x1);	/*clear low warning flag bit4*/
				break;
				case 3:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x8);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x2);		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x1);	/*clear low warning flag bit4*/
				break;
				case 2:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x8);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x2);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x1);	/*set low warning flag bit4*/
				break;
				case 1:
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x8);		/*clear high alarm flag bit7*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x4);	/*clear low alarm flag bit6*/
				glb_sram_qsfp_A0_memory[offset_addr] &= (~0x2);;		/*clear high warning flag bit5*/
				glb_sram_qsfp_A0_memory[offset_addr] |= (0x1);	/*set low warning flag bit4*/
				break;
			}

		}
	}
}
/*****************************************************************
**Name	  :   misc_specail_for_jatg()
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
void misc_specail_for_jatg(void)
{
	if(misc_get_lpmode_status()==FALSE_PUB)
	{
		i2cbg_specail_for_jtag();
		while(misc_get_lpmode_status()==FALSE_PUB)
		{

		}
	}
}
/*****************************************************************
**Name	  :   misc_specail_for_jatg()
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
void misc_specail_erase_fwcmd(void)
{
	volatile uInt32_PUB* paddr = ((volatile uInt32_PUB*)0x80014);
	if(glb_erase_fw_enable==TRUE_PUB)
	{
		FIQCLR = 0xffffffff;     // disable I2C0SIRQ
		IRQCLR = 0xffffffff;     // disable Timer1IRQ

		//i2cbg_specail_for_jtag();
		flash_erase(0x0000); 

		glb_erase_fw_enable=FALSE_PUB;
		while(TRUE_PUB)
		{
			FEEPRO = 0xFFFFFFFF;
			flash_erase(0x0000);
			flash_erase(0x0000);

			if(*paddr != 0xFFFFFFFF)
			{
				FEEDAT = 0x3CFF;
				FEEADR = 0xFFC3; 
				FEEMOD = FEEMOD|0x8; //Erase key enable 
				FEECON = 0x06; //Mass erase
			}
		}
	}
}
/*****************************************************************
**Name	  :   misc_diag()
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
void misc_diag(void)
{
	uInt8_PUB ch = 0;

	//for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
	{
		misc_diag_temperatur(ch);
		misc_diag_voltage(ch);
		misc_diag_txbias(ch);
		//misc_diag_txmod(ch);
		misc_diag_txpow(ch);
		misc_diag_rxpow(ch);
	}
}
/*****************************************************************
**Name	  :   misc_diag()
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
void misc_disable_channel(void)
{
	uInt8_PUB ch =0;
	
	for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
	{
		if(((glb_sram_qsfp_A0_memory[86]&(0x01<<ch))!=0)
			&&((glb_sram_map_gn1160[ch][0x3e]&0x40)==0))
		{
			/*write disable the tx*/
			glb_sram_map_gn1160[ch][0x3e] |=0x40;
			i2cbg_write_nbytes((E_I2CBG_TXn)ch,&glb_sram_map_gn1160[ch][0x3e],1,0x3e);
		}
		else if(((glb_sram_qsfp_A0_memory[86]&(0x01<<ch))==0)
			&&((glb_sram_map_gn1160[ch][0x3e]&0x40)!=0))
		{
			/*write enable the tx*/
			glb_sram_map_gn1160[ch][0x3e] &=(~0x40);
			i2cbg_write_nbytes((E_I2CBG_TXn)ch,&glb_sram_map_gn1160[ch][0x3e],1,0x3e);
		}
	}
}
/*****************************************************************
**Name	  :   misc_lookup_table()
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
#if 1
#define OFFSET_16BIT_RES		0x8000
const uInt16_PUB glb_lktb_mod[PUBCOM_GN1160_NUM][PUBCOM_LOOKUP_TABLE_NUM][2]=
{
	/*temperature,mod dac*/
	/*tx0*/
	{
		{0x3ce+OFFSET_16BIT_RES,0x640},
		{0xcd2+OFFSET_16BIT_RES,0x76c},
		{0x175a+OFFSET_16BIT_RES,0x834},
		{0x1b20+OFFSET_16BIT_RES,0x7e5},

	},
	/*tx1*/
	{
		{0x3ce+OFFSET_16BIT_RES,0x640},
		{0xcd2+OFFSET_16BIT_RES,0x76c},
		{0x175a+OFFSET_16BIT_RES,0x834},
		{0x1b20+OFFSET_16BIT_RES,0x7e5},
	},
	/*tx2*/
	{
		{0x3ce+OFFSET_16BIT_RES,0x640},
		{0xcd2+OFFSET_16BIT_RES,0x76c},
		{0x175a+OFFSET_16BIT_RES,0x834},
		{0x1b20+OFFSET_16BIT_RES,0x7e5},
	},
	/*tx3*/
	{
		{0x3ce+OFFSET_16BIT_RES,0x640},
		{0xcd2+OFFSET_16BIT_RES,0x76c},
		{0x175a+OFFSET_16BIT_RES,0x834},
		{0x1b20+OFFSET_16BIT_RES,0x7e5},
	},
};
const uInt16_PUB glb_lktb_bias[PUBCOM_GN1160_NUM][PUBCOM_LOOKUP_TABLE_NUM][2]=
{
	/*temperature,bias dac*/
	/*tx0*/
	{
		{0x3ce+OFFSET_16BIT_RES,0x300},
		{0xcd2+OFFSET_16BIT_RES,0x3e8},
		{0x175a+OFFSET_16BIT_RES,0x4b0},
		{0x1b20+OFFSET_16BIT_RES,0x578},

	},
	/*tx1*/
	{
		{0x3ce+OFFSET_16BIT_RES,0x300},
		{0xcd2+OFFSET_16BIT_RES,0x3e8},
		{0x175a+OFFSET_16BIT_RES,0x4b0},
		{0x1b20+OFFSET_16BIT_RES,0x578},
	},
	/*tx2*/
	{
		{0x3ce+OFFSET_16BIT_RES,0x300},
		{0xcd2+OFFSET_16BIT_RES,0x3e8},
		{0x175a+OFFSET_16BIT_RES,0x4b0},
		{0x1b20+OFFSET_16BIT_RES,0x578},
	},
	/*tx3*/
	{
		{0x3ce+OFFSET_16BIT_RES,0x300},
		{0xcd2+OFFSET_16BIT_RES,0x3e8},
		{0x175a+OFFSET_16BIT_RES,0x4b0},
		{0x1b20+OFFSET_16BIT_RES,0x578},
	},
};
const uInt16_PUB glb_lktb_jitter[PUBCOM_GN1160_NUM][PUBCOM_LOOKUP_TABLE_NUM][2]=
{
	/*temperature,bias dac*/
	/*tx0*/
	{
		{0x3ce+OFFSET_16BIT_RES,0x46},
		{0xcd2+OFFSET_16BIT_RES,0x28},
		{0x175a+OFFSET_16BIT_RES,0x28},
		{0x1b20+OFFSET_16BIT_RES,0x28},
	},
	/*tx1*/
	{
		{0x3ce+OFFSET_16BIT_RES,0x46},
		{0xcd2+OFFSET_16BIT_RES,0x28},
		{0x175a+OFFSET_16BIT_RES,0x28},
		{0x1b20+OFFSET_16BIT_RES,0x28},
	},
	/*tx2*/
	{
		{0x3ce+OFFSET_16BIT_RES,0x46},
		{0xcd2+OFFSET_16BIT_RES,0x28},
		{0x175a+OFFSET_16BIT_RES,0x28},
		{0x1b20+OFFSET_16BIT_RES,0x28},
	},
	/*tx3*/
	{
		{0x3ce+OFFSET_16BIT_RES,0x46},
		{0xcd2+OFFSET_16BIT_RES,0x28},
		{0x175a+OFFSET_16BIT_RES,0x28},
		{0x1b20+OFFSET_16BIT_RES,0x28},
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
		
	/*get the temprature */
	//module_temperature = (glb_sram_qsfp_A0_memory[22]<<8)|glb_sram_qsfp_A0_memory[23];

	for(ch =0;ch <E_I2CBG_TX_MAX;ch ++)
	{
		module_temperature = (glb_sram_map_gn1160[ch][0x08]<<8)|(glb_sram_map_gn1160[ch][0x09]<<0);
		if(module_temperature&0x8000)
			module_temperature &=(~0x8000);
		else
			module_temperature |=(0x8000);
			
		/*get bias map index*/
		i_max = PUBCOM_LOOKUP_TABLE_NUM;
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

		/*get mod map index*/
		i_max = PUBCOM_LOOKUP_TABLE_NUM;
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
		
		/*get jitter map index*/
		i_max = PUBCOM_LOOKUP_TABLE_NUM;
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
		

		/*modulation dac*/
		value_dac = glb_lktb_mod[ch][i_mod][1];
		buf_dac[0] = (value_dac>>8)&0xFF;
		buf_dac[1] = (value_dac>>0)&0xFF;	
		i2cbg_write_nbytes((E_I2CBG_TXn)ch,(uInt8_PUB *)buf_dac,sizeof(buf_dac),0x1D);

		/*bias dac*/
		value_dac = glb_lktb_bias[ch][i_bias][1];
		buf_dac[0] = (value_dac>>8)&0xFF;
		buf_dac[1] = (value_dac>>0)&0xFF;		
		i2cbg_write_nbytes((E_I2CBG_TXn)ch,(uInt8_PUB *)buf_dac,sizeof(buf_dac),0x1F);

		/*jitter dac*/
		value_dac = glb_lktb_jitter[ch][i_mod][1];
		buf_dac[0] = (value_dac>>0)&0xFF;
		i2cbg_write_nbytes((E_I2CBG_TXn)ch,(uInt8_PUB *)buf_dac,1,0x19);
	}
}
#else
const uInt16_PUB glb_lktb_bias[4][2]=
{
	/*temperature,bias dac*/
	{100,100},
	{200,200},
	{300,300},
	{400,400},

};
const uInt16_PUB glb_lktb_mod[4][2]=
{
	/*temperature,mod dac*/
	{100,100},
	{200,200},
	{300,300},
	{400,400},
};
void misc_lookup_table(void)
{
	uInt8_PUB ch =0;
	uInt8_PUB i_bias =0;
	uInt8_PUB i_mod =0;
	volatile uInt8_PUB i_max =0;
	
	volatile uInt8_PUB buf_dac[2];
	volatile uInt16_PUB module_temperature = 0;
	volatile uInt16_PUB value_dac = 0;
		
	/*get the temprature */
	module_temperature = (glb_sram_qsfp_A0_memory[22]<<8)|glb_sram_qsfp_A0_memory[23];

	/*get bias map index*/
	i_max = (sizeof(glb_lktb_bias)/4);
	for(i_bias=0;i_bias<i_max;i_bias++)
	{
		if(i_bias>=(i_max-1))
		{
			break;
		}
		else if(module_temperature<glb_lktb_bias[i_bias+1][0])
		{
			break;
		}
	}

	/*get mod map index*/
	i_max = (sizeof(glb_lktb_mod)/4);
	for(i_mod=0;i_mod<i_max;i_mod++)
	{
		if(i_mod>=(i_max-1))
		{
			break;
		}
		else if(module_temperature<glb_lktb_mod[i_mod+1][0])
		{
			break;
		}
	}
	
	/*bias dac*/
	value_dac = glb_lktb_bias[i_bias][1];
	buf_dac[0] = (value_dac>>8)&0xFF;
	buf_dac[1] = (value_dac>>0)&0xFF;
	for(ch =0;ch <E_I2CBG_TX_MAX;ch ++)
	{
		if(ch !=E_I2CBG_TX2) continue;
		
		i2cbg_write_nbytes((E_I2CBG_TXn)ch,(uInt8_PUB *)buf_dac,sizeof(buf_dac),0x1D);
	}

	/*modulation dac*/
	value_dac = glb_lktb_mod[i_mod][1];
	buf_dac[0] = (value_dac>>8)&0xFF;
	buf_dac[1] = (value_dac>>0)&0xFF;
	for(ch =0;ch <E_I2CBG_TX_MAX;ch ++)
	{
		if(ch !=E_I2CBG_TX2) continue;
		
		i2cbg_write_nbytes((E_I2CBG_TXn)ch,(uInt8_PUB *)buf_dac,sizeof(buf_dac),0x1F);
	}
}
#endif
/*****************************************************************
**Name	  :   misc_update_dac()
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
void misc_update_dac(void)
{
	uInt8_PUB ch =0;
	//volatile uInt8_PUB i_max =0;
	volatile uInt8_PUB buf_dac[2];

	Bool_PUB rc = FALSE_PUB;
	
	if(glb_passward_enable==TRUE_PUB)
	{
		if(glb_update_dac_enable==TRUE_PUB)
		{
			for(ch=0;ch<PUBCOM_GN1160_NUM;ch++)
			{
				/* modulation dac*/
				buf_dac[0] = glb_sram_map_dac[4*ch+0]&0x0f;
				buf_dac[1] = glb_sram_map_dac[4*ch+1]&0xff;		
				rc=i2cbg_write_nbytes((E_I2CBG_TXn)ch,(uInt8_PUB *)buf_dac,sizeof(buf_dac),0x1D);
				if(rc == FALSE_PUB)
					return;
				
				/*bias dac*/
				buf_dac[0] = glb_sram_map_dac[4*ch+2]&0x0f;
				buf_dac[1] = glb_sram_map_dac[4*ch+3]&0xff;	
				rc=i2cbg_write_nbytes((E_I2CBG_TXn)ch,(uInt8_PUB *)buf_dac,sizeof(buf_dac),0x1F);
				if(rc == FALSE_PUB)
					return;
			}
			glb_update_dac_enable = FALSE_PUB;
		}
	}
}
/*****************************************************************
**Name	  :   misc_register_wr()
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
void misc_register_wr(void)
{
	uInt8_PUB ch =0;
	uInt8_PUB opeate_mode = 1;
	uInt8_PUB reg_addr = 0;
	uInt8_PUB reg_value= 0;

	Bool_PUB rc = FALSE_PUB;
	
	if(glb_passward_enable==TRUE_PUB)
	{
		if(glb_update_reg_enable==TRUE_PUB)
		{
			ch=glb_sram_map_dac[PUBCOM_PAGE_OFFSET_REG_WR_START-128];
			opeate_mode=glb_sram_map_dac[PUBCOM_PAGE_OFFSET_REG_WR_START-128+1];
			reg_addr=glb_sram_map_dac[PUBCOM_PAGE_OFFSET_REG_WR_START-128+2];
			reg_value=glb_sram_map_dac[PUBCOM_PAGE_OFFSET_REG_WR_START-128+3];

			if((ch<PUBCOM_GN1160_NUM)&&(opeate_mode==1))
			{
				rc=i2cbg_write_nbytes((E_I2CBG_TXn)ch,&reg_value,1,reg_addr);
				if(rc == FALSE_PUB)
					return;
			}

			glb_update_reg_enable = FALSE_PUB;
		}
	}
}

