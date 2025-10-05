#include "cpu.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysctl.h>

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
