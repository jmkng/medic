#include <assert.h>
#include <stdio.h>
#include <sys/sysctl.h>

typedef enum {
    PHYSICAL,
    LOGICAL
} _medic_cpu_type;

int _medic_cpu(_medic_cpu_type type)
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
