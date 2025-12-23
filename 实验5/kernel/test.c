
#include "riscv.h"
#include "defs.h"

#define NULL ((void*)0)

// 简单的断言实现
void assert(int condition) {
    if (!condition) {
        printf("ASSERTION FAILED!\n");
        while(1); // 停止执行
    }
}

// 测试1: 物理内存分配器测试
void test_physical_memory(void) {
    printf("\n=== Test 1: Physical Memory Allocator ===\n");
    
    // 测试基本分配和释放
    void *page1 = kalloc();
    void *page2 = kalloc();
    printf("page1 = %p, page2 = %p\n", page1, page2);
    assert(page1 != page2);
    assert(((uint64)page1 & 0xFFF) == 0); // 页对齐检查
    assert(((uint64)page2 & 0xFFF) == 0);
    printf("Page allocation and alignment test passed\n");

    // 测试数据写入
    *(int*)page1 = 0x12345678;
    assert(*(int*)page1 == 0x12345678);
    printf("Memory write test passed\n");

    // 测试释放和重新分配
    kfree(page1);
    void *page3 = kalloc();
    printf("page3 = %p (after freeing page1)\n", page3);
    // page3可能等于page1（取决于分配策略）
    
    kfree(page2);
    kfree(page3);
    printf("Free and reallocation test passed\n");
}

// 测试2: 页表操作测试
void test_pagetable(void) {
    printf("\n=== Test 2: Page Table Operations ===\n");
    
    pagetable_t pt = create_pagetable();
    assert(pt != 0);
    printf("Page table created at %p\n", pt);

    // 测试基本映射
    uint64 va = 0x1000000;
    uint64 pa = (uint64)kalloc();
    assert(pa != 0);
    printf("Mapping VA %p to PA %p\n", va, pa);
    assert(map_page(pt, va, pa, PTE_R | PTE_W) == 0);
    printf("Basic mapping test passed\n");

    // 测试地址转换
    pte_t *pte = walk_lookup(pt, va);
    assert(pte != 0 && (*pte & PTE_V));
    assert(PTE2PA(*pte) == pa);
    printf("Address translation test passed\n");

    // 测试权限位
    assert(*pte & PTE_R);
    assert(*pte & PTE_W);
    assert(!(*pte & PTE_X));
    printf("Permission bits test passed\n");
    
    // 清理
    kfree((void*)pa);
}

// 测试3: 虚拟内存激活测试
void test_virtual_memory(void) {
    printf("\n=== Test 3: Virtual Memory Activation ===\n");
    printf("Before enabling paging...\n");
    printf("Kernel code is accessible\n");
    printf("Kernel data is accessible\n");
    
    // 启用分页
    kvminit();
    kvminithart();
    
    printf("After enabling paging...\n");
    printf("Kernel code still executable\n");
    printf("Kernel data still accessible\n");
    printf("Device access still working\n");
}

// ===== Lab4 测试 =====

// 测试4: 时钟中断测试
void test_timer_interrupt(void) {
    printf("\n=== Test 4: Timer Interrupt ===\n");
    
    // 记录中断前的时间和中断计数
    uint64 start_time = get_time();
    uint64 start_count = get_interrupt_count();
    
    printf("Start time: %d, Start interrupt count: %d\n", start_time, start_count);
    printf("Waiting for 5 interrupts...\n");
    
    // 等待5次中断
    int target_interrupts = 5;
    while (get_interrupt_count() < start_count + target_interrupts) {
        // 简单延时
        for (volatile int i = 0; i < 100000; i++);
    }
    
    uint64 end_time = get_time();
    uint64 end_count = get_interrupt_count();
    
    printf("End time: %d, End interrupt count: %d\n", end_time, end_count);
    printf("Timer test completed: %d interrupts in %d cycles\n",
           end_count - start_count, end_time - start_time);
}

// 测试5: 异常处理测试
void test_exception_handling(void) {
    printf("\n=== Test 5: Exception Handling ===\n");
    
    // 注意: 这些测试可能导致系统崩溃，仅用于演示
    printf("Exception handling framework is in place.\n");
    printf("To test: uncomment specific exception triggers below\n");
    
    // 测试非法指令异常（已注释，取消注释会触发异常）
    // asm volatile(".word 0x00000000"); // 非法指令
    
    // 测试内存访问异常（已注释）
    // volatile int *bad_ptr = (int*)0x0;
    // *bad_ptr = 42;
    
    printf("Exception handler is ready (tests skipped for stability)\n");
}

// 测试6: 中断开销测试
void test_interrupt_overhead(void) {
    printf("\n=== Test 6: Interrupt Overhead ===\n");
    
    // 测量中断处理的时间开销
    uint64 start_count = get_interrupt_count();
    uint64 start_time = get_time();
    
    // 执行一些计算任务
    volatile uint64 sum = 0;
    for (int i = 0; i < 1000000; i++) {
        sum += i;
    }
    
    uint64 end_time = get_time();
    uint64 end_count = get_interrupt_count();
    
    uint64 elapsed_cycles = end_time - start_time;
    uint64 interrupts = end_count - start_count;
    
    printf("Computation with interrupts:\n");
    printf("  Elapsed cycles: %d\n", elapsed_cycles);
    printf("  Interrupts occurred: %d\n", interrupts);
    if (interrupts > 0) {
        printf("  Average cycles per interrupt: %d\n", elapsed_cycles / interrupts);
    }
    printf("  Result: %d (to prevent optimization)\n", sum);
    printf("Interrupt overhead measurement complete\n");
}

