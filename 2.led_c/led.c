
int main()
{
	int i = 0;
	//����Ĵ�����ַ
	volatile unsigned long *gpkcon = (volatile unsigned long *)0x7F008800;
	volatile unsigned long *gpkdat = (volatile unsigned long *)0x7F008808;
	
	/* gpk7��Ϊ������� */
	*gpkcon=0x10000000; //0001 0000 0000 0000 0000 0000 0000 00000=0x10000000
	*gpkdat=0;			//���Ϊ0
	return 0;
}

