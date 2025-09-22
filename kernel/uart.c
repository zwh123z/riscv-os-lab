#include "uart.h"

// UART寄存器地址
#define UART_BASE 0x10000000
#define UART_THR 0x0  // 发送保持寄存器（Transmit Holding Register）
#define UART_LSR 0x5  // 线路状态寄存器（Line S
// // UART_LSR寄存器的THRE位（第5位）：1表示发送缓冲区为空，可写入新字符
#define UART_LSR_THRE (1 << 5)

// 输出一个字符
void uart_putc(char c) {
 // 1. 等待UART发送缓冲区为空（检查LSR的THRE位）
    // 循环读取LSR寄存器，直到THRE位为1
    while (!( *(volatile uint8_t *)(UART_BASE + UART_LSR) & UART_LSR_THRE));
    
    // 2. 将字符写入THR寄存器，发送到串口
    *(volatile uint8_t *)(UART_BASE + UART_THR) = c;

    // 额外处理：若为'\n'（换行符），自动添加'\r'（回车符），避免终端显示错乱
    if (c == '\n') {
        uart_putc('\r');
    }
}

// 输出字符串
void uart_puts(char *s) {
     while (*s != '\0') {
        uart_putc(*s);  // 逐个字符输出
        s++;            // 指针移动到下一个字符
}
}    