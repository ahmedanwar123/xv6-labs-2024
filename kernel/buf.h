struct buf {
    int valid;          // has data been read from disk?
    int disk;           // does disk "own" buf?
    uint dev;
    uint blockno;
    struct sleeplock lock;
    uint refcnt;
    struct buf *next;   // hash list
    uchar data[BSIZE];
    uint timestamp;     
};

