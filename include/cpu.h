#ifndef MEDIC_CPU_H
#define MEDIC_CPU_H

#include <stdlib.h>
#include <stddef.h>

/*
 * Return the number of physical processor cores available in the current
 * power management mode.
 *
 * @return
 *      Number of physical processor cores on success,
 *      or -1 on error.
 */
int32_t medic_cpu_num_physical(void);

/*
 * Returns the number of logical processor cores available in the current
 * power management mode.
 *
 * @return
 *      Number of logical processor cores on success,
 *      or -1 on error.
 */
int32_t medic_cpu_num_logical(void);

typedef struct {
    double load_1;
    double load_5;
    double load_15;
} MedicCpuRunQueueTriple;

/*
 * Returns the number of processes in the system run queue averaged
 * over 1, 5, and 15 minutes.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int32_t medic_cpu_run_queue_triple(MedicCpuRunQueueTriple* ml);

typedef struct {
    double user;
    double system;
    double nice;
    double idle;
} MedicCpuStat;

/*
 * Copy a `MedicCpuStat` with current statistics to ss.
 * The value is an aggregate over all processor cores.
 *
 * For per-core statistics, use `medic_cpu_stat_stream`.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int32_t medic_cpu_agg_stat(MedicCpuStat* ss);

/*
 * Callback type used by `medic_cpu_stream`.
 * Each invocation is passed a pointer to a `MedicCpu`.
 *
 * The pointer is only valid for the duration of the callback invocation;
 * if the value needs to persist, it must be copied by the caller.
 */
typedef void (*MedicCpuStatSink)(const MedicCpuStat* cpu, void* data);

/*
 * Stream processor core statistics.
 *
 * Calls the given callback for each processor.
 * Each callback invocation receives a pointer to a MedicUser struct,
 * valid only for the duration of the callback.
 *
 * For aggregate statistics, use `medic_cpu_agg_stat`.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int32_t medic_cpu_stat_stream(MedicCpuStatSink cb, void* data);

typedef struct {
    double total;
    double user;
    double system;
    double nice;
    double idle;
    double nonidle;
} MedicCpuDiff;

/*
 * Copies the difference between two `MedicCpuStat` to out.
 *
 * Capture an initial sample with `medic_cpu_agg_stat`,
 * wait the desired interval, then capture a second sample.
 * Pass both snapshots to this function to compute the usage over that interval.
 *
 * @return
 *      0 on success, or -1 on error.
 */
int32_t medic_cpu_stat_diff(const MedicCpuStat* start, const MedicCpuStat* end, MedicCpuDiff* out);

#endif
