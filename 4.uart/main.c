#include "uart.h"
int main()
{
 char c;
 uart_init(); 
 led();
 put_char('A');
 put_str("hello\r\n");
 while(1)
 {
  c=get_char();
  put_char(c+1);
 }
 return(0);
}