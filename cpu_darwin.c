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
    if (sysctlbyname(sysctl_name, &num, &size, NULL, 0) == -1) {
        return -1;
    }
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

int medic_load_avg(double loadavg[], size_t size)
{
    return getloadavg(loadavg, size);
}

MedicLoadAvg medic_load_avg_default(void)
{
    MedicLoadAvg mla = { 0 };

    double loadavg[3];
    int num = medic_load_avg(loadavg, 3);
    if (num == -1) {
        mla.error = -1;
        return mla;
    }

    mla.load1m = loadavg[0];
    mla.load5m = loadavg[1];
    mla.load15m = loadavg[2];
    mla.error = 0;

    return mla;
}

int medic_cpu_snapshot(MedicCpuSnapshot* ss)
{
    host_cpu_load_info_data_t cpuinfo;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;

    kern_return_t kr = host_statistics(mach_host_self(),
        HOST_CPU_LOAD_INFO,
        (host_info_t)&cpuinfo,
        &count);
    if (kr != KERN_SUCCESS) {
        return -1;
    }

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

int medic_cpu_snapshot_multi(MedicCpuSnapshot buffer[], size_t size)
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

    size_t filled = cpu_count < size ? cpu_count : size;
    for (size_t i = 0; i < filled; i++) {
        integer_t* ticks = &cpuinfo[i * CPU_STATE_MAX];
        buffer[i].user = (double)ticks[CPU_STATE_USER] / ticks_per_sec;
        buffer[i].system = (double)ticks[CPU_STATE_SYSTEM] / ticks_per_sec;
        buffer[i].nice = (double)ticks[CPU_STATE_NICE] / ticks_per_sec;
        buffer[i].idle = (double)ticks[CPU_STATE_IDLE] / ticks_per_sec;
    }
    vm_deallocate(mach_task_self(), (vm_address_t)cpuinfo, cpuinfo_count * sizeof(integer_t));

    return filled;
}

double medic_cpu_user_percent(const MedicCpuSnapshot* start, const MedicCpuSnapshot* end)
{
    if (!start || !end)
        return 0.0;

    double total_ticks_start = start->user + start->system + start->nice + start->idle;
    double total_ticks_end = end->user + end->system + end->nice + end->idle;

    double total_diff = total_ticks_end - total_ticks_start;
    double user_diff = end->user - start->user;

    if (total_diff <= 0.0)
        return 0.0;

    return 100.0 * user_diff / total_diff;
}

double medic_cpu_system_percent(const MedicCpuSnapshot* start, const MedicCpuSnapshot* end)
{
    if (!start || !end)
        return 0.0;

    double total_ticks_start = start->user + start->system + start->nice + start->idle;
    double total_ticks_end = end->user + end->system + end->nice + end->idle;

    double total_diff = total_ticks_end - total_ticks_start;
    double system_diff = end->system - start->system;

    if (total_diff <= 0.0)
        return 0.0;

    return 100.0 * system_diff / total_diff;
}

double medic_cpu_nice_percent(const MedicCpuSnapshot* start, const MedicCpuSnapshot* end)
{
    if (!start || !end)
        return 0.0;

    double total_ticks_start = start->user + start->system + start->nice + start->idle;
    double total_ticks_end = end->user + end->system + end->nice + end->idle;

    double total_diff = total_ticks_end - total_ticks_start;
    double nice_diff = end->nice - start->nice;

    if (total_diff <= 0.0)
        return 0.0;

    return 100.0 * nice_diff / total_diff;
}

double medic_cpu_idle_percent(const MedicCpuSnapshot* start, const MedicCpuSnapshot* end)
{
    if (!start || !end)
        return 0.0;

    double total_ticks_start = start->user + start->system + start->nice + start->idle;
    double total_ticks_end = end->user + end->system + end->nice + end->idle;

    double total_diff = total_ticks_end - total_ticks_start;
    double idle_diff = end->idle - start->idle;

    if (total_diff <= 0.0)
        return 0.0;

    return 100.0 * idle_diff / total_diff;
}

double medic_cpu_nonidle_percent(const MedicCpuSnapshot* start, const MedicCpuSnapshot* end)
{
    if (!start || !end)
        return 0.0;

    double total_ticks_start = start->user + start->system + start->nice + start->idle;
    double total_ticks_end = end->user + end->system + end->nice + end->idle;

    double total_diff = total_ticks_end - total_ticks_start;
    double idle_diff = end->idle - start->idle;

    if (total_diff <= 0.0)
        return 0.0;

    double working_ticks = total_diff - idle_diff;
    return 100.0 * working_ticks / total_diff;
}
