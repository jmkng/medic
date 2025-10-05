#include <stddef.h>

/**
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
int medic_physical_cpu(void);

/**
 * Returns the number of logical CPU cores available in the current
 * power management mode.
 *
 * Queries the system kernel on each call.
 * The result is unlikely to change during program execution,
 * callers may wish to cache the value.
 *
 * @return
 *      Number of logical CPU cores on success,
 *      or -1 on error.
 */
int medic_logical_cpu(void);

/**
 * Returns the number of processes in the system run queue averaged
 * over a period of time.
 *
 * Wraps the standard getloadavg(3) call.
 * The `size` parameter determines how many values are written to `loadavg`,
 * up to a maximum of 3 as defined by getloadavg(3).
 *
 * @return
 *      Number of samples successfully stored (up to `size`),
 *      or -1 on error.
 */
int medic_load_avg(double loadavg[], size_t size);

/**
 * Represents the system load average over standard time intervals.
 */
typedef struct {
    double load1m; /**< Load average over the past 1 minute. */
    double load5m; /**< Load average over the past 5 minutes. */
    double load15m; /**< Load average over the past 15 minutes. */
    int error; /**< 0 on success, -1 on error. */
} MedicLoadAvg;

/**
 * Returns the number of processes in the system run queue averaged
 * over 1, 5, and 15 minutes.
 *
 * This is a convenience wrapper around medic_load_avg() that
 * returns results in a structured form.
 *
 * @return
 *      A MedicLoadAvg struct containing load averages.
 *      The error field is 0 on success, or -1 on failure.
 */
MedicLoadAvg medic_load_avg_default(void);
