#line 1 "irq_arm.c"
 
typedef void (* tyVctHndlr) (void);

tyVctHndlr    IRQ     = (tyVctHndlr)0x0;
tyVctHndlr    SWI     = (tyVctHndlr)0x0;
tyVctHndlr    FIQ     = (tyVctHndlr)0x0;
tyVctHndlr    UNDEF   = (tyVctHndlr)0x0;
tyVctHndlr    PABORT  = (tyVctHndlr)0x0;
tyVctHndlr    DABORT  = (tyVctHndlr)0x0;

void	IRQ_Handler   (void) __irq;
void	SWI_Handler   (void) __irq;
void	FIQ_Handler   (void) __irq;
void	Undef_Handler (void) __irq;
void	PAbt_Handler  (void) __irq;
void	DAbt_Handler  (void) __irq;








 
#line 35 "irq_arm.c"

void	SWI_Handler(void) __irq
{
	if ( *SWI !=0x00)
	{
		SWI();
	}
}

void	Undef_Handler(void)__irq 
{
	if ( *UNDEF !=0x00)
	{
		UNDEF();
	}
}

void	PAbt_Handler(void) __irq
{
	if ( *PABORT !=0x00)
	{
		PABORT();
	}
}

void	DAbt_Handler(void) __irq
{
	if ( *DABORT !=0x00)
	{
		DABORT();
	}
}

