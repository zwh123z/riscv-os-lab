// kernel/uart.c
// 该文件实现基于QEMU virt平台的UART驱动
// 用于通过串口发送字符和字符串
// QEMU virt 机器的 UART 内存映射基地址
#define UART_BASE 0x10000000L

// THR: Transmit Holding Register (发送保持寄存器)
#define THR (volatile unsigned char *)(UART_BASE + 0x00)

// LSR: Line Status Register (线路状态寄存器),CPU 读取这个寄存器，可以获取 UART 的工作状态（比如发送缓冲区是否空闲、是否收到数据等）
#define LSR (volatile unsigned char *)(UART_BASE + 0x05)

// LSR_THRE: LSR 的第5位，为1表示 THR 为空，可以发送下一个字符
#define LSR_THRE (1 << 5)

/*
 * 发送一个字符到串口,通过轮询方式等待发送缓冲区空闲，然后写入字符
 */
void uart_putc(char c) {
    // 循环等待，直到 THR 寄存器为空
    while ((*LSR & LSR_THRE) == 0);
    // 向 THR 写入要发送的字符
    *THR = c;
}

/*
 * 发送一个以 '\0' 结尾的字符串到串口,
 *循环调用uart_putc发送每个字符，直到遇到字符串结束符'\0'
 */
void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}