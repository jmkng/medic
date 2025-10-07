#if defined(__APPLE__)

#include "mem.h"
#include <mach/mach.h>
#include <stdio.h>
#include <sys/sysctl.h>

uint64_t medic_mem_size(void)
{
    long long memsize;
    size_t size = sizeof(memsize);

    int mib_path[2] = { CTL_HW, HW_MEMSIZE };
    int code = sysctl(mib_path, 2, &memsize, &size, NULL, 0);
    if (code == -1)
        return 0;

    return memsize;
}

uint64_t medic_mem_used(uint64_t mem_size)
{
    if (mem_size == 0)
        return 0;

    vm_size_t page_size;
    if (host_page_size(mach_host_self(), &page_size) != KERN_SUCCESS) {
        return 0;
    }

    vm_statistics64_data_t vm_stat;
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
    if (host_statistics64(mach_host_self(), HOST_VM_INFO64, (host_info64_t)&vm_stat, &count) != KERN_SUCCESS) {
        return 0;
    }

    uint64_t free_bytes = (vm_stat.free_count - vm_stat.speculative_count) * page_size;
    // uint64_t wired_bytes = vm_stat.wire_count * page_size;
    // uint64_t anon_sub_purgeable_bytes = (vm_stat.internal_page_count - vm_stat.purgeable_count) * page_size;

    // This line was an accident.
    // It converts the external page count to bytes, and then adds the bytes to the purgeable page count.
    // Looks wrong, but it also happens to match "Memory Used" in Activity Monitor.
    // I'm leaving it until I understand why.
    uint64_t cached_bytes = vm_stat.purgeable_count + vm_stat.external_page_count * page_size;

    uint64_t used_bytes = mem_size - free_bytes - cached_bytes;
    return used_bytes;
}

#endif
