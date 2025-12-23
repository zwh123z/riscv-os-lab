#ifndef __FS_H__
#define __FS_H__

#include "param.h"
#include "sleeplock.h"
#include "spinlock.h"
#include "riscv.h"

#define ROOTINO 1
#define FSMAGIC 0x10203040

// 目录项
#define DIRSIZ  14
struct dirent {
    ushort inum;
    char name[DIRSIZ];
};

// inode 类型
#define T_DIR   1
#define T_FILE  2
#define T_DEV   3

#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof(uint32))
#define MAXFILE (NDIRECT + NINDIRECT)
#define IPB   (BSIZE / sizeof(struct dinode))
#define BPB   (BSIZE * 8)
#define IBLOCK(i, sb)     ((i) / IPB + (sb).inodestart)
#define BBLOCK(b, sb)     ((b) / BPB + (sb).bmapstart)

struct superblock {
    uint32 magic;
    uint32 size;
    uint32 nblocks;
    uint32 ninodes;
    uint32 nlog;
    uint32 logstart;
    uint32 inodestart;
    uint32 bmapstart;
};

struct dinode {
    short type;
    short major;
    short minor;
    short nlink;
    uint32 size;
    uint32 addrs[NDIRECT+1];
};

struct inode {
    uint32 dev;
    uint32 inum;
    int ref;
    struct sleeplock lock;
    int valid;

    short type;
    short major;
    short minor;
    short nlink;
    uint32 size;
    uint32 addrs[NDIRECT+1];
};

extern struct superblock sb;

#endif // __FS_H__