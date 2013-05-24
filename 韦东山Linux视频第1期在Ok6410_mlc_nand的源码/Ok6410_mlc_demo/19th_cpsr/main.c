
#include <stdio.h>
#include <nand.h>
#include "uart.h"
#include "lib/printf.h"
#include "lib/string.h"

char *str = "hello 100ask\n\r";

void hello(void)
{
	printf("%s", str);
}

void print_cpsr(unsigned int cpsr, char *why)
{	
	printf("print_cpsr %s cpsr = 0x%x\n\r", why, cpsr);
}

int main()
{
	int a, b;
	
//	printf("main %s cpsr = 0x%x\n\r", why, cpsr);

	while (1)
	{
		printf("please enter two number: \n\r");
		scanf("%d %d", &a, &b);
		printf("\n\r");
		printf("the sum of %d + %d is: %d\n\r", a, b, a+b);
	}
	
	return 0;
}

