#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <unistd.h>

long long medic_mem_size(void)
{
    long long memsize;
    size_t size = sizeof(memsize);

    int mib_path[2] = { CTL_HW, HW_MEMSIZE };
    int code = sysctl(mib_path, 2, &memsize, &size, NULL, 0);
    if (code == -1)
        return -1;

    return memsize;
}
