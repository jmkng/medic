#if defined(__APPLE__)

#include "cpu.h"
#include <assert.h>
#include <mach/host_info.h>
#include <mach/mach.h>
#include <mach/mach_host.h>
#include <mach/processor_info.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <unistd.h>

typedef enum {
    PHYSICAL,
    LOGICAL
} _MedicCpuType;

int _medic_cpu(_MedicCpuType type)
{
    assert(type == PHYSICAL || type == LOGICAL);
    const char* sysctl_name = NULL;

    switch (type) {
    case PHYSICAL:
        sysctl_name = "hw.physicalcpu";
        break;
    case LOGICAL:
        sysctl_name = "hw.logicalcpu";
        break;
    }

    int num;
    size_t size = sizeof(num);
    if (sysctlbyname(sysctl_name, &num, &size, NULL, 0) == -1)
        return -1;

    return num;
}

int medic_physical_cpu(void)
{
    return _medic_cpu(PHYSICAL);
}

int medic_logical_cpu(void)
{
    return _medic_cpu(LOGICAL);
}

int medic_load_avg(struct MedicLoad* ml)
{
    if (ml == NULL)
        return -1;

    double loadavg[3];
    int num = getloadavg(loadavg, 3);
    if (num == -1)
        return -1;

    ml->load_1 = loadavg[0];
    ml->load_5 = loadavg[1];
    ml->load_15 = loadavg[2];

    return 0;
}

int medic_cpu(struct MedicCpu* ss)
{
    if (ss == NULL)
        return -1;

    host_cpu_load_info_data_t cpuinfo;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;

    kern_return_t kr = host_statistics(mach_host_self(),
        HOST_CPU_LOAD_INFO,
        (host_info_t)&cpuinfo,
        &count);
    if (kr != KERN_SUCCESS)
        return -1;

    // https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/sysconf.3.html
    // _SC_CLK_TCK:
    //      The frequency of the statistics clock in ticks per second.
    long ticks_per_sec = sysconf(_SC_CLK_TCK);

    ss->user = (double)cpuinfo.cpu_ticks[CPU_STATE_USER] / ticks_per_sec;
    ss->system = (double)cpuinfo.cpu_ticks[CPU_STATE_SYSTEM] / ticks_per_sec;
    ss->nice = (double)cpuinfo.cpu_ticks[CPU_STATE_NICE] / ticks_per_sec;
    ss->idle = (double)cpuinfo.cpu_ticks[CPU_STATE_IDLE] / ticks_per_sec;

    return 0;
}

int medic_cpu_stream(MedicCpuSink cb, void* data)
{
    natural_t cpu_count;
    processor_info_array_t cpuinfo;
    mach_msg_type_number_t cpuinfo_count;

    kern_return_t kr = host_processor_info(mach_host_self(),
        PROCESSOR_CPU_LOAD_INFO,
        &cpu_count,
        &cpuinfo,
        &cpuinfo_count);
    if (kr != KERN_SUCCESS) {
        return -1;
    }

    // See: `medic_cpu_snapshot`.
    long ticks_per_sec = sysconf(_SC_CLK_TCK);

    for (size_t i = 0; i < cpu_count; i++) {
        integer_t* ticks = &cpuinfo[i * CPU_STATE_MAX];
        struct MedicCpu cpu;
        cpu.user = (double)ticks[CPU_STATE_USER] / ticks_per_sec;
        cpu.system = (double)ticks[CPU_STATE_SYSTEM] / ticks_per_sec;
        cpu.nice = (double)ticks[CPU_STATE_NICE] / ticks_per_sec;
        cpu.idle = (double)ticks[CPU_STATE_IDLE] / ticks_per_sec;
        cb(&cpu, data);
    }
    vm_deallocate(mach_task_self(), (vm_address_t)cpuinfo, cpuinfo_count * sizeof(integer_t));

    return 0;
}

int medic_cpu_diff(const struct MedicCpu* start,
    const struct MedicCpu* end,
    struct MedicCpuDiff* out)
{
    if (start == NULL || end == NULL || out == NULL)
        return -1;

    double total_ticks_start = start->user + start->system + start->nice + start->idle;
    double total_ticks_end = end->user + end->system + end->nice + end->idle;

    double total_diff = total_ticks_end - total_ticks_start;
    if (total_diff <= 0.0)
        return -1;

    double user_diff = end->user - start->user;
    out->user = user_diff;
    out->system = end->system - start->system;
    out->nice = end->nice - start->nice;
    double idle_diff = end->idle - start->idle;
    out->idle = idle_diff;
    out->nonidle = total_diff - idle_diff;

    return 0;
}

#endif
