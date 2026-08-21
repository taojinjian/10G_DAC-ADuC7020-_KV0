/***********************************************************
** xxxxxx Co.Ltd CopyRight
**
**File: main.c
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



/**
**extern
**/
extern volatile Bool_PUB glb_timer1_notice_updata ;


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
**Name	  :   main()
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

int main(void)
{
	init_module();

	while(1)
	{
		/*specail for jtag*/
		//misc_specail_for_jatg();
		
		/*erase the fw cmd*/
		misc_specail_erase_fwcmd();

		/*register write*/
//		misc_register_wr();

		/*update the dac for debug*/
		//misc_update_dac();
		
		/*check if need to update the flash*/
		flash_update_qsfp_table();

		/*SFF-8472 temperature: ADuC7020 internal temp -> A2[96]/[97]*/
		if(glb_timer1_notice_updata==TRUE_PUB)
		{
			glb_timer1_notice_updata = FALSE_PUB;
			misc_monitor_chip_temperatur();
			misc_monitor_chip_voltage();
		}

		/*monitor & diag*/
//		if(glb_timer1_notice_updata==TRUE_PUB)
//		{
//			misc_monitor();
//			misc_diag();
//			misc_disable_channel();
//			misc_lookup_table();
//		}
	}
}
