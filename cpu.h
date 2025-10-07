#ifndef MEDIC_CPU_H
#define MEDIC_CPU_H

#include <stddef.h>

/*
 * Returns the number of physical processor cores available in the current
 * power management mode.
 *
 * @return
 *      Number of physical processor cores on success,
 *      or -1 on error.
 */
int medic_physical_cpu(void);

/*
 * Returns the number of logical processor cores available in the current
 * power management mode.
 *
 * @return
 *      Number of logical processor cores on success,
 *      or -1 on error.
 */
int medic_logical_cpu(void);

struct MedicLoad {
    double load_1;
    double load_5;
    double load_15;
};

/*
 * Returns the number of processes in the system run queue averaged
 * over 1, 5, and 15 minutes.
 *
 * Wraps the posix function getloadavg(3).
 *
 * @return
 *      0 on success, or -1 on error.
 */
int medic_load_avg(struct MedicLoad* ml);

struct MedicCpu {
    double user;
    double system;
    double nice;
    double idle;
};

/*
 * Copies a MedicCpu struct with current statistics to ss.
 * The value is an aggregate over all processor cores.
 *
 * For per-core statistics, use `medic_cpu_stream`.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int medic_cpu(struct MedicCpu* ss);

/*
 * Callback type used by `medic_cpu_stream`.
 * Each invocation is passed a pointer to a `MedicCpu`.
 *
 * The pointer is only valid for the duration of the callback invocation;
 * if the value needs to persist, it must be copied by the caller.
 */
typedef void (*MedicCpuSink)(const struct MedicCpu* cpu, void* data);

/*
 * Streams processor core statistics.
 *
 * Calls the given callback for each processor.
 * Each callback invocation receives a pointer to a MedicUser struct,
 * valid only for the duration of the callback.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int medic_cpu_stream(MedicCpuSink cb, void* data);

struct MedicCpuDiff {
    double total;
    double user;
    double system;
    double nice;
    double idle;
    double nonidle;
};

/*
 * Copies the difference of two instances of `MedicCpu` to out.
 *
 * To use, capture an initial sample with `medic_cpu`,
 * wait the desired interval, then capture a second sample.
 * Pass both snapshots to this function to compute the usage over that interval.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int medic_cpu_diff(const struct MedicCpu* start, const struct MedicCpu* end, struct MedicCpuDiff* out);

#endif
