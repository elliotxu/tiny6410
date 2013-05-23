#include "uart.h"
//#include "stdio.h"

int main(void)
{
 char c;
 uart_init(); 
 led();
 put_char('A');
 put_char('B');
 printf("hello printf\r\n");
 printf("test=%d\r\n",10);
 while(1)
 {
  c=get_char();
  put_char(c+1);
  
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

