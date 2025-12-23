#ifndef __PARAM_H__
#define __PARAM_H__

#define NPROC        64// 系统支持的最大进程数
#define NCPU         1 // CPU核心数量
#define NOFILE       16 // 每个进程最多能打开的文件数
#define NFILE        100// 系统范围内同时打开的最大文件总数
#define NINODE       300// 内存中缓存的inode数量
#define NDEV         10// 最大设备数量
#define ROOTDEV      1 // 根文件系统所在设备号
#define MAXOPBLOCKS  10 // 单个文件系统操作涉及的最大块数
#define LOGSIZE      (MAXOPBLOCKS*3) // 日志区域大小（以块为单位）= 30块
#define NBUF         (MAXOPBLOCKS*3) // 块缓存（Buffer Cache）的大小 = 30块
#define MAXPATH      128// 最大文件路径长度（包括终止符）
#define BSIZE        1024 // 磁盘块大小（字节）= 1KB
#define FSSIZE       4096 // 文件系统的总块数 = 4096块

#endif // __PARAM_H__