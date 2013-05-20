#ifndef _UART_H_BAB
#define _UART_H_BAB

void uart_init(void);
char get_char(void);
void put_char(char c);
void put_str(const char *pStr);
#endif
