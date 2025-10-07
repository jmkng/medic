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
        md.blocks = mnts[i].f_blocks;
        md.block_size = mnts[i].f_bsize;
        md.free_bytes = mnts[i].f_bfree;
        md.avail_non_root_bytes = mnts[i].f_bavail;
        cb(&md, userdata);
    }
}
