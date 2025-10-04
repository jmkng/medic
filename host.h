#include <stddef.h>

/**
 * Returns the system boot time as a UNIX timestamp.
 *
 * Queries the system kernel on each call.
 * The result is unlikely to change during program execution,
 * callers may wish to cache the value.
 *
 * @return
 *      The boot time as seconds since the UNIX epoch (1970-01-01 00:00:00 UTC),
 *      or -1 if an error occurred while retrieving the boot time.
 */
long medic_host_boot_time(void);

/**
 * Returns the system uptime in seconds.
 *
 * Queries the system kernel on each call.
 * The result is unlikely to change during program execution,
 * callers may wish to cache the value.
 *
 * @return
 *      The uptime in seconds,
 *      or -1 if an error occurred while retrieving the boot time.
 */
long medic_host_uptime(void);

/**
 * Copies the system kernel version to buffer as a null terminated string.
 *
 * Queries the system kernel on each call.
 * The result is unlikely to change during program execution,
 * callers may wish to cache the value.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int medic_kernel_version(char* buffer, size_t size);

/**
 * Copies the system kernel type to buffer as a null terminated string.
 *
 * Queries the system kernel on each call.
 * The result is unlikely to change during program execution,
 * callers may wish to cache the value.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int medic_kernel_type(char* buffer, size_t size);
