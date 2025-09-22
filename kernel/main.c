#include "uart.h"
// 添加kmain函数原型声明
void kmain(void);

// 使用全局字符数组代替字符串常量，避免某些编译器的浮点处理
static char msg[] = "Hello OS\r\n";

// 内核主函数
void kmain() {
    // 输出启动信息
    uart_puts(msg);
    // 进入无限循环
    while (1) {
        // 空循环
    }
}
    