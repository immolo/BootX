#include <MacTypes.h>#include <CodeFragments.h>#include <MixedMode.h>#include "MoreCFMPatches.h"UInt32 __procinfo = kCStackBased | RESULT_SIZE(SIZE_CODE(sizeof(Boolean)));extern pascal UInt32 PatchPowerOff(UInt32 param);Boolean main(void){	THz					oldZone;	OSErr				err;	CFragConnectionID	bdyConnID;	Ptr					fragMain, foundSym;	CFragSymbolClass	foundSymClass;	Str255				fragErr;	Boolean				result = false;			oldZone = GetZone();	SetZone(SystemZone());		/* Locate Burgundy driver */	err = GetSharedLibrary("\pBurgundyPlugin", kPowerPCCFragArch, kFindCFrag, &bdyConnID, &fragMain, fragErr);	if (err != noErr)		goto bail;	/* Locate SndHWSetPowerState function */	err = FindSymbol(bdyConnID, "\pSndHWSetPowerState", &foundSym, &foundSymClass);	if (err != noErr) {		err = FindSymbol(bdyConnID, "\p__SndHWSetPowerState", &foundSym, &foundSymClass);		if (err != noErr)			goto bail;	}	/* Patch it */	err = MorePatchTVector((TVector *)foundSym, (TVector *)PatchPowerOff, 'BooX', 0);	if (err != noErr) {		/*DebugStr("\pPatch failed !");*/		goto bail;	}	result = true;bail:	SetZone(oldZone);	return result;}#pragma export onpascal UInt32 PatchPowerOff(UInt32 /*param*/){	/*DebugStr("\pBurgundy PowerOff");*/		return 0;}#pragma export off