#include "defs.h"
#include "param.h"
#include "fs.h"
#include "buf.h"
//3.日志层,保证崩溃一致性的核心机制
//原理：所有的元数据修改不直接写入实际位置，而是先写入磁盘上的日志区。
struct logheader {
    int n;
    int block[LOGSIZE];
};

struct log {
    struct spinlock lock;
    int start;
    int size;
    int outstanding;
    int committing;
    int dev;
    struct logheader lh;
};

static struct log log;

static void read_head(void);
static void write_head(void);
static void write_log(void);
static void install_trans(int recovering);

static void install_trans(int recovering) {
    for (int tail = 0; tail < log.lh.n; tail++) {
        struct buf *lbuf = bread(log.dev, log.start + tail + 1);
        struct buf *dbuf = bread(log.dev, log.lh.block[tail]);
        memmove(dbuf->data, lbuf->data, BSIZE);
        bwrite(dbuf);
        bunpin(dbuf);
        brelse(lbuf);
        brelse(dbuf);
    }
    if (!recovering) {
        log.lh.n = 0;
        struct buf *buf = bread(log.dev, log.start);
        struct logheader *hb = (struct logheader*)(buf->data);
        *hb = log.lh;
        bwrite(buf);
        brelse(buf);
    }
}

static void read_head(void) {
    struct buf *buf = bread(log.dev, log.start);
    struct logheader *hb = (struct logheader*)(buf->data);
    log.lh = *hb;
    brelse(buf);
}

static void write_head(void) {
    struct buf *buf = bread(log.dev, log.start);
    struct logheader *hb = (struct logheader*)(buf->data);
    *hb = log.lh;
    bwrite(buf);
    brelse(buf);
}

void initlog(int dev, struct superblock *sb) {
    if (sizeof(struct logheader) >= BSIZE) {
        panic("initlog: too big");
    }
    spinlock_init(&log.lock, "log");
    log.start = sb->logstart;
    log.size = sb->nlog;
    log.dev = dev;
    read_head();
    install_trans(1);
    log.lh.n = 0;
    write_head();
}
//事务流程
//开始一个文件系统操作事务。
void begin_op(void) {
    acquire(&log.lock);
    while (1) {
        if (log.committing) {
            sleep(&log, &log.lock);
        } else if (log.lh.n + (log.outstanding + 1) * MAXOPBLOCKS > LOGSIZE) {
            sleep(&log, &log.lock);
        } else {
            log.outstanding += 1;
            release(&log.lock);
            break;
        }
    }
}


static void write_log(void) {
    for (int tail = 0; tail < log.lh.n; tail++) {
        struct buf *to = bread(log.dev, log.start + tail + 1);
        struct buf *from = bread(log.dev, log.lh.block[tail]);
        memmove(to->data, from->data, BSIZE);
        bwrite(to);
        brelse(from);
        brelse(to);
    }
}
//替代 bwrite。将块标记为“需写入日志”，暂时留在内存中
void log_write(struct buf *b) {
    if (log.lh.n >= LOGSIZE || log.lh.n >= log.size - 1) {
        panic("log_write: too big");
    }
    if (log.outstanding < 1) {
        panic("log_write outside trans");
    }

    acquire(&log.lock);
    int i;
    for (i = 0; i < log.lh.n; i++) {
        if (log.lh.block[i] == b->blockno) {
            break;
        }
    }
    log.lh.block[i] = b->blockno;
    if (i == log.lh.n) {
        log.lh.n++;
        bpin(b);
    }
    release(&log.lock);
}
//结束事务，若日志区满了或事务组提交，则触发 commit
void end_op(void) {
    int do_commit = 0;

    acquire(&log.lock);
    log.outstanding--;
    if (log.committing) {
        panic("log.committing");
    }
    if (log.outstanding == 0) {
        do_commit = 1;
        log.committing = 1;
    } else {
        wakeup(&log);
    }
    release(&log.lock);

    if (do_commit) {
        if (log.lh.n > 0) {
            write_log();//将内存中的变动块写入磁盘的日志区
            install_trans(0);//将日志区的块复制到文件系统的实际位置
            log.lh.n = 0;
            write_head();//更新日志头
        }
        acquire(&log.lock);
        log.committing = 0;
        wakeup(&log);
        release(&log.lock);
    }
}
