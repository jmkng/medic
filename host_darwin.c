#include <stdio.h>
#include <sys/sysctl.h>
#include <sys/types.h>

// SYSCTL(3)
// https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/sysctl.3.html

long medic_host_boot_time(void)
{
    struct timeval boottime;
    size_t size = sizeof(boottime);

    int mib[2] = { CTL_KERN, KERN_BOOTTIME };
    int code = sysctl(mib, 2, &boottime, &size, NULL, 0) != 0;
    if (code == -1)
        return -1;

    return boottime.tv_sec;
}

long medic_host_uptime(void)
{
    long boot_time = medic_host_boot_time();
    if (boot_time == -1)
        return -1;

    long now = time(NULL);
    long diff_secs = now - boot_time;
    return diff_secs;
}
