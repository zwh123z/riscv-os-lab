# 交叉编译工具链
TOOLCHAIN = riscv64-unknown-elf-
CC = $(TOOLCHAIN)gcc
AS = $(TOOLCHAIN)as
LD = $(TOOLCHAIN)ld
OBJCOPY = $(TOOLCHAIN)objcopy
OBJDUMP = $(TOOLCHAIN)objdump
NM = $(TOOLCHAIN)nm

# 编译选项
CFLAGS = -nostdlib -nostartfiles -ffreestanding -Wall -Werror -march=rv64imac -mabi=lp64 -mcmodel=medany
ASFLAGS = -march=rv64imac -mabi=lp64
LDFLAGS = -T kernel/kernel.ld

# 源文件和目标文件
KERNEL_OBJS = kernel/entry.o kernel/main.o kernel/uart.o
KERNEL_ELF = kernel/kernel.elf
KERNEL_BIN = kernel/kernel.bin

# 默认目标
all: $(KERNEL_BIN)

# 生成二进制文件
$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@

# 链接内核
$(KERNEL_ELF): $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# 编译C文件
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# 汇编文件 - 使用gcc而不是as，以确保一致的编译选项
%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

# 清理目标文件
clean:
	rm -f $(KERNEL_OBJS) $(KERNEL_ELF) $(KERNEL_BIN)

# 运行QEMU模拟器
qemu: $(KERNEL_BIN)
	qemu-system-riscv64 -machine virt -bios none -kernel $(KERNEL_BIN) -nographic

# 调试模式
qemu-gdb: $(KERNEL_BIN)
	qemu-system-riscv64 -machine virt -bios none -kernel $(KERNEL_BIN) -nographic -s -S

# 查看内存布局
objdump: $(KERNEL_ELF)
	$(OBJDUMP) -h $<

# 查看符号表
nm: $(KERNEL_ELF)
	$(NM) $< | grep -E "(_start|_end|__bss_start|__bss_end|stack_top)"

.PHONY: all clean qemu qemu-gdb objdump nm