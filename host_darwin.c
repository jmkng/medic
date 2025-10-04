#include <stdio.h>
#include <sys/sysctl.h>

long medic_host_boot_time()
{
    struct timeval boottime;
    size_t size = sizeof(boottime);

    int mib[2] = { CTL_KERN, KERN_BOOTTIME };
    int code = sysctl(mib, 2, &boottime, &size, NULL, 0) != 0;
    if (code != 0) {
        perror("sysctl+kern.boottime");
        return 0;
    }

    return boottime.tv_sec;
}
