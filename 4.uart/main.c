#include "uart.h"
//#include "stdio.h"
#define export 
int main(void)
{
 char c;
 char temp;
 uart_init(); 
 led();
 put_char('A');
 put_char('B');
 printf("hello printf\r\n");
 printf("test=%d\r\n",10);
 while(1)
 {
	temp=get_char();
	printf("%c\r\n",temp);
	 switch(temp)
	 {
		case 9://Tab
		printf("d download\r\n");
		printf("t test memory\r\n");
		break;
		case 'r'://reset cpu
		printf("reset cpu ...\r\n");
		reset();
		break;
		case 'd':
		break;
		case 't':
		break;
		default:printf("no this item!\r\n");
	 } 
 }
 return(0);
}

unsigned char getc(void)
{
	return get_char();
}
void putc(char c)
{
	put_char(c);
}

