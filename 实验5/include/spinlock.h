// kernel/spinlock.h 
#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#include "riscv.h" 

// 自旋锁，保护共享数据，防止出现竞态条件
struct spinlock {
    uint64 locked;
    char *name;
    struct cpu *cpu;
};

void spinlock_init(struct spinlock *lk, char *name);
void acquire(struct spinlock *lk);
void release(struct spinlock *lk);

void push_off(void);
void pop_off(void);

// 内联函数
static inline void intr_off() {
    w_sstatus(r_sstatus() & ~SSTATUS_SIE);
}

static inline void intr_on() {
    w_sstatus(r_sstatus() | SSTATUS_SIE);
}

static inline int intr_get(void) {
    return (r_sstatus() & SSTATUS_SIE) ? 1 : 0;
}

#endif // __SPINLOCK_H__