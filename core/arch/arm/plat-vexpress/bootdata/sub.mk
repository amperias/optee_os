global-incdirs-y += .

ifeq ($(PLATFORM_FLAVOR_qemu_virt),y)
srcs-$(CFG_BOOT_DATA_PTA) += bootdata.c
endif