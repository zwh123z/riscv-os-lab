// kernel/spinlock.c
#include "defs.h"

void spinlock_init(struct spinlock *lk, char *name) {
    lk->name = name;
    lk->locked = 0;
    lk->cpu = 0;
}

// 获取锁
// 必须屏蔽中断以避免死锁
void acquire(struct spinlock *lk) {
    push_off(); // 1.屏蔽中断
    // 2. 检查死锁
    if (lk->locked && lk->cpu == mycpu()) {
        printf("acquire: re-acquire lock %s\n", lk->name);
        while(1);
    }

    // 使用 RISC-V 的 'amst' (atomic swap) 指令原子地将lk->locked设为1，返回原来的值
    while (__atomic_test_and_set(&lk->locked, __ATOMIC_ACQUIRE));

    // 记录持有锁的CPU，用于调试
    lk->cpu = mycpu();
}

// 释放锁
void release(struct spinlock *lk) {
    //1. 只有持有锁的 CPU 才能释放
    if (!lk->locked) {
        printf("release: lock %s not held\n", lk->name);
        while(1);
    }

    lk->cpu = 0;
    // 2. 原子操作释放锁
    __atomic_clear(&lk->locked, __ATOMIC_RELEASE);
    pop_off(); // 3.恢复之前的中断状态
}

// --- 中断状态保存 ---
//维护了一个计数器 ncli ，只有当计数器归零时，才真正调用 intr_on() 开启硬件中断。
// 记录 push_off/pop_off 的嵌套层数
void push_off(void) {
    int old = intr_get();
    intr_off();
    if (mycpu()->ncli == 0) {
        // 第一次屏蔽中断：保存原始的中断使能状态
        mycpu()->intena = old;
    }
    mycpu()->ncli += 1;
}

void pop_off(void) {
    if (intr_get()) {
        printf("pop_off: interrupts enabled\n");
        while(1);
    }
    mycpu()->ncli -= 1;
    if (mycpu()->ncli < 0) {
        printf("pop_off: ncli < 0\n");
        while(1);
    }
    // 嵌套层数为0，且原始中断状态是开启的：恢复中断
    if (mycpu()->ncli == 0 && mycpu()->intena) {
        intr_on();
    }
}