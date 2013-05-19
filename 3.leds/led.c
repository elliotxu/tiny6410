//¶¨Òå¼Ä´æÆ÷
#define GPKCON0 (*((volatile unsigned long *)0x7F008800))
#define GPKDAT 	(*((volatile unsigned long *)0x7F008808))
void delay()
{
	volatile int i = 0x10000;
	while (i--);
}
int main()
{	
	int i=0;
	/*set GPK4 GPK5 GPK6 GPK7 as GPIO output*/
	GPKCON0&=~((0xf<<16)|(0xf<<20)|(0xf<<24)|(0xf<<28));
	GPKCON0|=(1<<16)|(1<<20)|(1<<24)|(1<<28);
	/*gpk7Êä³ö0*/
	GPKDAT&=~((1<<4)|(1<<5)|(1<<6)|(1<<7));
	while(1)
	{
		GPKDAT=i<<4;
		i++;
		if(i==16)i=0;
		delay();
	}
	return 0;
}


