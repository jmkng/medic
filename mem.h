#include <stdint.h>

/*
 * Returns the total physical memory size of the system in bytes.
 *
 * @return
 *      Total memory size in bytes on success,
 *      or 0 on error.
 */
uint64_t medic_mem_size(void);

/*
 * Returns the physical memory in use by apps or the system.
 *
 * @return
 *      Physical memory in use by apps or the system,
 *      or 0 on error.
 */
uint64_t medic_mem_used(uint64_t total_bytes);
