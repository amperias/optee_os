#include <io.h>
#include <kernel/pseudo_ta.h>
#include <mm/core_memprot.h>
#include <mm/core_mmu.h>
#include <string.h>
#include <trace.h>
#include <pta_boot_data.h>
#include <kernel/huk_subkey.h>
#include <kernel/tee_common_otp.h>
#include <trace.h>
#include <utee_defines.h>
#include <bootdata/bootdata.h>
#include <crypto/crypto.h>
#include <string.h>

#define DEBUG 0


static TEE_Result pta_get_device_id(uint32_t param_types,
		TEE_Param params[TEE_NUM_PARAMS])
{
	DMSG("Start pta_get_device_id in BOOT_DATA_PTA");
	struct tee_hw_unique_key huk = { };
	TEE_Result res = TEE_SUCCESS;

	res = tee_otp_get_hw_unique_key(&huk);

	if(res != TEE_SUCCESS)
	{
		EMSG("tee_otp_get_hw_unique_key returned 0x%x\n",
				(unsigned int)res);
		return res;
	}
	memcpy(&params[0].memref.buffer, huk.data, HW_UNIQUE_KEY_LENGTH);
	params[0].memref.size = HW_UNIQUE_KEY_LENGTH;

	if(DEBUG)
	{
		for (int i = 0; i < HW_UNIQUE_KEY_LENGTH; i++)
		{
		    DMSG("%02X", huk.data[i]);
		}

	}
	return res;
}

static TEE_Result pta_get_bootloader_hash(uint32_t param_types,
		TEE_Param params[TEE_NUM_PARAMS])
{
	DMSG("Start pta_get_bootloader_hash in BOOT_DATA_PTA");
	
	if (params[1].memref.size < 32)
		return TEE_ERROR_BAD_PARAMETERS;


	// Get Bootloader data
	vaddr_t src_vaddr = 0;
	TEE_Result res = TEE_SUCCESS;

	src_vaddr = (vaddr_t)phys_to_virt(CFG_TEE_BOOT_DATA_START, MEM_AREA_IO_SEC, BOOT_DATA_HEADER_SIZE);

	if (!src_vaddr) {
		EMSG("Not enough memory mapped");
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	// TEST !!!
	uint8_t bootloader_data[512];
	size_t bootloader_data_size = 512;
	for (size_t i = 0; i < 12; i++)
	{
		bootloader_data[i] = i*2;
	}
	bootloader_data_size = sizeof(bootloader_data);
	DMSG("DATA SIZE = %d", bootloader_data_size);
	

	uint8_t bootloader_hash[TEE_SHA256_HASH_SIZE];
	res = hash_sha512_256_compute(bootloader_hash, &bootloader_data, bootloader_data_size);
	if(res != TEE_SUCCESS)
	{
		return res;
	}

	if(DEBUG)
	{
		DMSG("HASH VALUE:");
		for (size_t i = 0; i < TEE_SHA256_HASH_SIZE; i++)
		{
			DMSG("%02x", bootloader_hash[i]);
		}
	}
	memcpy(params[1].memref.buffer, bootloader_hash, TEE_SHA256_HASH_SIZE);
	params[1].memref.size = TEE_SHA256_HASH_SIZE;

	return TEE_SUCCESS;
}

static TEE_Result invoke_command(void *session_context __unused,
		uint32_t cmd_id,
		uint32_t param_types,
		TEE_Param params[TEE_NUM_PARAMS])
{
	TEE_Result res = TEE_SUCCESS;

	DMSG("command entry point[%d] for \"%s\"\n\n\n", cmd_id, PTA_BOOT_DATA_NAME);

	switch (cmd_id) {
		case PTA_BOOT_DATA_DEVICE_ID:
			res = pta_get_device_id(param_types, params);
			break;
		case PTA_BOOT_DATA_BOOTLOADER_HASH:
			res = pta_get_bootloader_hash(param_types, params);
			break;
		default:
			EMSG("cmd: %d Not supported %s", cmd_id, PTA_BOOT_DATA_NAME);
			res = TEE_ERROR_NOT_SUPPORTED;
			break;
	}

	return res;
}

pseudo_ta_register(.uuid = PTA_BOOT_DATA_UUID,
		.name = PTA_BOOT_DATA_NAME,
		.flags = PTA_DEFAULT_FLAGS,
		.invoke_command_entry_point = invoke_command);