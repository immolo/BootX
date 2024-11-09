#include <Types.h>#include <Memory.h>#include <Quickdraw.h>#include <Fonts.h>#include <Events.h>#include <Menus.h>#include <Errors.h>#include <Files.h>#include <Devices.h>#include <Windows.h>#include <Traps.h>#define RELOC		0#define TEST_PAINT	0/* Low memory globals */#define	kLM_SysZone		0x2a6#define	kLM_ApplZone	0x2aa#define kLM_TheZone		0x118#define kLM_BootMask	0xb0e#define kLM_BootDrive	0x210#define kLM_CurrentA5	0x904#define kLM_BufPtr		0x10c/* Increase system heap by this number of bytes */#define kSysZoneIncrease	0x200000 /* We ask for 2Mb */extern void __Startup__(void);#if TEST_PAINTextern void test_paint(void);#endifasm void __Startup__(void) {	/* First, the boot block code will have removed it's own	   return address from the stack, we recover it */		subq.l		#4, sp		#if TEST_PAINT		jsr			test_paint#endif		#if RELOC	/* We copy ourselves on the stack */		movea.l		a3,a0		_GetHandleSize		lea      	real_entry,a0		lea			__Startup__,a1		suba.l		a0,a1		add.l		a1,d0		suba.l		d0,sp		movea.l		sp,a1		_BlockMove		jmp			(a1)real_entry:			move.l		a3,-(sp)		_ReleaseResource		move.l		sp, 0x908	/* CurStackBase */#endif	/* This is in MacOS ... */		clr.l		0xa8c		/* RestProc */	/* We backup any register we'll be using */		move.l		a2, -(sp)		move.l		a3, -(sp)			/* We check if the Application heap and System heap are the	   same. If they are not, then we move the application base	   in order to increase the system zone size	 */		movea.l		kLM_SysZone, a2		cmpa.l		kLM_ApplZone, a2		beq			same_zone		movea.l		(a2), a0		adda.l		#kSysZoneIncrease, a0		_SetAppBasesame_zone:			/* Set the current zone to be the system zone */		move.l		a2, kLM_TheZone	#if !RELOC	/* Detach ourselves (we should already be locked) */		move.l		a3, -(sp)		_DetachResource#endif		/* Load the second code resource */		subq.l		#4, sp		pea			'boot'		move.w		#3, -(sp)		_GetResource		move.l		(sp)+, a2		cmpa.l		#0, a2		beq			load_error		/* Detach, move high and lock the resource. We don't use HLockHi, may not	   be in ROM */		move.l		a2, -(sp)		_DetachResource		movea.l		a2, a0		_MoveHHi		movea.l		a2, a0		_HLock		/* Call the resource' code */		move.l		(a2), a0		move.l		kLM_BufPtr, -(sp)		move.l		kLM_CurrentA5, -(sp)		move.l		a5, -(sp)		jsr			(a0)		move.l		(sp)+, a5		move.l		(sp)+, kLM_CurrentA5		move.l		(sp)+, kLM_BufPtr			/* We got out of the booter, we must tell MacOS not to try booting	   from this partition */		move.w		kLM_BootMask, d0		move.w		kLM_BootDrive, d1		bclr		d1, d0		move.w		d0, kLM_BootMask			/* Get rid of the boot resource and unlock myself*/		movea.l		a2, a0		_DisposeHandle#if !RELOC		movea.l		a3, a0		_HUnlock#endif		/* result code *///		moveq		#dsBadStartupDisk, d0//		moveq		#0, d0		moveq		#dsOldSystem, d0		_SysError				/* Exit (restore registers and return) */exit:		movea.l		(sp)+, a3		movea.l		(sp)+, a2		rtsload_error:			moveq		#dsBadStartupDisk, d0		bra.s		exit}#if TEST_PAINTvoid test_paint(void){	GDHandle	hdl;	PixMapPtr	pm;	UInt32		x,y;		hdl = LMGetMainDevice();	if (hdl == NULL || (**hdl).gdPMap == NULL)		return;	pm = *(**hdl).gdPMap;	if (pm->baseAddr == NULL)		return;		for(y=100; y<200; y++) {		unsigned char* line = (unsigned char*)pm->baseAddr + y * (pm->rowBytes & 0x3fff);		for (x=100; x<500; x++)			line[x] = 0;	}}#endif