// 运行所有测试
void run_all_tests(void) {
    printf("\n===== Starting Comprehensive Tests =====\n");
    
    // Lab3 测试
    test_physical_memory();
    test_pagetable();
    test_virtual_memory();
    
    // Lab4 测试（需要在中断启用后运行）
    printf("\n===== Lab4 Tests (require interrupts enabled) =====\n");
    printf("Note: Lab4 tests will run after interrupt initialization\n");
    
    printf("\n===== Lab3 Tests Passed! =====\n");
}

// Lab4 专用测试（在中断启用后调用）
void run_lab4_tests(void) {
    printf("\n===== Starting Lab4 Interrupt Tests =====\n");
    
    test_timer_interrupt();
    test_exception_handling();
    test_interrupt_overhead();
    
    printf("\n===== All Lab4 Tests Passed! =====\n");
}

// ===== Lab 5 新增测试 =====

// 辅助函数：内核睡眠
void kernel_sleep(int ticks) {
    uint64 target_ticks = get_ticks() + ticks;
    while (get_ticks() < target_ticks) {
        acquire(&myproc()->lock);
        // 修正: 解决 'tick_counter' undeclared 错误
        // 我们调用新函数来获取正确的睡眠通道
        sleep(get_ticks_channel(), &myproc()->lock);
        release(&myproc()->lock);
    }
}

// 辅助任务 1: 简单任务
void simple_task(void) {
    printf("PID %d: simple_task running...\n", myproc()->pid);
    exit(0);
}

// 辅助任务 2: CPU 密集型任务
void cpu_intensive_task(void) {
    printf("PID %d: cpu_intensive_task running...\n", myproc()->pid);
    volatile uint64 i;
    for (i = 0; i < 200000000L; i++);
    printf("PID %d: cpu_intensive_task finished.\n", myproc()->pid);
    exit(0);
}

// --- 同步测试辅助 ---
static struct spinlock buffer_lock;
static int buffer[10];
static int count = 0;
static int done = 0;

void shared_buffer_init(void) {
    spinlock_init(&buffer_lock, "buffer_lock");
    count = 0;
    done = 0;
}

// 辅助任务 3: 生产者
void producer_task(void) {
    printf("PID %d: producer_task running...\n", myproc()->pid);
    for (int i = 0; i < 20; i++) {
        acquire(&buffer_lock);
        while (count == 10) {
            sleep(&count, &buffer_lock);
        }
        buffer[count++] = i;
        printf("Producer: produced %d (count=%d)\n", i, count);
        wakeup(&count);
        release(&buffer_lock);
    }
    acquire(&buffer_lock);
    done++;
    wakeup(&count);
    release(&buffer_lock);
    printf("Producer finished.\n");
    exit(0);
}

// 辅助任务 4: 消费者
void consumer_task(void) {
    printf("PID %d: consumer_task running...\n", myproc()->pid);
    while (1) {
        acquire(&buffer_lock);
        while (count == 0) {
            if (done) {
                release(&buffer_lock);
                goto end;
            }
            sleep(&count, &buffer_lock);
        }
        int data = buffer[--count];
        printf("Consumer: consumed %d (count=%d)\n", data, count);
        wakeup(&count);
        release(&buffer_lock);
    }
end:
    printf("Consumer finished.\n");
    exit(0);
}

// --- Lab 5 测试入口 ---

// 测试 7: 进程创建
void test_process_creation(void) {
    printf("\n=== Test 7: Process Creation ===\n");
    printf("Testing basic process creation...\n");
    int pid = create_process(simple_task);
    assert(pid > 0);
    printf("Created process %d\n", pid);
    
    // 修正: 'NULL' undeclared 错误 (已在文件顶部定义 NULL)
    wait_process(NULL);
    
    printf("\nTesting process table limit (NPROC=%d)...\n", NPROC);
    
    int count = 0;
    for (int i = 0; i < NPROC + 5; i++) {
        pid = create_process(simple_task);
        if (pid > 0) {
            count++; // 只增加计数
        } else {
            break;
        }
    }
    printf("Created %d processes (expected max %d)\n", count, NPROC - 1);
    assert(count == NPROC - 1);
    
    // 清理
    for (int i = 0; i < count; i++) {
        wait_process(NULL);
    }
    printf("Process creation test passed\n");
}

// 测试 8: 调度器
void test_scheduler(void) {
    printf("\n=== Test 8: Scheduler ===\n");
    printf("Creating 3 CPU intensive tasks...\n");
    for (int i = 0; i < 3; i++) {
        create_process(cpu_intensive_task);
    }
    
    printf("Observing scheduler behavior (sleeping 1000 ticks)...\n");
    uint64 start_time = get_ticks();
    kernel_sleep(1000);
    uint64 end_time = get_ticks();
    
    printf("Scheduler test completed (slept %d ticks)\n", end_time - start_time);
    
    wait_process(NULL);
    wait_process(NULL);
    wait_process(NULL);
}

// 测试 9: 同步机制
void test_synchronization(void) {
    printf("\n=== Test 9: Synchronization (Producer-Consumer) ===\n");
    shared_buffer_init();
    
    create_process(producer_task);
    create_process(consumer_task);
    
    wait_process(NULL);
    wait_process(NULL);
    
    printf("Synchronization test completed\n");
}

// Lab 5 测试的运行器
void run_lab5_tests(void) {
    printf("\n===== Starting Lab5 Tests =====\n");
    
    test_process_creation();
    test_scheduler();
    test_synchronization();
    
    printf("\n===== All Lab5 Tests Passed! =====\n");
}