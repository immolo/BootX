#ifndef __LOW_LEVEL_BOOT_H__#define __LOW_LEVEL_BOOT_H__#include "EnterPPCSupervisor.h"#include "BootX.h"#include "debug_text.h"#define LOW_BOOT_SETUP_VIDEO	0x00000001#define LOW_BOOT_FORCE_SCSI_ON	0x00000002#define LOW_BOOT_CLOSE_VIDEO	0x00000004#define LOW_BOOT_RESET_ATA		0x00000008enum{	// 68k glue procInfo	uppLowLevelBoot68kProcInfo = kPascalStackBased		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(PPCRegisterList*)))		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(boot_infos_t*)))		 | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(void*)))		 | STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(UInt32)))		 | STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(void *))),	// PPC Glue procInfo	uppLowLevelBootPPCProcInfo = kPascalStackBased		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(void*)))		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(boot_infos_t*)))};extern pascal voidlow_boot(PPCRegisterList* regList, boot_infos_t* bi, void* ppc_glue, UInt32 flags, dt_context* in_dct);#endif