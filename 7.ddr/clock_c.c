
#define APLL_LOCK (*((volatile unsigned long *)0x7E00F000))
#define MPLL_LOCK (*((volatile unsigned long *)0x7E00F004))
#define EPLL_LOCK (*((volatile unsigned long *)0x7E00F008))

#define OTHERS    (*((volatile unsigned long *)0x7e00f900))

#define CLK_DIV0  (*((volatile unsigned long *)0x7E00F020))

#define ARM_RATIO    0  /* ARMCLK = DOUTAPLL / (ARM_RATIO + 1)=532/1=532MHz   		*/
#define HCLKX2_RATIO 1  /* HCLKX2 = HCLKX2IN / (HCLKX2_RATIO + 1) =532/(1+1)=266MHz	*/
#define HCLK_RATIO   1  /* HCLK = HCLKX2 / (HCLK_RATIO + 1)   = 266/(1+1)=133MHz    */
#define PCLK_RATIO   3  /* PCLK   = HCLKX2 / (PCLK_RATIO + 1) = 266/(3+1)=66.5MHz   */
#define MPLL_RATIO   0  /* DOUTMPLL = MOUTMPLL / (MPLL_RATIO + 1)=532/(0+1)=532MHz  */


#define APLL_CON  (*((volatile unsigned long *)0x7E00F00C))
#define MPLL_CON  (*((volatile unsigned long *)0x7E00F010))

#define SDIV	1
#define PDIV	3
#define MDIV	266	
#define PLL_ENABLE	( 1 << 31 )
#define APLL_VAL	( (PLL_ENABLE)|(MDIV<<16)|(PDIV<<8)|(SDIV<<0) )
//FOUT= MDIV * FIN/ (PDIV * 2^SDIV) =266 * 12 /(3 * 2^1)=266*12/6=532
#define MPLL_VAL 	APLL_VAL

#define EPLL_CON0	(*((volatile unsigned long *)0x7e00f014))
#define EPLL_CON1	(*((volatile unsigned long *)0x7e00f018))

#define EPLL0_VAL	( (2<<0)|(1<<8)|(32<<16)|PLL_ENABLE)
#define EPLL1_VAL	( 0 )
	
#define CLK_SRC  (*((volatile unsigned long *)0x7E00F01C))

void clock_init_c(void)
{
	APLL_LOCK = 0xffff;
	MPLL_LOCK = 0xffff;
	EPLL_LOCK = 0xffff;

	/* set async mode 当CPU时钟 != HCLK时，要设为异步模式 */
	OTHERS &= ~0xc0;
	while ((OTHERS & 0xf00) != 0);

	CLK_DIV0 = (ARM_RATIO) | (MPLL_RATIO << 4) | (HCLK_RATIO << 8) | (HCLKX2_RATIO << 9) | (PCLK_RATIO << 12);
	
	APLL_CON = APLL_VAL;  /* 532MHz */
	MPLL_CON = MPLL_VAL;  /* 532MHz */
	
	EPLL_CON0=EPLL0_VAL;
	EPLL_CON1=EPLL1_VAL;
	
	CLK_SRC = 0x07;
}

