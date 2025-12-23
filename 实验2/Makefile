# 交叉编译工具链
TOOLCHAIN = riscv64-unknown-elf-
CC = $(TOOLCHAIN)gcc
AS = $(TOOLCHAIN)as
LD = $(TOOLCHAIN)ld
OBJCOPY = $(TOOLCHAIN)objcopy
OBJDUMP = $(TOOLCHAIN)objdump
NM = $(TOOLCHAIN)nm

# 编译选项
CFLAGS = -nostdlib -nostartfiles -ffreestanding -Wall -Werror -march=rv64imac -mabi=lp64 -mcmodel=medany -Iinclude
ASFLAGS = -march=rv64imac -mabi=lp64
LDFLAGS = -T kernel/kernel.ld

# 构建目录
BUILD_DIR = build

# 源文件和目标 - 现在目标文件在 build 目录中
KERNEL_SRCS = kernel/entry.S \
			  kernel/main.c \
			  kernel/uart.c \
			  kernel/console.c \
			  kernel/printf.c
			  
KERNEL_OBJS = $(patsubst kernel/%.S,$(BUILD_DIR)/%.o,$(filter %.S,$(KERNEL_SRCS))) \
              $(patsubst kernel/%.c,$(BUILD_DIR)/%.o,$(filter %.c,$(KERNEL_SRCS)))

KERNEL_ELF = $(BUILD_DIR)/kernel.elf
KERNEL_BIN = $(BUILD_DIR)/kernel.bin

# 默认目标
all: $(BUILD_DIR) $(KERNEL_BIN)

# 创建构建目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 生成二进制文件
$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@

# 链接内核
$(KERNEL_ELF): $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# 编译C文件到build目录
$(BUILD_DIR)/%.o: kernel/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# 汇编文件到build目录 - 使用gcc而不是as，以确保一致的编译选项
$(BUILD_DIR)/%.o: kernel/%.S | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# 清理目标文件
clean:
	rm -rf $(BUILD_DIR)

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