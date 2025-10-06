#include <stddef.h>
#include <time.h>

/*
 * Returns the system boot time as a UNIX timestamp.
 *
 * @return
 *      The boot time as seconds since the UNIX epoch (1970-01-01 00:00:00 UTC),
 *      or -1 if an error occurred while retrieving the boot time.
 */
long medic_boot_time(void);

/*
 * Returns the system uptime in seconds.
 *
 * @return
 *      The uptime in seconds,
 *      or -1 if an error occurred while retrieving the boot time.
 */
long medic_uptime(void);

/*
 * Copies the system kernel version to buffer as a null terminated string.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int medic_kernel_version(char* buffer, size_t size);

/*
 * Copies the system kernel type to buffer as a null terminated string.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int medic_kernel_type(char* buffer, size_t size);

/*
 * Copies the system host architecture to buffer as a null terminated string.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int medic_arch(char* buffer, size_t size);

/*
 * Copies the system host name to buffer as a null terminated string.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int medic_hostname(char* buffer, size_t size);

typedef struct {
    char name[256];
    char tty[32];
} MedicUser;

/*
 * Callback type used by `medic_active_user_stream`.
 * Each invocation is passed a pointer to a null-terminated string representing a username.
 *
 * The pointer is only valid for the duration of the callback invocation;
 * if the value needs to persist, it must be copied by the caller.
 */
typedef void (*MedicUserSink)(const MedicUser* user, void* data);

/**
 * Streams currently active user sessions.
 *
 * Calls the given callback for each active user process.
 * Each callback invocation receives a pointer to a MedicUser struct,
 * valid only for the duration of the callback.
 */
void medic_active_user_stream(MedicUserSink cb, void* data);
