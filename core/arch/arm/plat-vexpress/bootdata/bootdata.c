#include <bootdata.h>
#include <io.h>
#include <trace.h>

static struct bootdata global_bootdata;


// Image id=21 loaded: 0xe100000 - 0xe1af8a8
// Image id=5 loaded: 0x60000000 - 0x60092514

void boot_data_init(uintptr_t pa_base, uint32_t size)
{
	DMSG("Boot data initialize start!!!");


	DMSG("Boot data initialized");
}