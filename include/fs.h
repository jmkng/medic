#ifndef MEDIC_FS_H
#define MEDIC_FS_H

#include <string.h>

#include <sys/mount.h>
#include <sys/param.h>

typedef struct {
    uint64_t blocks;
    uint64_t block_size; /* Blocks size in bytes. */
    uint64_t blocks_free;
    uint64_t blocks_avail_non_root;
} MedicFsMountStat;

typedef struct {
    const char* mountpoint;
    const char* device;
    const char* fstype;
    MedicFsMountStat stat;
} MedicFsMount;

/*
 * Callback type used by `medic_fs_mount_stream`.
 * Each invocation is passed a pointer to a file system.
 *
 * The pointer is only valid for the duration of the callback invocation;
 * if the value needs to persist, it must be copied by the caller.
 */
typedef void (*MedicFsMountSink)(const MedicFsMount* mount, void* data);

/*
 * Stream mounted file systems.
 *
 * Calls the given callback for each mounted file system.
 * Each callback invocation receives a pointer to a MedicMount struct,
 * valid only for the duration of the callback.
 */
void medic_fs_mount_stream(MedicFsMountSink cb, void* data);

/*
 * Copies file system statistics for mountpoint to out.
 *
 * When file systems are enumerated with `medic_mount_stream`,
 * they contain an initial set of statistics.
 * To retrieve updated statistics for one of those file systems at a later time,
 * pass the mountpoint field to this function.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int32_t medic_fs_mount_stat(const char* mountpoint, MedicFsMountStat* out);

#endif
