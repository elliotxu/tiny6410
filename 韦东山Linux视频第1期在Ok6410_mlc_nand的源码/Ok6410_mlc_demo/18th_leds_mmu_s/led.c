void delay()
{
	volatile int i = 0x1000000;
	while (i--);
}

int main()
{
	int i = 0;
	
	volatile unsigned long *gpmcon = \
		(volatile unsigned long *)0x10008820;
	volatile unsigned long *gpmdat = \
		(volatile unsigned long *)0x20008824;
	
	/* gpm0,1,2,3��Ϊ������� */
	*gpmcon = 0x1111;
	
	while (1)
	{
		*gpmdat = i;
		i++;
		if (i == 16)
			i = 0;
		delay();
	}
	
	return 0;
}

