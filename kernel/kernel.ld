
/* 最小系统内核链接脚本 */
OUTPUT_FORMAT("elf64-littleriscv")
OUTPUT_ARCH(riscv)

/* 内核起始地址 - 增加地址空间避免重定位错误 */
ENTRY(_start)
BASE_ADDRESS = 0x80000000;

SECTIONS
{
    /* 从BASE_ADDRESS开始布局 */
    . = BASE_ADDRESS;

    /* 代码段 - 4KB对齐 */
    .text : ALIGN(4096) {
        /* 启动代码放在最前面 */
        *(.text.entry)
        /* 其他代码 */
        *(.text .text.*)
        etext = .;  /* 代码段结束 */
    }

    /* 只读数据段 */
    .rodata : ALIGN(4096) {
        *(.rodata .rodata.*)
    }

    /* 数据段（已初始化数据） */
    .data : ALIGN(4096) {
        *(.data .data.*)
        edata = .;  /* 数据段结束 */
    }

    /* BSS段（未初始化数据） */
    .bss : ALIGN(4096) {
        __bss_start = .;       /* BSS段起始符号 */
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;         /* BSS段结束符号 */
    }

    /* 栈空间（8KB） */
    .stack : ALIGN(4096) {
        stack_bottom = .;
        . = . + 8192;          /* 栈大小8KB */
        stack_top = .;         /* 栈顶地址 */
    }

    /* 内核结束地址 */
    _end = .;
}
    
