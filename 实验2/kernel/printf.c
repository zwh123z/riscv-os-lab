#include <stdarg.h>
#include "defs.h"

// 打印整数（十进制/十六进制），最小实现，不用递归
static void printint(int num, int base, int sign) {
    char buf[32];
    int i = 0;
    unsigned int x;

    // 负数按无符号处理，避免递归与最小实现复杂度
    if (sign && num < 0) {
        // 处理负号并转为无符号
        x = (unsigned int)(-(long)num);  // 用 long 防止 -INT_MIN 溢出路径
    } else {
        x = (unsigned int)num;
    }

    // 转换数字
    do {
        int d = x % base;
        buf[i++] = (d < 10) ? ('0' + d) : ('a' + d - 10);
        x /= base;
    } while (x);

    // 负号
    if (sign && num < 0) {
        buf[i++] = '-';
    }

    // 反向输出,取模得到的数字是倒着
    while (--i >= 0) {
        cons_putc(buf[i]);
    }
}
//va_list: 用于定义一个指向参数列表的指针
//va_start: 初始化 va_list 指针，使其指向第一个可变参数
//va_arg: 从 va_list 中按类型取出下一个参数。
//va_end: 清理 va_list
int printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    for (; *fmt; fmt++) {
        if (*fmt != '%') {
            cons_putc(*fmt);
            continue;
        }
        fmt++;
        if (*fmt == 0) break;

        switch (*fmt) {
        case 'd': {
            int val = va_arg(ap, int);
            printint(val, 10, 1);
            break;
        }
        case 'x': {
            int val = va_arg(ap, int);
            printint(val, 16, 0);
            break;
        }
        
        case 's': {
            const char *s = va_arg(ap, const char *);
            if (!s) s = "(null)";
            while (*s) cons_putc(*s++);
            break;
        }
        case 'c': {
            char c = (char)va_arg(ap, int);
            cons_putc(c);
            break;
        }
        case '%': {
            cons_putc('%');
            break;
        }
        default:
            // 未知格式，按原样输出，便于错误恢复
            cons_putc('%');
            cons_putc(*fmt);
            break;
        }
    }



    va_end(ap);
    return 0;
}

// 清屏函数,向串口发送了 ANSI 转义序列
void clear_screen() {
    // "\033[2J" 和 "\033[H" 是 ANSI 转义序列
    // "\033[2J": 清除整个屏幕
    // "\033[H": 将光标移动到左上角 (第一行第一列)
    const char *seq = "\033[2J\033[H";
    while (*seq) {
        cons_putc(*seq++);
    }
}