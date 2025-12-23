#include "defs.h"

void test_printf_basic() {
printf("Testing integer: %d\n", 42);
printf("Testing negative: %d\n", -123);
printf("Testing zero: %d\n", 0);
printf("Testing hex: 0x%x\n", 0xABC);
printf("Testing string: %s\n", "Hello");
printf("Testing char: %c\n", 'X');
printf("Testing percent: %%\n");
}
void test_printf_edge_cases() {
printf("INT_MAX: %d\n", 2147483647);
printf("INT_MIN: %d\n", -2147483648);
printf("NULL string: %s\n", (char*)0);
printf("Empty string: %s\n", "");
}


void kmain()
{
       clear_screen(); // 清屏
    printf("===== Kernel Start =====\n");

    // 初始化物理内存分配器
    kinit();
    
    // 选项1: 运行测试（测试会内部调用 kvminit 和 kvminithart）
    run_all_tests();
    
    // kvminit();
    // kvminithart();
    // printf("\nInitialization complete. Entering halt state.\n");

    // 内核初始化完成，进入死循环
    while (1);
}