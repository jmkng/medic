#include <stddef.h>

/*
 * Returns the number of physical CPU cores available in the current
 * power management mode.
 *
 * @return
 *      Number of physical CPU cores on success,
 *      or -1 on error.
 */
int medic_physical_cpu(void);

/*
 * Returns the number of logical CPU cores available in the current
 * power management mode.
 *
 * @return
 *      Number of logical CPU cores on success,
 *      or -1 on error.
 */
int medic_logical_cpu(void);

/*
 * Returns the number of processes in the system run queue averaged
 * over various periods of time.
 *
 * Wraps the posix function getloadavg(3).
 * Up to `size` samples are retrieved and assigned to successive elements of `loadavg`.
 * The system imposes a maximum of 3 samples,
 * representing averages over the last 1, 5 and 15 minutes.
 *
 * See `medic_load_avg_default` for a more simple version of this function.
 *
 * @return
 *      Number of samples successfully stored (up to `size`),
 *      or -1 on error.
 */
int medic_load_avg(double loadavg[], size_t size);

/* System load average over standard time intervals. */
typedef struct {
    double load1m; /* Load average over the past 1 minute. */
    double load5m; /* Load average over the past 5 minutes. */
    double load15m; /* Load average over the past 15 minutes. */
    int error; /* 0 on success, -1 on error. */
} MedicLoadSnapshot;

/*
 * Returns the number of processes in the system run queue averaged
 * over 1, 5, and 15 minutes.
 *
 * This is a convenience wrapper around `medic_load_avg` that
 * returns results in a structured form.
 */
MedicLoadSnapshot medic_load_snapshot(void);

/*
 * Snapshot of CPU time spent in different categories since system boot,
 * normalized to seconds.
 */
typedef struct {
    double user;
    double system;
    double nice;
    double idle;
} MedicCpuSnapshot;

/*
 * Copies a MedicCpuSnapshot struct with current statistics to ss.
 * The value is an aggregate over all processor cores.
 *
 * For per-core statistics, use `medic_cpu_snapshot_multi`.
 *
 * @return
 *      0 on success; -1 on error.
 */
int medic_cpu_snapshot(MedicCpuSnapshot* ss);

/*
 * Copies up to `size` instances of `MedicCpuSnapshot` to buffer.
 * Each snapshot copied to buffer corresponds to a different processor core.
 *
 * @return
 *      The number of `MedicCpuSnapshot` copied to buffer,
 *      which will never be greater than `size`, or -1 on error.
 */
int medic_cpu_snapshot_multi(MedicCpuSnapshot buffer[], size_t size);

/*
 * Returns the percentage of CPU time spent in user mode between two snapshots.
 *
 * To use, capture an initial sample with `medic_cpu_snapshot()`,
 * wait the desired interval, then capture a second sample.
 * Pass both snapshots to this function to compute the percentage over that interval.
 *
 * @return
 *      Percentage of CPU time spent in user mode between two snapshots,
 *      or 0.0 when:
 *          - Pointer `start` or `end` is NULL.
 *          - Times represented in end are before start.
 */
double medic_cpu_user_percent(const MedicCpuSnapshot* start, const MedicCpuSnapshot* end);

/*
 * Returns the percentage of CPU time spent in system mode between two snapshots.
 *
 * To use, capture an initial sample with `medic_cpu_snapshot()`,
 * wait the desired interval, then capture a second sample.
 * Pass both snapshots to this function to compute the percentage over that interval.
 *
 * @return
 *      Percentage of CPU time spent in user mode between two snapshots,
 *      or 0.0 when:
 *          - Pointer `start` or `end` is NULL.
 *          - Times represented in end are before start.
 */
double medic_cpu_system_percent(const MedicCpuSnapshot* start, const MedicCpuSnapshot* end);

/*
 * Returns the percentage of CPU time spent in nice mode between two snapshots.
 *
 * To use, capture an initial sample with `medic_cpu_snapshot()`,
 * wait the desired interval, then capture a second sample.
 * Pass both snapshots to this function to compute the percentage over that interval.
 *
 * @return
 *      Percentage of CPU time spent in user mode between two snapshots,
 *      or 0.0 when:
 *          - Pointer `start` or `end` is NULL.
 *          - Times represented in end are before start.
 */
double medic_cpu_nice_percent(const MedicCpuSnapshot* start, const MedicCpuSnapshot* end);

/*
 * Returns the percentage of CPU time spent in idle mode between two snapshots.
 *
 * To use, capture an initial sample with `medic_cpu_snapshot()`,
 * wait the desired interval, then capture a second sample.
 * Pass both snapshots to this function to compute the percentage over that interval.
 *
 * @return
 *      Percentage of CPU time spent in user mode between two snapshots,
 *      or 0.0 when:
 *          - Pointer `start` or `end` is NULL.
 *          - Times represented in end are before start.
 */
double medic_cpu_idle_percent(const MedicCpuSnapshot* start, const MedicCpuSnapshot* end);

/*
 * Returns the percentage of CPU time spent in any non-idle mode (user/system/nice)
 * between two snapshots.
 *
 * To use, capture an initial sample with `medic_cpu_snapshot()`,
 * wait the desired interval, then capture a second sample.
 * Pass both snapshots to this function to compute the percentage over that interval.
 *
 * @return
 *      Percentage of CPU time spent in user mode between two snapshots,
 *      or 0.0 when:
 *          - Pointer `start` or `end` is NULL.
 *          - Times represented in end are before start.
 */
double medic_cpu_nonidle_percent(const MedicCpuSnapshot* start, const MedicCpuSnapshot* end);
