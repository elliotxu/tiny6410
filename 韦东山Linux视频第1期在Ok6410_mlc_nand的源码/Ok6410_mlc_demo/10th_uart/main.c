
#include "uart.h"

int main()
{
	char c;
	
	init_uart(); /* 波特率,数据位,停止位... */
	
	while (1)
	{
		c = getchar();
		putchar(c+1);
	}
	
	return 0;
}