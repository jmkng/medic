#include <string.h>
#include <sys/mount.h>
#include <sys/param.h>

struct MedicMountStat {
    uint64_t blocks;
    uint64_t block_size;
    uint64_t blocks_free;
    uint64_t blocks_avail_non_root;
};

struct MedicMount {
    const char* mountpoint;
    const char* device;
    const char* fstype;
    struct MedicMountStat stat;
};

/*
 * Callback type used by `medic_mount_stream`.
 * Each invocation is passed a pointer to a file system.
 *
 * The pointer is only valid for the duration of the callback invocation;
 * if the value needs to persist, it must be copied by the caller.
 */
typedef void (*MedicMountSink)(const struct MedicMount* mount, void* data);

/*
 * Streams mounted file systems.
 *
 * Calls the given callback for each mounted file system.
 * Each callback invocation receives a pointer to a MedicMount struct,
 * valid only for the duration of the callback.
 */
void medic_mount_stream(MedicMountSink cb, void* data);
