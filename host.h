#include <stddef.h>

/**
 * Returns the system boot time as a UNIX timestamp.
 *
 * This function queries the system kernel for the time at which the host machine was booted.
 * The caller should cache the result.
 *
 * @return
 *      The boot time as seconds since the UNIX epoch (1970-01-01 00:00:00 UTC),
 *      or -1 if an error occurred while retrieving the boot time.
 */
long medic_host_boot_time(void);

/**
 * Returns the system uptime in seconds.
 *
 * This function queries the system kernel for the time at which the
 * host machine was booted.
 *
 * @return
 *      The uptime in seconds,
 *      or -1 if an error occurred while retrieving the boot time.
 */
long medic_host_uptime(void);

/**
 * Copies the system kernel version to buffer as a null terminated string.
 *
 * This function queries the system kernel for the kernel version.
 * The caller should cache the result.
 *
 * @return
 *      The string assigned to buffer is always null-terminated.
 *      The caller should ensure the buffer is large enough to hold the version,
 *      256 bytes may be sufficient.
 */
int medic_kernel_version(char* buffer, size_t size);

/**
 * Copies the system kernel type to buffer as a null terminated string.
 *
 * This function queries the system kernel for the kernel type.
 * The caller should cache the result.
 *
 * @return
 *      The string assigned to buffer is always null-terminated.
 *      The caller should ensure the buffer is large enough to hold the version,
 *      256 bytes may be sufficient.
 */
int medic_kernel_type(char* buffer, size_t size);
