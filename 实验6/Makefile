# Makefile
TOOLCHAIN = riscv64-unknown-elf-
CC = $(TOOLCHAIN)gcc
LD = $(TOOLCHAIN)ld

CFLAGS = -Wall -Werror -O -fno-omit-frame-pointer -ggdb -mcmodel=medany -ffreestanding -nostdlib -mno-relax -Iinclude/
LDFLAGS = -T kernel/kernel.ld

# 构建目录配置
BUILD_DIR = build
KERNEL_ELF = $(BUILD_DIR)/kernel.elf

# 源文件列表
SRCS = \
    kernel/entry.S        \
    kernel/main.c         \
    kernel/uart.c         \
    kernel/console.c      \
    kernel/printf.c       \
    kernel/kalloc.c       \
    kernel/vm.c           \
    kernel/trap.c         \
    kernel/kernelvec.S    \
    kernel/proc.c         \
    kernel/swtch.S        \
    kernel/spinlock.c     \
    kernel/syscall.c      \
    kernel/sysproc.c      \
    kernel/sysfile.c      \
    kernel/test.c

# 生成对象文件路径（放在build目录下）
OBJS = $(patsubst kernel/%.c,$(BUILD_DIR)/kernel/%.o,$(filter %.c,$(SRCS))) \
       $(patsubst kernel/%.S,$(BUILD_DIR)/kernel/%.o,$(filter %.S,$(SRCS)))

# 创建构建目录
$(shell mkdir -p $(BUILD_DIR)/kernel)

all: $(KERNEL_ELF)

$(KERNEL_ELF): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
	@echo "Kernel built: $(KERNEL_ELF)"

# C源文件编译规则
$(BUILD_DIR)/kernel/%.o: kernel/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# 汇编源文件编译规则
$(BUILD_DIR)/kernel/%.o: kernel/%.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

qemu: $(KERNEL_ELF)
	qemu-system-riscv64 -machine virt -nographic -kernel $(KERNEL_ELF)

# 查看文件列表
list:
	@echo "Source files:"
	@for file in $(SRCS); do echo "  $$file"; done
	@echo "\nObject files (will be in $(BUILD_DIR)/):"
	@for file in $(OBJS); do echo "  $$file"; done

# 清理构建目录
clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned build directory: $(BUILD_DIR)"

# 同时清理并重新构建
rebuild: clean all

.PHONY: all qemu clean rebuild list