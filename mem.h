/*
 * Returns the number of physical CPU cores available in the current
 * power management mode.
 *
 * Queries the system kernel on each call.
 * The result is unlikely to change during program execution,
 * callers may wish to cache the value.
 *
 * @return
 *      Number of physical CPU cores on success,
 *      or -1 on error.
 */
long long medic_mem_size(void);
