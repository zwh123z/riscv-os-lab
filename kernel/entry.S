# 最小系统启动汇编代码
# 功能：初始化硬件环境，为C语言执行做准备

# 定义入口点
.global _start
_start:
    # 调试检查点1：输出'S'表示启动开始
    li t0, 0x10000000   # UART基地址
    li t1, 'S'          # 启动标记
    sb t1, 0(t0)        # 输出字符

    # 设置栈指针
    la sp, stack_top    # 栈顶地址加载到sp寄存器
    
    # 调试检查点2：输出'P'表示栈设置完成
    li t1, 'P'          
    sb t1, 0(t0)

    # 清零BSS段
    la t2, __bss_start  # BSS段起始地址
    la t3, __bss_end    # BSS段结束地址
bss_clear:
    bge t2, t3, bss_done # 如果已到结束地址则退出循环
    sw zero, 0(t2)      # 清零当前地址
    addi t2, t2, 4      # 移动到下一个字
    j bss_clear
bss_done:

    # 调试检查点3：输出'C'表示BSS段清零完成
    li t1, 'C'
    sb t1, 0(t0)

    # 跳转到C主函数
    call kmain          # 调用C语言主函数

    # 如果kmain返回，进入无限循环
infinite_loop:
    j infinite_loop

# 定义栈空间（8KB）
# 修改栈空间定义，确保16字节对齐
.section .stack
stack_bottom:
    .align 16          # 确保16字节对齐
    .space 8192        # 8KB栈空间
stack_top:             # 栈顶地址（栈向低地址增长）

# 定义BSS段标记（实际地址由链接器脚本确定）
.section .bss
.global __bss_start
__bss_start:
.global __bss_end
__bss_end: