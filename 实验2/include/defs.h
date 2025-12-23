// kernel/defs.h

//uart.c
void uart_putc(char c);
void uart_puts(const char *s);

//console.c
void cons_putc(char c);

//printf.c
void clear_screen();
int printf(const char *fmt, ...);