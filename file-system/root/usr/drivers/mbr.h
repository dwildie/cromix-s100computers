#define MBR_PARTITIONS 4
#define MBR_OFFSET 0x1be
#define MBR_ENTRY_SIZE 0x10
#define IDE_DRIVES 2

#define le2(a, o) ((a)[o] << 0 | (a)[(o)+1] << 8)
#define le4(a, o) (le2(a,o) | (a)[(o)+2] << 16 | (a)[(o)+3] << 24)
/*
struct mbr_partition {
    int status;
    int type;
    unsigned int start;
    unsigned int sectors;
};

struct mbr_table {
    struct mbr_partition partition[MBR_PARTITIONS];
};
*/
