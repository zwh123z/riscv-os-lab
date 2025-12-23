#include "defs.h"
#include "param.h"
#include "buf.h"
//2.块缓存层 ，负责管理内存中的磁盘块副本
static uint64 cache_hits;
static uint64 cache_misses;

struct {
    struct spinlock lock;
    struct buf buf[NBUF];
    struct buf head;
} bcache;

void binit(void) {
    struct buf *b;

    spinlock_init(&bcache.lock, "bcache");
    bcache.head.prev = &bcache.head;
    bcache.head.next = &bcache.head;

    for (b = bcache.buf; b < bcache.buf + NBUF; b++) {
        b->next = bcache.head.next;
        b->prev = &bcache.head;
        bcache.head.next->prev = b;
        bcache.head.next = b;
        initsleeplock(&b->lock, "buffer");
    }
}

static struct buf *bget(uint dev, uint blockno) {
    struct buf *b;

    acquire(&bcache.lock);

    // 如果缓冲区已存在，返回它
    for (b = bcache.head.next; b != &bcache.head; b = b->next) {
        if (b->dev == dev && b->blockno == blockno) {
            b->refcnt++;
            cache_hits++;
            release(&bcache.lock);
            acquiresleep(&b->lock);
            return b;
        }
    }

    // 找到最近最少使用的缓冲区
    for (b = bcache.head.prev; b != &bcache.head; b = b->prev) {
        if (b->refcnt == 0) {
            b->dev = dev;
            b->blockno = blockno;
            b->valid = 0;
            b->refcnt = 1;
            cache_misses++;
            release(&bcache.lock);
            acquiresleep(&b->lock);
            return b;
        }
    }

    panic("bget: no buffers");
    return 0;
}
//读取一个块。如果缓存中有且有效，直接返回；否则调用驱动读取。
struct buf *bread(uint dev, uint blockno) {
    struct buf *b = bget(dev, blockno);
    if (!b->valid) {
        virtio_disk_rw(b, 0);
        b->valid = 1;
    }
    return b;
}
// 将缓存块写入磁盘
void bwrite(struct buf *b) {
    if (!holdingsleep(&b->lock)) {
        panic("bwrite");
    }
    virtio_disk_rw(b, 1);
}
//释放块锁。如果引用计数为 0，将其移动到链表头部（表示最近最少使用），方便下次回收
void brelse(struct buf *b) {
    if (!holdingsleep(&b->lock)) {
        panic("brelse");
    }

    releasesleep(&b->lock);

    acquire(&bcache.lock);
    b->refcnt--;
    if (b->refcnt == 0) {
        b->next->prev = b->prev;
        b->prev->next = b->next;
        b->next = bcache.head.next;
        b->prev = &bcache.head;
        bcache.head.next->prev = b;
        bcache.head.next = b;
    }
    release(&bcache.lock);
}

void bpin(struct buf *b) {
    acquire(&bcache.lock);
    b->refcnt++;
    release(&bcache.lock);
}

void bunpin(struct buf *b) {
    acquire(&bcache.lock);
    b->refcnt--;
    release(&bcache.lock);
}

uint64 get_buffer_cache_hits(void) {
    return cache_hits;
}

uint64 get_buffer_cache_misses(void) {
    return cache_misses;
}