#define GPACON  	(*((volatile unsigned long *)0x7f008000))
#define ULCON0  	(*((volatile unsigned long *)0x7f005000))
#define UCON0  		(*((volatile unsigned long *)0x7f005004))
#define UFCON0  	(*((volatile unsigned long *)0x7f005008))
#define UMCON0  	(*((volatile unsigned long *)0x7f00500c))
#define UTRSTAT0 	(*((volatile unsigned long *)0x7f005010))
#define UFSTAT0  	(*((volatile unsigned long *)0x7f005018))
#define UTXH0  		(*((volatile unsigned char *)0x7f005020))
#define URXH0  		(*((volatile unsigned char *)0x7f005024))
#define UBRDIV0  	(*((volatile unsigned short *)0x7f005028))
#define UDIVSLOT0 	(*((volatile unsigned short *)0x7f00502c))
/* Bit Operate */
#define SET_BIT(x,y)    (x|=(1<<y))
#define CLR_BIT(x,y)    (x&=~(1<<y))
#define CPL_BIT(x,y)    (x^=(1<<y))
#define GET_BIT(x,y)    (x&(1<<y))
void uart_init(void)
{
/* Setup UART0 */
 GPACON&=~0XFF;
 GPACON|=0X22;
/* Setup ULCON0, UCON0, UFCON0, UMCON0 */
 ULCON0=0X3;  	/* 8N1 */
 UCON0=0X5;  	/* Enable UART Rx/Tx */
 UFCON0=0X01;  /* FIFO Enable */
 UMCON0=0;
/* Setup Bps */
/* DIV_VAL=(PCLK/(bps *16))-1 */
/* bps=115200Hz */
/* PCLK=66.5MHz */
/* DIV_VAL=(66500000/(115200*16)-1 */
/*        =35.08 */
/* UDIVSLOT0=(0.08*16 ge 1) */
 UBRDIV0=35;
 UDIVSLOT0=1;
}
char get_char(void)
{
 while((UFSTAT0&0X7F)==0); /* Wait Till Rx FIFO Not Emptly */
 return(URXH0);   /* Get char */
}
void put_char(char c)
{
 while(GET_BIT(UFSTAT0,14)); /* If Tx FIFO Is Full, Wait */
 UTXH0=c;   /* Write Data */
}

void put_str(const char *pStr)
{
	while(*pStr!='\0')
	{
		put_char(*pStr++);
	}
}
