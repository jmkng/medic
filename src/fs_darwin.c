#if defined(__APPLE__)

#include "fs.h"

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

int medic_mount_stat(const char* mountpoint, struct MedicMountStat* out)
{
    if (out == NULL)
        return -1;

    struct statfs fs;
    if (statfs(mountpoint, &fs) != 0)
        return -1;

    out->blocks = fs.f_blocks;
    out->block_size = fs.f_bsize;
    out->blocks_free = fs.f_bfree;
    out->blocks_avail_non_root = fs.f_bavail;

    return 0;
}

#endif
