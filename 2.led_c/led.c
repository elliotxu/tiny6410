//����Ĵ���
#define GPKCON0 (*((volatile unsigned long *)0x7F008800))
#define GPKDAT 	(*((volatile unsigned long *)0x7F008808))
int main()
{	
	/* gpk7��Ϊ������� */
	GPKCON0=GPKCON0&(~(0xf<<28))|(1<<28); //0001 0000 0000 0000 0000 0000 0000 00000=0x10000000
	/*gpk7���0*/
	GPKDAT=GPKDAT & (~(1<<7));
	return 0;
}

