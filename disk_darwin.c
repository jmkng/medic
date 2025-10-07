#include "disk.h"
#include <assert.h>

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
        cb(&md, userdata);
    }
}
