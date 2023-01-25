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


static TEE_Result pta_get_device_id(uint32_t param_types,
		TEE_Param params[TEE_NUM_PARAMS])
{
	EMSG("Start pta_get_device_id\n\n\n");
	struct tee_hw_unique_key huk = { };
	TEE_Result res = TEE_SUCCESS;

	return res;

	// res = tee_otp_get_hw_unique_key(&huk);

	// if(res != TEE_SUCCESS)
	// {
	// 	EMSG("tee_otp_get_hw_unique_key returned 0x%x\n",
	// 			(unsigned int)res);
	// 	return res;
	// }
	// //memcpy(&params->value.a, huk.data, HW_UNIQUE_KEY_LENGTH);
	// for (int i = 0; i < HW_UNIQUE_KEY_LENGTH; i++)
	// {
	//     if (i > 0) DMSG(":");
	//     DMSG("%02X", huk.data[i]);
	// }

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
			// res = pta_get_device_id(param_types, params);
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