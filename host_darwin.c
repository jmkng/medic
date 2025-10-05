#include "host.h"
#include <CoreFoundation/CoreFoundation.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <time.h>
#include <utmpx.h>

// SYSCTL(3)
// https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/sysctl.3.html

long medic_boot_time(void)
{
    struct timeval boottime;
    size_t size = sizeof(boottime);

    int mib_path[2] = { CTL_KERN, KERN_BOOTTIME };
    int code = sysctl(mib_path, 2, &boottime, &size, NULL, 0);
    if (code == -1)
        return -1;

    return boottime.tv_sec;
}

long medic_uptime(void)
{
    long boot_time = medic_boot_time();
    if (boot_time == -1)
        return -1;

    long now = time(NULL);
    long diff_secs = now - boot_time;
    return diff_secs;
}

int medic_kernel_version(char* buffer, size_t size)
{
    int mib_path[2] = { CTL_KERN, KERN_OSRELEASE };

    int code = sysctl(mib_path, 2, buffer, &size, NULL, 0);
    if (code == -1)
        return -1;

    return 0;
}

int medic_kernel_type(char* buffer, size_t size)
{
    int mib_path[2] = { CTL_KERN, KERN_OSTYPE };

    int code = sysctl(mib_path, 2, buffer, &size, NULL, 0);
    if (code == -1)
        return -1;

    return 0;
}

int medic_arch(char* buffer, size_t size)
{

    // HW_MACHINE is deprecated, but I'm not sure what the replacement is.
    // It says to use HW_PRODUCT but that isn't the same information.
    int mib_path[2] = { CTL_HW, HW_MACHINE };

    int code = sysctl(mib_path, 2, buffer, &size, NULL, 0);
    if (code == -1)
        return -1;

    return 0;
}

int medic_hostname(char* buffer, size_t size)
{
    int mib_path[2] = { CTL_KERN, KERN_HOSTNAME };

    int code = sysctl(mib_path, 2, buffer, &size, NULL, 0);
    if (code == -1)
        return -1;

    return 0;
}

void medic_active_user_stream(MedicUserSink cb, void* data)
{
    if (!cb)
        return;

    setutxent();
    struct utmpx* entry;
    while ((entry = getutxent()) != NULL) {
        if (entry->ut_type == USER_PROCESS && entry->ut_user[0]) {
            MedicUser user;
            strncpy(user.name, entry->ut_user, sizeof(user.name));
            strncpy(user.tty, entry->ut_line, sizeof(user.tty));
            assert(user.name[sizeof(user.name) - 1] == '\0');
            assert(user.tty[sizeof(user.tty) - 1] == '\0');
            cb(&user, data);
        }
    }
    endutxent();
}

///////////////////////////////////////////////////////////////////////////////
// Core Foundation
///////////////////////////////////////////////////////////////////////////////

int medic_product_version(char* buffer, size_t size)
{
    // https://developer.apple.com/documentation/corefoundation/cfurl
    CFURLRef system_version_plist_path = CFURLCreateWithFileSystemPath(
        kCFAllocatorDefault,
        CFSTR("/System/Library/CoreServices/SystemVersion.plist"),
        kCFURLPOSIXPathStyle,
        false);
    // ... "If a function fails to create a CFURL object,
    // it returns NULL, which you must be prepared to handle." ...
    if (!system_version_plist_path)
        return -1;

    CFReadStreamRef system_version_plist_stream = CFReadStreamCreateWithFile(kCFAllocatorDefault, system_version_plist_path);
    if (!system_version_plist_stream) {
        CFRelease(system_version_plist_path);
        return -1;
    }
    if (!CFReadStreamOpen(system_version_plist_stream)) {
        CFRelease(system_version_plist_stream);
        CFRelease(system_version_plist_path);
        return -1;
    }

    CFMutableDataRef data = CFDataCreateMutable(kCFAllocatorDefault, 0);
    unsigned char buf[4096];

    CFIndex bytes_read;
    while ((bytes_read = CFReadStreamRead(system_version_plist_stream, buf, sizeof(buf))) > 0) {
        CFDataAppendBytes(data, buf, bytes_read);
    }
    CFReadStreamClose(system_version_plist_stream);
    CFRelease(system_version_plist_stream);
    CFRelease(system_version_plist_path);
    if (bytes_read < 0) {
        CFRelease(data);
        return -1;
    }

    CFErrorRef error = NULL;
    CFPropertyListRef system_version_plist = CFPropertyListCreateWithData(
        kCFAllocatorDefault, data, kCFPropertyListImmutable, NULL, &error);
    CFRelease(data);
    if (!system_version_plist || CFGetTypeID(system_version_plist) != CFDictionaryGetTypeID()) {
        if (system_version_plist)
            CFRelease(system_version_plist);
        return -1;
    }

    CFDictionaryRef dict = (CFDictionaryRef)system_version_plist;
    CFStringRef product_version = CFDictionaryGetValue(dict, CFSTR("ProductVersion"));
    if (!product_version) {
        CFRelease(system_version_plist);
        return -1;
    }

    Boolean ok = CFStringGetCString(product_version, buffer, (CFIndex)size, kCFStringEncodingUTF8);
    CFRelease(system_version_plist);
    return ok ? 0 : -1;
}
