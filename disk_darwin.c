#include "disk.h"

void medic_mount_stream(MedicMountSink cb, void* userdata)
{
    struct statfs* mnts;
    int count = getmntinfo(&mnts, MNT_NOWAIT);
    if (count < 0)
        return;

    struct MedicMount md;
    for (int i = 0; i < count; i++) {
        md.mountpoint = mnts[i].f_mntonname;
        md.device = mnts[i].f_mntfromname;
        md.fstype = mnts[i].f_fstypename;
        struct MedicMountStat stat;
        stat.blocks = mnts[i].f_blocks;
        stat.block_size = mnts[i].f_bsize;
        stat.blocks_free = mnts[i].f_bfree;
        stat.blocks_avail_non_root = mnts[i].f_bavail;
        md.stat = stat;
        cb(&md, userdata);
    }
}
