
#include <stdint.h>
#include "proc.h" 
#include "riscv.h"
// 标准类型定义
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long int64;

// 定义常用的类型
typedef uint64 pte_t;      // 页表项
typedef uint64 *pagetable_t; // 页表
//uart.c
void uart_putc(char c);
void uart_puts(const char *s);

//console.c
void cons_putc(char c);

//printf.c
void clear_screen();
void printf(const char *fmt, ...);

// kalloc.c
void kinit();
void freerange(void *pa_start, void *pa_end);
void kfree(void *pa);
void *kalloc(void);

// test.c
void assert(int condition);  
void test_physical_memory(void); 
void test_pagetable(void);  
void test_virtual_memory(void);
void test_timer_interrupt(void);
void test_exception_handling(void);
void test_interrupt_overhead(void);
void run_all_tests(void);  
void run_lab4_tests(void);  // 添加这一行
void run_lab5_tests(void);

// vm.c
void kvminit(void);
void kvminithart(void);
pagetable_t create_pagetable(void);  
int map_page(pagetable_t pt, uint64 va, uint64 pa, int perm);  
pte_t *walk_lookup(pagetable_t pt, uint64 va);

// trap.c
void trap_init(void);
void clock_init(void);
uint64 get_time(void);
uint64 get_interrupt_count(void);
uint64 get_ticks(void);
void* get_ticks_channel(void);

// spinlock.c
void spinlock_init(struct spinlock *lk, char *name);
void acquire(struct spinlock *lk);
void release(struct spinlock *lk);
void push_off(void);
void pop_off(void);

// swtch.S
void swtch(struct context *old, struct context *new);