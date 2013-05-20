#define APLL_LOCK 	(*((volatile unsigned long *)0x7e00f000))
#define MPLL_LOCK 	(*((volatile unsigned long *)0x7e00f004))
#define EPLL_LOCK 	(*((volatile unsigned long *)0x7e00f008))
#define OTHERS  	(*((volatile unsigned long *)0x7e00f900))
#define CLK_DIV0 	(*((volatile unsigned long *)0x7e00f020))
#define ARM_RATIO 		0 		/* ARMCLK=DOUTAPLL/(ARM_RATIO+1) */
#define MPLL_RATIO 		0 		/* DOUTMPLL=MOUTMPLL/(MPLL_RATIO+1) */
#define HCLK_RATIO 		1 		/* HCLK=HCLKX2/(HCLK_RATIO+1) */
#define HCLKX2_RATIO  	1 		/* HCLKX2=HCLKX2IN/(HCLKX2_RATIO+1) */
#define PCLK_RATIO 		3 		/* PCLK=HCLKX2/(PCLK_RATIO+1) */

#define APLL_CON (*((volatile unsigned long *)0x7e00f00c))
/* FOUTAPLL=MDIV*FIN/(PDIV*2^SDIV)=266*12/(3*2^1)=532MHz */
#define APLL_CON_VAL ((1<<31)|(266<<16)|(3<<8)|(1))
#define MPLL_CON (*((volatile unsigned long *)0x7e00f010))
/* FOUTMPLL=MDIV*FIN/(PDIV*2^SDIV)=266*12/(3*2^1)=532MHz */
#define MPLL_CON_VAL ((1<<31)|(266<<16)|(3<<8)|(1))
#define CLK_SRC  (*((volatile unsigned long *)0x7e00f01c))
void clock_init()
{
/* Set Lock Time */
 APLL_LOCK=0XFFFF;
 MPLL_LOCK=0XFFFF;
 EPLL_LOCK=0XFFFF;
/* Set Async Mode */
 OTHERS&=(~0XC0);
 while((OTHERS&0XF00)!=0);
/* Set CLK_DIV0 */
 CLK_DIV0=(ARM_RATIO)|(MPLL_RATIO<<4)|(HCLK_RATIO<<8)|(HCLKX2_RATIO<<9)|(PCLK_RATIO<<12);
/* Set APLL_CON/MPLL_CON */
 APLL_CON=APLL_CON_VAL;
 MPLL_CON=MPLL_CON_VAL;
/* Start PLL */
 CLK_SRC=0X03;
}
