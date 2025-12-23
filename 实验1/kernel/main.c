#include "defs.h"

void kmain()
{
      // 直接调用串口输出 "Hello OS"
    uart_puts("hello OS!\n");
  // 内核不退出，进入无限循环

    while(1);

}