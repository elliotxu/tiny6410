
int main()
{
	int i = 0;
	//定义寄存器地址
	volatile unsigned long *gpkcon = (volatile unsigned long *)0x7F008800;
	volatile unsigned long *gpkdat = (volatile unsigned long *)0x7F008808;
	
	/* gpk7设为输出引脚 */
	*gpkcon=0x10000000; //0001 0000 0000 0000 0000 0000 0000 00000=0x10000000
	*gpkdat=0;			//输出为0
	return 0;
}

