#ifndef BOOT_DATA_H
#define BOOT_DATA_H

#include <mm/core_memprot.h>
#include <io.h>

#define BOOT_DATA_SIG_OFFSET             0x0000
#define BOOT_DATA_SIG_OFFSET_SIZE        0x0004
#define BOOT_DATA_SIG_VAL                0xAA55AA55
#define BOOT_DATA_CUR_LEN_OFF            0x0004
#define BOOT_DATA_CUR_LEN_SIZE           0x0004
#define BOOT_DATA_HEADER_SIZE            0x0008
#define BOOT_DATA_MAX_SIZE		0xFFFF8U

/*
 * @base: Base address of memory where boot log will be saved.
 * @max_size: Max size of memory reserved for boot log message
 */
struct bootdata {
	struct io_pa_va base;
	uint32_t bootdata_max_size;
};

void boot_data_init(uintptr_t pa_base, uint32_t size);

#endif /* BOOT_DATA_H */